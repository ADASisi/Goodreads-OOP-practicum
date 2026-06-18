 #pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "../modules/Book.h"
#include "../modules/Reader.h"
#include "../modules/Publisher.h"

class BookService {
private:
	std::vector<std::shared_ptr<Book>>& booksDB;
	
public:
	explicit BookService(std::vector<std::shared_ptr<Book>>& books);

	void searchBooks(const std::string& query) const;
	void addBookToProfile(Reader* currentReader, const std::string& bookName, int rating = -1);
	void publishBook(Publisher* publisher, const std::string& title, const std::string& author, Date releaseDate, unsigned int pages, const std::vector<Genre>& genres);
	void addSynopsis(Publisher* publisher, const std::string& title, const std::string& synopsis);

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
