#ifndef PROJECT_V2_STUDENTHELPER_H
#define PROJECT_V2_STUDENTHELPER_H

#include <string>
#include <vector>
#include "StudentMap.h"
#include "Student.h"
#include "Cities.h"
#include "StudentErrorFlag.h"

void splitStudentName(const std::string&, const std::string&, std::string&, const std::string&);

[[nodiscard]] std::string extractCityName(CityList& cityList, std::string&);

[[nodiscard]] StudentMap* buildStudentMap(std::vector<std::string>&);

[[nodiscard]] Student& buildStudentFromMap(StudentMap&, std::vector<std::string>&);

[[nodiscard]] bool checkStudentForInvalidData(Student&, StudentErrorFlag);

void fixStudentWithInvalidData(Student&, StudentErrorFlag);

#endif //PROJECT_V2_STUDENTHELPER_H