#pragma once
#include "Reader.h"
#include "Book.h"
#include "Message.h"

class Author : public Reader
{
public:
	Author(std::string username, std::string password, Date registerDate) : Reader(username, password, registerDate) {};
	TypeUsers getType() const override;

	std::unique_ptr<User> clone() const override;
	void addPublisher(std::string publisherName);
	void removePublisher(const std::string& publisherName);

	std::vector<std::string> getPublishers() const { return publishers; }

private:
	std::vector<std::shared_ptr<Book>> publishedBooks;
	std::vector<std::string> publishers;
};
