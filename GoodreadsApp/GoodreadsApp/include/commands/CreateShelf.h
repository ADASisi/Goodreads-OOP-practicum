#pragma once
#include "Command.h"
#include "../service/AuthService.h"
#include "../service/BookService.h"

class CreateShelf : public Command
{
private:
	AuthService& authService;
	BookService& bookService;
public:
	CreateShelf(AuthService& authService, BookService& bookService);
	void execute(const std::vector<std::string>& args) override;
};

