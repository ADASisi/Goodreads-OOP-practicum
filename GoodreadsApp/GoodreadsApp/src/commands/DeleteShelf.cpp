#include "../../include/commands/DeleteShelf.h"
#include "../../include/commands/CommandUtils.h"

DeleteShelf::DeleteShelf(AuthService& a, BookService& b) : authService(a), bookService(b) {}

void DeleteShelf::execute(const std::vector<std::string>& args) 
{
	if (args.size() != 2) 
	{ 
		std::cout << "Error: Usage: delete-shelf <name>\n"; 
		return; 
	}
	Reader* r = requireReader(authService, "Error: Reader or author login required.");
	if (r) bookService.deleteShelf(r, args[1]);
}
