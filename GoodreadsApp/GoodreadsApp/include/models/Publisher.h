#pragma once
#include "User.h"
#include "Author.h"
#include "Book.h"

class Publisher : public User
{
public:
	Publisher(std::string username, std::string password, Date registerDate) : User(username, password, registerDate) {};

	std::unique_ptr<User> clone() const override;

	TypeUsers getType() const override;

	void addAuthor(std::string authorName);
	void removeAuthor(const std::string& authorName);

	std::vector<std::string> getPublishedAuthors() const { return publishedAuthors; }
	

private:
	std::vector<std::string> publishedAuthors;
	std::vector<std::shared_ptr<Book>> publishedBooks;
};
