#pragma once
#include "Command.h"
#include "../service/AuthService.h"

class Register : public Command
{
private:
	AuthService& authService;
public:
	Register(AuthService& authService);
	void execute(const std::vector<std::string>& args) override;
};

