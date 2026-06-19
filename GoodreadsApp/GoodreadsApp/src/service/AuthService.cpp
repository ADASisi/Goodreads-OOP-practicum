#include "../../include/utils/HelperFunctions.h"
#include "../../include/service/AuthService.h"
#include <algorithm>

using namespace Helper;

AuthService::AuthService(std::vector<std::unique_ptr<User>>& usersDB)
	:usersDB(usersDB), currentUser(nullptr) {
}

bool AuthService::registerUser(const std::string& username, const std::string& password, TypeUsers type)
{
	auto it = std::find_if(usersDB.begin(), usersDB.end(), [&username](const auto& user) {
		return user->getUsername() == username;
		});
	if (it != usersDB.end()) return false;

	Date registerDate = getTodayDate();

	std::unique_ptr<User> tempUser = nullptr;

	if (type == TypeUsers::Reader)
	{
		tempUser = std::make_unique<Reader>(username, password, registerDate);
	}
	else if (type == TypeUsers::Author) 
	{
		tempUser = std::make_unique<Author>(username, password, registerDate);
	}
	else if (type == TypeUsers::Publisher) 
	{
		tempUser = std::make_unique<Publisher>(username, password, registerDate);
	}
	else 
	{
		return false;
	}

	usersDB.push_back(tempUser->clone());

	return true;
}


bool AuthService::login(const std::string& username, const std::string& password)
{
	if (currentUser) return false;

	auto it = std::find_if(usersDB.begin(), usersDB.end(), [&](const auto& user) {
		return user->getUsername() == username && user->getPassword() == password;
		});

	if (it != usersDB.end()) 
	{
		currentUser = it->get();
		return true;
	}
	return false;
}

void AuthService::logout()
{
	currentUser = nullptr;
}

User* AuthService::getCurrentUser() const
{
	return currentUser;
}

std::vector<std::unique_ptr<User>>& AuthService::getUsersDB()
{
	return usersDB;
}

const std::vector<std::unique_ptr<User>>& AuthService::getUsersDB() const
{
	return usersDB;
}
