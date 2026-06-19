#include "../../include/modules/Book.h"
#include "../../include/utils/HelperFunctions.h"

using namespace Helper;

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

std::string Book::getReleaseDate() const
{
    return formatDate(releaseDate);
}

std::string Book::getSynopsis() const
{
    return synopsis;
}

std::string Book::getGenres() const
{
    std::string result;
    for (size_t i = 0; i < genres.size(); i++) {
        if (i > 0) result += " ";
        result += genreToString(genres[i]);
    }
    return result;
}

unsigned int Book::getCountRatings() const
{
    return countRatings;
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
