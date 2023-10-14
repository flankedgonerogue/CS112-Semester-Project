#ifndef PROJECT_V2_STUDENTLIST_H
#define PROJECT_V2_STUDENTLIST_H


#include <list>
#include <vector>
#include <unordered_map>
#include "Student.h"
#include "Cities.h"
#include "StudentErrorFlag.h"

class StudentList {
private:
    std::vector<Student> list;
public:
    StudentList() = default;

    void addDataFromFile(const std::string&);

    void addDataFromFiles(const std::vector<std::string>&);

    void fillCityData(CityList& cityList);

    void fillGenderData();

    std::list<Student*>* getStudentsWithInvalidData(StudentErrorFlag);

    void writeToFile(const std::string&) const;

    [[nodiscard]] std::string getUniqueValuesString() const;

    [[nodiscard]] std::string getInvalidValuesString();

    [[nodiscard]] std::string getMaleVsFemaleString() const;

    [[nodiscard]] std::string toFormattedString() const;

    [[nodiscard]] int getSize() const;

    [[nodiscard]] std::vector<Student>& getList();

    std::unordered_map<std::string, int> getUniqueValues() const;
};


#endif //PROJECT_V2_STUDENTLIST_H