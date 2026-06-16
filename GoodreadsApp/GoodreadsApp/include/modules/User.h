#pragma once
#include<iostream>
#include<string>
#include<vector>
#include <memory>
#include "Date.h"

enum class TypeUsers
{
	Reader,
	Author,
	Publisher
};

class User
{
public:
	User(std::string username, std::string password, Date reg);
	//virtual ~User() = default;

	virtual std::unique_ptr<User> clone() const = 0;

	virtual TypeUsers getType() const = 0;
	std::string getUsername() const;
	std::string getPassword() const;
	Date getRegistrationDate() const;
	const std::vector<User*>& getFollowers() const;

	void addFollower(User* user);

	void removeFollowing(User* user);
	bool isFollowing(User* user);
	void addFollowing(User* user);

	void setUsername(const std::string& newUsername);
	void setPassword(const std::string& newPassword);
	void setRegisterDate(Date newDate);

private:
	std::string username;
	std::string password;
	Date registrationDate;
	std::vector<User*> followers;
	std::vector<User*> following;
};
