#include "../../include/modules/Author.h"
#include <algorithm>

TypeUsers Author::getType() const
{
	return TypeUsers::Author;
}

std::unique_ptr<User> Author::clone() const {
	return std::make_unique<Author>(*this);
}

void Author::addPublisher(std::string publisherName, bool showMessage)
{
	if (std::find(publishers.begin(), publishers.end(), publisherName) != publishers.end()) {
		if (showMessage) {
			std::cout << "You are already associated with publisher '" << publisherName << "'.\n";
		}
		return;
	}
	publishers.push_back(publisherName);
	if (showMessage) {
		std::cout << "Publisher '" << publisherName << "' added to your profile successfully.\n";
	}
}

void Author::removePublisher(const std::string& publisherName)
{
	publishers.erase(std::remove(publishers.begin(), publishers.end(), publisherName), publishers.end());
}

bool Author::worksWithPublisher(const std::string& publisherName) const
{
	return std::find(publishers.begin(), publishers.end(), publisherName) != publishers.end();
}

void Author::addPublishedBook(std::shared_ptr<Book> book)
{
	if (!book) return;
	for (const auto& publishedBook : publishedBooks) {
		if (publishedBook && publishedBook->getTitle() == book->getTitle()) {
			return;
		}
	}
	publishedBooks.push_back(book);
}
