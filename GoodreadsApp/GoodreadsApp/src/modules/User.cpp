#include "../../include/modules/User.h"
#include <algorithm>

User::User(std::string username, std::string password, Date reg)
	: username(username), password(password), registrationDate(reg)
{
}

std::string User::getUsername() const
{
	return username;
}

std::string User::getPassword() const
{
	return password;
}

Date User::getRegistrationDate() const
{
	return registrationDate;
}

void User::setUsername(const std::string& newUsername)
{
	this->username = newUsername;
}

void User::setPassword(const std::string& newPassword)
{
	this->password = newPassword;
}

void User::setRegisterDate(Date newDate)
{
	this->registrationDate = newDate;
}


const std::vector<User*>& User::getFollowers() const
{
	return followers;
}

void User::addFollower(User* user)
{
	if (!user) return;
	for (const auto& f : followers)
	{
		if (f->getUsername() == user->getUsername()) return;
	}
	followers.push_back(user);
}

bool User::isFollowing(User* user) const
{
	if (!user) return false;
	for (const auto& u : following)
	{
		if (u->getUsername() == user->getUsername())
			return true;
	}
	return false;
}

void User::addFollowing(User* user)
{
	if (!user || isFollowing(user))
		return;
	following.push_back(user);
}
