#pragma once

#include "User.h"
#include "Book.h"
#include "Shelf.h"
#include "Message.h"
#include <vector>
#include <string>
#include <iostream>
#include <optional>

class Reader : public User
{
public:
	Reader(string username, string password, Date registerDate) : User(username, password, registerDate) {}
	string getType() const override;

private:
	std::vector<std::shared_ptr<Book>> myBooks;
	std::vector<std::unique_ptr<Shelf>> shelves;
	std::vector<std::shared_ptr<Book>> favoriteBooks;
	optional<Date> birthday;
	std::vector<Message> inbox;
};
