#include "../../include/modules/Date.h"

Date::Date() : day(1), month(1), year(2000)
{
}

unsigned int Date::getYear() const
{
    return year;
}
unsigned int Date::getDay() const
{
    return day;
}

unsigned int Date::getMonth() const
{
    return month;
}

