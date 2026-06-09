#pragma once

#include <string>

class Message {
private:
	std::string sender;
	std::string receiver;
	std::string content;
	bool isRead;
};
