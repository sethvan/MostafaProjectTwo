#ifndef BORROWEDBOOK_H
#define BORROWEDBOOK_H

#include <QString>

class BorrowedBook
{
public:
  BorrowedBook(int id = 0, QString title = "");
  BorrowedBook(int id = 0, std::string stdTitle = "");

  int id;
  QString title;

  bool operator==(const BorrowedBook &rhs) const;
};

#endif // BORROWEDBOOK_H
