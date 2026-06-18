#pragma once
#include "Command.h"
#include "../service/AuthService.h"
#include "../service/SocialService.h"

class DeleteMessage : public Command
{
private:
	AuthService& authService;
	SocialService& socialService;
public:
	DeleteMessage(AuthService& authService, SocialService& socialService);
	void execute(const std::vector<std::string>& args) override;
};
