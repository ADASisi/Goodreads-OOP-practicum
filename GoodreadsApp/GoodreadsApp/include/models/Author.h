#pragma once
#include "Reader.h"
#include "Book.h"
#include "Message.h"

class Author : public Reader
{
	public:
	Author(std::string username, std::string password, Date registerDate) : Reader(username, password, registerDate) {}
	std::string getUserType() const override;

private:
	std::vector<std::shared_ptr<Book>> publishedBooks;
	std::vector<std::string> publishers;
};