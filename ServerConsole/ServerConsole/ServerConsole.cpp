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

int main()
{	
	uint8_t amountOfQuestions = 5;

	std::vector<QuestionSingleChoice> m_randomSingleChoiceQuestionsVector;
	std::vector<bool> m_selectedSingleChoiceQuestions;

	std::vector<QuestionMultipleChoice> m_randomMultipleChoiceQuestionsVector;
	std::vector<bool> m_selectedMultiplChoiceQuestions;

	std::string dataFilesPath = "resources/data_files";

	std::string filePathForSingleChoiceQuestion = dataFilesPath + "/questions/SingleChoiceQuestions.txt";
	std::string filePathForMultipleChoiceQuestion = dataFilesPath + "/questions/MultipleChoiceQuestions.txt";
	
	std::string filePathForDatabase = dataFilesPath + "/database/triviador.sqlite";

	Storage storage = createStorage(filePathForDatabase);
	storage.sync_schema();

	std::ifstream databaseFile(filePathForDatabase);

	if (databaseFile.bad() == false)
	{
		DatabaseStorage database(filePathForSingleChoiceQuestion, filePathForMultipleChoiceQuestion, storage);

		database.Initialize();

		std::cout << "\n";
	}
	
	if(databaseFile.good() == true)
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
						{"currentNumberOfPlayers", lobby.GetCurrentNumberOfPlayers()},
						{"maximNumberOfPlayers", lobby.GetMaximNumberOfPlayers()},
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

	// Check if a lobby 
	CROW_ROUTE(app, "/getAvailableLobby/")(
		[&storage]
		(const crow::request& req)
		{
			std::string game_type = req.url_params.get("gameType");
			
			auto findLobby = storage.get_all<Lobby>(where(c(&Lobby::GetGameType) == game_type and c(&Lobby::GetGameStatus) == "Room created"));

			if(findLobby.size() == 0)
				return crow::response(404, "No available lobby found!");
			else
			{
				crow::json::wvalue firstAvailableLobby = crow::json::wvalue
					{
						{"lobbyID", findLobby[0].GetLobbyID()},
						{"gameType", findLobby[0].GetGameType()},
						{"gameStatus", findLobby[0].GetGameStatus()},
						{"currentNumberOfPlayers", findLobby[0].GetCurrentNumberOfPlayers()},
						{"maximNumberOfPlayers", findLobby[0].GetMaximNumberOfPlayers()},
						{"player1", findLobby[0].GetPlayer1()},
						{"player2", findLobby[0].GetPlayer2()},
						{"player3", findLobby[0].GetPlayer3()},
						{"player4", findLobby[0].GetPlayer4()},
						{"player5", findLobby[0].GetPlayer5()},
						{"player6", findLobby[0].GetPlayer6()}
					};

				return crow::response(crow::json::wvalue{ firstAvailableLobby });
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
				{"currentNumberOfPlayers", lobby.GetCurrentNumberOfPlayers()},
				{"maximNumberOfPlayers", lobby.GetMaximNumberOfPlayers()},
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
			std::string room_number = "RANDOM_GENERATED_NUMBER";

			if (game_type == "2players")
			{
				storage.insert(Lobby(0, game_type, game_status, room_number, 0, 2, "", "", "", "", "", ""));

				return crow::response(200, "Lobby for '2players' has been created!");
			}
			else if (game_type == "3players")
			{
				storage.insert(Lobby(0, game_type, game_status, room_number, 0, 3, "", "", "", "", "", ""));

				return crow::response(200, "Lobby for '3players' has been created!");
			}
			else if (game_type == "4players")
			{
				storage.insert(Lobby(0, game_type, game_status, room_number, 0, 4, "", "", "", "", "", ""));

				return crow::response(200, "Lobby for '4players' has been created!");
			}
			else if (game_type == "customMode")
			{
				storage.insert(Lobby(0, game_type, game_status, room_number, 0, 6, "", "", "", "", "", ""));

				return crow::response(200, "Lobby for 'custoMode' has been created!");
			}
			else
			{
				return crow::response(404, "Game type not found!");
			}
		});

	auto& createNewLobby = CROW_ROUTE(app, "/createNewLobby").methods(crow::HTTPMethod::POST);
	createNewLobby(DatabaseStorage(storage));

	// Update a lobby by ID
	CROW_ROUTE(app, "/updateLobby/")(
		[&storage]
		(const crow::request& req)
		{
			std::string lobbyID = req.url_params.get("lobbyID");
			std::string gameType = req.url_params.get("gameType");
			std::string gameStatus = req.url_params.get("gameStatus");
			std::string roomNumber = req.url_params.get("roomNumber");
			std::string currentNumberOfPlayers = req.url_params.get("currentNumberOfPlayers");
			std::string maximNumberOfPlayers = req.url_params.get("maximNumberOfPlayers");
			std::string player1 = req.url_params.get("player1");
			std::string player2 = req.url_params.get("player2");
			std::string player3 = req.url_params.get("player3");
			std::string player4 = req.url_params.get("player4");
			std::string player5 = req.url_params.get("player5");
			std::string player6 = req.url_params.get("player6");
			
			try {
				auto lobby = storage.get<Lobby>(std::stoi(lobbyID));

				if (gameType.empty() == false)
					lobby.SetGameType(gameType);

				if (gameStatus.empty() == false)
					lobby.SetGameStatus(gameStatus);

				if (roomNumber.empty() == false)
					lobby.SetRoomNumber(roomNumber);

				if (currentNumberOfPlayers.empty() == false)
					lobby.SetCurrentNumberOfPlayers(std::stoi(currentNumberOfPlayers));

				if (maximNumberOfPlayers.empty() == false)
					lobby.SetMaximNumberOfPlayers(std::stoi(maximNumberOfPlayers));

				if (player1.empty() == false)
					lobby.SetPlayer1(player1);

				if (player2.empty() == false)
					lobby.SetPlayer2(player2);

				if (player3.empty() == false)
					lobby.SetPlayer3(player3);

				if (player4.empty() == false)
					lobby.SetPlayer4(player4);

				if (player5.empty() == false)
					lobby.SetPlayer5(player5);

				if (player6.empty() == false)
					lobby.SetPlayer6(player6);

				if (gameType.empty() == false || gameStatus.empty() == false || roomNumber.empty() == false || currentNumberOfPlayers.empty() == false || maximNumberOfPlayers.empty() == false || player1.empty() == false || player2.empty() == false || player3.empty() == false || player4.empty() == false || player5.empty() == false || player6.empty() == false)
				{
					storage.update(lobby);

					return crow::response("Lobby updated!");
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

	auto& updateLobby = CROW_ROUTE(app, "/updateLobby").methods(crow::HTTPMethod::POST);
	updateLobby(DatabaseStorage(storage));

	app.port(18080).multithreaded().run();

	return 0;
}