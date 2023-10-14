#ifndef PROJECT_V2_STUDENTMAP_H
#define PROJECT_V2_STUDENTMAP_H


#include <string>

class StudentMap {
    int ID = -1;
    int StudentName = -1;
    int FatherName = -1;
    int Gender = -1;
    int Address = -1;
    int DateOfBirth = -1;
    int RegistrationDate = -1;
    int RegistrationStatus = -1;
    int DegreeStatus = -1;
    int LastDegree = -1;

public:
    std::string city;
    int& operator[](int i);
    int& operator[](const std::string& str);
};


#endif //PROJECT_V2_STUDENTMAP_H