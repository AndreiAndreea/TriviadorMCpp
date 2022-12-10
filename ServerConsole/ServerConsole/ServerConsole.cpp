#include <vector>
#include <string>

#include "User.h"

using namespace sqlite_orm;

#include <vector>
#include <string>

int main()
{
    const std::string dataBase = "triviador.sqlite";

    UsersStorage userDatabase = CreateStorage(dataBase);

    userDatabase.sync_schema();

    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() {
        return "This is an example app of crow and sql-orm";
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
                auto users = userDatabase.get_all<User>(where(c(&User::GetUsername) == username and c(&User::GetPassword) == password));

                if (users.size() == 1)
                {
                    crow::json::wvalue user;
                
				    user["ID"] = users[0].GetID();
				    user["Username"] = users[0].GetUsername();
				    user["Password"] = users[0].GetPassword();
				    user["Email"] = users[0].GetEmail();
				    user["AccountCreationDate"] = users[0].GetAccountCreationDate();
				    user["TotalScore"] = users[0].GetTotalScore();
				    user["PlayedGames"] = users[0].GetPlayedGames();
				    user["WonGames"] = users[0].GetWonGames();
                
                    return crow::json::wvalue{ user } ;
                }
                else
                {
				    return crow::json::wvalue { "Invalid username or password!" };
                    //return crow::response(403);
                }
            }
            else
            {
				return crow::json::wvalue { "Complete all fields and try again!" };
                //return crow::response(401);
            }
        });

    // https://stackoverflow.com/a/630475/12388382
    auto& checkIfUserExistsInDatabase = CROW_ROUTE(app, "/checkuser").methods(crow::HTTPMethod::POST);
    checkIfUserExistsInDatabase(UserDatabaseControl(userDatabase));

    app.port(18080).multithreaded().run();

    return 0;
}