#include "../../include/utils/HelperFunctions.h"

#include <algorithm>
#include <cctype>
#include <ctime>
#include <sstream>
#include <vector>

namespace Helper
{
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
            if (!value.empty() && value[0] == '-') return false;
            pages = std::stoul(value, &parsed);
            return parsed == value.size() && pages > 0 && pages <= 10000;
        }
        catch (...) {
            return false;
        }
    }

    static bool isLeapYear(unsigned int year)
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

    Date getTodayDate()
    {
        std::time_t now = std::time(nullptr);
        std::tm localTime{};
        localtime_s(&localTime, &now);

        return Date(
            static_cast<unsigned int>(localTime.tm_mday),
            static_cast<unsigned int>(localTime.tm_mon + 1),
            static_cast<unsigned int>(localTime.tm_year + 1900)
        );
    }

    bool isDateAfter(const Date& left, const Date& right)
    {
        if (left.getYear() != right.getYear()) {
            return left.getYear() > right.getYear();
        }
        if (left.getMonth() != right.getMonth()) {
            return left.getMonth() > right.getMonth();
        }
        return left.getDay() > right.getDay();
    }

    bool isFutureDate(const Date& date)
    {
        return isDateAfter(date, getTodayDate());
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

    bool parseGenre(const std::string& value, Genre& genre)
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
        else if (name == "science-fiction" || name == "sci-fi" || name == "scifi") genre = Genre::ScienceFiction;
        else if (name == "biography") genre = Genre::Biography;
        else if (name == "history") genre = Genre::History;
        else if (name == "poetry") genre = Genre::Poetry;
        else if (name == "children") genre = Genre::Children;
        else if (name == "young-adult" || name == "ya") genre = Genre::YoungAdult;
        else if (name == "classic") genre = Genre::Classic;
        else if (name == "non-fiction" || name == "nonfiction") genre = Genre::NonFiction;
        else if (name == "comedy") genre = Genre::Comedy;
        else if (name == "crime") genre = Genre::Crime;
        else if (name == "philosophy") genre = Genre::Philosophy;
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
        case Genre::ScienceFiction: return "science-fiction";
        case Genre::Biography: return "biography";
        case Genre::History: return "history";
        case Genre::Poetry: return "poetry";
        case Genre::Children: return "children";
        case Genre::YoungAdult: return "young-adult";
        case Genre::Classic: return "classic";
        case Genre::NonFiction: return "non-fiction";
        case Genre::Comedy: return "comedy";
        case Genre::Crime: return "crime";
        case Genre::Philosophy: return "philosophy";
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

    std::string statusToString(Status status)
    {
        switch (status) {
        case Status::PlanToRead: return "plan-to-read";
        case Status::Reading: return "reading";
        case Status::Paused: return "paused";
        case Status::Dropped: return "dropped";
        }
        return "plan-to-read";
    }

    bool parseStatus(const std::string& text, Status& status)
    {
        std::string value = toLower(text);
        if (value == "plan-to-read") {
            status = Status::PlanToRead;
            return true;
        }
        if (value == "reading") {
            status = Status::Reading;
            return true;
        }
        if (value == "paused") {
            status = Status::Paused;
            return true;
        }
        if (value == "dropped") {
            status = Status::Dropped;
            return true;
        }
        return false;
    }
}
