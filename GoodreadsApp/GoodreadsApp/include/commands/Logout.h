#pragma once
#include "Command.h"
#include "../service/AuthService.h"

class LogOut : public Command
{
	private:
		AuthService& authService;
public:	
	LogOut(AuthService& authService);
	void execute(const std::vector<std::string>& args) override;
};
