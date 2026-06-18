#pragma once
#include "Command.h"
#include "../service/AuthService.h"

class Help : public Command
{
public:
	explicit Help(AuthService& authService);
	void execute(const std::vector<std::string>& args) override;

private:
	AuthService& authService;
};
