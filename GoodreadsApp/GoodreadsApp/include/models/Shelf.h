#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include "Book.h"

class Shelf
{
public:
	Shelf(const std::string& shelfName);
	std::string getName() const;
	const std::vector<std::shared_ptr<Book>>& getBooks() const;

	void addBook(std::shared_ptr<Book> book);
	void removeBook(const std::string& bookTitle);

private:
	std::string name;
	size_t countBooks = 0;
	Date createDate;
	std::vector<std::shared_ptr<Book>> containBooks;

};