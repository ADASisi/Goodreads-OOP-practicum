#pragma once
#include <string>

using namespace std;

class Validator
{
public:
	static bool isValidUsername(const string& username);
	static bool isValidPasswod(const string& password);
};