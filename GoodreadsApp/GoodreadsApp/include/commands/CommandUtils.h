#pragma once
#include "../service/AuthService.h"
#include "../utils/HelperFunctions.h"
#include "../utils/ServiceExceptions.h"
#include <iostream>

inline Reader* requireReader(AuthService& authService, const std::string& message)
{
	Reader* reader = dynamic_cast<Reader*>(authService.getCurrentUser());
	if (!reader) fail(message);
	return reader;
}

inline Author* requireAuthor(AuthService& authService, const std::string& message)
{
	Author* author = dynamic_cast<Author*>(authService.getCurrentUser());
	if (!author) fail(message);
	return author;
}

inline Publisher* requirePublisher(AuthService& authService, const std::string& message)
{
	Publisher* publisher = dynamic_cast<Publisher*>(authService.getCurrentUser());
	if (!publisher) fail(message);
	return publisher;
}

inline User* findUser(AuthService& authService, const std::string& username)
{
	for (const auto& user : authService.getUsersDB()) {
		if (Helper::toLower(user->getUsername()) == Helper::toLower(username)) return user.get();
	}
	return nullptr;
}

inline bool parseMessageIndex(const std::string& value, int& index)
{
	try 
	{
		size_t parsed = 0;
		index = std::stoi(value, &parsed);
		if (parsed != value.size() || index < 1) {
			fail("Error: Index must be a positive number.");
		}
		return true;
	}
	catch (...) 
	{
		fail("Error: Index must be a positive number.");
		return false;
	}
}
