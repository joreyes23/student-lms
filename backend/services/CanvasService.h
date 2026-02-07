#pragma once
#include <string>
#include <vector>

struct CanvasCourse {
    int canvas_id;
    std::string name;
    std::string course_code;
};

struct CanvasAssignment {
    int canvas_id;
    std::string name;
    std::string due_at;
    double points;
};

class CanvasService {
public:
    static std::vector<CanvasCourse> fetchCourses();
    static std::vector<CanvasAssignment> fetchAssignments(int canvasCourseId);
};
