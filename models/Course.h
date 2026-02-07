#pragma once
#include <string>

class Course {
public:
    int id;
    int canvasCourseId;
    std::string courseCode;
    std::string title;

    Course();
    Course(int id, int canvasCourseId,
           const std::string& courseCode,
           const std::string& title);
};
