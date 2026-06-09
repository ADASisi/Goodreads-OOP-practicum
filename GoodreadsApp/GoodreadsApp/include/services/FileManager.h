#pragma once
#include <string>
#include "AuthService.h"
#include "BookService.h"
#include "SocialService.h"

class FileManager {
private:
    std::string databaseFilePath;

public:
    explicit FileManager(const std::string& path);

    bool loadData(AuthService& auth, BookService& books, SocialService& social);

    bool saveData(const AuthService& auth, const BookService& books, const SocialService& social) const;
};