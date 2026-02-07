#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

class DbController : public HttpController<DbController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(DbController::ping, "/db/ping", Get);
    METHOD_LIST_END

    void ping(const HttpRequestPtr&,
              std::function<void (const HttpResponsePtr&)>&& callback);
};
