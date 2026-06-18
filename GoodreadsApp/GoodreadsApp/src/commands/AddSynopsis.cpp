#include "../../include/commands/AddSynopsis.h"
#include "../../include/commands/CommandUtils.h"

AddSynopsis::AddSynopsis(AuthService& a, BookService& b) : authService(a), bookService(b) {}

void AddSynopsis::execute(const std::vector<std::string>& args)
{
	Publisher* publisher = requirePublisher(authService, "Error: Only publishers can add a synopsis.");
	if (!publisher) 
		return;
	if (args.size() < 3) 
	{ 
		std::cout << "Error: Usage: add-synopsis <bookTitle> <synopsis>\n"; 
		return; 
	}
	std::string synopsis = args[2];
	for (size_t i = 3; i < args.size(); ++i) 
		synopsis += " " + args[i];
	bookService.addSynopsis(publisher, args[1], synopsis);
}
