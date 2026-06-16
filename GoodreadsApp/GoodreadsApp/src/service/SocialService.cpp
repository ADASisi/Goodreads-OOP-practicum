#include <algorithm>
#include <iostream> 
#include <sstream>
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

void SocialService::followUser(const std::string& username)
{
    User* currentUser = authService.getCurrentUser();
    Reader* currentReader = dynamic_cast<Reader*>(currentUser);

    if (!currentReader)
    {
        std::cout << "Error: You must be logged in to follow users.\n";
        return;
    }

    User* targetUser = findUserInDB(username);
    if (!targetUser)
    {
        std::cout << "Error: User '" << username << "' not found.\n";
        return;
    }

    if (toLower(targetUser->getUsername()) == toLower(currentReader->getUsername()))
    {
        std::cout << "Error: You cannot follow yourself.\n";
        return;
    }

    if (currentReader->isFollowing(targetUser))
    {
        std::cout << "You are already following " << targetUser->getUsername() << ".\n";
        return;
    }

    currentReader->addFollowing(targetUser);

    targetUser->addFollower(currentUser);

    Reader* targetReader = dynamic_cast<Reader*>(targetUser);
    if (targetReader)
    {
        Message msg(currentReader->getUsername(), targetReader->getUsername(), "Started following you.", false);
        targetReader->addMessageToInbox(msg);
    }

    std::cout << "You are now following " << targetUser->getUsername() << ".\n";
}

