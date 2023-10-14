#ifndef PROJECT_V2_COURSEHELPER_H
#define PROJECT_V2_COURSEHELPER_H


#include "CourseMap.h"
#include "Course.h"

[[nodiscard]] CourseMap* buildCourseMap(std::vector<std::string>&);

[[nodiscard]] Course& buildCourseFromMap(CourseMap&, std::vector<std::string>&);

[[nodiscard]] bool checkCourseForInvalidData(const Course&, const CourseErrorFlag flag);

void fixCourseWithInvalidData(Course&, CourseErrorFlag flag);

#endif //PROJECT_V2_COURSEHELPER_H