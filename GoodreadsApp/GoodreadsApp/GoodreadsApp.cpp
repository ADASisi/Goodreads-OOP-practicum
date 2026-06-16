#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "include/modules/Author.h"
#include "include/modules/Book.h"
#include "include/modules/Date.h"
#include "include/modules/Publisher.h"
#include "include/modules/Reader.h"
#include "include/modules/Shelf.h"
#include "include/modules/User.h"
#include "include/service/AuthService.h"
#include "include/service/BookService.h"
#include "include/service/SocialService.h"

void printSection(const std::string& title)
{
    std::cout << "\n==================================================\n";
    std::cout << title << "\n";
    std::cout << "==================================================\n";
}

void printStep(const std::string& text)
{
    std::cout << "\n-- " << text << " --\n";
}

Reader* currentReader(AuthService& authService)
{
    return dynamic_cast<Reader*>(authService.getCurrentUser());
}

void loginAs(AuthService& authService, const std::string& username)
{
    authService.logout();
    std::cout << "\nLogging in as '" << username << "': ";
    std::cout << (authService.login(username) ? "success" : "failed") << "\n";
}

int main()
{
    std::vector<std::shared_ptr<Book>> booksDatabase;
    std::vector<std::unique_ptr<User>> usersDatabase;

    BookService bookService(booksDatabase);
    AuthService authService(usersDatabase);
    SocialService socialService(authService);

    Date releaseDate(21, 9, 1937);

    printSection("AUTH SERVICE TESTS");

    printStep("Register users");
    std::cout << "Register alice01 reader: " << authService.registerUser("alice01", "AlicePass123!", TypeUsers::Reader) << "\n";
    std::cout << "Register bob000 reader: " << authService.registerUser("bob000", "BobPass12345!", TypeUsers::Reader) << "\n";
    std::cout << "Register charlie reader: " << authService.registerUser("charlie", "CharliePass1!", TypeUsers::Reader) << "\n";
    std::cout << "Register tolkien author: " << authService.registerUser("tolkien", "TolkienPass1!", TypeUsers::Author) << "\n";
    std::cout << "Register penguin publisher: " << authService.registerUser("penguin", "PenguinPass1!", TypeUsers::Publisher) << "\n";
    std::cout << "Register duplicate alice01: " << authService.registerUser("alice01", "AlicePass123!", TypeUsers::Reader) << " (expected 0)\n";

    printStep("Login with password");
    std::cout << "Wrong password: " << authService.login("alice01", "wrong") << " (expected 0)\n";
    std::cout << "Correct password: " << authService.login("alice01", "AlicePass123!") << " (expected 1)\n";
    std::cout << "Second login while already logged in: " << authService.login("bob000") << " (expected 0)\n";

    printSection("SOCIAL PROFILE AND BIRTHDAY TESTS");

    printStep("Profile before birthday");
    socialService.showProfile();

    printStep("Invalid birthday");
    socialService.addBirthday("31.02.2001");

    printStep("Set birthday");
    socialService.addBirthday("15.04.2001");
    socialService.showProfile();

    printStep("Clear birthday");
    socialService.addBirthday();
    socialService.showProfile();

    printStep("View another reader profile");
    socialService.showProfile("bob000");

    printStep("View publisher profile error");
    socialService.showProfile("penguin");

    printSection("BOOK SERVICE TESTS");

    printStep("Publish books");
    bookService.publishBook("The Hobbit", "tolkien", releaseDate, 310, { Genre::Fantasy, Genre::Adventure });
    bookService.publishBook("1984", "George Orwell", Date(8, 6, 1949), 328, { Genre::Fiction });
    bookService.publishBook("Dune", "Frank Herbert", Date(1, 8, 1965), 600, { Genre::Fantasy });
    bookService.publishBook("The Hobbit", "tolkien", releaseDate, 310, { Genre::Fantasy });

    printStep("Add synopsis");
    bookService.addSynopsis("The Hobbit", "A classic high-fantasy novel.");
    bookService.addSynopsis("Missing Book", "This should fail.");

    printStep("Search books");
    bookService.searchBooks("Hobbit");
    bookService.searchBooks("Done");
    bookService.searchBooks("No Match");

    printStep("Add books to Alice profile");
    Reader* alice = currentReader(authService);
    bookService.addBookToProfile(alice, "The Hobbit", 5);
    bookService.addBookToProfile(alice, "Dune", 4);
    bookService.addBookToProfile(alice, "The Hobbit", 5);
    bookService.addBookToProfile(alice, "Missing Book", 3);
    socialService.showProfile();

    printStep("Create shelves");
    bookService.createShelf(alice, "Favorites");
    bookService.createShelf(alice, "To Read");
    bookService.createShelf(alice, "Favorites");

    printStep("Add books to shelves");
    bookService.addBookToShelf(alice, "The Hobbit", "Favorites");
    bookService.addBookToShelf(alice, "Dune", "Favorites");
    bookService.addBookToShelf(alice, "Dune", "Favorites");
    bookService.addBookToShelf(alice, "1984", "Missing Shelf");
    bookService.showShelf(alice, nullptr, "Favorites");

    printStep("Remove book from shelf");
    bookService.removeBookFromShelf(alice, "Dune", "Favorites");
    bookService.showShelf(alice, nullptr, "Favorites");

    printStep("Delete book from profile and shelves");
    bookService.deleteBookFromProfile(alice, "The Hobbit");
    bookService.showShelf(alice, nullptr, "Favorites");
    socialService.showProfile();

    printStep("Delete shelf");
    bookService.deleteShelf(alice, "To Read");
    bookService.deleteShelf(alice, "Missing Shelf");
    socialService.showProfile();

    printSection("SOCIAL FOLLOW, FRIENDS, FOLLOWERS, INBOX TESTS");

    printStep("Alice follows Bob, Tolkien, and Penguin");
    socialService.followUser("bob000");
    socialService.followUser("tolkien");
    socialService.followUser("penguin");
    socialService.followUser("bob000");
    socialService.followUser("alice01");
    socialService.followUser("missing");

    printStep("Alice friends before Bob follows back");
    socialService.showFriends();
    socialService.showFollowers();

    loginAs(authService, "bob000");

    printStep("Bob inbox after Alice followed him");
    socialService.showInbox();
    socialService.showInbox("unread");
    socialService.readMessage(0);
    socialService.deleteMessage(0);
    socialService.showInbox();

    printStep("Bob follows Alice back");
    socialService.followUser("alice01");
    socialService.showFriends();
    socialService.showFriends("alice01");
    socialService.showFollowers();

    loginAs(authService, "alice01");

    printStep("Alice now has Bob as friend and a follow notification");
    socialService.showFriends();
    socialService.showFollowers();
    socialService.showInbox();

    printStep("Search users");
    socialService.searchUser("alice01");
    socialService.searchUser("missing");

    printSection("AUTHOR AND PUBLISHER OFFER TESTS");

    loginAs(authService, "penguin");

    printStep("Publisher sends offer to author");
    socialService.sendOffer("tolkien");
    socialService.sendOffer("alice01");

    loginAs(authService, "tolkien");

    printStep("Author inbox has follow notification and offer");
    socialService.showInbox();
    socialService.acceptOffer(0);
    socialService.acceptOffer(1);
    socialService.showInbox();

    printStep("Author leaves publisher");
    socialService.leavePublisher("penguin");
    socialService.leavePublisher("missing");

    printSection("NEW BOOK NOTIFICATION TEST");

    printStep("Notify followers of author/publisher");
    socialService.notifyNewBookPublished("penguin", "tolkien", "The Silmarillion");

    loginAs(authService, "alice01");
    socialService.showInbox();

    printSection("FINAL ERROR CASES");

    printStep("Publisher cannot use reader-only profile functions");
    loginAs(authService, "penguin");
    socialService.addBirthday("01.01.1990");
    socialService.showProfile();
    socialService.showFriends();
    socialService.showInbox();

    printStep("Logged-out behavior");
    authService.logout();
    socialService.showProfile();
    socialService.addBirthday("01.01.1990");
    socialService.showFriends();
    socialService.showFollowers();

    printSection("ALL TEST CALLS FINISHED");

    return 0;
}
