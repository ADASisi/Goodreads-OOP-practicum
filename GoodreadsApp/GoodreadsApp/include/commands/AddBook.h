#pragma once
#include "Command.h"
#include "../service/AuthService.h"
#include "../service/BookService.h"

class AddBook : public Command
{
private:
	AuthService& authService;
	BookService& bookService;

public:
	AddBook(AuthService& authService, BookService& bookService);
	void execute(const std::vector<std::string>& args) override;
};

