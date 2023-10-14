#include "../include/Student.h"

using namespace std;

string Student::toFormattedString() const {
    string str =
        "ID: " + to_string(this->ID) + ", "
        + "FirstName: " + this->FirstName + ", "
        + "LastName: " + this->LastName + ", "
        + "FatherName: " + this->FatherName + ", "
        + "Gender: " + this->Gender + ", "
        + "Address: " + this->Address + ", "
        + "City: " + this->City + ", "
        + "DateOfBirth: " + this->DateOfBirth.toFormattedString() + ", "
        + "RegistrationDate: " + this->RegistrationDate.toFormattedString() + ", "
        + "RegistrationStatus: " + this->RegistrationStatus + ", "
        + "DegreeStatus: " + this->DegreeStatus + ", "
        + "LastDegree: " + this->LastDegree;

    return str;
}