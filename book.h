#ifndef BOOK_H
#define BOOK_H
#include <QString>
#include <string>


class Book
{
public:
  Book(int id = 0, QString title = "", int qty = 10);
  Book(int id = 0, std::string stdTitle = "", int qty = 10);

  int id;
  QString title;
  int qty;

  bool operator==(const Book& rhs ) const;
};

#endif // BOOK_H
