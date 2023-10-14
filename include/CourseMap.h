#ifndef PROJECT_V2_COURSEMAP_H
#define PROJECT_V2_COURSEMAP_H


class CourseMap {
    int ID = -1;
    int Degree = -1;
    int Semester = -1;
    int Course = -1;
    int Discipline = -1;
    int Year = -1;
    int Marks = -1;

public:
    std::string city;
    int &operator[](int i);
    int& operator[](const std::string&);
};


#endif //PROJECT_V2_COURSEMAP_H