#pragma once

#include "User.h"
#include "Book.h"
#include "Shelf.h"
#include "Message.h"
#include "Date.h"
#include "../utils/Status.h"
#include <vector>
#include <string>
#include <iostream>
#include <optional>


class Reader : public User
{
public:
    Reader(std::string username, std::string password, Date registerDate);
    TypeUsers getType() const override;
    std::unique_ptr<User> clone() const override;
   
    const std::vector<std::shared_ptr<Book>>& getMyBooks() const;
    const std::vector<std::shared_ptr<Shelf>>& getShelves() const;
    const std::vector<std::shared_ptr<Book>>& getFavoriteBooks() const;
    const std::optional<Date>& getBirthday() const;
    void setBirthday(const Date& birthday);
    void clearBirthday();

    bool isFollowing(User* u);

    std::vector<Message>& getInbox();
    void addMessageToInbox(const Message& msg);
    bool isFollowing(User* u) const;

    void addBookToProfile(std::shared_ptr<Book> book);

    void removeBookFromProfile(const std::string& bookTitle);

    bool hasShelf(const std::string& shelfName) const;

    void addShelf(std::shared_ptr<Shelf> shelf);

    void removeShelf(const std::string& shelfName);

    Shelf* findShelf(const std::string& shelfName);

private:
    std::vector<std::shared_ptr<Book>> myBooks;
    std::vector<std::shared_ptr<Shelf>> shelves;
    std::vector<std::shared_ptr<Book>> favoriteBooks;
    std::optional<Date> birthday;
    std::vector<Message> inbox;
};

