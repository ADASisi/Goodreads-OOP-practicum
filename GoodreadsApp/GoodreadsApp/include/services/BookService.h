#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../models/Book.h"
#include "AuthService.h"

class BookService {
private:
    std::vector<std::unique_ptr<Book>>& booksDB;

public:
	explicit BookService(std::vector<std::unique_ptr<Book>>& books);

	void searchBooks(const std::string& query) const;
	void publishBook(const std::string& title, const std::string& author, const std::string& releaseDate, int pages, const std::vector<std::string>& genres);
	void addSynopsis(const std::string& title, const std::string& synopsis);
    
	void addBook(const std::string& title, const std::string& author, int rating = 0);
	void deleteBook(const std::string& title);

	void createShelf(User* currentUser, const std::string& shelfName);
	void deleteShelf(User* currentUser, const std::string& shelfName);
	void addBookToShelf(User* currentUser, const std::string& title, const std::string& shelfName);
	void removeBookFromShelf(User* currentUser, const std::string& title, const std::string& shelfName);
	void showShelf(const std::string& shelfName, const std::string& targetReader = "") const;
};