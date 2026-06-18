#include "../../include/service/FileManager.h"
#include "../../include/utils/HelperFunctions.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

namespace {
    User* findUser(AuthService& auth, const std::string& username)
    {
        for (const auto& user : auth.getUsersDB()) {
            if (toLower(user->getUsername()) == toLower(username)) return user.get();
        }
        return nullptr;
    }

    const User* findUser(const AuthService& auth, const std::string& username)
    {
        for (const auto& user : auth.getUsersDB()) {
            if (toLower(user->getUsername()) == toLower(username)) return user.get();
        }
        return nullptr;
    }

    std::shared_ptr<Book> findBook(BookService& books, const std::string& title)
    {
        return books.findBookInDB(title);
    }
}

FileManager::FileManager(const std::string& path)
    : databaseFilePath(path)
{
}

bool FileManager::loadData(AuthService& auth, BookService& books, SocialService& social)
{
    std::ifstream file(databaseFilePath);
    if (!file) {
        std::cout << "Error: Could not open database file '" << databaseFilePath << "' for reading.\n";
        return false;
    }

    auth.logout();
    auth.getUsersDB().clear();
    books.getBooksDB().clear();

    struct FollowRecord {
        std::string follower;
        std::string followed;
    };
    struct MessageRecord {
        std::string receiver;
        std::string sender;
        bool isRead = false;
        std::string content;
    };
    struct BirthdayRecord {
        std::string username;
        Date date;
    };
    struct ReaderBookRecord {
        std::string username;
        std::string title;
        Status status = Status::PlanToRead;
    };
    struct AuthorPublisherRecord {
        std::string author;
        std::string publisher;
    };
    struct ShelfRecord {
        std::string username;
        std::string shelf;
    };
    struct ShelfBookRecord {
        std::string username;
        std::string shelf;
        std::string title;
    };

    std::vector<FollowRecord> follows;
    std::vector<MessageRecord> messages;
    std::vector<BirthdayRecord> birthdays;
    std::vector<ReaderBookRecord> readerBooks;
    std::vector<AuthorPublisherRecord> authorPublishers;
    std::vector<ShelfRecord> shelves;
    std::vector<ShelfBookRecord> shelfBooks;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream input(line);
        std::string recordType;
        input >> recordType;

        if (recordType == "USER") {
            std::string username;
            std::string password;
            std::string typeText;
            TypeUsers type;
            if (!(input >> std::quoted(username) >> std::quoted(password) >> typeText) || !parseUserType(typeText, type)) {
                std::cout << "Error: Invalid USER record in database.\n";
                return false;
            }
            auth.registerUser(username, password, type);
        }
        else if (recordType == "BOOK") {
            std::string title;
            std::string author;
            std::string publisher;
            std::string releaseDateText;
            unsigned int day = 1;
            unsigned int month = 1;
            unsigned int year = 2000;
            unsigned int pages = 0;
            double sumRatings = 0.0;
            unsigned int countRatings = 0;
            std::string synopsis;
            std::vector<Genre> genres;

            if (!(input >> std::quoted(title) >> std::quoted(author))) {
                std::cout << "Error: Invalid BOOK record in database.\n";
                return false;
            }

            input >> std::ws;
            if (input.peek() == '"') {
                input >> std::quoted(publisher);
            }

            input >> std::ws;
            if (input.peek() == '"') {
                Date releaseDate;
                if (!(input >> std::quoted(releaseDateText)) || !parseDate(releaseDateText, releaseDate)) {
                    std::cout << "Error: Invalid BOOK record in database.\n";
                    return false;
                }
                day = releaseDate.getDay();
                month = releaseDate.getMonth();
                year = releaseDate.getYear();
                if (!(input >> pages)) {
                    std::cout << "Error: Invalid BOOK record in database.\n";
                    return false;
                }
            }
            else if (!(input >> day >> month >> year >> pages)) {
                    std::cout << "Error: Invalid BOOK record in database.\n";
                    return false;
            }

            if (input >> sumRatings >> countRatings) {
                input >> std::ws;
                if (input.peek() == '"') {
                    input >> std::quoted(synopsis);
                }

                std::string genreText;
                while (input >> genreText) {
                    Genre genre;
                    if (!parseGenre(genreText, genre)) {
                        std::cout << "Error: Invalid BOOK genre in database.\n";
                        return false;
                    }
                    genres.push_back(genre);
                }
            }
            if (genres.empty()) {
                genres.push_back(Genre::Fiction);
            }

            auto book = std::make_shared<Book>(title, author, Date(day, month, year), pages, genres);
            book->setPublishingHouse(publisher);
            if (!synopsis.empty()) {
                book->setSynopsis(synopsis);
            }
            if (countRatings > 0) {
                double rating = countRatings == 0 ? 0.0 : sumRatings / countRatings;
                for (unsigned int i = 0; i < countRatings; i++) {
                    book->addRating(rating);
                }
            }
            books.getBooksDB().push_back(book);
        }
        else if (recordType == "FOLLOW") {
            FollowRecord record;
            if (!(input >> std::quoted(record.follower) >> std::quoted(record.followed))) {
                std::cout << "Error: Invalid FOLLOW record in database.\n";
                return false;
            }
            follows.push_back(record);
        }
        else if (recordType == "MESSAGE") {
            MessageRecord record;
            if (!(input >> std::quoted(record.receiver) >> std::quoted(record.sender) >> record.isRead >> std::quoted(record.content))) {
                std::cout << "Error: Invalid MESSAGE record in database.\n";
                return false;
            }
            messages.push_back(record);
        }
        else if (recordType == "BIRTHDAY") {
            BirthdayRecord record;
            unsigned int day = 1, month = 1, year = 2000;
            if (!(input >> std::quoted(record.username) >> day >> month >> year)) {
                std::cout << "Error: Invalid BIRTHDAY record in database.\n";
                return false;
            }
            record.date = Date(day, month, year);
            birthdays.push_back(record);
        }
        else if (recordType == "READER_BOOK") {
            ReaderBookRecord record;
            if (!(input >> std::quoted(record.username) >> std::quoted(record.title))) {
                std::cout << "Error: Invalid READER_BOOK record in database.\n";
                return false;
            }
            std::string statusText;
            if (input >> statusText && !parseStatus(statusText, record.status)) {
                std::cout << "Error: Invalid READER_BOOK status in database.\n";
                return false;
            }
            readerBooks.push_back(record);
        }
        else if (recordType == "AUTHOR_PUBLISHER") {
            AuthorPublisherRecord record;
            if (!(input >> std::quoted(record.author) >> std::quoted(record.publisher))) {
                std::cout << "Error: Invalid AUTHOR_PUBLISHER record in database.\n";
                return false;
            }
            authorPublishers.push_back(record);
        }
        else if (recordType == "SHELF") {
            ShelfRecord record;
            if (!(input >> std::quoted(record.username) >> std::quoted(record.shelf))) {
                std::cout << "Error: Invalid SHELF record in database.\n";
                return false;
            }
            shelves.push_back(record);
        }
        else if (recordType == "SHELF_BOOK") {
            ShelfBookRecord record;
            if (!(input >> std::quoted(record.username) >> std::quoted(record.shelf) >> std::quoted(record.title))) {
                std::cout << "Error: Invalid SHELF_BOOK record in database.\n";
                return false;
            }
            shelfBooks.push_back(record);
        }
        else {
            std::cout << "Error: Unknown database record '" << recordType << "'.\n";
            return false;
        }
    }

    for (const auto& record : birthdays) {
        Reader* reader = dynamic_cast<Reader*>(findUser(auth, record.username));
        if (reader) reader->setBirthday(record.date);
    }

    for (const auto& record : readerBooks) {
        Reader* reader = dynamic_cast<Reader*>(findUser(auth, record.username));
        std::shared_ptr<Book> book = findBook(books, record.title);
        if (reader && book) reader->addBookToProfile(book, record.status);
    }

    for (const auto& record : authorPublishers) {
        Author* author = dynamic_cast<Author*>(findUser(auth, record.author));
        Publisher* publisher = dynamic_cast<Publisher*>(findUser(auth, record.publisher));
        if (author && publisher && !author->worksWithPublisher(publisher->getUsername())) {
            author->addPublisher(publisher->getUsername(), false);
        }
    }

    for (const auto& record : shelves) {
        Reader* reader = dynamic_cast<Reader*>(findUser(auth, record.username));
        if (reader && !reader->hasShelf(record.shelf)) {
            reader->addShelf(std::make_shared<Shelf>(record.shelf));
        }
    }

    for (const auto& record : shelfBooks) {
        Reader* reader = dynamic_cast<Reader*>(findUser(auth, record.username));
        std::shared_ptr<Book> book = findBook(books, record.title);
        if (reader && book) {
            Shelf* shelf = reader->findShelf(record.shelf);
            if (shelf) shelf->addBook(book);
        }
    }

    for (const auto& record : follows) {
        User* follower = findUser(auth, record.follower);
        User* followed = findUser(auth, record.followed);
        if (follower && followed) {
            follower->addFollowing(followed);
            followed->addFollower(follower);
        }
    }

    for (const auto& record : messages) {
        Reader* receiver = dynamic_cast<Reader*>(findUser(auth, record.receiver));
        if (receiver) receiver->addMessageToInbox(Message::restoreFromFile(record.sender, record.receiver, record.content, record.isRead));
    }

    std::cout << "Data loaded successfully from " << databaseFilePath << ".\n";
    return true;
}

