#include "include/Miscellaneous.h"
#include "include/Cities.h"
#include "include/StudentList.h"
#include "include/CourseList.h"
#include "include/StudentHelper.h"
#include "include/CourseHelper.h"
#include "include/CourseErrorFlag.h"

void printMenu() {
    Flanky::print("-- Student Data Analyzer 6000 --\n");
    Flanky::print("   1. Add data from file\n");
    Flanky::print("   2. Add data from folder\n");
    Flanky::print("   3. Fill city data\n");
    Flanky::print("   4. Fill gender data\n");
    Flanky::print("   5. Fix data with errors\n");
    Flanky::print("   6. Profile data\n");
    Flanky::print("   7. View data\n");
    Flanky::print("   8. Write data to file\n");
    Flanky::print("   9. Exit\n");
}

void handleOptionOne(StudentList& sList, CourseList& cList) {
    auto path = Flanky::getInputString("Enter path: ");

    if (path.empty()) return;

    if (!Flanky::checkIfFileExists(path)) {
        Flanky::print("Invalid file path.", Flanky::Color::RED);
        return;
    }

    auto input = Flanky::getInputString("Select datatype (Course/Student): ");

    if (tolower(input[0]) == 'c') {
        cList.addDataFromFile(path);
    } else if (tolower(input[0]) == 's') {
        sList.addDataFromFile(path);
    }

}

void handleOptionTwo(StudentList& sList, CourseList& cList) {
    Flanky::print("Note: Data in the directory must be of the same type!\n", Flanky::Color::YELLOW);

    auto input = Flanky::getInputString("Enter directory path: ");

    if (input.empty()) return;

    auto paths = Flanky::getFilePathsFromFolder(input);

    if (paths.empty()) {
        Flanky::print("This directory is empty.", Flanky::Color::RED);
        return;
    }

    input = Flanky::getInputString("Select datatype (Course/Student): ");

    if (tolower(input[0]) == 'c') {
        cList.addDataFromFiles(paths);
    } else if (tolower(input[0]) == 's') {
        sList.addDataFromFiles(paths);
    }

}

void handleOptionThree(StudentList& sList, CityList& cityList) {
    if (sList.getSize() == 0) {
        Flanky::print("Student list is empty. Please add students first.\n", Flanky::Color::RED);
        return;
    }

    if (cityList.isEmpty()) {
        // Search in default location
        cityList.loadCitiesFromFile("CityData.txt");

        if (cityList.isEmpty()) {
            Flanky::print("Unable to load city list. Application will proceed by you will have to enter all cities from beginning.", Flanky::Color::YELLOW);
        }
    }

    sList.fillCityData(cityList);

    cityList.writeCitiesToFile("CityData.txt");
}

void handleOptionFour(StudentList& sList) {
    if (sList.getSize() == 0) {
        Flanky::print("Student list is empty. Please add students first.\n", Flanky::Color::RED);
        return;
    }


    sList.fillGenderData();
}

void handleOptionFive(StudentList& sList, CourseList& cList) {
    for (int i = StudentErrorFlag::ID; i != StudentErrorFlag::LastDegree; i++ )
    {
        auto flag = static_cast<StudentErrorFlag>(i);
        auto list = sList.getStudentsWithInvalidData(flag);

        if (list->empty()) continue;

        Flanky::print("The following students have error in data " + StudentErrorFlagToString(flag) + '\n');
        for (auto* item : *list) {
            Flanky::print(item->toFormattedString() + '\n');
        }

        if (Flanky::prompt("Do you want to fix the data")) {
            for (auto* item : *list) {
                Flanky::print(item->toFormattedString() + '\n');
                fixStudentWithInvalidData(*item, flag);
            }
        }

        delete list;
    }

    for (int i = ID; i != CourseErrorFlag::Marks; i++ )
    {
        auto flag = static_cast<CourseErrorFlag>(i);
        auto list = cList.getCoursesWithInvalidData(flag);

        if (list->empty()) continue;

        Flanky::print("The following courses have error in data " + CourseErrorFlagToString(flag) + '\n');
        for (auto* item : *list) {
            Flanky::print(item->toFormattedString() + '\n');
        }

        if (Flanky::prompt("Do you want to fix the data")) {
            for (auto* item : *list) {
                Flanky::print(item->toFormattedString() + '\n');
                fixCourseWithInvalidData(*item, flag);
            }
        }

        delete list;
    }
}

