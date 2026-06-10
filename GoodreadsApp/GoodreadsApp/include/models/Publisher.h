#pragma once
#include "User.h"
#include "Author.h"
#include "Book.h"

class Publisher : public User
{
public:
	Publisher(std::string username, std::string password, Date registerDate) : User(username, password, registerDate) {};
	~Publisher();

	std::unique_ptr<User> clone() const override {
		return std::make_unique<Publisher>(*this);
	}
private:
	std::vector<std::string> publishedAuthors;
	std::vector<std::shared_ptr<Book>> publishedBooks;
};