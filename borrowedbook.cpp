#include "borrowedbook.h"

BorrowedBook::BorrowedBook(int id, QString title)
  : id{id}, title{title}
{

}

BorrowedBook::BorrowedBook(int id, std::string stdTitle)
  : title{QString::fromStdString(stdTitle)}
{

}

bool BorrowedBook::operator==(const BorrowedBook &rhs) const
{
  return title == rhs.title;
}
