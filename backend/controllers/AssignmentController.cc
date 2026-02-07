#include "AssignmentController.h"
#include "../db/Database.h"
#include <json/json.h>

void AssignmentController::getByCourse(
    const HttpRequestPtr&,
    std::function<void (const HttpResponsePtr&)>&& callback,
    int courseId)
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
        "SELECT id, title, due_date, points FROM assignments "
        "WHERE course_id = " + std::to_string(courseId);

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
        Json::Value a;
        a["id"] = std::stoi(row[0]);
        a["title"] = row[1];
        a["due_date"] = row[2] ? row[2] : "";
        a["points"] = row[3] ? row[3] : "0";
        arr.append(a);
    }

    mysql_free_result(res);

    auto jsonResp = HttpResponse::newHttpJsonResponse(arr);
    callback(jsonResp);
}
