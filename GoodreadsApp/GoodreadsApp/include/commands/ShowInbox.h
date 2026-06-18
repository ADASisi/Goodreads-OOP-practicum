#pragma once
#include "Command.h"
#include "../service/AuthService.h"
#include "../service/SocialService.h"

class ShowInbox : public Command
{
private:
	AuthService& authService;
	SocialService& socialService;

public:
	ShowInbox(AuthService& authService, SocialService& socialService);
	void execute(const std::vector<std::string>& args) override;
};

