#include "../../include/commands/DeleteMessage.h"
#include "../../include/commands/CommandUtils.h"

DeleteMessage::DeleteMessage(AuthService& a, SocialService& s) : authService(a), socialService(s) {}

void DeleteMessage::execute(const std::vector<std::string>& args) 
{
	if (args.size() != 2) 
	{ 
		std::cout << "Error: Usage: delete-msg <index>\n"; 
		return; 
	}
	int index; 
	if (parseMessageIndex(args[1], index))
		socialService.deleteMessage(index);
}
