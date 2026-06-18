#include <initializer_list>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "include/commands/AcceptOffer.h"
#include "include/commands/AddBirthday.h"
#include "include/commands/AddBook.h"
#include "include/commands/AddSynopsis.h"
#include "include/commands/AddToShelf.h"
#include "include/commands/CreateShelf.h"
#include "include/commands/DeleteBook.h"
#include "include/commands/DeleteMessage.h"
#include "include/commands/DeleteShelf.h"
#include "include/commands/Follow.h"
#include "include/commands/Followers.h"
#include "include/commands/Friends.h"
#include "include/commands/LeavePublisher.h"
#include "include/commands/Login.h"
#include "include/commands/Logout.h"
#include "include/commands/Offer.h"
#include "include/commands/Profile.h"
#include "include/commands/PublishBook.h"
#include "include/commands/ReadMessage.h"
#include "include/commands/Register.h"
#include "include/commands/RemoveFromShelf.h"
#include "include/commands/Search.h"
#include "include/commands/ShowInbox.h"
#include "include/commands/ShowShelf.h"

static void section(const std::string& title)
{
	std::cout << "\n==================================================\n";
	std::cout << title << "\n";
	std::cout << "==================================================\n";
}

static void test(Command& command, std::initializer_list<std::string> args)
{
	std::vector<std::string> values(args);
	std::cout << "\n> ";
	for (const std::string& value : values) std::cout << value << " ";
	std::cout << "\n";
	command.execute(values);
}

