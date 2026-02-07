#include "Course.h"

Course::Course()
    : id(0), canvasCourseId(0) {}

Course::Course(int id, int canvasCourseId,
               const std::string& courseCode,
               const std::string& title)
    : id(id),
      canvasCourseId(canvasCourseId),
      courseCode(courseCode),
      title(title) {}
