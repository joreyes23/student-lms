#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

class HealthController : public HttpController<HealthController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(HealthController::check, "/health", Get);
    METHOD_LIST_END

    void check(const HttpRequestPtr&,
               std::function<void (const HttpResponsePtr&)>&& callback);
};