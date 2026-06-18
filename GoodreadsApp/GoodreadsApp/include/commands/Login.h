#pragma once
#include "Command.h"
#include "../service/AuthService.h"

class Login : public Command
{
private:
		AuthService& authService;
public:
	Login(AuthService& authService);
	void execute(const std::vector<std::string>& args) override;
};
