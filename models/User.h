#pragma once
#include <string>

class User {
public:
    int id;
    int canvasUserId;
    std::string name;
    std::string email;

    User();
    User(int id, int canvasUserId,
         const std::string& name,
         const std::string& email);
};
