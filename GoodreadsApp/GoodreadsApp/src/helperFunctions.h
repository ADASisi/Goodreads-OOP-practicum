#pragma once
#include <algorithm>
#include <string>
#include<iostream>

std::string toLower(std::string str) 
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}