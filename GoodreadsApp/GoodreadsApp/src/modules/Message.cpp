#include "../../include/modules/Message.h"

Message::Message(const std::string sender, const std::string receiver, const std::string content)
	: sender(sender), receiver(receiver), content(content), isRead(false)
{
}

Message Message::restoreFromFile(const std::string& sender, const std::string& receiver, const std::string& content, bool isRead)
{
	Message message(sender, receiver, content);
	if (isRead) {
		message.markAsRead();
	}
	return message;
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

