#include "../../include/services/AuthService.h"
#include "../../include/utils/TypeUsers.h"

explicit AuthService::AuthService(std::vector<std::unique_ptr<User>>& usersDB)
	:usersDB(usersDB), currentUser(nullptr){
}

bool AuthService::registerUser(const std::string& username, const std::string& password, const std::string& type)
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
		tempUser = std::make_unique<Reader>(username, password, currentRegisterDate);
	}
	else if (type == TypeUsers::Author) {
		tempUser = std::make_unique<Author>(username, password, currentRegisterDate);
	}
	else if (type == TypeUsers::Publisher) {
		tempUser = std::make_unique<Publisher>(username, password, currentRegisterDate);
	}
	else {
		return false;
	}
	
	usersDB.push_back(tempUser->clone);

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

void AuthService::logout()
{
	currentUser = nullptr;
}