#pragma once
#include "Command.h"
#include "../service/AuthService.h"
#include "../service/SocialService.h"

class Friends : public Command
{
private:
	AuthService& authService;
	SocialService& socialService;
public:
	Friends(AuthService& authService, SocialService& socialService);
	void execute(const std::vector<std::string>& args) override;
};

