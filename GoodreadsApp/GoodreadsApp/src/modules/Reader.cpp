#include "../../include/modules/Reader.h"
#include <algorithm>

Reader::Reader(std::string username, std::string password, Date registerDate) : User(username, password, registerDate) {}

std::unique_ptr<User> Reader::clone() const
{
    return std::make_unique<Reader>(*this);
}

TypeUsers Reader::getType() const
{
    return TypeUsers::Reader;
}

const std::vector<std::shared_ptr<Book>>& Reader::getMyBooks() const
{
    return myBooks;
}

const std::vector<ReaderBook>& Reader::getProfileBooks() const
{
    return profileBooks;
}

const std::vector<std::shared_ptr<Shelf>>& Reader::getShelves() const
{
    return shelves;
}

const std::vector<std::shared_ptr<Book>>& Reader::getFavoriteBooks() const
{
    return favoriteBooks;
}

const std::optional<Date>& Reader::getBirthday() const
{
    return birthday;
}

void Reader::setBirthday(const Date& birthday)
{
    this->birthday = birthday;
}

void Reader::clearBirthday()
{
    birthday.reset();
}

std::vector<Message>& Reader::getInbox()
{
    return inbox;
}

void Reader::addMessageToInbox(const Message& msg)
{
    inbox.push_back(msg);
}

void Reader::addBookToProfile(std::shared_ptr<Book> book, Status status)
{
    profileBooks.push_back({ book, status });
    myBooks.push_back(book);
}

void Reader::removeBookFromProfile(const std::string& bookTitle)
{
    std::string target = bookTitle;
    std::transform(target.begin(), target.end(), target.begin(), ::tolower);

    myBooks.erase(
        std::remove_if(myBooks.begin(), myBooks.end(),
            [&](const std::shared_ptr<Book>& b) {
                if (!b) return false;
                std::string currentTitle = b->getTitle();
                std::transform(currentTitle.begin(), currentTitle.end(), currentTitle.begin(), ::tolower);
                return currentTitle == target;
            }),
        myBooks.end()
    );

    profileBooks.erase(
        std::remove_if(profileBooks.begin(), profileBooks.end(),
            [&](const ReaderBook& entry) {
                if (!entry.book) return false;
                std::string currentTitle = entry.book->getTitle();
                std::transform(currentTitle.begin(), currentTitle.end(), currentTitle.begin(), ::tolower);
                return currentTitle == target;
            }),
        profileBooks.end()
    );
}

bool Reader::hasShelf(const std::string& shelfName) const {
    for (const auto& shelf : shelves) {
        if (shelf->getName() == shelfName) return true;
    }
    return false;
}

void Reader::addShelf(std::shared_ptr<Shelf> shelf)
{
    shelves.push_back(std::move(shelf));
}

void Reader::removeShelf(const std::string& shelfName)
{
    shelves.erase(
        std::remove_if(shelves.begin(), shelves.end(),
            [&](const std::shared_ptr<Shelf>& s) { return s->getName() == shelfName; }),
        shelves.end()
    );
}

Shelf* Reader::findShelf(const std::string& shelfName)
{
    for (auto& shelf : shelves)
    {
        if (shelf->getName() == shelfName) return shelf.get();
    }
    return nullptr;
}
