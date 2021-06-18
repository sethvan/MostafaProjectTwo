#include "book.h"




Book::Book(int id, QString title, int qty)
  : id{id}, title{title}, qty{qty}
{

}

Book::Book(int id, std::string stdTitle, int qty)
  : id{id}, title{QString::fromStdString(stdTitle)}, qty{qty}
{

}

bool Book::operator==(const Book &rhs) const
{
  return title == rhs.title;
}
