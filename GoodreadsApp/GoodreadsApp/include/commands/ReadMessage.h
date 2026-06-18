#pragma once
#include "Command.h"
#include "../service/AuthService.h"
#include "../service/SocialService.h"

class ReadMessage : public Command
{
private:
	AuthService& authService;
	SocialService& socialService;

public:
	ReadMessage(AuthService& authService, SocialService& socialService);
	void execute(const std::vector<std::string>& args) override;
};

