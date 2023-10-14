#ifndef PROJECT_V2_STUDENTERRORFLAG_H
#define PROJECT_V2_STUDENTERRORFLAG_H

enum StudentErrorFlag {
    ID,
    FirstName,
    LastName,
    FatherName,
    Gender,
    Address,
    City,
    DateOfBirth,
    RegistrationDate,
    RegistrationStatus,
    DegreeStatus,
    LastDegree,
    Date
};

std::string StudentErrorFlagToString(StudentErrorFlag);

#endif //PROJECT_V2_STUDENTERRORFLAG_H