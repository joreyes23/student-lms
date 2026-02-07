#pragma once
#include <mysql/mysql.h>
#include <string>

class Database {
public:
    Database();
    ~Database();

    bool connect();
    bool ping();

    MYSQL* get();

private:
    MYSQL* conn;
};
