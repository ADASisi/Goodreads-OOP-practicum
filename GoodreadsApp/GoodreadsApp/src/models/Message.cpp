#include "../../include/models/Message.h"

Message::Message(const std::string sender, const std::string receiver, const std::string content, bool isRead)
	: sender(sender), receiver(receiver), content(content), isRead(isRead)
{
}

bool Message::getIsRead() const
{ 
	return isRead; 
}
std::string Message::getSender() const
{
	return sender;
}
std::string Message::getContent() const
{
	return content;
}
void Message::markAsRead()
{ 
	isRead = true; 
}

