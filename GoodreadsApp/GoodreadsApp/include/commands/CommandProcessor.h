#pragma once
#include "Command.h"
#include "../service/AuthService.h"
#include "../service/BookService.h"
#include "../service/SocialService.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class CommandProcessor
{
public:
	CommandProcessor(AuthService& authService, BookService& bookService, SocialService& socialService);
	void run();
	void execute(const std::string& line);

private:
	std::vector<std::unique_ptr<Command>> commands;
	std::unordered_map<std::string, Command*> commandMap;
	void add(const std::string& name, std::unique_ptr<Command> command);
};
