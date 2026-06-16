#include "../../include/service/BookService.h"
#include <iostream>
#include <algorithm>
#include <cmath>

BookService::BookService(std::vector<std::shared_ptr<Book>>& books) : booksDB(books) {}

std::string BookService::toLower(std::string str) const
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

int BookService::getLevenshteinDistance(const std::string& s1, const std::string& s2) const
{
    int m = s1.length(), n = s2.length();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));
    for (int i = 0; i <= m; i++) dp[i][0] = i;
    for (int j = 0; j <= n; j++) dp[0][j] = j;

    for (int i = 1; i <= m; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if (s1[i - 1] == s2[j - 1]) dp[i][j] = dp[i - 1][j - 1];
            else dp[i][j] = 1 + std::min({ dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1] });
        }
    }
    return dp[m][n];
}

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


void BookService::addBookToProfile(Reader* currentReader, const std::string& bookName, int rating)
{
    if (!currentReader)
    {
        std::cout << "Error: No user is currently logged in.\n";
        return;
    }

    std::shared_ptr<Book> book = findBookInDB(bookName);

    if (!book)
    {
        std::cout << "Error: Book not found in global database.\n";
        return;
    }

    for (const auto& b : currentReader->getMyBooks())
    {
        if (toLower(b->getTitle()) == toLower(bookName)) {
            std::cout << "Book is already in your profile.\n";
            return;
        }
    }
    currentReader->addBookToProfile(book);

    if (rating != -1)
    {
        book->addRating(rating);
    }

}

void BookService::createShelf(Reader* currentReader, const std::string& shelfName)
{
    if (!currentReader) return;

    if (currentReader->hasShelf(shelfName))
    {
        std::cout << "Shelf '" << shelfName << "' already exists!\n";
        return;
    }
    currentReader->addShelf(std::make_shared<Shelf>(shelfName));
    std::cout << "Shelf '" << shelfName << "' created successfully.\n";
}

void BookService::deleteShelf(Reader* currentReader, const std::string& shelfName)
{
    if (!currentReader) return;

    if (!currentReader->hasShelf(shelfName))
    {
        std::cout << "Error: Shelf '" << shelfName << "' does not exist.\n";
        return;
    }

    currentReader->removeShelf(shelfName);

    std::cout << "Shelf '" << shelfName << "' deleted.\n";
}

void BookService::addBookToShelf(Reader* currentReader, const std::string& title, const std::string& shelfName)
{
    if (!currentReader) return;
    Shelf* shelf = currentReader->findShelf(shelfName);
    if (!shelf)
    {
        std::cout << "Error: Shelf '" << shelfName << "' not found.\n";
        return;
    }

    std::shared_ptr<Book> book = findBookInDB(title);
    if (!book)
    {
        std::cout << "Error: Book '" << title << "' not found.\n";
        return;
    }

    for (const auto& b : shelf->getBooks())
    {
        if (toLower(b->getTitle()) == toLower(title))
        {
            std::cout << "Book is already on this shelf.\n";
            return;
        }
    }

    shelf->addBook(book);
    std::cout << "Book '" << book->getTitle() << "' added to shelf '" << shelfName << "'.\n";
}

void BookService::removeBookFromShelf(Reader* currentReader, const std::string& title, const std::string& shelfName)
{
    if (!currentReader) return;

    Shelf* shelf = currentReader->findShelf(shelfName);
    if (!shelf)
    {
        std::cout << "Error: Shelf '" << shelfName << "' not found.\n";
        return;
    }

    shelf->removeBook(title);
    std::cout << "Removed '" << title << "' from shelf '" << shelfName << "'.\n";
}

void BookService::deleteBookFromProfile(Reader* currentReader, const std::string& bookName)
{
    if (!currentReader) return;

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
    //proverka za priqtelstovo

    if (!readerToSearch)
    {
        std::cout << "Error: No reader specified.\n";
        return;
    }

    Shelf* shelf = readerToSearch->findShelf(shelfName);
    if (!shelf)
    {
        std::cout << "Error: Shelf '" << shelfName << "' not found.\n";
        return;
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

void BookService::publishBook(const std::string& title, const std::string& author, Date releaseDate, unsigned int pages, const std::vector<Genre>& genres)
{
    if (findBookInDB(title) != nullptr)
    {
        std::cout << "Error: A book with title '" << title << "' already exists in the system.\n";
        return;
    }

    auto newBook = std::make_shared<Book>(title, author, releaseDate, pages, genres);
    booksDB.push_back(newBook);

    std::cout << "Successfully published the book '" << title << "' by " << author << "!\n";

    //socialService.notifyNewBookPublished(...))
}

void BookService::addSynopsis(const std::string& title, const std::string& synopsis)
{
    std::shared_ptr<Book> book = findBookInDB(title);
    if (!book)
    {
        std::cout << "Error: Book '" << title << "' not found.\n";
        return;
    }

    book->setSynopsis(synopsis);
    std::cout << "Synopsis for '" << book->getTitle() << "' has been updated.\n";
}
