#include "../../include/service/LibrarySystem.h"
#include "../../include/utils/HelperFunctions.h"
#include "../../include/utils/ValidationExceptions.h"

LibrarySystem::LibrarySystem()
    : authService(allUsers),
    socialService(authService),
    bookService(allBooks, &socialService)
{
}

void LibrarySystem::executeCommand(const std::string& commandLine)
{
    std::vector<std::string> args = tokenize(commandLine);
    if (args.empty()) return;

    std::string command = toLower(args[0]);

    if (command == "help") {
        if (args.size() != 1) {
            std::cout << "Error: Usage: help\n";
            return;
        }
        showHelp();
        return;
    }

    if (command == "register") {
        if (args.size() != 4) {
            std::cout << "Error: Usage: register <username> <password> <reader|author|publisher>\n";
            return;
        }
        TypeUsers type;
        if (!parseUserType(args[3], type)) {
            std::cout << "Error: User type must be reader, author, or publisher.\n";
            return;
        }
        try {
            if (authService.registerUser(args[1], args[2], type)) {
                std::cout << "User registered successfully.\n";
            }
            else {
                std::cout << "Error: Could not register user. Username may already exist.\n";
            }
        }
        catch (const ValidationException& ex) {
            std::cout << "Error: " << ex.what() << "\n";
        }
        return;
    }

    if (command == "login") {
        if (args.size() < 2 || args.size() > 3) {
            std::cout << "Error: Usage: login <username> [password]\n";
            return;
        }
        bool loggedIn = (args.size() == 3)
            ? authService.login(args[1], args[2])
            : authService.login(args[1]);

        if (loggedIn) std::cout << "Login successful.\n";
        else std::cout << "Error: User not found, invalid password, or another user is already logged in.\n";
        return;
    }

    if (command == "logout") {
        if (args.size() != 1) {
            std::cout << "Error: Usage: logout\n";
            return;
        }
        if (!authService.getCurrentUser()) {
            std::cout << "Error: No user is currently logged in.\n";
            return;
        }
        authService.logout();
        std::cout << "Logout successful.\n";
        return;
    }

    if (command == "search") {
        if (args.size() != 2) {
            std::cout << "Error: Usage: search <name>\n";
            return;
        }
        bookService.searchBooks(args[1]);
        return;
    }

    if (command == "follow") {
        if (args.size() != 2) {
            std::cout << "Error: Usage: follow <username>\n";
            return;
        }
        socialService.followUser(args[1]);
        return;
    }

    if (command == "add-book") {
        if (args.size() != 3 && args.size() != 4) {
            std::cout << "Error: Usage: add-book <bookName> <status> [rating]\n";
            return;
        }
        std::string status = toLower(args[2]);
        if (status != "plan-to-read" && status != "reading" && status != "paused" && status != "dropped") {
            std::cout << "Error: Invalid status.\n";
            return;
        }

        int rating = -1;
        if (args.size() == 4) {
            try {
                rating = std::stoi(args[3]);
                if (rating < 0 || rating > 10) throw std::exception();
            }
            catch (...) {
                std::cout << "Error: Rating must be a number from 0 to 10.\n";
                return;
            }
        }

        Reader* currentReader = dynamic_cast<Reader*>(authService.getCurrentUser());
        bookService.addBookToProfile(currentReader, args[1], rating);
        return;
    }

    if (command == "create-shelf" || command == "delete-shelf") {
        if (args.size() != 2) {
            std::cout << "Error: Usage: " << command << " <name>\n";
            return;
        }
        Reader* currentReader = dynamic_cast<Reader*>(authService.getCurrentUser());
        if (command == "create-shelf") bookService.createShelf(currentReader, args[1]);
        else bookService.deleteShelf(currentReader, args[1]);
        return;
    }

    if (command == "add-to-shelf" || command == "remove-from-shelf") {
        if (args.size() != 3) {
            std::cout << "Error: Usage: " << command << " <bookName> <shelfName>\n";
            return;
        }
        Reader* currentReader = dynamic_cast<Reader*>(authService.getCurrentUser());
        if (command == "add-to-shelf") bookService.addBookToShelf(currentReader, args[1], args[2]);
        else bookService.removeBookFromShelf(currentReader, args[1], args[2]);
        return;
    }

    if (command == "delete-book") {
        if (args.size() != 2) {
            std::cout << "Error: Usage: delete-book <bookName>\n";
            return;
        }
        Reader* currentReader = dynamic_cast<Reader*>(authService.getCurrentUser());
        bookService.deleteBookFromProfile(currentReader, args[1]);
        return;
    }

    if (command == "show-shelf") {
        Reader* currentReader = dynamic_cast<Reader*>(authService.getCurrentUser());
        if (args.size() == 2) {
            bookService.showShelf(currentReader, currentReader, args[1]);
        }
        else if (args.size() == 3) {
            Reader* targetReader = dynamic_cast<Reader*>(findUser(args[1]));
            if (!currentReader || !targetReader) {
                std::cout << "Error: Reader not found.\n";
                return;
            }
            if (!currentReader->isFollowing(targetReader) || !targetReader->isFollowing(currentReader)) {
                std::cout << "Error: Access denied. You must be friends with this reader.\n";
                return;
            }
            bookService.showShelf(currentReader, targetReader, args[2]);
        }
        else {
            std::cout << "Error: Usage: show-shelf [reader] <name>\n";
        }
        return;
    }

    if (command == "add-birthday") {
        if (args.size() > 2) {
            std::cout << "Error: Usage: add-birthday [date]\n";
            return;
        }
        socialService.addBirthday(args.size() == 2 ? args[1] : "");
        return;
    }

    if (command == "profile") {
        if (args.size() > 2) {
            std::cout << "Error: Usage: profile [reader]\n";
            return;
        }
        socialService.showProfile(args.size() == 2 ? args[1] : "");
        return;
    }

    if (command == "show-inbox") {
        if (args.size() > 2) {
            std::cout << "Error: Usage: show-inbox [filter]\n";
            return;
        }
        std::string filter = (args.size() == 2) ? args[1] : "";
        socialService.showInbox(filter);
        return;
    }

    if (command == "read-msg" || command == "delete-msg" || command == "accept-offer") {
        if (args.size() != 2) {
            std::cout << "Error: Usage: " << command << " <index>\n";
            return;
        }
        int index = -1;
        try {
            index = std::stoi(args[1]);
        }
        catch (...) {
            std::cout << "Error: Index must be a number.\n";
            return;
        }

        if (command == "read-msg") socialService.readMessage(index);
        else if (command == "delete-msg") socialService.deleteMessage(index);
        else socialService.acceptOffer(index);
        return;
    }

    if (command == "friends") {
        if (args.size() > 2) {
            std::cout << "Error: Usage: friends [reader]\n";
            return;
        }
        std::string reader = (args.size() == 2) ? args[1] : "";
        socialService.showFriends(reader);
        return;
    }

    if (command == "followers") {
        if (args.size() != 1) {
            std::cout << "Error: Usage: followers\n";
            return;
        }
        socialService.showFollowers();
        return;
    }

    if (command == "publish") {
        Publisher* publisher = dynamic_cast<Publisher*>(authService.getCurrentUser());
        if (!publisher) {
            std::cout << "Error: Only publishers can publish books.\n";
            return;
        }
        if (args.size() < 6) {
            std::cout << "Error: Usage: publish <bookTitle> <authorName> <releaseDate> <pageCount> <genres...>\n";
            return;
        }

        unsigned int pageCount = 0;
        try {
            pageCount = std::stoul(args[4]);
            if (pageCount == 0) {
                std::cout << "Error: Page count must be greater than 0.\n";
                return;
            }
        }
        catch (...) {
            std::cout << "Error: Page count must be a positive number.\n";
            return;
        }

        Date releaseDate;
        if (!parseDate(args[3], releaseDate)) {
            std::cout << "Error: Invalid release date.\n";
            return;
        }

        std::vector<Genre> genres;
        for (size_t i = 5; i < args.size(); ++i) {
            Genre genre;
            if (!parseGenre(args[i], genre)) {
                std::cout << "Error: Unknown genre '" << args[i] << "'.\n";
                return;
            }
            genres.push_back(genre);
        }

        if (!dynamic_cast<Author*>(findUser(args[2]))) {
            std::cout << "Error: Author '" << args[2] << "' not found.\n";
            return;
        }

        bookService.publishBook(args[1], args[2], releaseDate, pageCount, genres, publisher->getUsername());
        return;
    }

    if (command == "add-synopsis") {
        Publisher* publisher = dynamic_cast<Publisher*>(authService.getCurrentUser());
        if (!publisher) {
            std::cout << "Error: Only publishers can add a synopsis.\n";
            return;
        }
        if (args.size() < 3) {
            std::cout << "Error: Usage: add-synopsis <bookTitle> <synopsis>\n";
            return;
        }


        size_t pos = commandLine.find(args[1]);
        pos = commandLine.find_first_not_of(" \t\n\r", pos + args[1].length());

        if (pos == std::string::npos) {
            std::cout << "Error: Synopsis cannot be empty.\n";
            return;
        }
        std::string synopsis = commandLine.substr(pos);

        bookService.addSynopsis(args[1], synopsis);
        return;
    }

    if (command == "offer" || command == "leave") {
        if (args.size() != 2) {
            std::cout << "Error: Usage: " << command << " <name>\n";
            return;
        }
        if (command == "offer") socialService.sendOffer(args[1]);
        else socialService.leavePublisher(args[1]);
        return;
    }

    std::cout << "Error: Unknown command '" << command << "'. Type 'help' for allowed commands.\n";
}

