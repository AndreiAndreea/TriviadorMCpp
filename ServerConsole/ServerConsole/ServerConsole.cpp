#define _CRT_SECURE_NO_WARNINGS
#include <ctime>
#include <vector>
#include <string>
#include <regex>
#include<iostream>

#include "Database.h"

using namespace sqlite_orm;

/*
* What should I use between POST and PUT ? Check out the link below to understand the difference:
*
* https://stackoverflow.com/a/630475/12388382
*/

std::vector<crow::json::wvalue> getASingleChoiceQuestionBatch
	(Storage& storage, uint8_t amountOfQuestions, 
	std::vector<bool>& m_selectedSingleChoiceQuestions, 
	std::vector<QuestionSingleChoice>& m_randomSingleChoiceQuestionsVector)
{
	//initializing the question usage check vector
	auto countSingleQuestions = storage.count<QuestionSingleChoice>();

	if (m_selectedSingleChoiceQuestions.size() != countSingleQuestions)
		m_selectedSingleChoiceQuestions.resize(countSingleQuestions + 1);

	//if there is an available batch to take
	if (m_randomSingleChoiceQuestionsVector.size() <= countSingleQuestions - amountOfQuestions)
	{
		std::vector<crow::json::wvalue> questionJson;

		for (uint8_t index = 0; index < amountOfQuestions; index++)
		{
			uint8_t randomID = rand() % countSingleQuestions + 1;

			while (m_selectedSingleChoiceQuestions[randomID] == true)
			{
				randomID = rand() % countSingleQuestions + 1;
			}

			m_randomSingleChoiceQuestionsVector.push_back(storage.get<QuestionSingleChoice>(randomID));

			auto question = m_randomSingleChoiceQuestionsVector.back();

			questionJson.push_back(crow::json::wvalue
				{
					{"question", question.GetQuestionText()},
					{"correctAnswer", question.GetAnswer()}
				});

			m_selectedSingleChoiceQuestions[randomID] = true;
		}

		return questionJson;
	}
	else
	{
		std::vector<crow::json::wvalue> questionJson;
		return questionJson;
	}
}

std::vector<crow::json::wvalue> getAMultipleChoiceQuestionBatch
	(Storage& storage, uint8_t amountOfQuestions, 
	std::vector<bool>& m_selectedMultiplChoiceQuestions, 
	std::vector<QuestionMultipleChoice>& m_randomMultipleChoiceQuestionsVector)
{
	//initializing the question usage check vector
	auto countMultipleQuestions = storage.count<QuestionMultipleChoice>();

	if (m_selectedMultiplChoiceQuestions.size() != countMultipleQuestions)
		m_selectedMultiplChoiceQuestions.resize(countMultipleQuestions + 1);

	//if there is an available batch to take
	if (m_randomMultipleChoiceQuestionsVector.size() <= countMultipleQuestions - amountOfQuestions)
	{
		std::vector<crow::json::wvalue> questionJson;

		for (uint8_t index = 0; index < amountOfQuestions; index++)
		{
			uint8_t randomID = rand() % countMultipleQuestions + 1;

			while (m_selectedMultiplChoiceQuestions[randomID] == true)
			{
				randomID = rand() % countMultipleQuestions + 1;
			}

			m_randomMultipleChoiceQuestionsVector.push_back(storage.get<QuestionMultipleChoice>(randomID));

			auto question = m_randomMultipleChoiceQuestionsVector.back();

			questionJson.push_back(crow::json::wvalue
				{
					{"question", question.GetQuestionText()},
					{"correctAnswer", question.GetCorrectAnswer()},
					{"answer1", question.GetAnswer1()},
					{"answer2", question.GetAnswer2()},
					{"answer3", question.GetAnswer3()},
					{"answer4", question.GetAnswer4()}
				});

			m_selectedMultiplChoiceQuestions[randomID] = true;
		}

		return questionJson;
	}
	else
	{
		std::vector<crow::json::wvalue> questionJson;
		return questionJson;
	}
}

bool isValidEmail(const std::string& email)
{
	std::regex validEmailPattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");

	return regex_match(email, validEmailPattern);
}

void SetAllOnlineUsersToOfflineStatus(Storage& storage)
{
	auto allUsers = storage.get_all<User>(where(c(&User::GetConnectStatus) == "Online"));

	if (allUsers.size() > 0)
	{
		std::cout << "Found " << allUsers.size() << " online user" << (allUsers.size() > 1 ? "s" : "") << ". Setting " << (allUsers.size() > 1 ? "them" : "it") << " to offline status...\n";

		for (auto& user : allUsers)
		{
			user.SetConnectStatus("Offline");
			storage.update(user);
		}

		std::cout << (allUsers.size() > 1 ? "All" : "The") << " online user" << (allUsers.size() > 1 ? "s are" : " is") << " now offline!\n";
	}
}

