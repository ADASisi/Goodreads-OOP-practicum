#pragma once
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include "../../include/modules/Date.h"
#include "../../include/modules/User.h"
#include "../../include/utils/Genre.h"

namespace {
    std::string toLower(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(),
            [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
        return str;
    }

    bool isBlank(const std::string& value)
    {
        return value.find_first_not_of(" \t\n\r") == std::string::npos;
    }

    bool isLeapYear(unsigned int year)
    {
        return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
    }

    bool parseDate(const std::string& value, Date& date)
    {
        unsigned int day = 0, month = 0, year = 0;
        size_t i = 0;
        size_t len = value.length();

        while (i < len && value[i] >= '0' && value[i] <= '9') 
        {
            day = day * 10 + (value[i] - '0');
            i++;
        }

        if (i == 0 || i >= len) return false;
        char sep1 = value[i++];

        if (sep1 != '.' && sep1 != '/' && sep1 != '-') return false;

        size_t startMonth = i;
        while (i < len && value[i] >= '0' && value[i] <= '9') 
        {
            month = month * 10 + (value[i] - '0');
            i++;
        }

        if (i == startMonth || i >= len) return false;
        char sep2 = value[i++];

        if (sep1 != sep2) return false; 

        size_t startYear = i;
        while (i < len && value[i] >= '0' && value[i] <= '9') 
        {
            year = year * 10 + (value[i] - '0');
            i++;
        }

        if (i == startYear) return false;
        while (i < len) 
        {
            if (value[i] != ' ' && value[i] != '\t' && value[i] != '\r' && value[i] != '\n') return false;
            i++;
        }

        if (month < 1 || month > 12 || day < 1) return false;

        const unsigned int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        unsigned int maxDay = daysInMonth[month - 1];
        if (month == 2 && isLeapYear(year)) 
        {
            maxDay = 29;
        }

        if (day > maxDay) return false;

        date = Date(day, month, year);
        return true;
    }

    bool parseUserType(const std::string& value, TypeUsers& type)
    {
        std::string lowerValue = toLower(value);
        if (lowerValue == "reader") {
            type = TypeUsers::Reader;
            return true;
        }
        if (lowerValue == "author") {
            type = TypeUsers::Author;
            return true;
        }
        if (lowerValue == "publisher") {
            type = TypeUsers::Publisher;
            return true;
        }
        return false;
    }

    bool parseGenre(const std::string& value, Genre& genre)
    {
        std::string lowerValue = toLower(value);
        if (lowerValue == "romance") genre = Genre::Romance;
        else if (lowerValue == "fantasy") genre = Genre::Fantasy;
        else if (lowerValue == "fiction") genre = Genre::Fiction;
        else if (lowerValue == "thriller") genre = Genre::Thriller;
        else if (lowerValue == "mystery") genre = Genre::Mystery;
        else if (lowerValue == "horror") genre = Genre::Horror;
        else if (lowerValue == "drama") genre = Genre::Drama;
        else if (lowerValue == "adventure") genre = Genre::Adventure;
        else return false;

        return true;
    }

    std::string formatDate(const Date& date)
    {
        return std::to_string(date.getDay()) + "." +
            std::to_string(date.getMonth()) + "." +
            std::to_string(date.getYear());
    }

    int getLevenshteinDistance(const std::string& s1, const std::string& s2)
    {
        int m = s1.length(), n = s2.length();
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));
        for (int i = 0; i <= m; i++) dp[i][0] = i;
        for (int j = 0; j <= n; j++) dp[0][j] = j;

        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (s1[i - 1] == s2[j - 1]) dp[i][j] = dp[i - 1][j - 1];
                else dp[i][j] = 1 + std::min({ dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1] });
            }
        }
        return dp[m][n];
    }

    std::vector<std::string> tokenize(const std::string& str) {
        std::vector<std::string> tokens;
        std::string current;
        for (char ch : str) {
            if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current.clear();
                }
            }
            else {
                current.push_back(ch);
            }
        }
        if (!current.empty()) {
            tokens.push_back(current);
        }
        return tokens;
    }
}
