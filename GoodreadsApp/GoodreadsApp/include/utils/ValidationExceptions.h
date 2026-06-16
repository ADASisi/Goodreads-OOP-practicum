#pragma once
#include <stdexcept>
#include <string>

class ValidationException : public std::runtime_error
{
public:
	explicit ValidationException(const std::string& message);
};

class InvalidUsernameException : public ValidationException
{
public:
	explicit InvalidUsernameException(const std::string& message);
};

class InvalidPasswordException : public ValidationException
{
public:
	explicit InvalidPasswordException(const std::string& message);
};
