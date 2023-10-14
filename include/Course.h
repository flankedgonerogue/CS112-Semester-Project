#ifndef PROJECT_V2_COURSE_H
#define PROJECT_V2_COURSE_H


#include <string>

class Course {
public:
    int ID{};
    std::string Degree;
    std::string Semester;
    std::string Course;
    std::string Discipline;
    int Year{};
    int Marks{};
    std::string sourceCity;

    [[nodiscard]] std::string toFormattedString() const;
};


#endif //PROJECT_V2_COURSE_H