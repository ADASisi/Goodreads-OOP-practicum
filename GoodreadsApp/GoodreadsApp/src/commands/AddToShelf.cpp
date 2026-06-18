#include "../../include/commands/AddToShelf.h"
#include "../../include/commands/CommandUtils.h"

AddToShelf::AddToShelf(AuthService& a, BookService& b) : authService(a), bookService(b) {}

void AddToShelf::execute(const std::vector<std::string>& args) 
{
	if (args.size() != 3) 
	{ 
		std::cout << "Error: Usage: add-to-shelf <bookName> <shelfName>\n"; 
		return; 
	}
	Reader* r = requireReader(authService, "Error: Reader or author login required.");
	if (r) bookService.addBookToShelf(r, args[1], args[2]);
}
