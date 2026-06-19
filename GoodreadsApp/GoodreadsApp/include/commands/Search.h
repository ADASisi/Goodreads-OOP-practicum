#pragma once
#include "Command.h"
#include "../service/AuthService.h"
#include "../service/BookService.h"
#include "../service/SocialService.h"

class Search : public Command
{
private:
	BookService& bookService;
	SocialService& socialService;

public:
	Search(BookService& bookService, SocialService& socialService);
	void execute(const std::vector<std::string>& args) override;
};
