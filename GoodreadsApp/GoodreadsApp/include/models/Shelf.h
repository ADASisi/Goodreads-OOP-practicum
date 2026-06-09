#pragma once
#include <string>

class Shelf
{
public:
	Shelf();
	~Shelf();

private:
	std::string name;
	size_t countBooks;
	Date createDate;
	std::vector<std::shared_ptr<Book>> containBooks;

};