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

            if (username.empty() == false)
            {
                auto users = userDatabase.get_all<User>(where(c(&User::GetUsername) == username));

                if (users.size() == 1)
                {
                    return crow::response(200);
                }
                else
                {
                    return crow::response(403);
                }
            }
            else
            {
                return crow::response(401);
            }
        });

    // https://stackoverflow.com/a/630475/12388382
    auto& checkIfUserExistsInDatabase = CROW_ROUTE(app, "/checkuser").methods(crow::HTTPMethod::POST);
    checkIfUserExistsInDatabase(UserDatabaseControl(userDatabase));

    app.port(18080).multithreaded().run();

    return 0;
}