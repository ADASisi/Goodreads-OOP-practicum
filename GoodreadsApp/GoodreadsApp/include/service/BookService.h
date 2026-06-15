#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../models/Book.h"
#include "../models/Reader.h"

class User;

class BookService {
private:
	std::vector<std::shared_ptr<Book>>& booksDB;
	int getLevenshteinDistance(const std::string& s1, const std::string& s2) const;

	std::string toLower(std::string str) const;
public:
	explicit BookService(std::vector<std::shared_ptr<Book>>& books);

	void searchBooks(const std::string& query) const;
	void addBookToProfile(Reader* currentReader, const std::string& bookName, int rating = -1);
	void publishBook(const std::string& title, const std::string& author, Date releaseDate, unsigned int pages, const std::vector<Genre>& genres);
	void addSynopsis(const std::string& title, const std::string& synopsis);

	void deleteBookFromProfile(Reader* currentReader, const std::string& bookName);

	void createShelf(Reader* currentUser, const std::string& shelfName);
	void deleteShelf(Reader* currentUser, const std::string& shelfName);
	void addBookToShelf(Reader* currentUser, const std::string& title, const std::string& shelfName);
	void removeBookFromShelf(Reader* currentUser, const std::string& title, const std::string& shelfName);
	void showShelf(Reader* currentReader, Reader* targetReader, const std::string& shelfName) const;

	std::shared_ptr<Book> findBookInDB(const std::string& title) const;
	std::vector<std::shared_ptr<Book>>& getBooksDB();
	const std::vector<std::shared_ptr<Book>>& getBooksDB() const;
};
