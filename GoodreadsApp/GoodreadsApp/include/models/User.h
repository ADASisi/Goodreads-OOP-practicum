#pragma once
#include<string>
#include<vector>
#include "Date.h"

class User
{
public:
	User(std::string username, std::string password, Date reg);
	virtual ~User() {}

	virtual std::string getType() const = 0;

private:
	std::string username;
	std::string password;
	Date registrationDate;
	std::vector<std::weak_ptr<User>> followers;
};