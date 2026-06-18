#include "../../include/commands/RemoveFromShelf.h"
#include "../../include/commands/CommandUtils.h"

RemoveFromShelf::RemoveFromShelf(AuthService& a, BookService& b) : authService(a), bookService(b) {}

void RemoveFromShelf::execute(const std::vector<std::string>& args) 
{
	if (args.size() != 3) 
	{ 
		std::cout << "Error: Usage: remove-from-shelf <bookName> <shelfName>\n"; 
		return; 
	}
	Reader* r = requireReader(authService, "Error: Reader or author login required.");
	if (r) bookService.removeBookFromShelf(r, args[1], args[2]);
}
