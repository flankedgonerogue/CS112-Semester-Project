#include "../include/StudentMap.h"

int &StudentMap::operator[](const int i) {
    switch (i) {
        case 1:
            return ID;
        case 2:
            return StudentName;
        case 3:
            return FatherName;
        case 4:
            return Gender;
        case 5:
            return Address;
        case 6:
            return DateOfBirth;
        case 7:
            return RegistrationDate;
        case 8:
            return RegistrationStatus;
        case 9:
            return DegreeStatus;
        case 10:
            return LastDegree;
        default:
            throw std::exception();
    }
}

int &StudentMap::operator[](const std::string& str) {
    if (str == "ID")
        return ID;
    else if (str == "StudentName")
        return StudentName;
    else if (str == "FatherName")
        return FatherName;
    else if (str == "Gender")
        return Gender;
    else if (str == "Address")
        return Address;
    else if (str == "DateOfBirth")
        return DateOfBirth;
    else if (str == "RegistrationDate")
        return RegistrationDate;
    else if (str == "RegistrationStatus")
        return RegistrationStatus;
    else if (str == "DegreeStatus")
        return DegreeStatus;
    else if (str == "LastDegree")
        return LastDegree;
}
