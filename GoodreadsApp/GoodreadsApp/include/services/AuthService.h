#pragma once
#include <string>
#include <memory>
#include "../models/User.h"
#include "../models/Reader.h"
#include "../models/Author.h"
#include "../models/Publisher.h"

class AuthService {
private:
	std::vector<std::unique_ptr<User>>& usersDB;
	User* currentUser;

public:
	explicit AuthService(std::vector<std::unique_ptr<User>>& usersDB);

	bool registerUser(const std::string& username, const std::string& password, const std::string& type);
	bool login(const std::string& username, const std::string& password);
	void logout();

};