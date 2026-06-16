#include "../../include/modules/Author.h"
#include <algorithm>

TypeUsers Author::getType() const
{
	return TypeUsers::Author;
}

std::unique_ptr<User> Author::clone() const {
	return std::make_unique<Author>(*this);
}

void Author::addPublisher(std::string publisherName)
{
	if (std::find(publishers.begin(), publishers.end(), publisherName) != publishers.end()) {
		std::cout << "You are already associated with publisher '" << publisherName << "'.\n";
		return;
	}
	publishers.push_back(publisherName);
	std::cout << "Publisher '" << publisherName << "' added to your profile successfully.\n";
}

void Author::removePublisher(const std::string& publisherName)
{
	publishers.erase(std::remove(publishers.begin(), publishers.end(), publisherName), publishers.end());
}
