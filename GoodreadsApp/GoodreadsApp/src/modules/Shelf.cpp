#include "../../include/modules/Shelf.h"
#include "../../include/utils/HelperFunctions.h"
#include <algorithm>

using namespace Helper;

Shelf::Shelf(const std::string& shelfName) : name(shelfName), createDate(getTodayDate()) {}

Shelf::Shelf(const std::string& shelfName, Date createDate) : name(shelfName), createDate(createDate) {}

std::string Shelf::getName() const
{
    return name;
}

const std::vector<std::shared_ptr<Book>>& Shelf::getBooks() const
{
    return containBooks;
}

size_t Shelf::getBookCount() const
{
    return countBooks;
}

Date Shelf::getCreateDate() const
{
    return createDate;
}

void Shelf::addBook(std::shared_ptr<Book> book)
{
    containBooks.push_back(book);
    countBooks = containBooks.size();
}

void Shelf::removeBook(const std::string& bookTitle) {
    std::string target = bookTitle;
    std::transform(target.begin(), target.end(), target.begin(), ::tolower);

    containBooks.erase(
        std::remove_if(containBooks.begin(), containBooks.end(),
            [&](const std::shared_ptr<Book>& b) {
                if (!b) return false;
                std::string currentTitle = b->getTitle();
                std::transform(currentTitle.begin(), currentTitle.end(), currentTitle.begin(), ::tolower);
                return currentTitle == target;
            }),
        containBooks.end()
    );
    countBooks = containBooks.size();
}
