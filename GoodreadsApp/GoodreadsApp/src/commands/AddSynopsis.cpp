#include "../../include/commands/AddSynopsis.h"
#include "../../include/commands/CommandUtils.h"

AddSynopsis::AddSynopsis(AuthService& a, BookService& b) : authService(a), bookService(b) {}

void AddSynopsis::execute(const std::vector<std::string>& args)
{
	Publisher* publisher = requirePublisher(authService, "Error: Only publishers can add a synopsis.");
	if (args.size() < 3) 
	{ 
		fail("Error: Usage: add-synopsis <bookTitle> <synopsis>");
	}
	if (!isValidBookTitle(args[1]))
	{
		fail("Error: Book title must be 1 to 100 characters long.");
	}
	std::string synopsis = args[2];
	for (size_t i = 3; i < args.size(); ++i) 
		synopsis += " " + args[i];
	if (isBlank(synopsis))
	{
		fail("Error: Synopsis cannot be empty.");
	}
	bookService.addSynopsis(publisher, args[1], synopsis);
}
