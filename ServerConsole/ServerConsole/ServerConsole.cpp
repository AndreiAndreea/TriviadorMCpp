#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <ctime>
#include <regex>
#include <tuple>

#include <random>
#include <algorithm>
#include <iterator>

#include "Database.h"

using namespace sqlite_orm;

uint8_t amountOfQuestions = 5;

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
	std::random_device rd;
	std::mt19937 g(rd());

	std::vector<Room> m_savedRooms;

	int indexVectorSingleChoiceQuestions, indexVectorMultipleChoiceQuestions;

	std::string dataFilesPath = "resources/data_files";

	std::string filePathForSingleChoiceQuestion = dataFilesPath + "/questions/SingleChoiceQuestions.txt";
	std::string filePathForMultipleChoiceQuestion = dataFilesPath + "/questions/MultipleChoiceQuestions.txt";

	std::string filePathForDatabase = dataFilesPath + "/database/triviador.sqlite";

	Storage storage = createStorage(filePathForDatabase);
	storage.sync_schema();

	std::vector<QuestionSingleChoice> allSingleChoiceQuestions = storage.get_all<QuestionSingleChoice>();
	std::shuffle(allSingleChoiceQuestions.begin(), allSingleChoiceQuestions.end(), g);

	std::vector<QuestionMultipleChoice> allMultipleChoiceQuestions = storage.get_all<QuestionMultipleChoice>();
	std::shuffle(allMultipleChoiceQuestions.begin(), allMultipleChoiceQuestions.end(), g);

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

	indexVectorMultipleChoiceQuestions = indexVectorSingleChoiceQuestions = 0;

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
	
	//=======================\\
	//Single choice questions\\
	//=======================\\

	// Get all single choice questions
	CROW_ROUTE(app, "/getSingleQuestions")(
		[&storage]
		()
		{
			std::vector<crow::json::wvalue> vectorOfQuestion;

			for (const auto& question : storage.iterate<QuestionSingleChoice>())
			{
				vectorOfQuestion.push_back(crow::json::wvalue
					{
						{"ID", question.GetID()},
						{"Question", question.GetQuestionText()},
						{"CorrectAnswer", question.GetAnswer()}
					});
			}

			return crow::json::wvalue{ vectorOfQuestion };
		});

	auto& getSingleQuestions = CROW_ROUTE(app, "/getSingleQuestions").methods(crow::HTTPMethod::POST);
	getSingleQuestions(DatabaseStorage(storage));

	// Get the same single choice question - needed to sync generated questions for players
	CROW_ROUTE(app, "/getASingleChoiceQuestion")(
		[&storage, &allSingleChoiceQuestions, &indexVectorSingleChoiceQuestions]
		()
		{
			auto question = allSingleChoiceQuestions[indexVectorSingleChoiceQuestions];
			
			return crow::response(crow::json::wvalue
				{
					{"question", question.GetQuestionText()},
					{"correctAnswer", question.GetAnswer()}
				});
		});

	auto& getASingleChoiceQuestion = CROW_ROUTE(app, "/getASingleChoiceQuestion").methods(crow::HTTPMethod::POST);
	getASingleChoiceQuestion(DatabaseStorage(storage));

	//=========================\\
	//Multiple choice questions\\
	//=========================\\

	// Get all multiple choice questions
	CROW_ROUTE(app, "/getMultipleQuestions")(
		[&storage]
		()
		{
			std::vector<crow::json::wvalue> vectorOfQuestions;

			for (const auto& question : storage.iterate<QuestionMultipleChoice>())
			{
				vectorOfQuestions.push_back(crow::json::wvalue
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

			return crow::json::wvalue{ vectorOfQuestions };
		});

	auto& getMultipleQuestions = CROW_ROUTE(app, "/getMultipleQuestions").methods(crow::HTTPMethod::POST);
	getMultipleQuestions(DatabaseStorage(storage));

	// Get the same multiple choice question - needed to sync generated questions for players
	CROW_ROUTE(app, "/getAMultipleChoiceQuestion")(
		[&storage, &allMultipleChoiceQuestions, &indexVectorMultipleChoiceQuestions]
		()
		{
			auto question = allMultipleChoiceQuestions[indexVectorMultipleChoiceQuestions];

			return crow::response(crow::json::wvalue
				{
					{"question", question.GetQuestionText()},
					{"correctAnswer", question.GetCorrectAnswer()},
					{"answer1", question.GetAnswer1()},
					{"answer2", question.GetAnswer2()},
					{"answer3", question.GetAnswer3()},
					{"answer4", question.GetAnswer4()}
				});
		});

	auto& getAMultipleChoiceQuestion = CROW_ROUTE(app, "/getAMultipleChoiceQuestion").methods(crow::HTTPMethod::POST);
	getAMultipleChoiceQuestion(DatabaseStorage(storage));

	// Create a route that will return a random question from the database (it can be single or multiple choice

	/*
	=================
	=  ROOM ROUTES  =
	=================
	*/

	// Get all lobbies details
	CROW_ROUTE(app, "/getAlllobbiesDetails/")(
		[&storage]
		(const crow::request& req)
		{
			std::vector<crow::json::wvalue> lobbies;

			for (const auto& room : storage.iterate<Room>())
			{
				lobbies.push_back(crow::json::wvalue
					{
						{"roomID", room.GetRoomID()},
						{"gameType", room.GetGameType()},
						{"gameStatus", room.GetGameStatus()},
						{"roomNumber", room.GetRoomNumber()},
						{"currentNumberOfPlayers", room.GetCurrentNumberOfPlayers()},
						{"maximNumberOfPlayers", room.GetMaximNumberOfPlayers()},
						{"numberOfReadyPlayers", room.GetNumberOfReadyPlayers()},
						{"player1", room.GetPlayer1()},
						{"player2", room.GetPlayer2()},
						{"player3", room.GetPlayer3()},
						{"player4", room.GetPlayer4()},
						{"player5", room.GetPlayer5()},
						{"player6", room.GetPlayer6()}
					});
			}

			return crow::response(crow::json::wvalue{ lobbies });
		});

	auto& getAllLobbiesDetails = CROW_ROUTE(app, "/getAlllobbiesDetails").methods(crow::HTTPMethod::POST);
	getAllLobbiesDetails(DatabaseStorage(storage));

	// Get the first available room for the player by game type
	CROW_ROUTE(app, "/getAvailableRoom/")(
		[&storage]
		(const crow::request& req)
		{
			std::string game_type = req.url_params.get("gameType");

			auto foundRoom = storage.get_all<Room>(where(c(&Room::GetGameType) == game_type and (c(&Room::GetGameStatus) == EnumGameStatusToString(ROOM_CREATED) or c(&Room::GetGameStatus) == EnumGameStatusToString(WAITING_FOR_PLAYERS))));

			if (foundRoom.size() == 0)
				return crow::response(404, "No available room found!");
			else
			{
				bool found = false;

				for (auto& room : foundRoom)
				{
					if (room.GetCurrentNumberOfPlayers() < room.GetMaximNumberOfPlayers())
					{
						found = true;
						return crow::response(crow::json::wvalue
							{
								{"roomID", room.GetRoomID()},
								{"gameType", room.GetGameType()},
								{"gameStatus", room.GetGameStatus()},
								{"roomNumber", room.GetRoomNumber()},
								{"currentNumberOfPlayers", room.GetCurrentNumberOfPlayers()},
								{"maximNumberOfPlayers", room.GetMaximNumberOfPlayers()},
								{"numberOfReadyPlayers", room.GetNumberOfReadyPlayers()},
								{"player1", room.GetPlayer1()},
								{"player2", room.GetPlayer2()},
								{"player3", room.GetPlayer3()},
								{"player4", room.GetPlayer4()},
								{"player5", room.GetPlayer5()},
								{"player6", room.GetPlayer6()}
							});
					}
					else
						continue;
				}

				if (!found)
					return crow::response(404, "No available room found!");
			}
		});

	auto& getAvailableRoom = CROW_ROUTE(app, "/getAvailableRoom").methods(crow::HTTPMethod::POST);
	getAvailableRoom(DatabaseStorage(storage));

	// Get a specific room details
	CROW_ROUTE(app, "/getRoomDetails/")(
		[&storage]
		(const crow::request& req)
		{
			std::string roomID = req.url_params.get("roomID");

			auto getRoom = storage.get<Room>(roomID);

			crow::json::wvalue roomDetails = crow::json::wvalue
			{
				{"roomID", getRoom.GetRoomID()},
				{"gameType", getRoom.GetGameType()},
				{"gameStatus", getRoom.GetGameStatus()},
				{"roomNumber", getRoom.GetRoomNumber()},
				{"currentNumberOfPlayers", getRoom.GetCurrentNumberOfPlayers()},
				{"maximNumberOfPlayers", getRoom.GetMaximNumberOfPlayers()},
				{"numberOfReadyPlayers", getRoom.GetNumberOfReadyPlayers()},
				{"player1", getRoom.GetPlayer1()},
				{"player2", getRoom.GetPlayer2()},
				{"player3", getRoom.GetPlayer3()},
				{"player4", getRoom.GetPlayer4()},
				{"player5", getRoom.GetPlayer5()},
				{"player6", getRoom.GetPlayer6()}
			};

			return crow::response(crow::json::wvalue{ roomDetails });
		});

	auto& getRoomDetails = CROW_ROUTE(app, "/getRoomDetails").methods(crow::HTTPMethod::POST);
	getRoomDetails(DatabaseStorage(storage));

	// Get user's lats 5 matches history
	CROW_ROUTE(app, "/getMatchHistory/")(
		[&storage]
		(const crow::request& req)
		{
			std::string username = req.url_params.get("username");
			std::vector<crow::json::wvalue> matchHistoryJson;

			if (username.empty() == false)
			{
				auto foundMatches = storage.get_all<Room>(where(
					c(&Room::GetPlayer1) == username or
					c(&Room::GetPlayer2) == username or
					c(&Room::GetPlayer3) == username or
					c(&Room::GetPlayer4) == username or
					c(&Room::GetPlayer5) == username or
					c(&Room::GetPlayer6) == username));

				// keep the last 5 matches, if there are more than 5
				if (foundMatches.size() > 5)
					foundMatches.erase(foundMatches.begin(), foundMatches.begin() + foundMatches.size() - 5);

				for (auto& findMatchInDatabase : foundMatches)
				{
					matchHistoryJson.push_back(crow::json::wvalue
						{
							{"room_number", findMatchInDatabase.GetRoomNumber()},
							{ "winner", findMatchInDatabase.GetWinner() },
							{ "number_of_players", findMatchInDatabase.GetCurrentNumberOfPlayers() },
							{ "player1", findMatchInDatabase.GetPlayer1() },
							{ "player2", findMatchInDatabase.GetPlayer2() },
							{ "player3", findMatchInDatabase.GetPlayer3() },
							{ "player4", findMatchInDatabase.GetPlayer4() },
							{ "player5", findMatchInDatabase.GetPlayer5() },
							{ "player6", findMatchInDatabase.GetPlayer6() }
						});
				}
				
				crow::json::wvalue final = std::move(matchHistoryJson);

				return crow::response(std::move(final));
			}
			else
			{
				return crow::response(404, "Username not found!");
			}
		});

	auto& getMatchHistory = CROW_ROUTE(app, "/getMatchHistory").methods(crow::HTTPMethod::POST);
	getMatchHistory(DatabaseStorage(storage)); 
	
	// Get the color used to color territories for a specific player in a specific room
	CROW_ROUTE(app, "/getPlayerColor/")(
		[&storage]
		(const crow::request& req)
		{
			std::string roomID = req.url_params.get("roomID");
		    std::string username = req.url_params.get("username");

			auto getRoom = storage.get<Room>(roomID);
			
			crow::json::wvalue findUserColorInRoom = crow::json::wvalue
			{
				{"color", getRoom.GetColorForPlayer(username)}
			};

			if (getRoom.GetColorForPlayer(username) == "Player not found!")
				return crow::response(404, "Player not found in room!");
			else
				return crow::response(crow::json::wvalue{ findUserColorInRoom });
		});

	auto& getPlayerColor = CROW_ROUTE(app, "/getPlayerColor").methods(crow::HTTPMethod::POST);
	getPlayerColor(DatabaseStorage(storage));
	
	// Get a player's from a specific room response time
	CROW_ROUTE(app, "/getResponseTime/")(
		[&storage]
		(const crow::request& req)
		{
			std::string roomID = req.url_params.get("roomID");
			std::string username = req.url_params.get("username");

			auto room = storage.get<Room>(roomID);

			crow::json::wvalue findUserResponseTimeInRoom = crow::json::wvalue
			{
				{"responseTime", room.GetResponseTime(username)}
			};

			if (room.GetResponseTime(username) == float())
				return crow::response(404, "Player not found in room!");
			else
				return crow::response(crow::json::wvalue{ findUserResponseTimeInRoom });
		});
	
	auto& getResponseTime = CROW_ROUTE(app, "/getResponseTime").methods(crow::HTTPMethod::POST);
	getResponseTime(DatabaseStorage(storage));
	
	// Set a player's from a specific room response time
	CROW_ROUTE(app, "/setAnswerDetails/")(
		[&storage, &m_savedRooms]
		(const crow::request& req)
		{
			std::string roomID = req.url_params.get("roomID");
			std::string username = req.url_params.get("username");
			std::string playerAnswer = req.url_params.get("playerAnswer");
			std::string responseTime = req.url_params.get("responseTime");
			
			auto room = storage.get<Room>(roomID);

			auto it = std::find(m_savedRooms.begin(), m_savedRooms.end(), room);

			if (it == m_savedRooms.end())
			{
				room.SetPlayerAnswer(username, playerAnswer);
				room.SetResponseTime(username, std::stof(responseTime));

				m_savedRooms.push_back(room);
			}
			else
			{
				it->SetPlayerAnswer(username, playerAnswer);
				it->SetResponseTime(username, std::stof(responseTime));
			}

			return crow::response(200, "Response time updated!");
		});

	auto& setAnswerDetails = CROW_ROUTE(app, "/setAnswerDetails").methods(crow::HTTPMethod::POST);
	setAnswerDetails(DatabaseStorage(storage));

	// Get a player's from a specific room response time
	CROW_ROUTE(app, "/getAnswerDetails/")(
		[&storage, &m_savedRooms]
		(const crow::request& req)
		{
			std::string roomID = req.url_params.get("roomID");

			auto room = storage.get<Room>(roomID);

			auto foundRoom = std::find(m_savedRooms.begin(), m_savedRooms.end(), room);

			std::vector<crow::json::wvalue> playerDataVector;

			if (foundRoom != m_savedRooms.end())
			{
				for (auto& player : (*foundRoom).GetPlayersInfo())
				{
					playerDataVector.push_back(crow::json::wvalue
						{
							{"username", std::get<0>(player)},
							{"color", std::get<1>(player)},
							{"playerAnswer", std::get<2>(player)},
							{"responseTime", std::get<3>(player)}
						});
				}

				return crow::response(crow::json::wvalue{ playerDataVector });
			}
			else
			{
				return crow::response(404, "Room not found!");
			}
		});

	auto& getAnswerDetails = CROW_ROUTE(app, "/getAnswerDetails").methods(crow::HTTPMethod::POST);
	getAnswerDetails(DatabaseStorage(storage));
	
	// Create a new room by the gameType
	CROW_ROUTE(app, "/createNewRoom/")(
		[&storage]
		(const crow::request& req)
		{
			std::string game_type = req.url_params.get("gameType");

			std::string game_status = EnumGameStatusToString(ROOM_CREATED);

			std::string room_number = GenerateRoomNumber(game_type);


			if (game_type == "2players")
			{
				storage.insert(Room(0, game_type, game_status, room_number, 0, 2, 0, "", "", "", "", "", ""));

				return crow::response(200, "Room for '2players' has been created!");
			}
			else if (game_type == "3players")
			{
				storage.insert(Room(0, game_type, game_status, room_number, 0, 3, 0, "", "", "", "", "", ""));

				return crow::response(200, "Room for '3players' has been created!");
			}
			else if (game_type == "4players")
			{
				storage.insert(Room(0, game_type, game_status, room_number, 0, 4, 0, "", "", "", "", "", ""));

				return crow::response(200, "Room for '4players' has been created!");
			}
			else if (game_type == "customMode")
			{
				storage.insert(Room(0, game_type, game_status, room_number, 0, 6, 0, "", "", "", "", "", ""));

				return crow::response(200, "Room for 'custoMode' has been created!");
			}
			else
			{
				return crow::response(404, "Game type not found!");
			}
		});

	auto& createNewRoom = CROW_ROUTE(app, "/createNewRoom").methods(crow::HTTPMethod::POST);
	createNewRoom(DatabaseStorage(storage));

	// Join room by room ID, firstEmptyPlayerSeatID and playerUsername
	CROW_ROUTE(app, "/joinRoom/")(
		[&storage]
		(const crow::request& req)
		{
			std::string roomID = req.url_params.get("roomID");
			std::string firstEmptyPlayerSeatID = req.url_params.get("firstEmptyPlayerSeatID");
			std::string playerUsername = req.url_params.get("playerUsername");

			try {
				auto room = storage.get<Room>(std::stoi(roomID));

				if (firstEmptyPlayerSeatID.empty() == false && playerUsername.empty() == false)
				{
					if (firstEmptyPlayerSeatID == "1")
						room.SetPlayer1(playerUsername);
					else if (firstEmptyPlayerSeatID == "2")
						room.SetPlayer2(playerUsername);
					else if (firstEmptyPlayerSeatID == "3")
						room.SetPlayer3(playerUsername);
					else if (firstEmptyPlayerSeatID == "4")
						room.SetPlayer4(playerUsername);
					else if (firstEmptyPlayerSeatID == "5")
						room.SetPlayer5(playerUsername);
					else if (firstEmptyPlayerSeatID == "6")
						room.SetPlayer6(playerUsername);

					room.SetCurrentNumberOfPlayers(room.GetCurrentNumberOfPlayers() + 1);

					storage.update(room);

					return crow::response(playerUsername + "joined the room!");
				}
				else
				{
					return crow::response(404, "No data to update!");
				}
			}
			catch (std::system_error e) {
				return crow::response(404, "Room not found!");
			}
		});

	auto& joinRoom = CROW_ROUTE(app, "/joinRoom").methods(crow::HTTPMethod::POST);
	joinRoom(DatabaseStorage(storage));

	// Join room by room ID and firstEmptyPlayerSeatID
	CROW_ROUTE(app, "/leaveRoom/")(
		[&storage]
		(const crow::request& req)
		{
			std::string roomID = req.url_params.get("roomID");
			std::string firstEmptyPlayerSeatID = req.url_params.get("firstEmptyPlayerSeatID");

			try {
				auto room = storage.get<Room>(std::stoi(roomID));

				if (firstEmptyPlayerSeatID.empty() == false)
				{
					if (firstEmptyPlayerSeatID == "1")
						room.SetPlayer1("");
					else if (firstEmptyPlayerSeatID == "2")
						room.SetPlayer2("");
					else if (firstEmptyPlayerSeatID == "3")
						room.SetPlayer3("");
					else if (firstEmptyPlayerSeatID == "4")
						room.SetPlayer4("");
					else if (firstEmptyPlayerSeatID == "5")
						room.SetPlayer5("");
					else if (firstEmptyPlayerSeatID == "6")
						room.SetPlayer6("");

					room.SetCurrentNumberOfPlayers(room.GetCurrentNumberOfPlayers() - 1);

					storage.update(room);

					return crow::response("Player left the room!");
				}
				else
				{
					return crow::response(404, "No data to update!");
				}
			}
			catch (std::system_error e) {
				return crow::response(404, "Room not found!");
			}
		});

	// Get the first empty player seat ID from a room by ID
	CROW_ROUTE(app, "/getFirstEmptyPlayerSeatID/")(
		[&storage]
		(const crow::request& req)
		{
			std::string roomID = req.url_params.get("roomID");

			try {
				auto room = storage.get<Room>(std::stoi(roomID));

				if (room.GetPlayer1().empty() == true)
				{
					return crow::response(200, "1");
				}
				else if (room.GetPlayer2().empty() == true)
				{
					return crow::response(200, "2");
				}
				else if (room.GetPlayer3().empty() == true)
				{
					return crow::response(200, "3");
				}
				else if (room.GetPlayer4().empty() == true)
				{
					return crow::response(200, "4");
				}
				else if (room.GetPlayer5().empty() == true)
				{
					return crow::response(200, "5");
				}
				else if (room.GetPlayer6().empty() == true)
				{
					return crow::response(200, "6");
				}
				else
				{
					return crow::response(401, "Room is full!");
				}
			}
			catch (std::system_error e) {
				return crow::response(404, "Room not found!");
			}
		});

	auto& getFirstEmptyPlayerSeatID = CROW_ROUTE(app, "/getFirstEmptyPlayerSeatID").methods(crow::HTTPMethod::POST);
	getFirstEmptyPlayerSeatID(DatabaseStorage(storage));

	// Leave room for playerUsername by closing the game
	CROW_ROUTE(app, "/leaveRoomForced/")(
		[&storage]
		(const crow::request& req)
		{
			std::string playerUsername = req.url_params.get("playerUsername");

			try
			{
				auto lobbies = storage.get_all<Room>(where(
					c(&Room::GetPlayer1) == playerUsername ||
					c(&Room::GetPlayer2) == playerUsername ||
					c(&Room::GetPlayer3) == playerUsername ||
					c(&Room::GetPlayer4) == playerUsername ||
					c(&Room::GetPlayer5) == playerUsername ||
					c(&Room::GetPlayer6) == playerUsername
					));

				if (lobbies.size())
				{
					auto room = lobbies[0];

					if (room.GetPlayer1() == playerUsername)
						room.SetPlayer1("");
					else if (room.GetPlayer2() == playerUsername)
						room.SetPlayer2("");
					else if (room.GetPlayer3() == playerUsername)
						room.SetPlayer3("");
					else if (room.GetPlayer4() == playerUsername)
						room.SetPlayer4("");
					else if (room.GetPlayer5() == playerUsername)
						room.SetPlayer5("");
					else if (room.GetPlayer6() == playerUsername)
						room.SetPlayer6("");

					room.SetCurrentNumberOfPlayers(room.GetCurrentNumberOfPlayers() - 1);

					if (room.GetNumberOfReadyPlayers() > 0)
						room.SetNumberOfReadyPlayers(room.GetNumberOfReadyPlayers() - 1);

					storage.update(room);

					return crow::response(playerUsername + " left the room by closing the game!");
				}
				else
				{
					return crow::response(405, "No data to update!");
				}
			}
			catch (std::system_error e) {
				return crow::response(404, "Room not found!");
			}
		});

	auto& leaveRoomForced = CROW_ROUTE(app, "/leaveRoomForced").methods(crow::HTTPMethod::POST);
	leaveRoomForced(DatabaseStorage(storage));

	// Route for increasing the number of ready players
	CROW_ROUTE(app, "/increaseNumberOfReadyPlayers/")(
		[&storage]
		(const crow::request& req)
		{
			std::string roomID = req.url_params.get("roomID");

			try {
				auto room = storage.get<Room>(std::stoi(roomID));

				if (room.GetMaximNumberOfPlayers() > room.GetNumberOfReadyPlayers())
				{
					room.SetNumberOfReadyPlayers(room.GetNumberOfReadyPlayers() + 1);

					storage.update(room);

					return crow::response("Number of ready players increased!");
				}
				else
				{
					return crow::response(208, "Maximum number of ready players reached!");
				}
			}
			catch (std::system_error e) {
				return crow::response(404, "Room not found!");
			}
		});

	auto& increaseNumberOfReadyPlayers = CROW_ROUTE(app, "/increaseNumberOfReadyPlayers").methods(crow::HTTPMethod::POST);
	increaseNumberOfReadyPlayers(DatabaseStorage(storage));

	// Route for decreasing the number of ready players
	CROW_ROUTE(app, "/resetNumberOfReadyPlayers/")(
		[&storage]
		(const crow::request& req)
		{
			std::string roomID = req.url_params.get("roomID");

			try {
				auto room = storage.get<Room>(std::stoi(roomID));

				if (room.GetNumberOfReadyPlayers() == 0)
					return crow::response(208, "Number of ready players is already 0!");
				else
				{
					room.SetNumberOfReadyPlayers(0);

					storage.update(room);

					return crow::response("Number of ready players has been reset!");
				}
			}
			catch (std::system_error e) {
				return crow::response(404, "Room not found!");
			}
		});

	auto& resetNumberOfReadyPlayers = CROW_ROUTE(app, "/resetNumberOfReadyPlayers").methods(crow::HTTPMethod::POST);
	resetNumberOfReadyPlayers(DatabaseStorage(storage));

	// Route to see if a room is ready to begin
	CROW_ROUTE(app, "/isRoomReady/")(
		[&storage]
		(const crow::request& req)
		{
			std::string roomID = req.url_params.get("roomID");

			try {
				auto room = storage.get<Room>(std::stoi(roomID));

				if (room.GetCurrentNumberOfPlayers() == room.GetNumberOfReadyPlayers() && room.GetNumberOfReadyPlayers() == room.GetMaximNumberOfPlayers())
				{
					room.SetGameStatus(EnumGameStatusToString(READY));

					storage.update(room);

					return crow::response(200, "Game is ready to begin!");
				}
				else
				{
					room.SetGameStatus(EnumGameStatusToString(WAITING_FOR_PLAYERS));

					storage.update(room);

					return crow::response(200, "Game is not ready to begin!");
				}
			}
			catch (std::system_error e) {
				return crow::response(404, "Room not found!");
			}
		});

	auto& isRoomReady = CROW_ROUTE(app, "/isRoomReady").methods(crow::HTTPMethod::POST);
	isRoomReady(DatabaseStorage(storage));

	app.port(18080).multithreaded().run();

	return 0;
}