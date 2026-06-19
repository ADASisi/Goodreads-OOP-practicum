#pragma once

#include <string>
#include "../../include/modules/Date.h"
#include "../../include/utils/Genre.h"
#include "../../include/utils/Status.h"
#include "../../include/utils/TypeUsers.h"

namespace Helper
{
    bool isBlank(const std::string& value);
    bool isValidUsername(const std::string& username);
    bool isValidPassword(const std::string& password);
    std::string getPasswordValidationError(const std::string& password);
    bool isValidBookTitle(const std::string& title);
    bool parsePositivePageCount(const std::string& value, unsigned int& pages);

    bool parseDate(const std::string& value, Date& date);
    std::string formatDate(const Date& date);
    Date getTodayDate();
    bool isDateAfter(const Date& left, const Date& right);
    bool isFutureDate(const Date& date);

    std::string toLower(std::string str);
    int getLevenshteinDistance(const std::string& s1, const std::string& s2);

    bool parseGenre(const std::string& value, Genre& genre);
    std::string genreToString(Genre genre);

    std::string userTypeToString(TypeUsers type);
    bool parseUserType(const std::string& text, TypeUsers& type);

    std::string statusToString(Status status);
    bool parseStatus(const std::string& text, Status& status);
}
