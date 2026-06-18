#include "../../include/commands/ReadMessage.h"
#include "../../include/commands/CommandUtils.h"

ReadMessage::ReadMessage(AuthService& a, SocialService& s) : authService(a), socialService(s) {}

void ReadMessage::execute(const std::vector<std::string>& args) 
{
	if (args.size() != 2) 
	{ 
		fail("Error: Usage: read-msg <index>");
	}
	int index; 
	parseMessageIndex(args[1], index);
	socialService.readMessage(index);
}
