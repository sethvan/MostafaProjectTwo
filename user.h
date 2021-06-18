#ifndef USER_H
#define USER_H
#include <list>
#include "borrowedbook.h"
#include <QString>

class User
{
public:
  User(QString name = "");
  User(std::string stdName = "");

  QString name;
  std::list<BorrowedBook> borrowedList;

  bool operator==(const User& rhs) const;

};

#endif // USER_H
