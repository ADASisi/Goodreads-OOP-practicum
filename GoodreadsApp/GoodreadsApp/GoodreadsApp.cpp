#include <fstream>
#include <memory>
#include <vector>

#include "include/commands/CommandProcessor.h"
#include "include/service/FileManager.h"

int main()
{
	std::vector<std::unique_ptr<User>> users;
	std::vector<std::shared_ptr<Book>> books;

	AuthService authService(users);
	BookService bookService(books);
	SocialService socialService(authService);
	FileManager fileManager("goodreads_data.txt");

	std::ifstream database("goodreads_data.txt");
	if (database.good()) {
		fileManager.loadData(authService, bookService, socialService);
	}

	CommandProcessor commandProcessor(authService, bookService, socialService);
	commandProcessor.run();

	fileManager.saveData(authService, bookService, socialService);
	return 0;
}
