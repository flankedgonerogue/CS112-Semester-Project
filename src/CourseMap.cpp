#include <string>
#include "../include/CourseMap.h"

int &CourseMap::operator[](const int i) {
    switch (i) {
        case 1:
            return ID;
        case 2:
            return Degree;
        case 3:
            return Semester;
        case 4:
            return Course;
        case 5:
            return Discipline;
        case 6:
            return Year;
        case 7:
            return Marks;
    }
}

int &CourseMap::operator[](const std::string& str) {
    if (str == "ID") return this->ID;
    else if (str == "Degree") return this->Degree;
    else if (str == "Semester") return this->Semester;
    else if (str == "Course") return this->Course;
    else if (str == "Discipline") return this->Discipline;
    else if (str == "Year") return this->Year;
    else if (str == "Marks") return this->Marks;
}
