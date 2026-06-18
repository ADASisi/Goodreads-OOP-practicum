#include "../../include/commands/PublishBook.h"
#include "../../include/commands/CommandUtils.h"

PublishBook::PublishBook(AuthService& a, BookService& b, SocialService& s)
	: authService(a), bookService(b), socialService(s) {}

void PublishBook::execute(const std::vector<std::string>& args)
{
	Publisher* publisher = requirePublisher(authService, "Error: Only publishers can publish books.");
	if (!publisher) return;
	if (args.size() < 6) 
	{
		std::cout << "Error: Usage: publish <bookTitle> <authorName> <releaseDate> <pageCount> <genres...>\n";
		return;
	}
	Author* author = dynamic_cast<Author*>(findUser(authService, args[2]));
	if (!author) 
	{
		std::cout << "Error: Author '" << args[2] << "' not found.\n";
		return;
	}

	Date date;
	if (!parseDate(args[3], date)) 
	{ 
		std::cout << "Error: Invalid release date.\n"; 
		return; 
	}
	unsigned int pages = 0;
	try 
	{
		size_t parsed = 0;
		if (!args[4].empty() && args[4][0] == '-') throw std::exception();
		pages = std::stoul(args[4], &parsed);
		if (parsed != args[4].size() || pages == 0) throw std::exception();
	}
	catch (...) 
	{ 
		std::cout << "Error: Page count must be a positive number.\n"; 
		return; 
	}

	std::vector<Genre> genres;
	for (size_t i = 5; i < args.size(); ++i) 
	{
		Genre genre;
		if (!parseGenre(args[i], genre)) 
		{ 
			std::cout << "Error: Unknown genre '" << args[i] << "'.\n"; 
			return; 
		}
		genres.push_back(genre);
	}
	if (bookService.findBookInDB(args[1])) {
		std::cout << "Error: A book with title '" << args[1] << "' already exists in the system.\n";
		return;
	}
	bookService.publishBook(publisher, args[1], args[2], date, pages, genres);
	socialService.notifyNewBookPublished(publisher, author, args[1]);
}
