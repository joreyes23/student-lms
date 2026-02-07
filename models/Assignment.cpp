#include "Assignment.h"

Assignment::Assignment()
    : id(0), canvasAssignmentId(0),
      courseId(0), points(0.0) {}

Assignment::Assignment(int id, int canvasAssignmentId,
                       int courseId,
                       const std::string& title,
                       const std::string& dueDate,
                       double points)
    : id(id),
      canvasAssignmentId(canvasAssignmentId),
      courseId(courseId),
      title(title),
      dueDate(dueDate),
      points(points) {}