int main()
{
	std::vector<std::unique_ptr<User>> users;
	std::vector<std::shared_ptr<Book>> books;

	AuthService authService(users);
	BookService bookService(books);
	SocialService socialService(authService);

	Register registerCommand(authService);
	Login loginCommand(authService);
	LogOut logoutCommand(authService);
	Search searchCommand(authService, bookService);
	Follow followCommand(authService, socialService);
	AddBook addBookCommand(authService, bookService);
	CreateShelf createShelfCommand(authService, bookService);
	DeleteShelf deleteShelfCommand(authService, bookService);
	AddToShelf addToShelfCommand(authService, bookService);
	RemoveFromShelf removeFromShelfCommand(authService, bookService);
	DeleteBook deleteBookCommand(authService, bookService);
	ShowShelf showShelfCommand(authService, bookService);
	ShowInbox showInboxCommand(authService, socialService);
	ReadMessage readMessageCommand(authService, socialService);
	DeleteMessage deleteMessageCommand(authService, socialService);
	Friends friendsCommand(authService, socialService);
	AddBirthday addBirthdayCommand(authService, socialService);
	Profile profileCommand(authService, socialService);
	PublishBook publishCommand(authService, bookService, socialService);
	AddSynopsis addSynopsisCommand(authService, bookService);
	Offer offerCommand(authService, socialService);
	AcceptOffer acceptOfferCommand(authService, socialService);
	LeavePublisher leavePublisherCommand(authService, socialService);
	Followers followersCommand(authService, socialService);

	section("REGISTER AND LOGIN VALIDATION");
	test(registerCommand, { "register", "invalid", "pass", "unknown" });
	test(registerCommand, { "register", "alice", "Alice123!", "reader" });
	test(registerCommand, { "register", "bob", "Bob12345!", "reader" });
	test(registerCommand, { "register", "tolkien", "Author123!", "author" });
	test(registerCommand, { "register", "penguin", "Publisher123!", "publisher" });
	test(registerCommand, { "register", "alice", "duplicate", "reader" });
	test(loginCommand, { "login", "alice", "wrong" });

	section("PUBLISHER COMMANDS");
	test(loginCommand, { "login", "penguin", "Publisher123!" });
	test(publishCommand, { "publish", "The Hobbit", "tolkien", "21.09.1937", "310", "fantasy", "adventure" });
	test(publishCommand, { "publish", "Dune", "tolkien", "01.08.1965", "600", "fiction", "adventure" });
	test(publishCommand, { "publish", "Broken Book", "missing-author", "01.01.2000", "100", "fiction" });
	test(addSynopsisCommand, { "add-synopsis", "The Hobbit", "A", "classic", "fantasy", "adventure." });
	test(offerCommand, { "offer", "tolkien" });
	test(logoutCommand, { "logout" });

	section("ALICE READER COMMANDS");
	test(loginCommand, { "login", "alice", "Alice123!" });
	test(searchCommand, { "search", "th Hobit" });
	test(followCommand, { "follow", "bob" });
	test(followCommand, { "follow", "tolkien" });
	test(followCommand, { "follow", "penguin" });
	test(followCommand, { "follow", "alice" });
	test(addBookCommand, { "add-book", "The Hobbit", "reading", "9" });
	test(addBookCommand, { "add-book", "Dune", "plan-to-read" });
	test(addBookCommand, { "add-book", "Dune", "invalid-status" });
	test(createShelfCommand, { "create-shelf", "favorites" });
	test(addToShelfCommand, { "add-to-shelf", "The Hobbit", "favorites" });
	test(addToShelfCommand, { "add-to-shelf", "Dune", "favorites" });
	test(showShelfCommand, { "show-shelf", "favorites" });
	test(addBirthdayCommand, { "add-birthday", "15.04.2001" });
	test(profileCommand, { "profile" });
	test(logoutCommand, { "logout" });

	section("PUBLISH NOTIFICATION AFTER FOLLOWING");
	test(loginCommand, { "login", "penguin", "Publisher123!" });
	test(publishCommand, { "publish", "Silmarillion", "tolkien", "15.09.1977", "365", "fantasy" });
	test(logoutCommand, { "logout" });

	section("AUTHOR COMMANDS");
	test(loginCommand, { "login", "tolkien", "Author123!" });
	test(showInboxCommand, { "show-inbox" });
	test(showInboxCommand, { "show-inbox", "friend-requests" });
	test(acceptOfferCommand, { "accept-offer", "0" });
	test(followersCommand, { "followers" });
	test(leavePublisherCommand, { "leave", "penguin" });
	test(logoutCommand, { "logout" });

	section("BOB READER COMMANDS");
	test(loginCommand, { "login", "bob", "Bob12345!" });
	test(showInboxCommand, { "show-inbox" });
	test(deleteMessageCommand, { "delete-msg", "0" });
	test(readMessageCommand, { "read-msg", "0" });
	test(deleteMessageCommand, { "delete-msg", "0" });
	test(followCommand, { "follow", "alice" });
	test(friendsCommand, { "friends" });
	test(addBookCommand, { "add-book", "Dune", "reading", "8" });
	test(createShelfCommand, { "create-shelf", "favorites" });
	test(addToShelfCommand, { "add-to-shelf", "Dune", "favorites" });
	test(showShelfCommand, { "show-shelf", "favorites" });
	test(logoutCommand, { "logout" });

	section("FRIEND ACCESS AND CLEANUP");
	test(loginCommand, { "login", "alice", "Alice123!" });
	test(friendsCommand, { "friends" });
	test(friendsCommand, { "friends", "bob" });
	test(showShelfCommand, { "show-shelf", "bob", "favorites" });
	test(showInboxCommand, { "show-inbox" });
	test(readMessageCommand, { "read-msg", "0" });
	test(deleteMessageCommand, { "delete-msg", "0" });
	test(removeFromShelfCommand, { "remove-from-shelf", "Dune", "favorites" });
	test(deleteBookCommand, { "delete-book", "The Hobbit" });
	test(showShelfCommand, { "show-shelf", "favorites" });
	test(deleteShelfCommand, { "delete-shelf", "favorites" });
	test(addBirthdayCommand, { "add-birthday" });
	test(profileCommand, { "profile" });
	test(logoutCommand, { "logout" });
	test(logoutCommand, { "logout" });

	section("ALL COMMAND TESTS FINISHED");
	return 0;
}
