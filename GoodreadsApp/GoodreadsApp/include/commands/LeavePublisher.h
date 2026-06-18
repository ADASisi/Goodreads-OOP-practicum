#pragma once
#include "Command.h"
#include "../service/AuthService.h"
#include "../service/SocialService.h"

class LeavePublisher : public Command
{
private:
	AuthService& authService;
	SocialService& socialService;
public:
	LeavePublisher(AuthService& authService, SocialService& socialService);
	void execute(const std::vector<std::string>& args) override;
};

