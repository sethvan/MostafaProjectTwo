#ifndef LIBRARYAPP_H
#define LIBRARYAPP_H
#include <vector>
#include "book.h"
#include "user.h"

class libraryApp
{
public:
  libraryApp();

  std::vector<Book> library;
  std::vector<User> users;

};

#endif // LIBRARYAPP_H
