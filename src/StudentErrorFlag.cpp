#include <string>
#include "../include/StudentErrorFlag.h"

std::string StudentErrorFlagToString(enum StudentErrorFlag flag) {
    switch (flag) {
        case ID:
            return "ID";
        case FirstName:
            return "FirstName";
        case LastName:
            return "LastName";
        case FatherName:
            return "FatherName";
        case Gender:
            return "Gender";
        case Address:
            return "Address";
        case City:
            return "City";
        case DateOfBirth:
            return "Date of Birth";
        case RegistrationDate:
            return "Registration Date";
        case RegistrationStatus:
            return "Registration Status";
        case DegreeStatus:
            return "Degree Status";
        case LastDegree:
            return "Last Degree";
        case Date:
            return "Date";
    }
}