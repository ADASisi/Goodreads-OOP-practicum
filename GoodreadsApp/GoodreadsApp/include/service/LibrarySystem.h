#pragma once 
#include <iostream>
#include <vector>

#include "AuthService.h"
#include "BookService.h"
#include "SocialService.h"

class LibrarySystem {
private:
	std::vector<std::unique_ptr<User>> allUsers;
	std::vector<std::shared_ptr<Book>> allBooks;

	AuthService authService;
	SocialService socialService;
	BookService bookService;

public:
	LibrarySystem();
	void run();
	void executeCommand(const std::string& commandLine);

	User* findUser(const std::string& username);
	Book* findBook(const std::string& title);

	void showHelp() const;
};