User* LibrarySystem::findUser(const std::string& username)
{
    std::string lowerUsername = toLower(username);
    for (const auto& user : allUsers) {
        if (toLower(user->getUsername()) == lowerUsername) {
            return user.get();
        }
    }
    return nullptr;
}

Book* LibrarySystem::findBook(const std::string& title)
{
    std::shared_ptr<Book> book = bookService.findBookInDB(title);
    return book ? book.get() : nullptr;
}

void LibrarySystem::run()
{
    std::cout << "Library system started. Type 'help' for commands.\n";

    std::string commandLine;
    while (std::cout << "> " && std::getline(std::cin, commandLine)) {
        std::vector<std::string> args = tokenize(commandLine);
        if (args.empty()) continue;

        if (toLower(args[0]) == "exit") {
            if (args.size() > 1) {
                std::cout << "Error: Usage: exit\n";
                continue;
            }
            std::cout << "Exiting library system.\n";
            break;
        }
        executeCommand(commandLine);
    }
}

void LibrarySystem::showHelp() const
{
	std::cout << "Common commands:\n";
	std::cout << "help\n";
	std::cout << "register <username> <password> <reader|author|publisher>\n";
	std::cout << "login <username> <password>\n";
	std::cout << "logout\n";
	std::cout << "exit\n";
	std::cout << "\nReader/Author commands:\n";
	std::cout << "search <name>\n";
	std::cout << "follow <username>\n";
	std::cout << "add-book <bookName> <status> [rating]\n";
	std::cout << "create-shelf <name>\n";
	std::cout << "delete-shelf <name>\n";
	std::cout << "add-to-shelf <bookName> <shelfName>\n";
	std::cout << "remove-from-shelf <bookName> <shelfName>\n";
	std::cout << "delete-book <bookName>\n";
	std::cout << "show-shelf [reader] <name>\n";
	std::cout << "add-birthday [date]\n";
	std::cout << "profile [reader]\n";
	std::cout << "show-inbox [filter]\n";
	std::cout << "read-msg <index>\n";
	std::cout << "delete-msg <index>\n";
	std::cout << "friends [reader]\n";
	std::cout << "\nPublisher commands:\n";
	std::cout << "publish <bookTitle> <authorName> <releaseDate> <pageCount> <genres...>\n";
	std::cout << "add-synopsis <bookTitle> <synopsis>\n";
	std::cout << "offer <author>\n";
	std::cout << "\nAuthor commands:\n";
	std::cout << "accept-offer <index>\n";
	std::cout << "leave <publisher>\n";
	std::cout << "followers\n";
}