void handleOptionSix(StudentList& sList, CourseList& cList) {
    Flanky::print(sList.getUniqueValuesString(),Flanky::Color::CYAN);
    Flanky::print(cList.getUniqueValuesString(),Flanky::Color::CYAN);

    Flanky::print("All null values were replaced with invalid values so no null values.\n", Flanky::Color::YELLOW);

    Flanky::print(sList.getInvalidValuesString(),Flanky::Color::CYAN);
    Flanky::print(cList.getInvalidValuesString(),Flanky::Color::CYAN);


    Flanky::print("Total Courses: " + std::to_string(cList.getSize()) + '\n');

    Flanky::print(sList.getMaleVsFemaleString());

    int numOfPeople = 0;
    std::unordered_map<int, int> coursePerSemester{{},{}};
    for (const auto& item1 : cList.getList())
        coursePerSemester[(item1.ID + item1.Year) * (item1.Semester == "Fall" ? 1000 : 2000)]++;

    for (const auto& item1 : coursePerSemester) {
        if (item1.second > 5) numOfPeople++;
    }
    Flanky::print("Total number of people taking more than 5 courses: " + std::to_string(numOfPeople) + '\n');

    Flanky::print("Unique IDs: " + std::to_string(sList.getUniqueValues()["ID"]) + " Total Students: " + std::to_string(sList.getSize()) + '\n');

    std::unordered_map<std::string, int> peopleCountsPerUni{{},{}};

    for (const auto& item : sList.getList())
        peopleCountsPerUni[item.sourceCity]++;

    std::unordered_map<std::string, int> yearMax, yearMin, yearDiff, semCount;
    for (const auto& item : cList.getList()) {
        if (item.Year > yearMax[item.sourceCity]) yearMax[item.sourceCity] = item.Year;
        if ((item.Year < yearMin[item.sourceCity] && item.Year != -1) || yearMin[item.sourceCity] == 0) yearMin[item.sourceCity] = item.Year;
        yearDiff[item.sourceCity] = 0;
    }

    for (auto& map : yearDiff) {
        map.second = yearMax[map.first] - yearMin[map.first];
        semCount[map.first] = map.second * 2;

        auto a = map.first;
        auto b = map.second;
        auto c = semCount[map.first];
        auto d = yearMax[map.first];
        auto e = yearMin[map.first];
        auto f = peopleCountsPerUni[map.first];

        Flanky::print(
                "Average number of students per semester from city " + map.first + ": "
                + std::to_string((double) peopleCountsPerUni[map.first] / (double) semCount[map.first]) + '\n'
                );

        Flanky::print(
                "Average number of students per year from city " + map.first + ": "
                + std::to_string((double) peopleCountsPerUni[map.first] / (double) yearDiff[map.first]) + '\n'
        );
    }

}

void handleOptionSeven(StudentList& sList, CourseList& cList) {
    auto input = Flanky::getInputString("Select datatype (Course/course): ");

    if (tolower(input[0]) == 'c') {
        Flanky::print(cList.toFormattedString());
    } else if (tolower(input[0]) == 's') {
        Flanky::print(sList.toFormattedString());
    }
}

void handleOptionEight(StudentList& sList, CourseList& cList) {
    auto path = Flanky::getInputString("Enter path: ");

    auto input = Flanky::getInputString("Select data type (Courses/Students): ");

    if (tolower(input[0]) == 'c') {
        cList.writeToFile(path);
    } else if (tolower(input[0]) == 's') {
        sList.writeToFile(path);
    }
}

int main() {
    CityList cityList;
    StudentList sList;
    CourseList cList;

    while (true) {
        printMenu();
        auto choice = Flanky::getInput<int>("Select an option: ");

        if (choice == 9) break;

        switch (choice) {
            case 1:
                handleOptionOne(sList, cList);
                break;
            case 2:
                handleOptionTwo(sList, cList);
                break;
            case 3:
                handleOptionThree(sList, cityList);
                break;
            case 4:
                handleOptionFour(sList);
                break;
            case 5:
                handleOptionFive(sList, cList);
                break;
            case 6:
                handleOptionSix(sList, cList);
                break;
            case 7:
                handleOptionSeven(sList, cList);
                break;
            case 8:
                handleOptionEight(sList, cList);
                break;
            default:
                break;
        }
    }

    system("pause");

    return 0;
}