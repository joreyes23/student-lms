#pragma once
#include <string>

class Assignment {
public:
    int id;
    int canvasAssignmentId;
    int courseId;
    std::string title;
    std::string dueDate;   // keep string for now
    double points;

    Assignment();
    Assignment(int id, int canvasAssignmentId,
               int courseId,
               const std::string& title,
               const std::string& dueDate,
               double points);
};
