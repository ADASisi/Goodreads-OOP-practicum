#include "../../include/service/AuthService.h"
<<<<<<< HEAD
#include "../../include/utils/ServiceExceptions.h"
#include "../../include/utils/ValidationUtils.h"
=======
>>>>>>> parent of ab2a32f (add custom exceptions)

AuthService::AuthService(std::vector<std::unique_ptr<User>>& usersDB)
	: usersDB(usersDB), currentUser(nullptr) {
}

bool AuthService::registerUser(const std::string& username, const std::string& password, TypeUsers type)
{
<<<<<<< HEAD
	ValidationUtils::validateCredentials(username, password);

	for (const auto& user : usersDB) {
		if (user->getUsername() == username) return false;
=======
	for (const auto& user : usersDB)
	{
		if (user->getUsername() == username)
		{
			return false;
		}
>>>>>>> parent of ab2a32f (add custom exceptions)
	}

	std::unique_ptr<User> tempUser = nullptr;
	if (type == TypeUsers::Reader)         tempUser = std::make_unique<Reader>(username, password, Date());
	else if (type == TypeUsers::Author)    tempUser = std::make_unique<Author>(username, password, Date());
	else if (type == TypeUsers::Publisher) tempUser = std::make_unique<Publisher>(username, password, Date());
	else return false;

	usersDB.push_back(tempUser->clone());
	return true;
}

bool AuthService::login(const std::string& username, const std::string& password)
{
	if (currentUser) return false; 

	for (const auto& user : usersDB) {
		if (user->getUsername() == username && user->getPassword() == password) {
			currentUser = user.get();
			return true;
		}
	}
	return false;
}

bool AuthService::login(const std::string& username)
{
	if (currentUser) return false;

	for (const auto& user : usersDB) {
		if (user->getUsername() == username) {
			currentUser = user.get();
			return true;
		}
	}
	return false;
}

bool AuthService::logout()
{
	if (!currentUser) return false;
	currentUser = nullptr;
	return true;
}

void AuthService::logoutOrThrow()
{
	if (!logout()) {
		throw UnauthorizedException("Error: No user is currently logged in.");
	}
}

User* AuthService::getCurrentUser() const { return currentUser; }
Reader* AuthService::getCurrentReader() const { return currentUser ? currentUser->asReader() : nullptr; }
Author* AuthService::getCurrentAuthor() const { return currentUser ? currentUser->asAuthor() : nullptr; }
Publisher* AuthService::getCurrentPublisher() const { return currentUser ? currentUser->asPublisher() : nullptr; }

Reader* AuthService::requireCurrentReader(const std::string& errorMessage) const
{
	Reader* reader = getCurrentReader();
	if (!reader) {
		throw ForbiddenException(errorMessage);
	}
	return reader;
}

Publisher* AuthService::requireCurrentPublisher(const std::string& errorMessage) const
{
	Publisher* publisher = getCurrentPublisher();
	if (!publisher) {
		throw ForbiddenException(errorMessage);
	}
	return publisher;
}

std::vector<std::unique_ptr<User>>& AuthService::getUsersDB() { return usersDB; }
const std::vector<std::unique_ptr<User>>& AuthService::getUsersDB() const { return usersDB; }
