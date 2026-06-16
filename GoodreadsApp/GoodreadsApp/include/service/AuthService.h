#pragma once
#include <string>
#include <memory>
#include "../modules/User.h"
#include "../modules/Reader.h"
#include "../modules/Author.h"
#include "../modules/Publisher.h"

class AuthService {
private:
	std::vector<std::unique_ptr<User>>& usersDB;
	User* currentUser;

public:
	explicit AuthService(std::vector<std::unique_ptr<User>>& usersDB);

	bool registerUser(const std::string& username, const std::string& password, TypeUsers type);

	bool login(const std::string& username, const std::string& password);
	bool login(const std::string& username);
	void logout();
	User* getCurrentUser() const;
	std::vector<std::unique_ptr<User>>& getUsersDB();
	const std::vector<std::unique_ptr<User>>& getUsersDB() const;
};
