#include <algorithm>
#include <iostream> 
#include <stdexcept>
#include "../../include/modules/Reader.h"
#include "../../include/service/AuthService.h"
#include "../../include/service/SocialService.h"
#include "../../include/utils/HelperFunctions.h"

SocialService::SocialService(AuthService& authService) : authService(authService) {}


User* SocialService::findUserInDB(const std::string& username) const {
    const auto& db = authService.getUsersDB();
    for (auto& u : db) {
        if (toLower(u->getUsername()) == toLower(username)) {
            return u.get();
        }
    }
    return nullptr;
}

Reader* SocialService::currentReaderOrError(const std::string& errorMessage) const
{
    Reader* reader = dynamic_cast<Reader*>(authService.getCurrentUser());
    if (!reader) {
        throw std::runtime_error(errorMessage);
    }
    return reader;
}

Author* SocialService::currentAuthorOrError(const std::string& errorMessage) const
{
    Author* author = dynamic_cast<Author*>(authService.getCurrentUser());
    if (!author) {
        throw std::runtime_error(errorMessage);
    }
    return author;
}

Publisher* SocialService::currentPublisherOrError(const std::string& errorMessage) const
{
    Publisher* publisher = dynamic_cast<Publisher*>(authService.getCurrentUser());
    if (!publisher) {
        throw std::runtime_error(errorMessage);
    }
    return publisher;
}

Reader* SocialService::findReaderInDB(const std::string& username) const
{
    User* user = findUserInDB(username);
    return dynamic_cast<Reader*>(user);
}

Author* SocialService::findAuthorInDB(const std::string& username) const
{
    User* user = findUserInDB(username);
    return dynamic_cast<Author*>(user);
}

Publisher* SocialService::findPublisherInDB(const std::string& username) const
{
    User* user = findUserInDB(username);
    return dynamic_cast<Publisher*>(user);
}

void SocialService::followUser(User* targetUser)
{
    Reader* currentReader = currentReaderOrError("Error: You must be logged in to follow users.");
    if (!currentReader) return;

    if (!targetUser)
    {
        throw std::runtime_error("Error: User not found.");
    }

    if (toLower(targetUser->getUsername()) == toLower(currentReader->getUsername()))
    {
        throw std::runtime_error("Error: You cannot follow yourself.");
    }

    if (currentReader->isFollowing(targetUser))
    {
        throw std::runtime_error("Error: You are already following " + targetUser->getUsername() + ".");
    }

    currentReader->addFollowing(targetUser);

    targetUser->addFollower(currentReader);

    Reader* targetReader = dynamic_cast<Reader*>(targetUser);
    if (targetReader)
    {
        Message msg(currentReader->getUsername(), targetReader->getUsername(), "Started following you.", false);
        targetReader->addMessageToInbox(msg);
    }

    std::cout << "You are now following " << targetUser->getUsername() << ".\n";
}

void SocialService::showFriends(const std::string& targetUsername) const {
    Reader* readerToView = nullptr;

    if (targetUsername.empty()) {
        readerToView = currentReaderOrError("Error: You must be logged in as a reader or author to view your friends.");
        if (!readerToView) return;
    }
    else {
        User* targetUser = findUserInDB(targetUsername);
        if (!targetUser) {
            throw std::runtime_error("Error: User '" + targetUsername + "' not found.");
        }

        readerToView = findReaderInDB(targetUsername);
        if (!readerToView) {
            throw std::runtime_error("Error: User '" + targetUsername + "' is not a reader.");
        }
    }

    std::cout << "Friends of " << readerToView->getUsername() << ":\n";
    bool hasFriends = false;

    for (User* follower : readerToView->getFollowers()) {
        if (!follower || follower == readerToView) {
            continue;
        }

        if (readerToView->isFollowing(follower)) {
            std::cout << "- " << follower->getUsername() << "\n";
            hasFriends = true;
        }
    }

    if (!hasFriends) {
        std::cout << "(No friends found)\n";
    }
}

void SocialService::showFollowers(Author* author) const {
    if (!author) {
        throw std::runtime_error("Error: Author is required to view followers.");
    }

    std::cout << "Followers of " << author->getUsername() << ":\n";
    bool hasFollowers = false;

    const auto& db = authService.getUsersDB();
    for (auto& user : db) {
        Reader* reader = dynamic_cast<Reader*>(user.get());
        if (reader && reader->isFollowing(author)) {
            std::cout << "- " << reader->getUsername() << "\n";
            hasFollowers = true;
        }
    }

    if (!hasFollowers) {
        std::cout << "(No followers yet)\n";
    }
}

