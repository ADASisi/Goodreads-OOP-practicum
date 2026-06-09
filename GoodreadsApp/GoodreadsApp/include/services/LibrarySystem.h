#pragma once 
#include <vector>
#include <memory>	
#include<string>

#include "AuthService.h"
#include "BookService.h"
#include "SocialService.h"
#include "FileManager.h"

class LibrarySystem {
private:
	std::vector<std::unique_ptr<User>> allUsers;
	std::vector<std::unique_ptr<Book>> allBooks;
	
	AuthService authService;
	BookService bookService;
	SocialService socialService;
	FileManager fileManager;

public:
	LibrarySystem();
	void run();
	void executeCommand(const std::string& commandLine);

	User* findUser(const std::string& username);
	Book* findBook(const std::string& title);

	void showHelp() const;
};
