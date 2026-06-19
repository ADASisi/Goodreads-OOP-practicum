#include "../../include/commands/DeleteBook.h"
#include "../../include/commands/CommandUtils.h"

using namespace Helper;

DeleteBook::DeleteBook(AuthService& a, BookService& b) : authService(a), bookService(b) {}

void DeleteBook::execute(const std::vector<std::string>& args) 
{
	if (args.size() != 2) 
	{ 
		fail("Error: Usage: delete-book <bookName>");
	}
	if (!isValidBookTitle(args[1]))
	{
		fail("Error: Book name must be 1 to 100 characters long.");
	}
	Reader* r = requireReader(authService, "Error: Reader or author login required.");
	bookService.deleteBookFromProfile(r, args[1]);
}