void SocialService::addBirthday(const std::string& date) 
{
    Reader* currentReader = currentReaderOrError("Error: You must be logged in as a reader or author to update your birthday.");
    if (!currentReader) return;

    if (isBlank(date)) 
    {
        if (currentReader->getBirthday().has_value()) 
        {
            currentReader->clearBirthday();
            std::cout << "Birthday information deleted from your profile.\n";
        }
        else {
            std::cout << "No birthday information was set in your profile.\n";
        }
        return;
    }

    Date birthday;
    if (!parseDate(date, birthday)) {
        throw std::runtime_error("Error: Invalid date. Use format dd.mm.yyyy, dd/mm/yyyy, or dd-mm-yyyy.");
    }

    currentReader->setBirthday(birthday);
    std::cout << "Birthday updated to " << formatDate(birthday) << ".\n";
}

void SocialService::showProfile(const std::string& reader) const 
{
    Reader* readerToView = nullptr;

    if (reader.empty()) 
    {
        readerToView = currentReaderOrError("Error: You must be logged in as a reader or author to view your profile.");
        if (!readerToView) return;
    }
    else 
    {
        User* targetUser = findUserInDB(reader);
        if (!targetUser) 
        {
            throw std::runtime_error("Error: User '" + reader + "' not found.");
        }

        readerToView = findReaderInDB(reader);
        if (!readerToView) 
        {
            throw std::runtime_error("Error: User '" + reader + "' is not a reader or author.");
        }
    }

    std::cout << "Profile of " << readerToView->getUsername() << ":\n";
    std::cout << "Name: " << readerToView->getUsername() << "\n";

    if (readerToView->getBirthday().has_value())
    {
        std::cout << "Birthday: " << formatDate(readerToView->getBirthday().value()) << "\n";
    }
    else 
    {
        std::cout << "Birthday: (Not set)\n";
    }

    std::cout << "Shelves:\n";
    if (readerToView->getShelves().empty()) 
    {
        std::cout << "- (No shelves)\n";
    }
    else 
    {
        for (const auto& shelf : readerToView->getShelves()) 
        {
            std::cout << "- " << shelf->getName() << " (" << shelf->getBooks().size() << " books)\n";
        }
    }

    std::cout << "Favorite books:\n";
    if (readerToView->getFavoriteBooks().empty()) 
    {
        std::cout << "- (No favorite books)\n";
    }
    else 
    {
        for (const auto& book : readerToView->getFavoriteBooks()) 
        {
            if (book) 
            {
                std::cout << "- " << book->getTitle() << " by " << book->getAuthor() << "\n";
            }
        }
    }

    std::cout << "Registration date: " << formatDate(readerToView->getRegistrationDate()) << "\n";
}

void SocialService::showInbox(const std::string& filter) const {
    Reader* currentReader = currentReaderOrError("Error: You must be logged in as a Reader/Author to view your inbox.");
    if (!currentReader) return;

    std::cout << "Inbox for " << currentReader->getUsername() << ":\n";
    const auto& inbox = currentReader->getInbox();

    if (inbox.empty()) {
        std::cout << "(Your inbox is empty)\n";
        return;
    }

    std::string lowerFilter = toLower(filter);
    int index = 0;
    bool displayedAny = false;

    for (const auto& msg : inbox) {
        bool showMessage = true;
        if (!lowerFilter.empty()) {
            bool isFriendRequest = msg.getContent().find("Started following you.") != std::string::npos;
            bool isJobOffer = msg.getContent().find("JOB_OFFER:") == 0;

            if (lowerFilter == "friend" || lowerFilter == "friends" || lowerFilter == "friend-requests") {
                showMessage = isFriendRequest;
            }
            else if (lowerFilter == "job" || lowerFilter == "job-offers") {
                showMessage = isJobOffer;
            }
            else {
                showMessage = false;
            }
        }

        if (!showMessage) {
            index++;
            continue;
        }

        std::cout << "[" << index << "] ";
        std::cout << "From: " << msg.getSender() << " -> " << msg.getContent();
        std::cout << (msg.getIsRead() ? " (Read)" : " (Unread)") << "\n";

        index++;
        displayedAny = true;
    }

    if (!displayedAny) {
        std::cout << "(No messages match the filter)\n";
    }
}

void SocialService::readMessage(int index) {
    Reader* currentReader = currentReaderOrError("Error: You must be logged in as a Reader/Author to read messages.");
    if (!currentReader) return;
    auto& inbox = currentReader->getInbox();

    if (index < 0 || index >= static_cast<int>(inbox.size())) {
        throw std::runtime_error("Error: Invalid message index.");
    }

    inbox[index].markAsRead();
    std::cout << "Message [" << index << "] read.\n";
    std::cout << inbox[index].getContent();
}

