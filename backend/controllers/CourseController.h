#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

class CourseController : public HttpController<CourseController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(CourseController::getAll, "/courses", Get);
    METHOD_LIST_END

    void getAll(const HttpRequestPtr&,
                std::function<void (const HttpResponsePtr&)>&& callback);
};
