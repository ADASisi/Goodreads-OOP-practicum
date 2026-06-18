#include "../../include/commands/CreateShelf.h"
#include "../../include/commands/CommandUtils.h"

CreateShelf::CreateShelf(AuthService& a, BookService& b) : authService(a), bookService(b) {}

void CreateShelf::execute(const std::vector<std::string>& args) 
{
	if (args.size() != 2) 
	{ 
		std::cout << "Error: Usage: create-shelf <name>\n"; 
		return; 
	}
	Reader* r = requireReader(authService, "Error: Reader or author login required.");
	if (r) bookService.createShelf(r, args[1]);
}
