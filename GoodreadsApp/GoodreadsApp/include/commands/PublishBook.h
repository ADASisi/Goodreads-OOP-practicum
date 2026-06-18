#pragma once
#include "Command.h"
#include "../service/AuthService.h"
#include "../service/BookService.h"
#include "../service/SocialService.h"

class PublishBook : public Command
{
private:
	AuthService& authService;
	BookService& bookService;
	SocialService& socialService;

public:
	PublishBook(AuthService& authService, BookService& bookService, SocialService& socialService);
	void execute(const std::vector<std::string>& args) override;
};
