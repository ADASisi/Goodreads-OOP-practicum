#include "../../include/service/AuthService.h"

AuthService::AuthService(std::vector<std::unique_ptr<User>>& usersDB)
	:usersDB(usersDB), currentUser(nullptr) {
}

bool AuthService::registerUser(const std::string& username, const std::string& password, TypeUsers type)
{
	for (const auto& user : usersDB)
	{
		if (user->getUsername() == username)
		{
			return false;
		}
	}

	Date registerDate;

	std::unique_ptr<User> tempUser = nullptr;

	if (type == TypeUsers::Reader) {
		tempUser = std::make_unique<Reader>(username, password, registerDate);
	}
	else if (type == TypeUsers::Author) {
		tempUser = std::make_unique<Author>(username, password, registerDate);
	}
	else if (type == TypeUsers::Publisher) {
		tempUser = std::make_unique<Publisher>(username, password, registerDate);
	}
	else {
		return false;
	}

	usersDB.push_back(tempUser->clone());

	return true;
}


bool AuthService::login(const std::string& username, const std::string& password)
{
	if (currentUser != nullptr)
	{
		return false;
	}

	for (const auto& user : usersDB)
	{
		if (user->getUsername() == username && user->getPassword() == password)
		{
			currentUser = user.get();
			return true;
		}
	}

	return false;
}

bool AuthService::login(const std::string& username)
{
	if (currentUser != nullptr)
	{
		return false;
	}

	for (const auto& user : usersDB)
	{
		if (user->getUsername() == username)
		{
			currentUser = user.get();
			return true;
		}
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