void SocialService::deleteMessage(int index) {
    Reader* currentReader = currentReaderOrError("Error: You must be logged in as a Reader/Author to delete messages.");
    if (!currentReader) return;
    auto& inbox = currentReader->getInbox();

    if (index < 0 || index >= inbox.size()) {
        throw std::runtime_error("Error: Invalid message index.");
    }

    if (!inbox[index].getIsRead()) {
        throw std::runtime_error("Error: You can only delete messages that you have already read.");
    }

    inbox.erase(inbox.begin() + index);
    std::cout << "Message [" << index << "] deleted successfully.\n";
}

void SocialService::sendOffer(const std::string& authorUsername) {
    Publisher* currentPublisher = currentPublisherOrError("Error: Only publishers can send job offers.");
    if (!currentPublisher) return;

    Author* targetAuthor = findAuthorInDB(authorUsername);
    if (!targetAuthor) {
        throw std::runtime_error("Error: '" + authorUsername + "' is not an author or does not exist.");
    }

    std::string offerContent = "JOB_OFFER: Publisher " + currentPublisher->getUsername() + " wants to sign a contract with you.";
    Message msg(currentPublisher->getUsername(), targetAuthor->getUsername(), offerContent, false);
    targetAuthor->addMessageToInbox(msg);

    std::cout << "Offer successfully sent to " << authorUsername << ".\n";
}

void SocialService::acceptOffer(int index) {
    Author* currentAuthor = currentAuthorOrError("Error: Only authors can accept publisher offers.");
    if (!currentAuthor) return;

    auto& inbox = currentAuthor->getInbox();
    if (index < 0 || index >= static_cast<int>(inbox.size())) {
        throw std::runtime_error("Error: Invalid message index.");
    }

    Message& msg = inbox[index];
    if (msg.getContent().find("JOB_OFFER:") != 0) {
        throw std::runtime_error("Error: This message is not a job offer.");
    }

    std::string publisherName = msg.getSender();

    Publisher* publisher = findPublisherInDB(publisherName);

    if (!publisher) {
        throw std::runtime_error("Error: The publisher who sent this offer no longer exists.");
    }

    currentAuthor->addPublisher(publisherName);
    publisher->addAuthor(currentAuthor->getUsername());

    inbox.erase(inbox.begin() + index);
    std::cout << "Success! You are now partnered with publisher " << publisherName << ".\n";
}

void SocialService::leavePublisher(const std::string& publisherUsername)
{
    Author* currentAuthor = currentAuthorOrError("Error: Only authors can leave a publisher.");
    if (!currentAuthor) return;

    Publisher* publisher = findPublisherInDB(publisherUsername);
    if (!publisher) {
        throw std::runtime_error("Error: Publisher '" + publisherUsername + "' not found.");
    }
    currentAuthor->removePublisher(publisherUsername);
    publisher->removeAuthor(currentAuthor->getUsername());

    std::cout << "You successfully terminated your contract with " << publisherUsername << ".\n";
}

void SocialService::notifyNewBookPublished(Publisher* publisher, Author* author, const std::string& bookTitle) 
{
    const auto& db = authService.getUsersDB();
    for (const auto& user : db)
    {
        Reader* reader = dynamic_cast<Reader*>(user.get());
        if (!reader) continue;

        bool followsAuthor = author ? reader->isFollowing(author) : false;
        bool followsPublisher = publisher ? reader->isFollowing(publisher) : false;

        if (followsAuthor || followsPublisher) 
        {
            std::string notification = "New book published by ";
            if (followsAuthor && followsPublisher) 
            {
                notification += "author " + author->getUsername() + " and publisher " + publisher->getUsername();
            }
            else if (followsAuthor) 
            {
                notification += "author " + author->getUsername();
            }
            else 
            {
                notification += "publisher " + publisher->getUsername();
            }
            notification += ": " + bookTitle;

            Message msg("System", reader->getUsername(), notification, false);
            reader->addMessageToInbox(msg);
        }
    }
}

void SocialService::searchUser(const std::string& query) 
{
    std::cout << "Users:\n";
	User* user = findUserInDB(query);
    
    user ? std::cout << "- " << user->getUsername() << " (" << (user->getType() == TypeUsers::Reader ? "Reader" : user->getType() == TypeUsers::Author ? "Author" : "Publisher") << ")\n"
		: std::cout << "(No users match the search criteria)\n";
}


