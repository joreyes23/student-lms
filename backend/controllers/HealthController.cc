#include "HealthController.h"

void HealthController::check(
    const drogon::HttpRequestPtr&,
    std::function<void (const drogon::HttpResponsePtr&)>&& callback)
{
    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setBody("OK");
    callback(resp);
}


