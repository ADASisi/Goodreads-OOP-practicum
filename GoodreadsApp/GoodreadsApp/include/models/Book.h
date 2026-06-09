#pragma once
#include <iostream>
#include "../utils/Genre.h"

class Book
{
	public:
	Book() = default;
	Book(const std::string& title, const std::string& author, int year)
		: title(title), author(author), year(year) {};
	std::string getTitle() const;
	std::string getAuthor() const;
	int getYear() const;
private:
	std::string title;
	std::string author;
	std::string publishers;
	std::string synopsis = "No synopsis available.";
	Genre genre;
	double sumRatings = 0.0;
	unsigned int countRatings = 0;
	Date releaseDate;
	unsigned int pageCount = 0;
};
