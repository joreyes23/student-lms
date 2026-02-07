#include "UserController.h"
#include "../db/Database.h"
#include "../services/NotificationService.h"
#include <json/json.h>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <cstring>

void UserController::getAll(
    const HttpRequestPtr&,
    std::function<void (const HttpResponsePtr&)>&& callback)
{
    Database db;
    auto resp = HttpResponse::newHttpResponse();

    if (!db.connect()) {
        resp->setStatusCode(k500InternalServerError);
        resp->setBody("DB connect failed");
        callback(resp);
        return;
    }

    MYSQL* conn = db.get();
    mysql_query(conn, "SELECT id, name, email FROM users");
    MYSQL_RES* res = mysql_store_result(conn);

    Json::Value arr(Json::arrayValue);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(res))) {
        Json::Value u;
        u["id"] = std::stoi(row[0]);
        u["name"] = row[1];
        u["email"] = row[2];
        arr.append(u);
    }

    mysql_free_result(res);

    auto jsonResp = HttpResponse::newHttpJsonResponse(arr);
    callback(jsonResp);
}

void UserController::create(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr&)>&& callback)
{
    auto json = req->getJsonObject();
    auto resp = HttpResponse::newHttpResponse();

    if (!json || !json->isMember("name") || !json->isMember("email")) {
        resp->setStatusCode(k400BadRequest);
        resp->setBody("Invalid JSON");
        callback(resp);
        return;
    }

    Database db;
    if (!db.connect()) {
        resp->setStatusCode(k500InternalServerError);
        resp->setBody("DB connect failed");
        callback(resp);
        return;
    }

    MYSQL* conn = db.get();

    std::string name = (*json)["name"].asString();
    std::string email = (*json)["email"].asString();

    std::string query =
        "INSERT INTO users (name, email) VALUES ('" +
        name + "','" + email + "')";

    if (mysql_query(conn, query.c_str()) != 0) {
        resp->setStatusCode(k500InternalServerError);
        resp->setBody(mysql_error(conn));
        callback(resp);
        return;
    }

    resp->setBody("User created");
    callback(resp);
}

void UserController::getCourses(
    const HttpRequestPtr&,
    std::function<void (const HttpResponsePtr&)>&& callback,
    int userId)
{
    Database db;

    if (!db.connect()) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k500InternalServerError);
        resp->setBody("DB connect failed");
        callback(resp);
        return;
    }

    MYSQL* conn = db.get();

    std::string query =
        "SELECT c.id, c.course_code, c.title "
        "FROM courses c "
        "JOIN enrollments e ON e.course_id = c.id "
        "WHERE e.user_id = " + std::to_string(userId);

    if (mysql_query(conn, query.c_str()) != 0) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k500InternalServerError);
        resp->setBody(mysql_error(conn));
        callback(resp);
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    Json::Value arr(Json::arrayValue);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(res))) {
        Json::Value c;
        c["id"] = std::stoi(row[0]);
        c["course_code"] = row[1];
        c["title"] = row[2];
        arr.append(c);
    }

    mysql_free_result(res);

    auto jsonResp = HttpResponse::newHttpJsonResponse(arr);
    callback(jsonResp);
}

void UserController::getTodo(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr&)>&& callback,
    int userId)
{
    // ---- READ QUERY PARAM ----
    int days = 0;
    auto daysParam = req->getParameter("days");
    if (!daysParam.empty()) {
        days = std::stoi(daysParam);
    }

    Database db;
    if (!db.connect()) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k500InternalServerError);
        resp->setBody("DB connect failed");
        callback(resp);
        return;
    }

    MYSQL* conn = db.get();

    std::string query =
        "SELECT a.id, a.title, a.due_date, a.points, c.course_code, "
        "DATEDIFF(a.due_date, NOW()) AS days_left "
        "FROM assignments a "
        "JOIN courses c ON a.course_id = c.id "
        "JOIN enrollments e ON e.course_id = c.id "
        "WHERE e.user_id = " + std::to_string(userId) + " ";

    if (days > 0) {
        query += "AND a.due_date <= DATE_ADD(NOW(), INTERVAL " +
                 std::to_string(days) + " DAY) ";
    }

    query +=
    "ORDER BY (a.due_date < NOW()) DESC, "
    "(a.points / NULLIF(DATEDIFF(a.due_date, NOW()), 0)) DESC";


    if (mysql_query(conn, query.c_str()) != 0) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k500InternalServerError);
        resp->setBody(mysql_error(conn));
        callback(resp);
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    Json::Value arr(Json::arrayValue);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(res))) {
    Json::Value t;

    int daysLeft = 0;
    if (row[5] && std::strlen(row[5]) > 0) {
    daysLeft = std::stoi(row[5]);
    }
    
   
    bool overdue = true;

    double points = row[3] ? std::stod(row[3]) : 0.0;
    double priority = (daysLeft > 0) ? (points / daysLeft) : points;

    priority = std::round(priority * 100.0) / 100.0;

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << priority;

    t["id"] = std::stoi(row[0]);
    t["title"] = row[1];
    t["due_date"] = row[2] ? row[2] : "";
    t["points"] = points;
    t["course"] = row[4];
    t["days_left"] = daysLeft;
    t["overdue"] = overdue;
    t["priority"] = ss.str();

    std::string message =
    "Assignment '" + std::string(row[1]) +
    "' is overdue in " + std::string(row[4]);

   if (overdue) {
    std::string message =
        "Assignment '" + std::string(row[1]) +
        "' is overdue in " + std::string(row[4]);

    NotificationService::notify(
        "jo@csuf.edu",
        message
    );
}
    
    arr.append(t);
}


    mysql_free_result(res);

    auto jsonResp = HttpResponse::newHttpJsonResponse(arr);
    callback(jsonResp);
    }

void UserController::getOverdue(
    const HttpRequestPtr&,
    std::function<void (const HttpResponsePtr&)>&& callback,
    int userId)
{
    Database db;
    if (!db.connect()) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k500InternalServerError);
        resp->setBody("DB connect failed");
        callback(resp);
        return;
    }

    MYSQL* conn = db.get();

    std::string query =
        "SELECT a.id, a.title, a.due_date, a.points, c.course_code, "
        "DATEDIFF(a.due_date, NOW()) AS days_left "
        "FROM assignments a "
        "JOIN courses c ON a.course_id = c.id "
        "JOIN enrollments e ON e.course_id = c.id "
        "WHERE e.user_id = " + std::to_string(userId) + " "
        "AND a.due_date < NOW() "
        "ORDER BY (a.points / NULLIF(DATEDIFF(a.due_date, NOW()), 0)) DESC";

    if (mysql_query(conn, query.c_str()) != 0) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k500InternalServerError);
        resp->setBody(mysql_error(conn));
        callback(resp);
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    Json::Value arr(Json::arrayValue);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(res))) {
        Json::Value t;

        int daysLeft = 0;
        if (row[5] && std::strlen(row[5]) > 0) {
            daysLeft = std::stoi(row[5]);
        }

        double points = row[3] ? std::stod(row[3]) : 0.0;
        double priority = (daysLeft != 0) ? (points / std::abs(daysLeft)) : points;

        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << priority;

        t["id"] = std::stoi(row[0]);
        t["title"] = row[1];
        t["due_date"] = row[2] ? row[2] : "";
        t["points"] = points;
        t["course"] = row[4];
        t["days_left"] = daysLeft;
        t["overdue"] = true;
        t["priority"] = ss.str();

        arr.append(t);
    }

    mysql_free_result(res);

    auto jsonResp = HttpResponse::newHttpJsonResponse(arr);
    callback(jsonResp);
}
