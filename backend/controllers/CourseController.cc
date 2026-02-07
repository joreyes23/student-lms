#include "CourseController.h"
#include "../db/Database.h"
#include <json/json.h>

void CourseController::getAll(
    const HttpRequestPtr&,
    std::function<void (const HttpResponsePtr&)>&& callback)
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
    mysql_query(conn, "SELECT id, course_code, title FROM courses");
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
