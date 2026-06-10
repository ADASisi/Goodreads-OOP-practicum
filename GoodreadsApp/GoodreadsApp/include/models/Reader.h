#pragma once

#include "User.h"
#include "Book.h"
#include "Shelf.h"
#include "Message.h"
#include "Date.h"
#include "../utils/TypeUsers.h"
#include <vector>
#include <string>
#include <iostream>
#include <optional>

class Reader : public User
{
public:
	Reader(std::string username, std::string password, Date registerDate) : User(username, password, registerDate) {}
	TypeUsers getType() const override;

	std::unique_ptr<User> clone() const override {
		return std::make_unique<Reader>(*this);
	}

private:
	std::vector<std::shared_ptr<Book>> myBooks;
	std::vector<std::unique_ptr<Shelf>> shelves;
	std::vector<std::shared_ptr<Book>> favoriteBooks;
	std::optional<Date> birthday;
	std::vector<Message> inbox;
};