bool FileManager::saveData(const AuthService& auth, const BookService& books, const SocialService& social) const
{
    std::ofstream file(databaseFilePath);
    if (!file) {
        std::cout << "Error: Could not open database file '" << databaseFilePath << "' for writing.\n";
        return false;
    }

    for (const auto& user : auth.getUsersDB()) {
        file << "USER "
            << std::quoted(user->getUsername()) << " "
            << std::quoted(user->getPassword()) << " "
            << userTypeToString(user->getType()) << "\n";
    }

    for (const auto& book : books.getBooksDB()) {
        file << "BOOK "
            << std::quoted(book->getTitle()) << " "
            << std::quoted(book->getAuthor()) << " "
            << std::quoted(book->getPublishingHouse()) << " "
            << std::quoted(book->getReleaseDate()) << " "
            << book->getPageCount() << " "
            << book->getSumRatings() << " "
            << book->getCountRatings() << " "
            << std::quoted(book->getSynopsis()) << " "
            << book->getGenres() << "\n";
    }

    for (const auto& user : auth.getUsersDB()) {
        Reader* reader = dynamic_cast<Reader*>(user.get());
        if (!reader) continue;

        if (reader->getBirthday().has_value()) {
            const Date& date = reader->getBirthday().value();
            file << "BIRTHDAY "
                << std::quoted(reader->getUsername()) << " "
                << date.getDay() << " "
                << date.getMonth() << " "
                << date.getYear() << "\n";
        }

        for (const auto& entry : reader->getProfileBooks()) {
            if (entry.book) {
                file << "READER_BOOK "
                    << std::quoted(reader->getUsername()) << " "
                    << std::quoted(entry.book->getTitle()) << " "
                    << statusToString(entry.status) << "\n";
            }
        }

        Author* author = dynamic_cast<Author*>(user.get());
        if (author) {
            for (const auto& publisherName : author->getPublishers()) {
                file << "AUTHOR_PUBLISHER "
                    << std::quoted(author->getUsername()) << " "
                    << std::quoted(publisherName) << "\n";
            }
        }

        for (const auto& shelf : reader->getShelves()) {
            if (!shelf) continue;
            file << "SHELF "
                << std::quoted(reader->getUsername()) << " "
                << std::quoted(shelf->getName()) << "\n";

            for (const auto& book : shelf->getBooks()) {
                if (book) {
                    file << "SHELF_BOOK "
                        << std::quoted(reader->getUsername()) << " "
                        << std::quoted(shelf->getName()) << " "
                        << std::quoted(book->getTitle()) << "\n";
                }
            }
        }
    }

    for (const auto& followedUser : auth.getUsersDB()) {
        for (User* follower : followedUser->getFollowers()) {
            if (follower && findUser(auth, follower->getUsername())) {
                file << "FOLLOW "
                    << std::quoted(follower->getUsername()) << " "
                    << std::quoted(followedUser->getUsername()) << "\n";
            }
        }
    }

    for (const auto& user : auth.getUsersDB()) {
        Reader* reader = dynamic_cast<Reader*>(user.get());
        if (!reader) continue;

        for (const auto& message : reader->getInbox()) {
            file << "MESSAGE "
                << std::quoted(reader->getUsername()) << " "
                << std::quoted(message.getSender()) << " "
                << message.getIsRead() << " "
                << std::quoted(message.getContent()) << "\n";
        }
    }

    std::cout << "Data saved successfully to " << databaseFilePath << ".\n";
    return true;
}
