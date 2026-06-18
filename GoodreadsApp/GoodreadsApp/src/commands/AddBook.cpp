#include "../../include/commands/AddBook.h"
#include "../../include/commands/CommandUtils.h"

AddBook::AddBook(AuthService& authService, BookService& bookService)
	: authService(authService), bookService(bookService) {}

void AddBook::execute(const std::vector<std::string>& args)
{
	if (args.size() != 3 && args.size() != 4) 
	{ 
		fail("Error: Usage: add-book <bookName> <status> [rating]");
	}
	if (!isValidBookTitle(args[1]))
	{
		fail("Error: Book name must be 1 to 100 characters long.");
	}
	Status status = Status::PlanToRead;
	if (!parseStatus(args[2], status))
	{
		fail("Error: Invalid status.");
	}
	double rating = -1;
	if (args.size() == 4) 
	{
		try 
		{
			size_t parsed = 0;
			rating = std::stod(args[3], &parsed);
			if (parsed != args[3].size() || rating < 0 || rating > 10) {
				fail("Error: Rating must be from 0 to 10.");
			}
		}
		catch (...) 
		{ 
			fail("Error: Rating must be from 0 to 10.");
		}
	}
	Reader* reader = requireReader(authService, "Error: Reader or author login required.");
	bookService.addBookToProfile(reader, args[1], status, rating);
}
