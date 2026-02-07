#include "User.h"

User::User()
    : id(0), canvasUserId(0) {}

User::User(int id, int canvasUserId,
           const std::string& name,
           const std::string& email)
    : id(id),
      canvasUserId(canvasUserId),
      name(name),
      email(email) {}
