#pragma once
#include <algorithm>
#include <cctype>
#include <string>
#include <sstream>
#include <vector>
#include "../../include/modules/Date.h"
#include "../../include/utils/Genre.h"
#include "../../include/utils/TypeUsers.h"

namespace {
    bool isBlank(const std::string& value)
    {
        return value.find_first_not_of(" \t\n\r") == std::string::npos;
    }

    bool isValidUsername(const std::string& username)
    {
        return username.length() >= 6 && username.length() <= 24 && !isBlank(username);
    }

    bool isValidPassword(const std::string& password)
    {
        bool hasLowercase = false;
        bool hasUppercase = false;
        bool hasNonLetter = false;

        for (unsigned char ch : password) {
            if (std::islower(ch)) {
                hasLowercase = true;
            }
            else if (std::isupper(ch)) {
                hasUppercase = true;
            }
            else if (!std::isalpha(ch)) {
                hasNonLetter = true;
            }
        }

        return password.length() >= 12 && password.length() <= 36 &&
            hasLowercase && hasUppercase && hasNonLetter;
    }

    std::string getPasswordValidationError(const std::string& password)
    {
        if (password.length() < 12) {
            return "Error: Password must be at least 12 characters long.";
        }
        if (password.length() > 36) {
            return "Error: Password must be at most 36 characters long.";
        }

        bool hasLowercase = false;
        bool hasUppercase = false;
        bool hasNonLetter = false;

        for (unsigned char ch : password) {
            if (std::islower(ch)) {
                hasLowercase = true;
            }
            else if (std::isupper(ch)) {
                hasUppercase = true;
            }
            else if (!std::isalpha(ch)) {
                hasNonLetter = true;
            }
        }

        if (!hasLowercase) {
            return "Error: Password must contain at least one lowercase letter.";
        }
        if (!hasUppercase) {
            return "Error: Password must contain at least one uppercase letter.";
        }
        if (!hasNonLetter) {
            return "Error: Password must contain at least one non-letter character.";
        }

        return "";
    }

    bool isValidBookTitle(const std::string& title)
    {
        return !isBlank(title) && title.length() <= 100;
    }

    bool parsePositivePageCount(const std::string& value, unsigned int& pages)
    {
        try {
            size_t parsed = 0;
            if (!value.empty() && value[0] == '-') throw std::exception();
            pages = std::stoul(value, &parsed);
            return parsed == value.size() && pages > 0 && pages <= 10000;
        }
        catch (...) {
            return false;
        }
    }

    bool isLeapYear(unsigned int year)
    {
        return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
    }

    bool parseDate(const std::string& value, Date& date)
    {
        std::stringstream ss(value);
        unsigned int day = 0, month = 0, year = 0;
        char firstSeparator = '\0', secondSeparator = '\0';

        if (!(ss >> day >> firstSeparator >> month >> secondSeparator >> year)) {
            return false;
        }

        ss >> std::ws;
        if (!ss.eof()) {
            return false;
        }

        if (firstSeparator != secondSeparator ||
            (firstSeparator != '.' && firstSeparator != '/' && firstSeparator != '-')) {
            return false;
        }

        if (month < 1 || month > 12 || day < 1) {
            return false;
        }

        const unsigned int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        unsigned int maxDay = daysInMonth[month - 1];
        if (month == 2 && isLeapYear(year)) {
            maxDay = 29;
        }

        if (day > maxDay) {
            return false;
        }

        date = Date(day, month, year);
        return true;
    }

    std::string formatDate(const Date& date)
    {
        return std::to_string(date.getDay()) + "." +
            std::to_string(date.getMonth()) + "." +
            std::to_string(date.getYear());
    }

    std::string toLower(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }

    int getLevenshteinDistance(const std::string& s1, const std::string& s2)
    {
        size_t m = s1.length(), n = s2.length();
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));
        for (size_t i = 0; i <= m; i++) dp[i][0] = static_cast<int>(i);
        for (size_t j = 0; j <= n; j++) dp[0][j] = static_cast<int>(j);

        for (size_t i = 1; i <= m; i++)
        {
            for (size_t j = 1; j <= n; j++)
            {
                if (s1[i - 1] == s2[j - 1]) dp[i][j] = dp[i - 1][j - 1];
                else dp[i][j] = 1 + std::min({ dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1] });
            }
        }
        return dp[m][n];
    }

    static bool parseGenre(const std::string& value, Genre& genre)
    {
        std::string name = toLower(value);
        if (name == "romance") genre = Genre::Romance;
        else if (name == "fantasy") genre = Genre::Fantasy;
        else if (name == "fiction") genre = Genre::Fiction;
        else if (name == "thriller") genre = Genre::Thriller;
        else if (name == "mystery") genre = Genre::Mystery;
        else if (name == "horror") genre = Genre::Horror;
        else if (name == "drama") genre = Genre::Drama;
        else if (name == "adventure") genre = Genre::Adventure;
        else return false;
        return true;
    }

    std::string genreToString(Genre genre)
    {
        switch (genre) {
        case Genre::Romance: return "romance";
        case Genre::Fantasy: return "fantasy";
        case Genre::Fiction: return "fiction";
        case Genre::Thriller: return "thriller";
        case Genre::Mystery: return "mystery";
        case Genre::Horror: return "horror";
        case Genre::Drama: return "drama";
        case Genre::Adventure: return "adventure";
        }
        return "none";
    }

    std::string userTypeToString(TypeUsers type)
    {
        switch (type) {
        case TypeUsers::Reader: return "reader";
        case TypeUsers::Author: return "author";
        case TypeUsers::Publisher: return "publisher";
        }
        return "reader";
    }

    bool parseUserType(const std::string& text, TypeUsers& type)
    {
        if (text == "reader") {
            type = TypeUsers::Reader;
            return true;
        }
        if (text == "author") {
            type = TypeUsers::Author;
            return true;
        }
        if (text == "publisher") {
            type = TypeUsers::Publisher;
            return true;
        }
        return false;
	}

}
