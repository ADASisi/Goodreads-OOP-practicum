#include "../../include/commands/Search.h"
#include "../../include/commands/CommandUtils.h"

Search::Search(AuthService& a, BookService& b) : authService(a), bookService(b) {}

void Search::execute(const std::vector<std::string>& args)
{
	if (args.size() != 2) 
	{ 
		std::cout << "Error: Usage: search <name>\n"; 
		return; 
	}
	std::string query = toLower(args[1]);
	std::cout << "Users:\n";
	bool found = false;
	for (const auto& user : authService.getUsersDB()) 
	{
		std::string name = toLower(user->getUsername());
		if (name.find(query) != std::string::npos || getLevenshteinDistance(query, name) <= 2)
		{
			const char* type = user->getType() == TypeUsers::Reader ? "Reader" :
				user->getType() == TypeUsers::Author ? "Author" : "Publisher";
			std::cout << user->getUsername() << " (" << type << ")\n";
			found = true;
		}
	}
	if (!found) std::cout << "(No users match the search criteria)\n";
	std::cout << "\n";
	bookService.searchBooks(args[1]);
}
