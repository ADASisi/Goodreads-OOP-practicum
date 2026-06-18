#include "../../include/modules/Book.h"

Book::Book(const std::string& title, const std::string& author, Date releaseDate, unsigned int pageCount, std::vector<Genre> genres)
    : title(title), author(author), releaseDate(releaseDate), pageCount(pageCount), genres(genres)
{
}

std::string Book::getTitle() const
{
    return title;
}

std::string Book::getAuthor() const
{
    return author;
}

std::string Book::getPublishingHouse() const
{
    return publishingHouse;
}

unsigned int Book::getCountRatings() const
{
    return countRatings;
}

unsigned int Book::getYear() const
{
    return releaseDate.getYear();
}

double Book::getSumRatings() const
{
    return sumRatings;
}

void Book::setSynopsis(const std::string& synopsis)
{
    this->synopsis = synopsis;
}

void Book::setPublishingHouse(const std::string& publishingHouse)
{
    this->publishingHouse = publishingHouse;
}

void Book::addRating(double rating)
{
    sumRatings += rating;
    countRatings++;
}

unsigned int Book::getPageCount() const
{
    return pageCount;
}
