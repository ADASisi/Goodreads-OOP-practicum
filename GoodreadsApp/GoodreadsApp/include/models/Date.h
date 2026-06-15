#pragma once

class Date
{
public:
	Date();
	Date(unsigned int day, unsigned int month, unsigned int year) : day(day), month(month), year(year) {};

	unsigned int getDay() const;
	unsigned int getMonth() const;
	unsigned int getYear() const;

private:
	unsigned int day;
	unsigned int month;
	unsigned int year;
};