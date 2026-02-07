#include <iostream>
#include "SyncController.h"
#include "../services/CanvasService.h"
#include "../db/Database.h"

void SyncController::syncCourses(
    const HttpRequestPtr&,
    std::function<void (const HttpResponsePtr&)>&& callback)
{
    std::cout << ">>> syncAssignments HIT" << std::endl;

    Database db;
    if (!db.connect()) {
        auto r = HttpResponse::newHttpResponse();
        r->setBody("DB error");
        callback(r);
        return;
    }

    auto courses = CanvasService::fetchCourses();
    MYSQL* conn = db.get();

    for (auto& c : courses) {
        std::string q =
            "INSERT IGNORE INTO courses (canvas_course_id, course_code, title) "
            "VALUES (" +
            std::to_string(c.canvas_id) + ", '" +
            c.course_code + "', '" +
            c.name + "')";

        mysql_query(conn, q.c_str());
    }

    auto resp = HttpResponse::newHttpResponse();
    resp->setBody("Canvas courses synced");
    callback(resp);
}
void SyncController::syncAssignments(
    const HttpRequestPtr&,
    std::function<void (const HttpResponsePtr&)>&& callback,
    int canvasCourseId)
{
std::cout << ">>> syncAssignments HIT" << std::endl;

    Database db;
    if (!db.connect()) {
        auto r = HttpResponse::newHttpResponse();
        r->setBody("DB error");
        callback(r);
        return;
    }

    auto assigns = CanvasService::fetchAssignments(canvasCourseId);
    MYSQL* conn = db.get();

    // Map canvas_course_id -> internal course id
    std::string q =
        "SELECT id FROM courses WHERE canvas_course_id = " +
        std::to_string(canvasCourseId) + " LIMIT 1";
    mysql_query(conn, q.c_str());
    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        mysql_free_result(res);
        auto r = HttpResponse::newHttpResponse();
        r->setBody("Course not found");
        callback(r);
        return;
    }
    int courseId = std::stoi(row[0]);
    mysql_free_result(res);

    for (auto& a : assigns) {
        std::string due =
            a.due_at.empty() ? "NULL" : ("'" + a.due_at.substr(0, 19) + "'");
        std::string ins =
            "INSERT INTO assignments "
            "(canvas_assignment_id, course_id, title, due_date, points) "
            "VALUES (" +
            std::to_string(a.canvas_id) + ", " +
            std::to_string(courseId) + ", '" +
            a.name + "', " + due + ", " +
            std::to_string(a.points) + ") "
            "ON DUPLICATE KEY UPDATE "
            "title=VALUES(title), due_date=VALUES(due_date), points=VALUES(points)";
        mysql_query(conn, ins.c_str());
    }

    auto r = HttpResponse::newHttpResponse();
    r->setBody("Canvas assignments synced");
    callback(r);
}

