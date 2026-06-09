#pragma once
#include <string>

#include "AuthService.h"

class SocialService {
public:
	SocialService();

	void followUser(User* currentUser, const std::string& username);
	void showFriends(const std::string& reader = "") const;
	void showFollowers() const;
	
	void showInbox(User* currentUser, const std::string& filter = "") const;
	void readMessage(User* currentUser, int index);
	void deleteMessage(User* currentUser, int index);

	void sendOffer(const std::string& authorUsername);
	void acceptOffer(int index);
	void leavePublisher(const std::string& publisherUsername);

	void notifyNewBookPublished(const std::string& publisher, const std::string& bookTitle, const std::string& BookTitle);
};