void SocialService::showFriends(const std::string& targetUsername) const {
    User* currentUser = authService.getCurrentUser();
    Reader* readerToView = nullptr;

    if (targetUsername.empty()) {
        if (!currentUser) {
            std::cout << "Error: You must be logged in to view your friends.\n";
            return;
        }

        readerToView = dynamic_cast<Reader*>(currentUser);
        if (!readerToView) {
            std::cout << "Error: Only readers and authors can have friends.\n";
            return;
        }
    }
    else {
        User* targetUser = findUserInDB(targetUsername);
        if (!targetUser) {
            std::cout << "Error: User '" << targetUsername << "' not found.\n";
            return;
        }

        readerToView = dynamic_cast<Reader*>(targetUser);
        if (!readerToView) {
            std::cout << "Error: User '" << targetUsername << "' is not a reader or author.\n";
            return;
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

void SocialService::showFollowers() const {
    User* currentUser = authService.getCurrentUser();
    if (!currentUser) {
        std::cout << "Error: You must be logged in to view your followers.\n";
        return;
    }

    std::cout << "Followers of " << currentUser->getUsername() << ":\n";
    bool hasFollowers = false;

    const auto& db = authService.getUsersDB();
    for (auto& user : db) {
        Reader* reader = dynamic_cast<Reader*>(user.get());
        if (reader && reader->isFollowing(currentUser)) {
            std::cout << "- " << reader->getUsername() << "\n";
            hasFollowers = true;
        }
    }

    if (!hasFollowers) {
        std::cout << "(No followers yet)\n";
    }
}

void SocialService::addBirthday(const std::string& date) {
    User* currentUser = authService.getCurrentUser();
    Reader* currentReader = dynamic_cast<Reader*>(currentUser);

    if (!currentReader) {
        std::cout << "Error: You must be logged in as a reader or author to update your birthday.\n";
        return;
    }

    if (isBlank(date)) {
        if (currentReader->getBirthday().has_value()) {
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
        std::cout << "Error: Invalid date. Use format dd.mm.yyyy, dd/mm/yyyy, or dd-mm-yyyy.\n";
        return;
    }

    currentReader->setBirthday(birthday);
    std::cout << "Birthday updated to " << formatDate(birthday) << ".\n";
}

void SocialService::showProfile(const std::string& reader) const {
    User* currentUser = authService.getCurrentUser();
    Reader* readerToView = nullptr;

    if (reader.empty()) {
        if (!currentUser) {
            std::cout << "Error: You must be logged in to view your profile.\n";
            return;
        }

        readerToView = dynamic_cast<Reader*>(currentUser);
        if (!readerToView) {
            std::cout << "Error: Only readers and authors have reader profiles.\n";
            return;
        }
    }
    else {
        User* targetUser = findUserInDB(reader);
        if (!targetUser) {
            std::cout << "Error: User '" << reader << "' not found.\n";
            return;
        }

        readerToView = dynamic_cast<Reader*>(targetUser);
        if (!readerToView) {
            std::cout << "Error: User '" << reader << "' is not a reader or author.\n";
            return;
        }
    }

    std::cout << "Profile of " << readerToView->getUsername() << ":\n";
    std::cout << "Name: " << readerToView->getUsername() << "\n";

    if (readerToView->getBirthday().has_value()) {
        std::cout << "Birthday: " << formatDate(readerToView->getBirthday().value()) << "\n";
    }
    else {
        std::cout << "Birthday: (Not set)\n";
    }

    std::cout << "Shelves:\n";
    if (readerToView->getShelves().empty()) {
        std::cout << "- (No shelves)\n";
    }
    else {
        for (const auto& shelf : readerToView->getShelves()) {
            std::cout << "- " << shelf->getName() << " (" << shelf->getBooks().size() << " books)\n";
        }
    }

    std::cout << "Favorite books:\n";
    if (readerToView->getFavoriteBooks().empty()) {
        std::cout << "- (No favorite books)\n";
    }
    else {
        for (const auto& book : readerToView->getFavoriteBooks()) {
            if (book) {
                std::cout << "- " << book->getTitle() << " by " << book->getAuthor() << "\n";
            }
        }
    }

    std::cout << "Registration date: " << formatDate(readerToView->getRegistrationDate()) << "\n";
}

void SocialService::showInbox(const std::string& filter) const {
    User* currentUser = authService.getCurrentUser();
    Reader* currentReader = dynamic_cast<Reader*>(currentUser);

    if (!currentReader) {
        std::cout << "Error: You must be logged in as a Reader/Author to view your inbox.\n";
        return;
    }

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
        if (lowerFilter == "unread" && msg.getIsRead())
        {
            index++; continue;
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
    User* currentUser = authService.getCurrentUser();
    Reader* currentReader = dynamic_cast<Reader*>(currentUser);

    if (!currentReader) return;
    auto& inbox = currentReader->getInbox();

    if (index < 0 || index >= static_cast<int>(inbox.size())) {
        std::cout << "Error: Invalid message index.\n";
        return;
    }

    inbox[index].markAsRead();
    std::cout << "Message [" << index << "] read.\n";
    std::cout << inbox[index].getContent();
}

void SocialService::deleteMessage(int index) {
    User* currentUser = authService.getCurrentUser();
    Reader* currentReader = dynamic_cast<Reader*>(currentUser);

    if (!currentReader) return;
    auto& inbox = currentReader->getInbox();

    if (index < 0 || index >= static_cast<int>(inbox.size())) {
        std::cout << "Error: Invalid message index.\n";
        return;
    }

    if (!inbox[index].getIsRead()) {
        std::cout << "Error: You can only delete messages that you have already read.\n";
        return;
    }

    inbox.erase(inbox.begin() + index);
    std::cout << "Message [" << index << "] deleted successfully.\n";
}

void SocialService::sendOffer(const std::string& authorUsername) {
    User* currentUser = authService.getCurrentUser();
    Publisher* currentPublisher = dynamic_cast<Publisher*>(currentUser);

    if (!currentPublisher) {
        std::cout << "Error: Only publishers can send job offers.\n";
        return;
    }

    User* targetUser = findUserInDB(authorUsername);
    Author* targetAuthor = dynamic_cast<Author*>(targetUser);

    if (!targetAuthor) {
        std::cout << "Error: '" << authorUsername << "' is not an author or does not exist.\n";
        return;
    }

    std::string offerContent = "JOB_OFFER: Publisher " + currentPublisher->getUsername() + " wants to sign a contract with you.";
    Message msg(currentPublisher->getUsername(), targetAuthor->getUsername(), offerContent, false);
    targetAuthor->addMessageToInbox(msg);

    std::cout << "Offer successfully sent to " << authorUsername << ".\n";
}

void SocialService::acceptOffer(int index) {
    User* currentUser = authService.getCurrentUser();
    Author* currentAuthor = dynamic_cast<Author*>(currentUser);

    if (!currentAuthor) {
        std::cout << "Error: Only authors can accept publisher offers.\n";
        return;
    }

    auto& inbox = currentAuthor->getInbox();
    if (index < 0 || index >= static_cast<int>(inbox.size())) {
        std::cout << "Error: Invalid message index.\n";
        return;
    }

    Message& msg = inbox[index];
    if (msg.getContent().find("JOB_OFFER:") != 0) {
        std::cout << "Error: This message is not a job offer.\n";
        return;
    }

    std::string publisherName = msg.getSender();

    User* pubUser = findUserInDB(publisherName);
    Publisher* publisher = dynamic_cast<Publisher*>(pubUser);

    if (!publisher) {
        std::cout << "Error: The publisher who sent this offer no longer exists.\n";
        return;
    }

    currentAuthor->addPublisher(publisherName);
    publisher->addAuthor(currentAuthor->getUsername());

    inbox.erase(inbox.begin() + index);
    std::cout << "Success! You are now partnered with publisher " << publisherName << ".\n";
}

void SocialService::leavePublisher(const std::string& publisherUsername)
{
    User* currentUser = authService.getCurrentUser();
    Author* currentAuthor = dynamic_cast<Author*>(currentUser);

    if (!currentAuthor) {
        std::cout << "Error: Only authors can leave a publisher.\n";
        return;
    }

    User* pubUser = findUserInDB(publisherUsername);
    Publisher* publisher = dynamic_cast<Publisher*>(pubUser);

    if (!publisher) {
        std::cout << "Publisher '" << publisherUsername << "' not found.\n";
        return;
    }
    currentAuthor->removePublisher(publisherUsername);
    publisher->removeAuthor(currentAuthor->getUsername());

    std::cout << "You successfully terminated your contract with " << publisherUsername << ".\n";
}

void SocialService::notifyNewBookPublished(const std::string& publisherName, const std::string& authorName, const std::string& bookTitle) {
    User* auth = findUserInDB(authorName);
    User* pub = findUserInDB(publisherName);

    std::string notification = "Alert: New book published! '" + bookTitle + "' by " + authorName;
    if (pub) notification += " (Publisher: " + publisherName + ")";

    const auto& db = authService.getUsersDB();
    for (auto& user : db) {
        Reader* reader = dynamic_cast<Reader*>(user.get());
        if (!reader) continue;

        bool followsAuthor = auth ? reader->isFollowing(auth) : false;
        bool followsPublisher = pub ? reader->isFollowing(pub) : false;

        if (followsAuthor || followsPublisher) {
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


