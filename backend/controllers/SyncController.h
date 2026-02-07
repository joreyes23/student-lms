#pragma once

#include <drogon/HttpController.h>
#include <functional>

using namespace drogon;

class SyncController : public HttpController<SyncController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(SyncController::syncCourses,
                  "/sync/canvas/courses", Post);

    ADD_METHOD_TO(SyncController::syncAssignments,
                  "/sync/canvas/assignments/{1}", Post);
    METHOD_LIST_END

    void syncCourses(
        const HttpRequestPtr&,
        std::function<void (const HttpResponsePtr&)>&& callback);

    void syncAssignments(
        const HttpRequestPtr&,
        std::function<void (const HttpResponsePtr&)>&& callback,
        int canvasCourseId);
};

