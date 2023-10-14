#include "../include/Course.h"

std::string Course::toFormattedString() const {
    std::string str = "ID: " + std::to_string(ID) + ", "
                 + "Degree: " + Degree + ", "
                 + "Semester: " + Semester + ", "
                 + "Course: " + Course + ", "
                 + "Discipline: " + Discipline + ", "
                 + "Year: " + std::to_string(Year) + ", "
                 + "Marks: " + std::to_string(Marks);

    return str;
}