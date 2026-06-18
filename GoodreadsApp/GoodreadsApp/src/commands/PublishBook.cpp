#include "../../include/commands/PublishBook.h"
#include "../../include/commands/CommandUtils.h"
#include "../../include/utils/HelperFunctions.h"

PublishBook::PublishBook(AuthService& a, BookService& b, SocialService& s)
	: authService(a), bookService(b), socialService(s) {}

void PublishBook::execute(const std::vector<std::string>& args)
{
	Publisher* publisher = requirePublisher(authService, "Error: Only publishers can publish books.");
	if (args.size() < 6) 
	{
		fail("Error: Usage: publish <bookTitle> <authorName> <releaseDate> <pageCount> <genres...>");
	}
	if (!isValidBookTitle(args[1]))
	{
		fail("Error: Book title must be 1 to 100 characters long.");
	}
	Author* author = dynamic_cast<Author*>(findUser(authService, args[2]));
	if (!author) 
	{
		fail("Error: Author '" + args[2] + "' not found.");
	}
	if (!author->worksWithPublisher(publisher->getUsername()))
	{
		fail("Error: Author '" + author->getUsername() +
			"' does not work with publisher '" + publisher->getUsername() + "'.");
	}

	Date date;
	if (!parseDate(args[3], date)) 
	{ 
		fail("Error: Invalid release date.");
	}
	if (isFutureDate(date))
	{
		fail("Error: Release date cannot be in the future.");
	}
	unsigned int pages = 0;
	if (!parsePositivePageCount(args[4], pages))
	{
		fail("Error: Page count must be a positive number up to 10000.");
	}

	std::vector<Genre> genres;
	for (size_t i = 5; i < args.size(); ++i) 
	{
		Genre genre;
		if (!parseGenre(args[i], genre)) 
		{ 
			fail("Error: Unknown genre '" + args[i] + "'.");
		}
		genres.push_back(genre);
	}
	if (bookService.findBookInDB(args[1])) {
		fail("Error: A book with title '" + args[1] + "' already exists in the system.");
	}
	bookService.publishBook(publisher, args[1], args[2], date, pages, genres);
	socialService.notifyNewBookPublished(publisher, author, args[1]);
}
