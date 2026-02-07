#include "CanvasService.h"
#include <drogon/HttpClient.h>
#include <json/json.h>
#include <drogon/drogon.h>

using namespace drogon;

static std::string getToken() {
    return app().getCustomConfig()["canvas"]["token"].asString();
}

static std::string getBaseUrl() {
    return app().getCustomConfig()["canvas"]["base_url"].asString();
}

std::vector<CanvasCourse> CanvasService::fetchCourses()
{
    std::vector<CanvasCourse> out;

    auto client = HttpClient::newHttpClient(getBaseUrl());
    HttpRequestPtr req = HttpRequest::newHttpRequest();
    req->setMethod(Get);
    req->setPath("/api/v1/courses");
    req->addHeader("Authorization", "Bearer " + getToken());

    auto resp = client->sendRequest(req);

    if (resp.first != ReqResult::Ok) return out;

    auto json = resp.second->getJsonObject();
    for (auto& c : *json) {
        CanvasCourse cc;
        cc.canvas_id = c["id"].asInt();
        cc.name = c["name"].asString();
        cc.course_code = c["course_code"].asString();
        out.push_back(cc);
    }

    return out;
}

std::vector<CanvasAssignment>
CanvasService::fetchAssignments(int canvasCourseId)
{
    std::vector<CanvasAssignment> out;

    auto client = HttpClient::newHttpClient(getBaseUrl());
    HttpRequestPtr req = HttpRequest::newHttpRequest();
    req->setMethod(Get);
    req->setPath("/api/v1/courses/" +
                 std::to_string(canvasCourseId) +
                 "/assignments");
    req->addHeader("Authorization", "Bearer " + getToken());

    auto resp = client->sendRequest(req);
    if (resp.first != ReqResult::Ok) return out;

    auto json = resp.second->getJsonObject();
    for (auto& a : *json) {
        CanvasAssignment ca;
        ca.canvas_id = a["id"].asInt();
        ca.name = a["name"].asString();
        ca.due_at =
            a.isMember("due_at") && !a["due_at"].isNull()
                ? a["due_at"].asString()
                : "";
        ca.points =
            a.isMember("points_possible") &&
            !a["points_possible"].isNull()
                ? a["points_possible"].asDouble()
                : 0.0;
        out.push_back(ca);
    }
    return out;
}
