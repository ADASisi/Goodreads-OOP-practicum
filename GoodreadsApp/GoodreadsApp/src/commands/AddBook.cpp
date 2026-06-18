#include "../../include/commands/AddBook.h"
#include "../../include/commands/CommandUtils.h"

AddBook::AddBook(AuthService& authService, BookService& bookService)
	: authService(authService), bookService(bookService) {}

void AddBook::execute(const std::vector<std::string>& args)
{
	if (args.size() != 3 && args.size() != 4) 
	{ 
		std::cout << "Error: Usage: add-book <bookName> <status> [rating]\n"; 
		return; 
	}
	std::string status = toLower(args[2]);
	if (status != "plan-to-read" && status != "reading" && status != "paused" && status != "dropped") 
	{
		std::cout << "Error: Invalid status.\n"; 
		return;
	}
	int rating = -1;
	if (args.size() == 4) 
	{
		try 
		{
			size_t parsed = 0;
			rating = std::stoi(args[3], &parsed);
			if (parsed != args[3].size() || rating < 0 || rating > 10) throw std::exception();
		}
		catch (...) 
		{ 
			std::cout << "Error: Rating must be from 0 to 10.\n"; 
			return; 
		}
	}
	Reader* reader = requireReader(authService, "Error: Reader or author login required.");
	if (reader) bookService.addBookToProfile(reader, args[1], rating);
}
