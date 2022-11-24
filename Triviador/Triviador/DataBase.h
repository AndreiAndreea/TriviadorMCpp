#pragma once
#include <string>
#include <vector>

#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>

#include "Utils.h"
#include "Player.h"

class DataBase
{
public:
	DataBase(std::string IP);

private:
	std::string m_IP; 
};