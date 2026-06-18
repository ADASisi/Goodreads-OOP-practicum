#include "../../include/commands/CommandProcessor.h"
#include "../../include/commands/CommandUtils.h"
#include "../../include/commands/AcceptOffer.h"
#include "../../include/commands/AddBirthday.h"
#include "../../include/commands/AddBook.h"
#include "../../include/commands/AddSynopsis.h"
#include "../../include/commands/AddToShelf.h"
#include "../../include/commands/CreateShelf.h"
#include "../../include/commands/DeleteBook.h"
#include "../../include/commands/DeleteMessage.h"
#include "../../include/commands/DeleteShelf.h"
#include "../../include/commands/Follow.h"
#include "../../include/commands/Followers.h"
#include "../../include/commands/Friends.h"
#include "../../include/commands/Help.h"
#include "../../include/commands/LeavePublisher.h"
#include "../../include/commands/Login.h"
#include "../../include/commands/Logout.h"
#include "../../include/commands/Offer.h"
#include "../../include/commands/Profile.h"
#include "../../include/commands/PublishBook.h"
#include "../../include/commands/ReadMessage.h"
#include "../../include/commands/Register.h"
#include "../../include/commands/RemoveFromShelf.h"
#include "../../include/commands/Search.h"
#include "../../include/commands/ShowInbox.h"
#include "../../include/commands/ShowShelf.h"
#include "../../include/utils/HelperFunctions.h"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <sstream>

static std::vector<std::string> tokenizeCommand(const std::string& line)
{
	std::istringstream stream(line);
	std::vector<std::string> args;
	std::string value;
	while (stream >> value) args.push_back(value);
	return args;
}

CommandProcessor::CommandProcessor(AuthService& auth, BookService& books, SocialService& social)
{
	add("help", std::make_unique<Help>(auth));
	add("register", std::make_unique<Register>(auth));
	add("login", std::make_unique<Login>(auth));
	add("logout", std::make_unique<LogOut>(auth));
	add("search", std::make_unique<Search>(auth, books));
	add("follow", std::make_unique<Follow>(auth, social));
	add("add-book", std::make_unique<AddBook>(auth, books));
	add("create-shelf", std::make_unique<CreateShelf>(auth, books));
	add("delete-shelf", std::make_unique<DeleteShelf>(auth, books));
	add("add-to-shelf", std::make_unique<AddToShelf>(auth, books));
	add("remove-from-shelf", std::make_unique<RemoveFromShelf>(auth, books));
	add("delete-book", std::make_unique<DeleteBook>(auth, books));
	add("show-shelf", std::make_unique<ShowShelf>(auth, books));
	add("show-inbox", std::make_unique<ShowInbox>(auth, social));
	add("read-msg", std::make_unique<ReadMessage>(auth, social));
	add("delete-msg", std::make_unique<DeleteMessage>(auth, social));
	add("friends", std::make_unique<Friends>(auth, social));
	add("add-birthday", std::make_unique<AddBirthday>(auth, social));
	add("profile", std::make_unique<Profile>(auth, social));
	add("publish", std::make_unique<PublishBook>(auth, books, social));
	add("add-synopsis", std::make_unique<AddSynopsis>(auth, books));
	add("offer", std::make_unique<Offer>(auth, social));
	add("accept-offer", std::make_unique<AcceptOffer>(auth, social));
	add("leave", std::make_unique<LeavePublisher>(auth, social));
	add("followers", std::make_unique<Followers>(auth, social));
}

void CommandProcessor::add(const std::string& name, std::unique_ptr<Command> command)
{
	commandMap[name] = command.get();
	commands.push_back(std::move(command));
}

void CommandProcessor::execute(const std::string& line)
{
	std::vector<std::string> args = tokenizeCommand(line);
	if (args.empty()) return;
	try {
		auto it = commandMap.find(toLower(args[0]));
		if (it == commandMap.end()) {
			fail("Error: Unknown command.");
		}
		it->second->execute(args);
	}
	catch (const std::exception& error) {
		std::cout << error.what() << "\n";
	}
}

void CommandProcessor::run()
{
	std::cout << "Goodreads command system started. Type exit to stop.\n";
	std::string line;
	while (std::cout << "\n> " && std::getline(std::cin, line)) {
		std::vector<std::string> args = tokenizeCommand(line);
		if (!args.empty() && toLower(args[0]) == "exit") {
			if (args.size() != 1) {
				try {
					fail("Error: Usage: exit");
				}
				catch (const std::exception& error) {
					std::cout << error.what() << "\n";
				}
				continue;
			}
			std::cout << "Exiting.\n";
			break;
		}
		execute(line);
		
	}
}
