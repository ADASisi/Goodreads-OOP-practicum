#pragma once

#include <string>

class Message {
public:
	Message(const std::string& sender, const std::string& receiver, const std::string content);

	static Message restoreFromFile(const std::string& sender, const std::string& receiver, const std::string& content, bool isRead);

	bool getIsRead() const;

	const std::string getSender() const;
	const std::string getContent() const;

	void markAsRead();

private:
	std::string sender;
	std::string receiver;
	std::string content;
	bool isRead;
};
