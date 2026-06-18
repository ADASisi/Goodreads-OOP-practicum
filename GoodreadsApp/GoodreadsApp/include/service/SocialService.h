#pragma once
#include <string>
#include <vector>
#include "../modules/User.h"
#include "AuthService.h"

class SocialService {
private:
	AuthService& authService;

	User* findUserInDB(const std::string& username) const;
	Reader* currentReaderOrError(const std::string& errorMessage) const;
	Author* currentAuthorOrError(const std::string& errorMessage) const;
	Publisher* currentPublisherOrError(const std::string& errorMessage) const;
	Reader* findReaderInDB(const std::string& username) const;
	Author* findAuthorInDB(const std::string& username) const;
	Publisher* findPublisherInDB(const std::string& username) const;
	
public:
	SocialService(AuthService& authService);

	void followUser(User* targetUser);
	void showFriends(const std::string& reader = "") const;
	void showFollowers(Author* author) const;
	void addBirthday(const std::string& date = "");
	void showProfile(const std::string& reader = "") const;

	void showInbox(const std::string& filter = "") const;
	void readMessage(int index);
	void deleteMessage(int index);

	void sendOffer(const std::string& authorUsername);
	void acceptOffer(int index);
	void leavePublisher(const std::string& publisherUsername);

	void notifyNewBookPublished(Publisher* publisher, Author* author, const std::string& bookTitle);
	void searchUser(const std::string& query);

};
