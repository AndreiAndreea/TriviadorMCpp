#include <vector>
#include <string>
#include <regex>

#include "Database.h"

using namespace sqlite_orm;

bool isValidEmail(const std::string& email)
{
	std::regex validEmailPattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");

	return regex_match(email, validEmailPattern);
}

void initializeFirstDatabase(std::string singleQuestionPath, std::string multipleQuestionPath)
{
	DatabaseStorage storage(singleQuestionPath, multipleQuestionPath);

	storage.Initialize();
}

int main()
{	
	std::string dataFilesPath = "resources/data_files";

	std::string filePathForSingleChoiceQuestion = dataFilesPath + "/questions/SingleChoiceQuestions.txt";
	std::string filePathForMultipleChoiceQuestion = dataFilesPath + "/questions/MultipleChoiceQuestions.txt";
	
	std::string filePathForDatabase = dataFilesPath + "/database/triviador.sqlite";

	std::vector<QuestionSingleChoice> m_randomSingleChoiceQuestionsVector;
	std::vector<bool> m_selectedSingleChoiceQuestions;

	std::vector<QuestionMultipleChoice> m_randomMultipleChoiceQuestionsVector;
	std::vector<bool> m_selectedMultiplChoiceQuestions;
	
	uint8_t amountOfQuestions = 5;

	initializeFirstDatabase(filePathForSingleChoiceQuestion, filePathForMultipleChoiceQuestion);

	UsersStorage userDatabase = CreateUsersStorage(filePathForDatabase);
	userDatabase.sync_schema();

	QuestionsStorage questionsDatabase = CreateQuestionsStorage(filePathForDatabase);
	questionsDatabase.sync_schema();

	crow::SimpleApp app;

	CROW_ROUTE(app, "/")([]() {
		return crow::response(200, "Successfully connected to the server.");
		});

	CROW_ROUTE(app, "/users")(
		[&userDatabase]()
		{
			std::vector<crow::json::wvalue> usersJson;

			for (const auto& user : userDatabase.iterate<User>())
			{
				usersJson.push_back(crow::json::wvalue
				{
					{"ID", user.GetID()},
					{"Username", user.GetUsername()},
					{"Password", user.GetPassword()},
					{"Email", user.GetEmail()},
					{"AccountCreationDate", user.GetAccountCreationDate()},
					{"TotalScore", user.GetTotalScore()},
					{"PlayedGames", user.GetPlayedGames()},
					{"WonGames", user.GetWonGames()}
				});
			}

			return crow::json::wvalue{ usersJson };
		});

	CROW_ROUTE(app, "/checkuser/")(
		[&userDatabase]
		(const crow::request& req)
		{
			std::string username = req.url_params.get("username");
			std::string password = req.url_params.get("password");

			if (username.empty() == false && password.empty() == false)
			{
				auto findUserInDatabase = userDatabase.get_all<User>(where(c(&User::GetUsername) == username and c(&User::GetPassword) == password));

				if (findUserInDatabase.size() == 1)
				{
					crow::json::wvalue user = crow::json::wvalue
					{
						{"ID", findUserInDatabase[0].GetID()},
						{"Username", findUserInDatabase[0].GetUsername()},
						{"Password", findUserInDatabase[0].GetPassword()},
						{"Email", findUserInDatabase[0].GetEmail()},
						{"AccountCreationDate", findUserInDatabase[0].GetAccountCreationDate()},
						{"TotalScore", findUserInDatabase[0].GetTotalScore()},
						{"PlayedGames", findUserInDatabase[0].GetPlayedGames()},
						{"WonGames", findUserInDatabase[0].GetWonGames()}
					};

					return  crow::response(user);
				}
				else
				{
					return crow::response(404, "Username and password not found!");
				}
			}
			else
			{
				return crow::response(500, "Not acceptable! Complete all fields and try again!");
			}
		});

	// https://stackoverflow.com/a/630475/12388382
	auto& checkIfUserExistsInDatabase = CROW_ROUTE(app, "/checkuser").methods(crow::HTTPMethod::POST);
	checkIfUserExistsInDatabase(UserDatabaseControl(userDatabase));

	CROW_ROUTE(app, "/registeruser/")(
		[&userDatabase]
		(const crow::request& req)
		{
			std::string username = req.url_params.get("username");
			std::string password = req.url_params.get("password");
			std::string email = req.url_params.get("email");
			std::string accountCreationDate = req.url_params.get("accountCreationDate");

			if (username.empty() == false && password.empty() == false && email.empty() == false)
			{
				if (isValidEmail(email) == true)
				{
					auto users = userDatabase.get_all<User>(where(c(&User::GetUsername) == username or c(&User::GetEmail) == email));

					uint32_t countUsers = userDatabase.count<User>();

					if (users.size() == 0)
					{
						userDatabase.insert(User(countUsers + 1, username, password, email, accountCreationDate, "0", "0", "0"));

						return crow::response(200, "User registered successfully!");
					}
					else
					{
						return crow::response(409,"An account already exists with this username or email!");
					}
				}
				else
				{
					return crow::response(404,"Invalid email!");
				}
			}
			else
			{
				return crow::response(500, "Complete all fields and try again!");
			}
		});

	// https://stackoverflow.com/a/630475/12388382
	auto& registerNewPlayer = CROW_ROUTE(app, "/registeruser").methods(crow::HTTPMethod::PUT);
	registerNewPlayer(UserDatabaseControl(userDatabase));

	CROW_ROUTE(app, "/getSingleQuestions")(
		[&questionsDatabase]
		()
		{
			std::vector<crow::json::wvalue> usersJson;

			for (const auto& question : questionsDatabase.iterate<QuestionSingleChoice>())
			{
				usersJson.push_back(crow::json::wvalue
					{
						{"ID", question.GetID()},
						{"Question", question.GetQuestionText()},
						{"CorrectAnswer", question.GetAnswer()}
					});
			}

			return crow::json::wvalue{ usersJson };
		}
		);

	auto& getSingleQuestions = CROW_ROUTE(app, "/getSingleQuestions").methods(crow::HTTPMethod::POST);
	getSingleQuestions(UserDatabaseControl(userDatabase));

	//the ID of questions isn't get correctly from DB
	CROW_ROUTE(app, "/getAmountOfSingleQuestions")(
		[&questionsDatabase, &amountOfQuestions, &m_selectedSingleChoiceQuestions, &m_randomSingleChoiceQuestionsVector]
		()
		{
			auto countSingleQuestions = questionsDatabase.count<QuestionSingleChoice>();

			if(m_selectedSingleChoiceQuestions.size() != countSingleQuestions)
				m_selectedSingleChoiceQuestions.resize(countSingleQuestions + 1);
		
			if (m_randomSingleChoiceQuestionsVector.size() <= countSingleQuestions - amountOfQuestions)
			{
				for (uint8_t index = 0; index < amountOfQuestions; index++)
				{
					uint8_t randomID = rand() % countSingleQuestions + 1;

					//can we use std::find ? (maybe needs to change bool to int)
					while (m_selectedSingleChoiceQuestions[randomID] == true)
					{
						randomID = rand() % countSingleQuestions + 1;
					}

					m_randomSingleChoiceQuestionsVector.push_back(questionsDatabase.get<QuestionSingleChoice>(randomID));

					m_selectedSingleChoiceQuestions[randomID] = true;
				}

				std::vector<crow::json::wvalue> usersJson;

				for (const auto& question : m_randomSingleChoiceQuestionsVector)
				{
					usersJson.push_back(crow::json::wvalue
						{
							{"question", question.GetQuestionText()},
							{"correctAnswer", question.GetAnswer()}
						});
				}

				return crow::response(crow::json::wvalue{ usersJson });
			}
			else
			{
				return crow::response(404, "No more single choice questions found! You have selected all the single choice questions.");
			}
		}
		);

	auto& getAmountOfSingleQuestions = CROW_ROUTE(app, "/getAmountOfSingleQuestions").methods(crow::HTTPMethod::POST);
	getAmountOfSingleQuestions(UserDatabaseControl(userDatabase));

	CROW_ROUTE(app, "/getMultipleQuestions")(
		[&questionsDatabase]
		()
		{
			std::vector<crow::json::wvalue> usersJson;

			for (const auto& question : questionsDatabase.iterate<QuestionMultipleChoice>())
			{
				usersJson.push_back(crow::json::wvalue
					{
						{"ID", question.GetID()},
						{"question", question.GetQuestionText()},
						{"correctAnswer", question.GetCorrectAnswer()},
						{"answer1", question.GetAnswer1()},
						{"answer2", question.GetAnswer2()},
						{"answer3", question.GetAnswer3()},
						{"answer4", question.GetAnswer4()}
					});
			}

			return crow::json::wvalue{ usersJson };
		}
		);

		auto& getMultipleQuestions = CROW_ROUTE(app, "/getMultipleQuestions").methods(crow::HTTPMethod::POST);
		getMultipleQuestions(UserDatabaseControl(userDatabase));

	//the ID of questions isn't get correctly from DB
	CROW_ROUTE(app, "/getAmountOfMultipleQuestions")(
		[&questionsDatabase, &amountOfQuestions, &m_selectedMultiplChoiceQuestions, &m_randomMultipleChoiceQuestionsVector]
		()
		{
			auto countSingleQuestions = questionsDatabase.count<QuestionSingleChoice>();

			if (m_selectedMultiplChoiceQuestions.size() != countSingleQuestions)
				m_selectedMultiplChoiceQuestions.resize(countSingleQuestions + 1);

			if (m_randomMultipleChoiceQuestionsVector.size() <= countSingleQuestions - amountOfQuestions)
			{
				for (uint8_t index = 0; index < amountOfQuestions; index++)
				{
					uint8_t randomID = rand() % countSingleQuestions + 1;

					//can we use std::find ? (maybe needs to change bool to int)
					while (m_selectedMultiplChoiceQuestions[randomID] == true)
					{
						randomID = rand() % countSingleQuestions + 1;
					}

					m_randomMultipleChoiceQuestionsVector.push_back(questionsDatabase.get<QuestionMultipleChoice>(randomID));

					m_selectedMultiplChoiceQuestions[randomID] = true;
				}

				std::vector<crow::json::wvalue> usersJson;

				for (const auto& question : m_randomMultipleChoiceQuestionsVector)
				{
					usersJson.push_back(crow::json::wvalue
						{
							{"question", question.GetQuestionText()},
							{"correctAnswer", question.GetCorrectAnswer()},
							{"answer1", question.GetAnswer1()},
							{"answer2", question.GetAnswer2()},
							{"answer3", question.GetAnswer3()},
							{"answer4", question.GetAnswer4()}
						});
				}

				return crow::response(crow::json::wvalue{ usersJson });
			}
			else
			{
				return crow::response(404, "No more single choice questions found! You have selected all the single choice questions.");
			}
		}
		);

	auto& getAmountOfMultipleQuestions = CROW_ROUTE(app, "/getAmountOfMultipleQuestions").methods(crow::HTTPMethod::POST);
	getAmountOfMultipleQuestions(UserDatabaseControl(userDatabase));
	
	app.port(18080).multithreaded().run();

	return 0;
}