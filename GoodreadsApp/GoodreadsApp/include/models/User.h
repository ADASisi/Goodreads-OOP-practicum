#pragma once
#include<iostream>
#include<string>
#include<vector>
#include "Date.h"
#include "../utils/TypeUsers.h"

class User
{
public:
	User(std::string username, std::string password, Date reg);
	virtual ~User() {}

	virtual std::unique_ptr<User> clone() const = 0;

	virtual TypeUsers getType() const = 0;
	std::string getUsername() const;
	std::string getPassword() const;

	void setUsername(const std::string& newUsername);
	void setPassword(const std::string& newPassword);
	void setRegisterDate(Date newDate);
private:
	std::string username;
	std::string password;
	Date registrationDate;
	std::vector<std::weak_ptr<User>> followers;
};