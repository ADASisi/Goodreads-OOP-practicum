#pragma once
#include<string>

using namespace std;

class User
{
public:
	User(string username, string pasword, Date reg);
	virtual ~User() {}

	virtual string getType() const = 0;

private:
	string username;
	string password;
	Date registrationDate;
	vector<User*> followers;
};