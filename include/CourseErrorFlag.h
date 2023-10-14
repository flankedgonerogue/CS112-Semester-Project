#ifndef PROJECT_V2_COURSEERRORFLAG_H
#define PROJECT_V2_COURSEERRORFLAG_H


enum CourseErrorFlag {
    IDE,
    Degree,
    Semester,
    CourseE,
    Discipline,
    Year,
    Marks
};

std::string CourseErrorFlagToString(CourseErrorFlag);

#endif //PROJECT_V2_COURSEERRORFLAG_H