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

    app.port(18080).multithreaded().run();

    return 0;
}