#pragma once
#include <stdexcept>
#include <string>

class ServiceException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class UnauthorizedException : public ServiceException {
public:
    UnauthorizedException(const std::string& msg = "Error: You must be logged in to perform this action.")
        : ServiceException(msg) {
    }
};

class ForbiddenException : public ServiceException {
public:
    ForbiddenException(const std::string& msg = "Error: You do not have permission to perform this action.")
        : ServiceException(msg) {
    }
};

class NotFoundException : public ServiceException {
public:
    NotFoundException(const std::string& msg) : ServiceException(msg) {}
};

class BadRequestException : public ServiceException {
public:
    BadRequestException(const std::string& msg) : ServiceException(msg) {}
};

inline void fail(const std::string& message)
{
    throw BadRequestException(message);
}
