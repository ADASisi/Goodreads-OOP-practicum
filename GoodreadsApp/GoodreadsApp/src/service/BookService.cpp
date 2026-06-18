#include "../../include/service/BookService.h"
#include "../../include/service/SocialService.h"
#include "../../include/utils/HelperFunctions.h"
#include "../../include/utils/ServiceExceptions.h"
#include <iostream>
#include <algorithm>
#include <cmath>

BookService::BookService(std::vector<std::shared_ptr<Book>>& books) : booksDB(books) {}

std::shared_ptr<Book> BookService::findBookInDB(const std::string& title) const
{
    for (auto& book : booksDB)
    {
        if (toLower(book->getTitle()) == toLower(title))
        {
            return book;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Book>>& BookService::getBooksDB()
{
    return booksDB;
}

const std::vector<std::shared_ptr<Book>>& BookService::getBooksDB() const
{
    return booksDB;
}

void BookService::searchBooks(const std::string& query) const
{
    std::cout << "Books:\n";
    std::string lowerQuery = toLower(query);
    bool found = false;

    for (const auto& book : booksDB)
    {
        std::string lowerTitle = toLower(book->getTitle());

        if (lowerTitle.find(lowerQuery) != std::string::npos || getLevenshteinDistance(lowerQuery, lowerTitle) <= 2)
        {
            double avgRating = 0.0;
            if (book->getCountRatings() > 0)
            {
                avgRating = book->getSumRatings() / book->getCountRatings();
            }

            std::cout << book->getTitle() << " (" << avgRating << ")\n";
            found = true;
        }
    }
    if (!found)
    {
        std::cout << "(No books match the search criteria)\n";
    }
}


void BookService::addBookToProfile(Reader* currentReader, const std::string& bookName, Status status, double rating)
{
    std::shared_ptr<Book> book = findBookInDB(bookName);

    if (!book)
    {
        throw NotFoundException("Error: Book not found in global database.");
    }

    for (const auto& b : currentReader->getMyBooks())
    {
        if (toLower(b->getTitle()) == toLower(bookName)) 
        {
            throw BadRequestException("Error: Book is already in your profile.");
        }
    }
    currentReader->addBookToProfile(book, status);

    if (rating != -1)
    {
        book->addRating(rating);
    }

    std::cout << "Book '" << book->getTitle() << "' added to your profile";
    std::cout << " as " << statusToString(status);
    if (rating != -1)
    {
        std::cout << " with rating " << rating;
    }
    std::cout << ".\n";
}

void BookService::createShelf(Reader* currentReader, const std::string& shelfName)
{
    if (currentReader->hasShelf(shelfName))
    {
        throw BadRequestException("Error: Shelf '" + shelfName + "' already exists.");
    }
    currentReader->addShelf(std::make_shared<Shelf>(shelfName));
    std::cout << "Shelf '" << shelfName << "' created successfully.\n";
}

void BookService::deleteShelf(Reader* currentReader, const std::string& shelfName)
{
    if (!currentReader->hasShelf(shelfName))
    {
        throw NotFoundException("Error: Shelf '" + shelfName + "' does not exist.");
    }

    currentReader->removeShelf(shelfName);

    std::cout << "Shelf '" << shelfName << "' deleted.\n";
}

void BookService::addBookToShelf(Reader* currentReader, const std::string& title, const std::string& shelfName)
{
    Shelf* shelf = currentReader->findShelf(shelfName);
    if (!shelf)
    {
        throw NotFoundException("Error: Shelf '" + shelfName + "' not found.");
    }

    std::shared_ptr<Book> book = findBookInDB(title);
    if (!book)
    {
        throw NotFoundException("Error: Book '" + title + "' not found.");
    }

    for (const auto& b : shelf->getBooks())
    {
        if (toLower(b->getTitle()) == toLower(title))
        {
            throw BadRequestException("Error: Book is already on this shelf.");
        }
    }

    shelf->addBook(book);
    std::cout << "Book '" << book->getTitle() << "' added to shelf '" << shelfName << "'.\n";
}

void BookService::removeBookFromShelf(Reader* currentReader, const std::string& title, const std::string& shelfName)
{
    Shelf* shelf = currentReader->findShelf(shelfName);
    if (!shelf)
    {
        throw NotFoundException("Error: Shelf '" + shelfName + "' not found.");
    }

    shelf->removeBook(title);
    std::cout << "Removed '" << title << "' from shelf '" << shelfName << "'.\n";
}

void BookService::deleteBookFromProfile(Reader* currentReader, const std::string& bookName)
{
    currentReader->removeBookFromProfile(bookName);

    for (const auto& shelf : currentReader->getShelves())
    {
        shelf->removeBook(bookName);
    }

    std::cout << "Book '" << bookName << "' completely removed from your profile and all your shelves.\n";
}

void BookService::showShelf(Reader* currentReader, Reader* targetReader, const std::string& shelfName) const
{
    Reader* readerToSearch = targetReader ? targetReader : currentReader;

    if (!readerToSearch)
    {
        throw BadRequestException("Error: No reader specified.");
    }

    Shelf* shelf = readerToSearch->findShelf(shelfName);
    if (!shelf)
    {
        throw NotFoundException("Error: Shelf '" + shelfName + "' not found.");
    }

    std::cout << "========================================\n";
    std::cout << "Shelf Name: " << shelf->getName() << "\n";
    std::cout << "========================================\n";

    if (shelf->getBooks().empty())
    {
        std::cout << "(This shelf is empty)\n";
        return;
    }

    for (const auto& book : shelf->getBooks())
    {
        double avgRating = 0.0;
        if (book->getCountRatings() > 0)
        {
            avgRating = book->getSumRatings() / book->getCountRatings();
        }
        std::cout << "- " << book->getTitle() << " by " << book->getAuthor()
            << " | Rating: " << avgRating << " | Pages: " << book->getPageCount() << "\n";
    }
}

void BookService::publishBook(Publisher* publisher, const std::string& title, const std::string& author, Date releaseDate, unsigned int pages, const std::vector<Genre>& genres)
{
    if (!publisher)
    {
        throw BadRequestException("Error: Publisher is required.");
    }

    if (findBookInDB(title) != nullptr)
    {
        throw BadRequestException("Error: A book with title '" + title + "' already exists in the system.");
    }

    auto newBook = std::make_shared<Book>(title, author, releaseDate, pages, genres);
    newBook->setPublishingHouse(publisher->getUsername());
    booksDB.push_back(newBook);

    std::cout << "Successfully published the book '" << title << "' by " << author << "!\n";
}

void BookService::addSynopsis(Publisher* publisher, const std::string& title, const std::string& synopsis)
{
    if (!publisher)
    {
        throw BadRequestException("Error: Publisher is required.");
    }

    std::shared_ptr<Book> book = findBookInDB(title);
    if (!book)
    {
        throw NotFoundException("Error: Book '" + title + "' not found.");
    }

    if (toLower(book->getPublishingHouse()) != toLower(publisher->getUsername()))
    {
        throw ForbiddenException("Error: Only the publisher who published this book can add a synopsis.");
    }

    book->setSynopsis(synopsis);
    std::cout << "Synopsis for '" << book->getTitle() << "' has been updated.\n";
}
