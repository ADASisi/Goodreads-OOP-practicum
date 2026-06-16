#pragma once
#include <string>
#include <vector>
#include "../modules/User.h"
#include "AuthService.h"

class SocialService {
private:
	AuthService& authService;

	User* findUserInDB(const std::string& username) const;
	
public:
	SocialService(AuthService& authService);

	void followUser(const std::string& username);
	void showFriends(const std::string& reader = "") const;
	void showFollowers() const;
	void addBirthday(const std::string& date = "");
	void showProfile(const std::string& reader = "") const;

	void showInbox(const std::string& filter = "") const;
	void readMessage(int index);
	void deleteMessage(int index);

	void sendOffer(const std::string& authorUsername);
	void acceptOffer(int index);
	void leavePublisher(const std::string& publisherUsername);

	void notifyNewBookPublished(const std::string& publisherName, const std::string& authorName, const std::string& bookTitle);
	void searchUser(const std::string& query);

};