std::string GenerateRoomNumber(std::string& game_type)
{
	std::string room_number = "";

	if (std::regex_match(game_type, std::regex(".*\\d+.*")))
	{
		for (const auto& c : game_type)
		{
			if (isdigit(c))
			{
				room_number += c;
			}
		}
		room_number += "P_";
	}
	else
	{
		room_number += "CM_";
	}

	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	std::ostringstream oss;
	oss << std::put_time(&tm, "%m%d%Y%M%H%S");

	room_number += oss.str();

	return room_number;
}

int main()
{
	uint8_t amountOfQuestions = 5;

	std::vector<QuestionSingleChoice> m_randomSingleChoiceQuestionsVector;
	std::vector<bool> m_selectedSingleChoiceQuestions;

	std::vector<QuestionMultipleChoice> m_randomMultipleChoiceQuestionsVector;
	std::vector<bool> m_selectedMultipleChoiceQuestions;

	std::string dataFilesPath = "resources/data_files";

	std::string filePathForSingleChoiceQuestion = dataFilesPath + "/questions/SingleChoiceQuestions.txt";
	std::string filePathForMultipleChoiceQuestion = dataFilesPath + "/questions/MultipleChoiceQuestions.txt";

	std::string filePathForDatabase = dataFilesPath + "/database/triviador.sqlite";

	Storage storage = createStorage(filePathForDatabase);
	storage.sync_schema();

	std::vector<crow::json::wvalue> batchOfSingleQuestions = getASingleChoiceQuestionBatch(storage, amountOfQuestions, m_selectedSingleChoiceQuestions, m_randomSingleChoiceQuestionsVector);
	std::vector<crow::json::wvalue> batchOfMultipleQuestions = getAMultipleChoiceQuestionBatch(storage, amountOfQuestions, m_selectedMultipleChoiceQuestions, m_randomMultipleChoiceQuestionsVector);

	std::ifstream databaseFile(filePathForDatabase);

	if (databaseFile.bad() == false)
	{
		DatabaseStorage database(filePathForSingleChoiceQuestion, filePathForMultipleChoiceQuestion, storage);

		database.Initialize();

		std::cout << "\n";
	}

	if (databaseFile.good() == true)
	{
		std::cout << "Loading the database...\n";

		SetAllOnlineUsersToOfflineStatus(storage);
	}

	std::cout << "\n";

	crow::SimpleApp app;

	/*
	==========================
	=  SERVER STATUS ROUTES  =
	==========================
	*/

	// Check if server is online
	CROW_ROUTE(app, "/")([]() {
		return crow::response(200, "Successfully connected to the server.");
		});

	/*
	=================
	=  USER ROUTES  =
	=================
	*/

	// Get all users
	CROW_ROUTE(app, "/users")(
		[&storage]()
		{
			std::vector<crow::json::wvalue> usersJson;

			for (const auto& user : storage.iterate<User>())
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
						{"WonGames", user.GetWonGames()},
						{"ConnectStatus", user.GetConnectStatus()}
					});
			}

			return crow::json::wvalue{ usersJson };
		});

	// Login user
	CROW_ROUTE(app, "/loginuser/")(
		[&storage]
		(const crow::request& req)
		{
			std::string username = req.url_params.get("username");
			std::string password = req.url_params.get("password");

			if (username.empty() == false && password.empty() == false)
			{
				auto findUserInDatabase = storage.get_all<User>(where(c(&User::GetUsername) == username and c(&User::GetPassword) == password));

				if (findUserInDatabase.size() == 1)
				{
					if (findUserInDatabase[0].GetConnectStatus() == "Offline")
					{
						findUserInDatabase[0].SetConnectStatus("Online");
						storage.update(findUserInDatabase[0]);

						crow::json::wvalue user = crow::json::wvalue
						{
							{"ID", findUserInDatabase[0].GetID()},
							{"Username", findUserInDatabase[0].GetUsername()},
							{"Password", findUserInDatabase[0].GetPassword()},
							{"Email", findUserInDatabase[0].GetEmail()},
							{"AccountCreationDate", findUserInDatabase[0].GetAccountCreationDate()},
							{"TotalScore", findUserInDatabase[0].GetTotalScore()},
							{"PlayedGames", findUserInDatabase[0].GetPlayedGames()},
							{"WonGames", findUserInDatabase[0].GetWonGames()},
							{"ConnectStatus", findUserInDatabase[0].GetConnectStatus()}
						};

						return  crow::response(user);
					}
					else
					{
						return crow::response(409, "User is already logged in.");
					}
				}
				else
				{
					return crow::response(404, "Username and password not found!");
				}
			}
			else
			{
				return crow::response(406, "Not acceptable! Complete all fields and try again!");
			}
		});

	auto& loginUser = CROW_ROUTE(app, "/loginuser").methods(crow::HTTPMethod::POST);
	loginUser(DatabaseStorage(storage));

	// Logout user
	CROW_ROUTE(app, "/logoutuser/")(
		[&storage]
		(const crow::request& req)
		{
			std::string username = req.url_params.get("username");

			auto findUserInDatabase = storage.get_all<User>(where(c(&User::GetUsername) == username));

			if (findUserInDatabase.size() == 1)
			{
				if (findUserInDatabase[0].GetConnectStatus() == "Online")
				{
					findUserInDatabase[0].SetConnectStatus("Offline");
					storage.update(findUserInDatabase[0]);

					return crow::response(200, "User successfully logged out.");
				}
				else
				{
					return crow::response(409, "User is already logged out.");
				}
			}
			else
			{
				return crow::response(404, "User not found!");
			}
		});

	auto& logoutUser = CROW_ROUTE(app, "/logoutuser").methods(crow::HTTPMethod::POST);
	logoutUser(DatabaseStorage(storage));

	// Register user
	CROW_ROUTE(app, "/registeruser/")(
		[&storage]
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
					auto users = storage.get_all<User>(where(c(&User::GetUsername) == username or c(&User::GetEmail) == email));

					uint32_t countUsers = storage.count<User>();

					if (users.size() == 0)
					{
						storage.insert(User(countUsers + 1, username, password, email, accountCreationDate, "0", "0", "0", "Offline"));

						return crow::response(200, "User registered successfully!");
					}
					else
					{
						return crow::response(409, "An account already exists with this username or email!");
					}
				}
				else
				{
					return crow::response(404, "Invalid email!");
				}
			}
			else
			{
				return crow::response(406, "Complete all fields and try again!");
			}
		});

	auto& registerNewPlayer = CROW_ROUTE(app, "/registeruser").methods(crow::HTTPMethod::PUT);
	registerNewPlayer(DatabaseStorage(storage));

	// Update user details
	CROW_ROUTE(app, "/updateuser/")(
		[&storage]
		(const crow::request& req)
		{
			std::string currentUsername = req.url_params.get("current_username");
			std::string newUsername = req.url_params.get("new_username");
			std::string newPassword = req.url_params.get("new_password");
			std::string newEmail = req.url_params.get("new_email");

			auto users = storage.get_all<User>(where(c(&User::GetUsername) == currentUsername));

			if (users.size() == 1)
			{
				auto user = users[0];

				if (newUsername != "")
					user.SetUsername(newUsername);

				if (newPassword != "")
					user.SetPassword(newPassword);

				if (newEmail != "")
					user.SetEmail(newEmail);

				storage.update(user);

				return crow::response(200, "User updated!");
			}
			else
			{
				return crow::response(404, "User not found!");
			}
		});

	auto& updateUser = CROW_ROUTE(app, "/updateuser").methods(crow::HTTPMethod::POST);
	updateUser(DatabaseStorage(storage));

	// Get user details
	CROW_ROUTE(app, "/getuserdata/")(
		[&storage]
		(const crow::request& req)
		{
			std::string username = req.url_params.get("username");

			if (username.empty() == false)
			{
				auto findUserInDatabase = storage.get_all<User>(where(c(&User::GetUsername) == username));

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
						{"WonGames", findUserInDatabase[0].GetWonGames()},
						{"ConnectStatus", findUserInDatabase[0].GetConnectStatus()}
					};

					return  crow::response(user);
				}
				else
				{
					return crow::response(404, "Username not found!");
				}
			}
			else
			{
				return crow::response(406, "Not acceptable! Complete all fields and try again!");
			}
		});


	auto& getUserData = CROW_ROUTE(app, "/getuserdata").methods(crow::HTTPMethod::POST);
	getUserData(DatabaseStorage(storage));

	// Find if the username is already in use
	CROW_ROUTE(app, "/findusernameduplicate/")(
		[&storage]
		(const crow::request& req)
		{
			std::string newUsername = req.url_params.get("new_username");

			auto users = storage.get_all<User>(where(c(&User::GetUsername) == newUsername));

			if (users.size() == 1)
			{
				return crow::response(200, "Username already exists!");
			}
			else
			{
				return crow::response(404, "Username not found!");
			}
		});

	auto& findUsernameDuplicate = CROW_ROUTE(app, "/findusernameduplicate").methods(crow::HTTPMethod::POST);
	findUsernameDuplicate(DatabaseStorage(storage));

	// Find if the password is already in use
	CROW_ROUTE(app, "/findpasswordduplicate/")(
		[&storage]
		(const crow::request& req)
		{
			std::string newPassword = req.url_params.get("new_password");

			auto users = storage.get_all<User>(where(c(&User::GetPassword) == newPassword));

			if (users.size() == 1)
			{
				return crow::response(200, "Password already exists!");
			}
			else
			{
				return crow::response(404, "Password not found!");
			}
		});

	auto& findPasswordDuplicate = CROW_ROUTE(app, "/findpasswordduplicate").methods(crow::HTTPMethod::POST);
	findPasswordDuplicate(DatabaseStorage(storage));

	// Find if the email is already in use
	CROW_ROUTE(app, "/findemailduplicate/")(
		[&storage]
		(const crow::request& req)
			{
				std::string newEmail = req.url_params.get("new_email");

		auto users = storage.get_all<User>(where(c(&User::GetEmail) == newEmail));

		if (users.size() == 1)
		{
			return crow::response(200, "Email already exists!");
		}
		else
		{
			return crow::response(404, "Email not found!");
		}
		});

	auto& findEmailDuplicate = CROW_ROUTE(app, "/findemailduplicate").methods(crow::HTTPMethod::POST);
	findEmailDuplicate(DatabaseStorage(storage));

	/*
	======================
	=  QUESTIONS ROUTES  =
	======================
	*/

	// Get all single choice questions
	CROW_ROUTE(app, "/getSingleQuestions")(
		[&storage]
		()
		{
			std::vector<crow::json::wvalue> usersJson;

			for (const auto& question : storage.iterate<QuestionSingleChoice>())
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
	getSingleQuestions(DatabaseStorage(storage));

	// Get a specific amount of single choice questions 
	// * BUG maybe from json : the ID of questions isn't get correctly from DB *
	/*
	CROW_ROUTE(app, "/getAmountOfSingleQuestions")(
		[&storage, &amountOfQuestions, &m_selectedSingleChoiceQuestions, &m_randomSingleChoiceQuestionsVector]
		()
		{
			auto countSingleQuestions = storage.count<QuestionSingleChoice>();

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

					m_randomSingleChoiceQuestionsVector.push_back(storage.get<QuestionSingleChoice>(randomID));

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
	getAmountOfSingleQuestions(DatabaseStorage(storage));
	*/

	// Get the same single choice question - needed to sync generated questions for players
	CROW_ROUTE(app, "/getASingleChoiceQuestion")(
		[&storage, &batchOfSingleQuestions]
		()
		{
			return crow::response(crow::json::wvalue{ batchOfSingleQuestions[batchOfSingleQuestions.size() - 1] });
		});

	auto& getASingleChoiceQuestion = CROW_ROUTE(app, "/getASingleChoiceQuestion").methods(crow::HTTPMethod::POST);
	getASingleChoiceQuestion(DatabaseStorage(storage));

	CROW_ROUTE(app, "/getRandomSingleQuestion")(
		[&storage, &amountOfQuestions, &m_selectedSingleChoiceQuestions, &m_randomSingleChoiceQuestionsVector, &batchOfSingleQuestions]
		()
		{
			//checking if the batch needs repopulating
			if (batchOfSingleQuestions.size() <= 1)
			batchOfSingleQuestions = getASingleChoiceQuestionBatch(storage, amountOfQuestions, m_selectedSingleChoiceQuestions, m_randomSingleChoiceQuestionsVector);

			//after getting / repopulating the batch of questions we select a random one

			//uint16_t randomPosition = rand() % m_randomSingleChoiceQuestionsVector.size();
			uint16_t randomPosition = rand() % batchOfSingleQuestions.size();
			int index = m_randomSingleChoiceQuestionsVector.size() - randomPosition;

			crow::json::wvalue SCQuestion = (crow::json::wvalue
				{
					//{"question",  m_randomSingleChoiceQuestionsVector.at(randomPosition).GetQuestionText()},
					//{"correctAnswer", m_randomSingleChoiceQuestionsVector.at(randomPosition).GetAnswer()}

					{"question", m_randomSingleChoiceQuestionsVector.at(index).GetQuestionText()},
					{"correctAnswer", m_randomSingleChoiceQuestionsVector.at(index).GetAnswer()}
				});

			m_selectedSingleChoiceQuestions[index] = true;

			batchOfSingleQuestions.erase(batchOfSingleQuestions.begin() + randomPosition);

			return crow::response(crow::json::wvalue{ SCQuestion });
		}
	);

	auto& getRandomSingleQuestion = CROW_ROUTE(app, "/getRandomSingleQuestion").methods(crow::HTTPMethod::POST);
	getRandomSingleQuestion(DatabaseStorage(storage));

	// Get all multiple choice questions
	CROW_ROUTE(app, "/getMultipleQuestions")(
		[&storage]
		()
		{
			std::vector<crow::json::wvalue> usersJson;

			for (const auto& question : storage.iterate<QuestionMultipleChoice>())
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
	getMultipleQuestions(DatabaseStorage(storage));

	// Get a specific amount of multiple choice questions 
	// * BUG maybe from json : the ID of questions isn't get correctly from DB *
	/*
	CROW_ROUTE(app, "/getAmountOfMultipleQuestions")(
		[&storage, &amountOfQuestions, &m_selectedMultiplChoiceQuestions, &m_randomMultipleChoiceQuestionsVector]
		()
		{
			auto countSingleQuestions = storage.count<QuestionSingleChoice>();

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

					m_randomMultipleChoiceQuestionsVector.push_back(storage.get<QuestionMultipleChoice>(randomID));

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
	getAmountOfMultipleQuestions(DatabaseStorage(storage));
	*/

	// Get the same multiple choice question - needed to sync generated questions for players
	CROW_ROUTE(app, "/getAMultipleChoiceQuestion")(
		[&storage, &batchOfMultipleQuestions]
		()
		{
			return crow::response(crow::json::wvalue{ batchOfMultipleQuestions[batchOfMultipleQuestions.size() - 1] });
		});

	auto& getAMultipleChoiceQuestion = CROW_ROUTE(app, "/getAMultipleChoiceQuestion").methods(crow::HTTPMethod::POST);
	getAMultipleChoiceQuestion(DatabaseStorage(storage));

	CROW_ROUTE(app, "/getRandomMultipleQuestion")(
		[&storage, &amountOfQuestions, &m_selectedMultipleChoiceQuestions, &m_randomMultipleChoiceQuestionsVector, &batchOfMultipleQuestions]
		()
		{
			//checking if the batch needs repopulating
			if (batchOfMultipleQuestions.size() <= 1)
			batchOfMultipleQuestions = getAMultipleChoiceQuestionBatch(storage, amountOfQuestions, m_selectedMultipleChoiceQuestions, m_randomMultipleChoiceQuestionsVector);

			//after getting / repopulating the batch of questions we select a random one
			uint16_t randomPosition = rand() % batchOfMultipleQuestions.size();
			int index = m_randomMultipleChoiceQuestionsVector.size() - randomPosition;

			crow::json::wvalue MCQuestion = (crow::json::wvalue
				{
					{"question", m_randomMultipleChoiceQuestionsVector.at(index).GetQuestionText()},
					{"correctAnswer", m_randomMultipleChoiceQuestionsVector.at(index).GetCorrectAnswer()},
					{"answer1", m_randomMultipleChoiceQuestionsVector.at(index).GetAnswer1()},
					{"answer2", m_randomMultipleChoiceQuestionsVector.at(index).GetAnswer2()},
					{"answer3", m_randomMultipleChoiceQuestionsVector.at(index).GetAnswer3()},
					{"answer4", m_randomMultipleChoiceQuestionsVector.at(index).GetAnswer4()}
				});

			m_selectedMultipleChoiceQuestions[index] = true;

			batchOfMultipleQuestions.erase(batchOfMultipleQuestions.begin() + randomPosition);

			return crow::response(crow::json::wvalue{ MCQuestion });
		}
	);

	auto& getRandomMultipleQuestion = CROW_ROUTE(app, "/getRandomMultipleQuestion").methods(crow::HTTPMethod::POST);
	getRandomMultipleQuestion(DatabaseStorage(storage));

	/*
	==================
	=  LOBBY ROUTES  =
	==================
	*/

	// Get all lobbies details
	CROW_ROUTE(app, "/getAlllobbiesDetails/")(
		[&storage]
		(const crow::request& req)
		{
			std::vector<crow::json::wvalue> lobbies;

			for (const auto& lobby : storage.iterate<Lobby>())
			{
				lobbies.push_back(crow::json::wvalue
					{
						{"lobbyID", lobby.GetLobbyID()},
						{"gameType", lobby.GetGameType()},
						{"gameStatus", lobby.GetGameStatus()},
						{"roomNumber", lobby.GetRoomNumber()},
						{"currentNumberOfPlayers", lobby.GetCurrentNumberOfPlayers()},
						{"maximNumberOfPlayers", lobby.GetMaximNumberOfPlayers()},
						{"numberOfReadyPlayers", lobby.GetNumberOfReadyPlayers()},
						{"player1", lobby.GetPlayer1()},
						{"player2", lobby.GetPlayer2()},
						{"player3", lobby.GetPlayer3()},
						{"player4", lobby.GetPlayer4()},
						{"player5", lobby.GetPlayer5()},
						{"player6", lobby.GetPlayer6()}
					});
			}

			return crow::response(crow::json::wvalue{ lobbies });
		});

	auto& getAllLobbiesDetails = CROW_ROUTE(app, "/getAlllobbiesDetails").methods(crow::HTTPMethod::POST);
	getAllLobbiesDetails(DatabaseStorage(storage));

	// Get the first available lobby for the player by game type
	CROW_ROUTE(app, "/getAvailableLobby/")(
		[&storage]
		(const crow::request& req)
		{
			std::string game_type = req.url_params.get("gameType");

			auto findLobby = storage.get_all<Lobby>(where(c(&Lobby::GetGameType) == game_type and (c(&Lobby::GetGameStatus) == "Room created" or c(&Lobby::GetGameStatus) == "Waiting for players")));

			if(findLobby.size() == 0)
				return crow::response(404, "No available lobby found!");
			else
			{
				bool found = false;

				for (auto& lobby : findLobby)
				{
					if (lobby.GetCurrentNumberOfPlayers() < lobby.GetMaximNumberOfPlayers())
					{
						found = true;
						return crow::response(crow::json::wvalue
							{
								{"lobbyID", lobby.GetLobbyID()},
								{"gameType", lobby.GetGameType()},
								{"gameStatus", lobby.GetGameStatus()},
								{"roomNumber", lobby.GetRoomNumber()},
								{"currentNumberOfPlayers", lobby.GetCurrentNumberOfPlayers()},
								{"maximNumberOfPlayers", lobby.GetMaximNumberOfPlayers()},
								{"numberOfReadyPlayers", lobby.GetNumberOfReadyPlayers()},
								{"player1", lobby.GetPlayer1()},
								{"player2", lobby.GetPlayer2()},
								{"player3", lobby.GetPlayer3()},
								{"player4", lobby.GetPlayer4()},
								{"player5", lobby.GetPlayer5()},
								{"player6", lobby.GetPlayer6()}
							});
					}
					else
						continue;
				}
				
				if (!found)
					return crow::response(404, "No available lobby found!");
			}
		});

	auto& getAvailableLobby = CROW_ROUTE(app, "/getAvailableLobby").methods(crow::HTTPMethod::POST);
	getAvailableLobby(DatabaseStorage(storage));

	// Get a specific lobby details
	CROW_ROUTE(app, "/getLobbyDetails/")(
		[&storage]
		(const crow::request& req)
		{
			std::string lobbyID = req.url_params.get("lobbyID");

			auto lobby = storage.get<Lobby>(lobbyID);

			crow::json::wvalue lobbyDetails = crow::json::wvalue
			{
				{"lobbyID", lobby.GetLobbyID()},
				{"gameType", lobby.GetGameType()},
				{"gameStatus", lobby.GetGameStatus()},
				{"roomNumber", lobby.GetRoomNumber()},
				{"currentNumberOfPlayers", lobby.GetCurrentNumberOfPlayers()},
				{"maximNumberOfPlayers", lobby.GetMaximNumberOfPlayers()},
				{"numberOfReadyPlayers", lobby.GetNumberOfReadyPlayers()},
				{"player1", lobby.GetPlayer1()},
				{"player2", lobby.GetPlayer2()},
				{"player3", lobby.GetPlayer3()},
				{"player4", lobby.GetPlayer4()},
				{"player5", lobby.GetPlayer5()},
				{"player6", lobby.GetPlayer6()}
			};

			return crow::response(crow::json::wvalue{ lobbyDetails });
		});

	auto& getLobbyDetails = CROW_ROUTE(app, "/getLobbyDetails").methods(crow::HTTPMethod::POST);
	getLobbyDetails(DatabaseStorage(storage));

	// Create a new lobby by the gameType
	CROW_ROUTE(app, "/createNewLobby/")(
		[&storage]
		(const crow::request& req)
		{
			std::string game_type = req.url_params.get("gameType");

			std::string game_status = "Room created";
			//std::string room_number = "RANDOM_GENERATED_NUMBER";
			std::string room_number = GenerateRoomNumber(game_type);


			if (game_type == "2players")
			{
				storage.insert(Lobby(0, game_type, game_status, room_number, 0, 2, 0, "", "", "", "", "", ""));

				return crow::response(200, "Lobby for '2players' has been created!");
			}
			else if (game_type == "3players")
			{
				storage.insert(Lobby(0, game_type, game_status, room_number, 0, 3, 0, "", "", "", "", "", ""));

				return crow::response(200, "Lobby for '3players' has been created!");
			}
			else if (game_type == "4players")
			{
				storage.insert(Lobby(0, game_type, game_status, room_number, 0, 4, 0, "", "", "", "", "", ""));

				return crow::response(200, "Lobby for '4players' has been created!");
			}
			else if (game_type == "customMode")
			{
				storage.insert(Lobby(0, game_type, game_status, room_number, 0, 6, 0, "", "", "", "", "", ""));

				return crow::response(200, "Lobby for 'custoMode' has been created!");
			}
			else
			{
				return crow::response(404, "Game type not found!");
			}
		});

	auto& createNewLobby = CROW_ROUTE(app, "/createNewLobby").methods(crow::HTTPMethod::POST);
	createNewLobby(DatabaseStorage(storage));

	// Join lobby by lobby ID, firstEmptyPlayerSeatID and playerUsername
	CROW_ROUTE(app, "/joinLobby/")(
		[&storage]
		(const crow::request& req)
		{
			std::string lobbyID = req.url_params.get("lobbyID");
			std::string firstEmptyPlayerSeatID = req.url_params.get("firstEmptyPlayerSeatID");
			std::string playerUsername = req.url_params.get("playerUsername");

			try {
				auto lobby = storage.get<Lobby>(std::stoi(lobbyID));

				if (firstEmptyPlayerSeatID.empty() == false && playerUsername.empty() == false)
				{
					if (firstEmptyPlayerSeatID == "1")
						lobby.SetPlayer1(playerUsername);
					else if (firstEmptyPlayerSeatID == "2")
						lobby.SetPlayer2(playerUsername);
					else if (firstEmptyPlayerSeatID == "3")
						lobby.SetPlayer3(playerUsername);
					else if (firstEmptyPlayerSeatID == "4")
						lobby.SetPlayer4(playerUsername);
					else if (firstEmptyPlayerSeatID == "5")
						lobby.SetPlayer5(playerUsername);
					else if (firstEmptyPlayerSeatID == "6")
						lobby.SetPlayer6(playerUsername);

					lobby.SetCurrentNumberOfPlayers(lobby.GetCurrentNumberOfPlayers() + 1);

					storage.update(lobby);

					return crow::response(playerUsername + "joined the lobby!");
				}
				else
				{
					return crow::response(404, "No data to update!");
				}
			}
			catch (std::system_error e) {
				return crow::response(404, "Lobby not found!");
			}
		});

	auto& joinLobby = CROW_ROUTE(app, "/joinLobby").methods(crow::HTTPMethod::POST);
	joinLobby(DatabaseStorage(storage));

	// Join lobby by lobby ID and firstEmptyPlayerSeatID
	CROW_ROUTE(app, "/leaveLobby/")(
		[&storage]
		(const crow::request& req)
		{
			std::string lobbyID = req.url_params.get("lobbyID");
			std::string firstEmptyPlayerSeatID = req.url_params.get("firstEmptyPlayerSeatID");

			try {
				auto lobby = storage.get<Lobby>(std::stoi(lobbyID));

				if (firstEmptyPlayerSeatID.empty() == false)
				{
					if (firstEmptyPlayerSeatID == "1")
						lobby.SetPlayer1("");
					else if (firstEmptyPlayerSeatID == "2")
						lobby.SetPlayer2("");
					else if (firstEmptyPlayerSeatID == "3")
						lobby.SetPlayer3("");
					else if (firstEmptyPlayerSeatID == "4")
						lobby.SetPlayer4("");
					else if (firstEmptyPlayerSeatID == "5")
						lobby.SetPlayer5("");
					else if (firstEmptyPlayerSeatID == "6")
						lobby.SetPlayer6("");

					lobby.SetCurrentNumberOfPlayers(lobby.GetCurrentNumberOfPlayers() - 1);

					storage.update(lobby);

					return crow::response("Player left the lobby!");
				}
				else
				{
					return crow::response(404, "No data to update!");
				}
			}
			catch (std::system_error e) {
				return crow::response(404, "Lobby not found!");
			}
		});

	// Get the first empty player seat ID from a lobby by ID
	CROW_ROUTE(app, "/getFirstEmptyPlayerSeatID/")(
		[&storage]
		(const crow::request& req)
		{
			std::string lobbyID = req.url_params.get("lobbyID");

			try {
				auto lobby = storage.get<Lobby>(std::stoi(lobbyID));

				if (lobby.GetPlayer1().empty() == true)
				{
					return crow::response(200, "1");
				}
				else if (lobby.GetPlayer2().empty() == true)
				{
					return crow::response(200, "2");
				}
				else if (lobby.GetPlayer3().empty() == true)
				{
					return crow::response(200, "3");
				}
				else if (lobby.GetPlayer4().empty() == true)
				{
					return crow::response(200, "4");
				}
				else if (lobby.GetPlayer5().empty() == true)
				{
					return crow::response(200, "5");
				}
				else if (lobby.GetPlayer6().empty() == true)
				{
					return crow::response(200, "6");
				}
				else
				{
					return crow::response(401, "Lobby is full!");
				}
			}
			catch (std::system_error e) {
				return crow::response(404, "Lobby not found!");
			}
		});

	auto& getFirstEmptyPlayerSeatID = CROW_ROUTE(app, "/getFirstEmptyPlayerSeatID").methods(crow::HTTPMethod::POST);
	getFirstEmptyPlayerSeatID(DatabaseStorage(storage));

	// Leave lobby for playerUsername by closing the game
	CROW_ROUTE(app, "/leaveLobbyForced/")(
		[&storage]
		(const crow::request& req)
		{
			std::string playerUsername = req.url_params.get("playerUsername");

			try
			{
				auto lobbies = storage.get_all<Lobby>(where(
					c(&Lobby::GetPlayer1) == playerUsername ||
					c(&Lobby::GetPlayer2) == playerUsername ||
					c(&Lobby::GetPlayer3) == playerUsername ||
					c(&Lobby::GetPlayer4) == playerUsername ||
					c(&Lobby::GetPlayer5) == playerUsername ||
					c(&Lobby::GetPlayer6) == playerUsername
					));

				if (lobbies.size() == 1)
				{
					auto lobby = lobbies[0];

					if (lobby.GetPlayer1() == playerUsername)
						lobby.SetPlayer1("");
					else if (lobby.GetPlayer2() == playerUsername)
						lobby.SetPlayer2("");
					else if (lobby.GetPlayer3() == playerUsername)
						lobby.SetPlayer3("");
					else if (lobby.GetPlayer4() == playerUsername)
						lobby.SetPlayer4("");
					else if (lobby.GetPlayer5() == playerUsername)
						lobby.SetPlayer5("");
					else if (lobby.GetPlayer6() == playerUsername)
						lobby.SetPlayer6("");

					lobby.SetCurrentNumberOfPlayers(lobby.GetCurrentNumberOfPlayers() - 1);

					if (lobby.GetNumberOfReadyPlayers() > 0)
						lobby.SetNumberOfReadyPlayers(lobby.GetNumberOfReadyPlayers() - 1);

					storage.update(lobby);

					return crow::response(playerUsername + " left the lobby by closing the game!");
				}
				else
				{
					return crow::response(404, "No data to update!");
				}
			}
			catch (std::system_error e) {
				return crow::response(404, "Lobby not found!");
			}
		});

	auto& leaveLobbyForced = CROW_ROUTE(app, "/leaveLobbyForced").methods(crow::HTTPMethod::POST);
	leaveLobbyForced(DatabaseStorage(storage));

	// Route for increasing the number of ready players
	CROW_ROUTE(app, "/increaseNumberOfReadyPlayers/")(
		[&storage]
		(const crow::request& req)
		{
			std::string lobbyID = req.url_params.get("lobbyID");

			try {
				auto lobby = storage.get<Lobby>(std::stoi(lobbyID));

				if (lobby.GetMaximNumberOfPlayers() > lobby.GetNumberOfReadyPlayers())
				{
					lobby.SetNumberOfReadyPlayers(lobby.GetNumberOfReadyPlayers() + 1);

					storage.update(lobby);

					return crow::response("Number of ready players increased!");
				}
				else
				{
					return crow::response(208, "Maximum number of ready players reached!");
				}
			}
			catch (std::system_error e) {
				return crow::response(404, "Lobby not found!");
			}
		});

	auto& increaseNumberOfReadyPlayers = CROW_ROUTE(app, "/increaseNumberOfReadyPlayers").methods(crow::HTTPMethod::POST);
	increaseNumberOfReadyPlayers(DatabaseStorage(storage));

	// Route for decreasing the number of ready players
	CROW_ROUTE(app, "/resetNumberOfReadyPlayers/")(
		[&storage]
		(const crow::request& req)
		{
			std::string lobbyID = req.url_params.get("lobbyID");

			try {
				auto lobby = storage.get<Lobby>(std::stoi(lobbyID));

				if (lobby.GetNumberOfReadyPlayers() == 0)
					return crow::response(208, "Number of ready players is already 0!");
				else
				{
					lobby.SetNumberOfReadyPlayers(0);

					storage.update(lobby);

					return crow::response("Number of ready players has been reset!");
				}
			}
			catch (std::system_error e) {
				return crow::response(404, "Lobby not found!");
			}
		});

	auto& resetNumberOfReadyPlayers = CROW_ROUTE(app, "/resetNumberOfReadyPlayers").methods(crow::HTTPMethod::POST);
	resetNumberOfReadyPlayers(DatabaseStorage(storage));

	// Route to see if a lobby is ready to begin
	CROW_ROUTE(app, "/isLobbyReady/")(
		[&storage]
		(const crow::request& req)
		{
			std::string lobbyID = req.url_params.get("lobbyID");

			try {
				auto lobby = storage.get<Lobby>(std::stoi(lobbyID));

				if (lobby.GetCurrentNumberOfPlayers() == lobby.GetNumberOfReadyPlayers() && lobby.GetNumberOfReadyPlayers() == lobby.GetMaximNumberOfPlayers())
				{
					lobby.SetGameStatus("Ready");

					storage.update(lobby);

					return crow::response(200, "Game is ready to begin!");
				}
				else
				{
					lobby.SetGameStatus("Waiting for players");

					storage.update(lobby);

					return crow::response(200, "Game is not ready to begin!");
				}
			}
			catch (std::system_error e) {
				return crow::response(404, "Lobby not found!");
			}
		});

	auto& isLobbyReady = CROW_ROUTE(app, "/isLobbyReady").methods(crow::HTTPMethod::POST);
	isLobbyReady(DatabaseStorage(storage));

	app.port(18080).multithreaded().run();

	return 0;
}