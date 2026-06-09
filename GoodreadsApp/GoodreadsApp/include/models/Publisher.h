#pragma once
#include "User.h"
#include "Author.h"
#include "Book.h"

class Publisher : public User
{
public:
	Publisher();
	~Publisher();

private:
	std::vector<std::string> publishedAuthors;
	std::vector<std::shared_ptr<Book>> publishedBooks;
};