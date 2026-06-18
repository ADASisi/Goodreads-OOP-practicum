#pragma once
#include "Command.h"
#include "../service/AuthService.h"
#include "../service/BookService.h"

class AddToShelf : public Command
{
private:
	AuthService& authService;
	BookService& bookService;

public:
	AddToShelf(AuthService& authService, BookService& bookService);
	void execute(const std::vector<std::string>& args) override;
};

