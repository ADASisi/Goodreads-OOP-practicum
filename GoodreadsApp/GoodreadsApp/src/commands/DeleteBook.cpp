#include "../../include/commands/DeleteBook.h"
#include "../../include/commands/CommandUtils.h"

DeleteBook::DeleteBook(AuthService& a, BookService& b) : authService(a), bookService(b) {}

void DeleteBook::execute(const std::vector<std::string>& args) 
{
	if (args.size() != 2) 
	{ 
		std::cout << "Error: Usage: delete-book <bookName>\n"; 
		return;
	}
	Reader* r = requireReader(authService, "Error: Reader or author login required.");
	if (r) bookService.deleteBookFromProfile(r, args[1]);
}
