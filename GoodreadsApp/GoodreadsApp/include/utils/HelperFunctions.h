#pragma once
#include <string>
#include <sstream>
#include "../../include/modules/Date.h"

namespace {
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
}