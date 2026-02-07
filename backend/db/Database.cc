#include "Database.h"
#include <iostream>

Database::Database() : conn(nullptr) {}

Database::~Database() {
    if (conn) {
        mysql_close(conn);
    }
}

bool Database::connect() {
    conn = mysql_init(nullptr);
    if (!conn) return false;

    return mysql_real_connect(
        conn,
        "127.0.0.1",
        "lms",
        "strongpassword",
        "student_lms",
        3306,
        nullptr,
        0
    ) != nullptr;
}

bool Database::ping() {
    if (!conn) return false;
    return mysql_query(conn, "SELECT 1") == 0;
}

MYSQL* Database::get() {
    return conn;
}
