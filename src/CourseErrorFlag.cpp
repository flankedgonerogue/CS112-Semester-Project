#include <string>
#include "../include/CourseErrorFlag.h"

std::string CourseErrorFlagToString(CourseErrorFlag flags) {
    switch (flags) {
        case IDE:
            return "ID";
        case Degree:
            return "Degree";
        case Semester:
            return "Semester";
        case CourseE:
            return "Course";
        case Discipline:
            return "Discipline";
        case Year:
            return "Year";
        case Marks:
            return "Marks";
    }
}