#pragma once
#include "Command.h"
#include "../service/AuthService.h"
#include "../service/SocialService.h"

class AddBirthday : public Command
{
private:
	AuthService& authService;
	SocialService& socialService;

public:
	AddBirthday(AuthService& authService, SocialService& socialService);
	void execute(const std::vector<std::string>& args) override;
};

