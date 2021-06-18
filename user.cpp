#include "user.h"



User::User(QString name)
  : name{name}
{

}

User::User(std::string stdName)
  : name{QString::fromStdString(stdName)}
{

}

bool User::operator==(const User &rhs) const
{
  return name == rhs.name;
}


