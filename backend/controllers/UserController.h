#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

class UserController : public HttpController<UserController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(UserController::getAll, "/users", Get);
    ADD_METHOD_TO(UserController::create, "/users", Post);
    ADD_METHOD_TO(UserController::getCourses, "/users/{1}/courses", Get);
    ADD_METHOD_TO(UserController::getTodo, "/users/{1}/todo", Get);
    ADD_METHOD_TO(UserController::getOverdue, "/users/{1}/overdue", Get);
    METHOD_LIST_END

    void getAll(const HttpRequestPtr&,
                std::function<void (const HttpResponsePtr&)>&& callback);

    void create(const HttpRequestPtr&,
            std::function<void (const HttpResponsePtr&)>&& callback);

    void getCourses(const HttpRequestPtr&,
                std::function<void (const HttpResponsePtr&)>&& callback,
                int userId);

    void getTodo(const HttpRequestPtr&,
             std::function<void (const HttpResponsePtr&)>&& callback,
             int userId);

    void getOverdue(const HttpRequestPtr&,
                std::function<void (const HttpResponsePtr&)>&& callback,
                int userId);
};
