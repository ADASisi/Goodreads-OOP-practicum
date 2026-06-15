#include "../../include/models/Publisher.h"
#include <algorithm>

std::unique_ptr<User> Publisher::clone() const {
	return std::make_unique<Publisher>(*this);
}

TypeUsers Publisher::getType() const
{
	return TypeUsers::Publisher;
}

void Publisher::addAuthor(std::string authorName)
{
	publishedAuthors.push_back(authorName);
}

void Publisher::removeAuthor(const std::string& authorName)
{
	publishedAuthors.erase(std::remove(publishedAuthors.begin(), publishedAuthors.end(), authorName), publishedAuthors.end());
}
