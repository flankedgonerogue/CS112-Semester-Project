#ifndef PROJECT_V2_COURSELIST_H
#define PROJECT_V2_COURSELIST_H


#include <list>
#include <vector>
#include "Course.h"
#include "CourseErrorFlag.h"

class CourseList {
private:
    std::vector<Course> list;
public:
    CourseList() = default;

    void addDataFromFile(const std::string&);

    void addDataFromFiles(const std::vector<std::string>&);

    std::list<Course*>* getCoursesWithInvalidData(CourseErrorFlag);

    void writeToFile(const std::string&) const;

    [[nodiscard]] std::string getUniqueValuesString() const;

    [[nodiscard]] std::string toFormattedString() const;

    [[nodiscard]] int getSize() const;

    [[nodiscard]] std::vector<Course>& getList();

    std::string getInvalidValuesString();
};


#endif //PROJECT_V2_COURSELIST_H