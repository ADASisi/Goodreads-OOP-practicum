#pragma once
#include <iostream>
#include <string>
#include<vector>
#include "Date.h"
#include "../utils/Genre.h"

class Book
{
public:
	Book(const std::string& title, const std::string& author, Date releaseDate, unsigned int pageCount, std::vector<Genre> genres);

	std::string getTitle() const;
	std::string getAuthor() const;
	std::string getPublishingHouse() const;
	std::string getReleaseDate() const;
	std::string getSynopsis() const;
	std::string getGenres() const;
	unsigned int getCountRatings() const;
	double getSumRatings() const;
	void setSynopsis(const std::string& synopsis);
	void setPublishingHouse(const std::string& publishingHouse);

	void addRating(double rating);
	unsigned int getPageCount() const;

private:
	std::string title;
	std::string author;
	std::string publishingHouse;
	std::string synopsis = "No synopsis available.";
	std::vector<Genre> genres;
	double sumRatings = 0.0;
	unsigned int countRatings = 0;
	Date releaseDate;
	unsigned int pageCount = 0;
};
