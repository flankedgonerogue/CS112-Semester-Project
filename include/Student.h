#ifndef PROJECT_V2_STUDENT_H
#define PROJECT_V2_STUDENT_H

#include <string>
#include "Date.h"

class Student {
public:
    int ID{};
    std::string FirstName;
    std::string LastName;
    std::string FatherName;
    char Gender{};
    std::string Address;
    std::string City;
    Flanky::Date DateOfBirth;
    Flanky::Date RegistrationDate;
    char RegistrationStatus{};
    char DegreeStatus{};
    std::string LastDegree;
    std::string sourceCity;

    [[nodiscard]] std::string toFormattedString() const;
};


#endif //PROJECT_V2_STUDENT_H