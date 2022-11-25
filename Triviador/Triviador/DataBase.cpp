#include "DataBase.h"

DataBase::DataBase(Storage& storage)
	: m_db {storage}
{
}

void populateStorage(Storage& storage)
{
	std::vector<User> users = {
		User{ -1, "MilkAna",  "1234AnaLapte", "1","3","1", "2022-08-10"}
	};
	
	storage.insert_range(users.begin(), users.end());
}
