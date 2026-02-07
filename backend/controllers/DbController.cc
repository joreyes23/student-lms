#include "DbController.h"
#include "../db/Database.h"

void DbController::ping(
    const drogon::HttpRequestPtr&,
    std::function<void (const drogon::HttpResponsePtr&)>&& callback)
{
    Database db;
    auto resp = drogon::HttpResponse::newHttpResponse();

    if (!db.connect()) {
        resp->setStatusCode(drogon::k500InternalServerError);
        resp->setBody("DB connect failed");
    } else if (!db.ping()) {
        resp->setStatusCode(drogon::k500InternalServerError);
        resp->setBody("DB ping failed");
    } else {
        resp->setBody("DB OK");
    }

    callback(resp);
}
