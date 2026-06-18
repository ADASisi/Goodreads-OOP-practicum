#pragma once
#include "../service/AuthService.h"
#include "../utils/HelperFunctions.h"
#include <iostream>

inline Reader* requireReader(AuthService& authService, const std::string& message)
{
	Reader* reader = dynamic_cast<Reader*>(authService.getCurrentUser());
	if (!reader) std::cout << message << "\n";
	return reader;
}

inline Author* requireAuthor(AuthService& authService, const std::string& message)
{
	Author* author = dynamic_cast<Author*>(authService.getCurrentUser());
	if (!author) std::cout << message << "\n";
	return author;
}

inline Publisher* requirePublisher(AuthService& authService, const std::string& message)
{
	Publisher* publisher = dynamic_cast<Publisher*>(authService.getCurrentUser());
	if (!publisher) std::cout << message << "\n";
	return publisher;
}

inline User* findUser(AuthService& authService, const std::string& username)
{
	for (const auto& user : authService.getUsersDB()) {
		if (toLower(user->getUsername()) == toLower(username)) return user.get();
	}
	return nullptr;
}

inline bool parseMessageIndex(const std::string& value, int& index)
{
	try 
	{
		size_t parsed = 0;
		index = std::stoi(value, &parsed);
		if (parsed != value.size() || index < 0) throw std::exception();
		return true;
	}
	catch (...) 
	{
		std::cout << "Error: Index must be a non-negative number.\n";
		return false;
	}
}

static bool parseGenre(const std::string& value, Genre& genre)
{
	std::string name = toLower(value);
	if (name == "romance") genre = Genre::Romance;
	else if (name == "fantasy") genre = Genre::Fantasy;
	else if (name == "fiction") genre = Genre::Fiction;
	else if (name == "thriller") genre = Genre::Thriller;
	else if (name == "mystery") genre = Genre::Mystery;
	else if (name == "horror") genre = Genre::Horror;
	else if (name == "drama") genre = Genre::Drama;
	else if (name == "adventure") genre = Genre::Adventure;
	else return false;
	return true;
}