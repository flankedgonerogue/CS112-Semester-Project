#include "../include/CourseList.h"
#include "../include/CSV.h"
#include "../include/CourseHelper.h"
#include "../include/Miscellaneous.h"
#include "../include/CourseErrorFlag.h"

void CourseList::addDataFromFile(const std::string& path) {
    Flanky::CSVFile file(path);

    auto map = buildCourseMap(file.getLine(0));

    for (int i = 1; i < file.getNumberOfLines(); i++) {
        auto Course = buildCourseFromMap(*map, file.getLine(i));
        this->list.push_back(Course);
    }

    delete map;
}

void CourseList::addDataFromFiles(const std::vector<std::string>& paths) {
    // Breaking condition for recursive calls
    if (paths.empty()) return;

    Flanky::CSVFile* file;

    try {
        file = new Flanky::CSVFile(paths[0]);
    } catch (std::exception& e) {
        Flanky::print(e.what(), Flanky::Color::RED);
        exit(-2);
    }

    // Get a map from the first file
    auto firstFieldsFromFirstFile = file->getLine(0);
    auto map = buildCourseMap(firstFieldsFromFirstFile);

    // Get data from the first file
    for (int i = 1; i < file->getNumberOfLines(); i++) {
        auto Course = buildCourseFromMap(*map, file->getLine(i));
        this->list.push_back(Course);
    }

    delete file;

    std::vector<std::string> differentMapPaths;

    // Get data from all the files using a map from the first file
    for (int i = 1; i < paths.size(); i++) {
        try {
            file = new Flanky::CSVFile(paths[i]);
        } catch (std::exception& e) {
            Flanky::print(e.what(), Flanky::Color::RED);
            exit(-2);
        }


        // Check if the fields are the same as the first file in the file being read.
        auto firstFieldsFromOtherFiles = file->getLine(0);
        bool areFieldsSame = true;
        for (int j = 0; j < firstFieldsFromOtherFiles.size(); j++) {
            if (firstFieldsFromOtherFiles[j] != firstFieldsFromFirstFile[j]) {
                areFieldsSame = false;
                break;
            }
        }

        // If they aren't, add those paths to a different list.
        if (!areFieldsSame) {
            differentMapPaths.push_back(paths[i]);
            continue;
        }

        for (int j = 1; j < file->getNumberOfLines(); j++) {
            auto Course = buildCourseFromMap(*map, file->getLine(j));
            this->list.push_back(Course);
        }

        delete file;
    }

    // Add data from files which have different maps
    addDataFromFiles(differentMapPaths);
}

std::list<Course*>* CourseList::getCoursesWithInvalidData(CourseErrorFlag flag) {
    auto subList = new std::list<Course*>;

    for (auto& course : this->list) {
        if (checkCourseForInvalidData(course, flag)) subList->push_back(&course);
    }

    return subList;
}

void CourseList::writeToFile(const std::string& path) const {
    std::ofstream file(path);

    if (!file.is_open()) throw std::fstream::failure("Failed to open file to write course data");

    file << this->toFormattedString();

    file.flush();

    file.close();
}

std::string CourseList::getUniqueValuesString() const {
    std::vector<int> cIDs, cYears, cMarks;
    std::vector<std::string> cDegrees, cSemesters, cCourses, cDisciplines;
    for (const auto& item : this->list) {
        bool isUnique1 = true, isUnique2 = true, isUnique3 = true, isUnique4 = true, isUnique5 = true, isUnique6 = true, isUnique7 = true;

        for (const auto& item1 : cIDs) {
            if (item.ID == item1) {
                isUnique1 = false;
                break;
            }
        }
        for (const auto& item1 : cDegrees) {
            if (item.Degree == item1) {
                isUnique2 = false;
                break;
            }
        }
        for (const auto& item1 : cSemesters) {
            if (item.Semester == item1) {
                isUnique3 = false;
                break;
            }
        }
        for (const auto& item1 : cCourses) {
            if (item.Course == item1) {
                isUnique4 = false;
                break;
            }
        }
        for (const auto& item1 : cDisciplines) {
            if (item.Discipline == item1) {
                isUnique5 = false;
                break;
            }
        }
        for (const auto& item1 : cYears) {
            if (item.Year == item1) {
                isUnique6 = false;
                break;
            }
        }
        for (const auto& item1 : cMarks) {
            if (item.Marks == item1) {
                isUnique7 = false;
                break;
            }
        }


        if (isUnique1) cIDs.push_back(item.ID);
        if (isUnique2) cDegrees.push_back(item.Degree);
        if (isUnique3) cSemesters.push_back(item.Semester);
        if (isUnique4) cCourses.push_back(item.Course);
        if (isUnique5) cDisciplines.push_back(item.Discipline);
        if (isUnique6) cYears.push_back(item.Year);
        if (isUnique7) cMarks.push_back(item.Marks);
    }

    std::string str;

    str.append("Unique values for course field ID: " + std::to_string(cIDs.size()) + '\n');
    str.append("Unique values for course field Degree: " + std::to_string(cDegrees.size()) + '\n');
    str.append("Unique values for course field Semester: " + std::to_string(cSemesters.size()) + '\n');
    str.append("Unique values for course field Course: " + std::to_string(cCourses.size()) + '\n');
    str.append("Unique values for course field Discipline: " + std::to_string(cDisciplines.size()) + '\n');
    str.append("Unique values for course field Year: " + std::to_string(cYears.size()) + '\n');
    str.append("Unique values for course field Marks: " + std::to_string(cMarks.size()) + '\n');

    return str;
}

std::string CourseList::getInvalidValuesString() {
    std::string str;

    for (int i = 0; i != CourseErrorFlag::Marks; i++ )
    {
        auto flag = static_cast<CourseErrorFlag>(i);
        auto data = this->getCoursesWithInvalidData(flag);

        str.append("Invalid data for course field " + CourseErrorFlagToString(flag) + ": " + std::to_string(data->size()) + '\n');

        delete data;
    }

    return str;
}

std::string CourseList::toFormattedString() const {
    std::string str;
    for (const auto& item : this->list) {
        str += item.toFormattedString() + '\n';
    }

    return str;
}

int CourseList::getSize() const {
    return (int) this->list.size();
}

std::vector<Course>& CourseList::getList() {
    return this->list;
}
