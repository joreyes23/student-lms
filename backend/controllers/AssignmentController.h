#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

class AssignmentController : public HttpController<AssignmentController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(AssignmentController::getByCourse,
                  "/courses/{1}/assignments", Get);
    METHOD_LIST_END

    void getByCourse(const HttpRequestPtr&,
                     std::function<void (const HttpResponsePtr&)>&& callback,
                     int courseId);
};
