#include <string>
#include <vector>
#include "../include/Course.h"
#include "../include/CourseMap.h"
#include "../include/Miscellaneous.h"
#include "../include/CourseErrorFlag.h"

CourseMap* buildCourseMap(std::vector<std::string>& fields) {
    auto map = new CourseMap;

    Flanky::print("Please map the columns for standardization.\n");

    // Fill the map
    for (int i = 0; i < fields.size(); i++) {
        // Print list
        Flanky::print("Map " + fields[i] + " to:\n");
        if ((*map)[1] == -1) Flanky::print("    1. ID\n");
        if ((*map)[2] == -1) Flanky::print("    2. Degree\n");
        if ((*map)[3] == -1) Flanky::print("    3. Semester\n");
        if ((*map)[4] == -1) Flanky::print("    4. Course\n");
        if ((*map)[5] == -1) Flanky::print("    5. Discipline\n");
        if ((*map)[6] == -1) Flanky::print("    6. Year\n");
        if ((*map)[7] == -1) Flanky::print("    7. Marks\n");

        // Get input
        int input = Flanky::getInput<int>("Enter number: ");

        // Handle incorrect input
        if (input < 1 || input > 7) {
            Flanky::print("Invalid input range! Please try again.\n", Flanky::Color::RED);
            i--;
            continue;
        }

        // Set mapping, if already set -> print error
        if ((*map)[input] == -1) {
            (*map)[input] = i;
        } else {
            Flanky::print("Already mapped to this value, please try another value.\n", Flanky::Color::RED);
            i--;
        }
    }

    auto city = Flanky::getInputString("Which city is this data for? ");
    Flanky::toLowerCase(city);
    map->city = city;
    while (!Flanky::prompt("Is this correct")) {
        city = Flanky::getInputString("Which city is this data for? ");
        Flanky::toLowerCase(city);
        map->city = city;
    }

    Flanky::print("The selected mapping is:\n");

    std::string str;
    str.append("Mapping ").append(((*map)[1] == -1) ? "MISSING" : fields[(*map)[1]]).append(" -> ID\n")
    .append("Mapping ").append(((*map)[2] == -1) ? "MISSING" : fields[(*map)[2]]).append(" -> Degree\n")
    .append("Mapping ").append(((*map)[3] == -1) ? "MISSING" : fields[(*map)[3]]).append(" -> Semester\n")
    .append("Mapping ").append(((*map)[4] == -1) ? "MISSING" : fields[(*map)[4]]).append(" -> Course\n")
    .append("Mapping ").append(((*map)[5] == -1) ? "MISSING" : fields[(*map)[5]]).append(" -> Discipline\n")
    .append( "Mapping ").append(((*map)[6] == -1) ? "MISSING" : fields[(*map)[6]]).append(" -> Year\n")
    .append("Mapping ").append(((*map)[7] == -1) ? "MISSING" : fields[(*map)[7]]).append(" -> Marks\n");

    Flanky::print(str, Flanky::CYAN);

    if (!Flanky::prompt("Is the mapping correct")) {
        delete map;
        return buildCourseMap(fields);
    }

    return map;
}

[[nodiscard]] Course& buildCourseFromMap(CourseMap& map, std::vector<std::string>& fields) {
    auto course = new Course;

    // ID
    try {
        course->ID = stoi(fields[map["ID"]]);
    } catch (...) {
        // Ignore exception
        course->ID = -1;
    }

    // Degree
    try {
        if (map["Degree"] == -1) course->Degree = fields[map["Discipline"]].length() > 2 ? "MS" : "BS";
        else course->Degree = fields[map["Degree"]];
    } catch (...) {
        course->Degree = "ERROR";
    }


    // Semester
    try {
        if (map["Year"] == -1) {
            course->Semester = fields[map["Semester"]].substr(0, fields[map["Semester"]].length() - 2);
        }
        else {
            course->Semester = fields[map["Semester"]];
        }
    } catch (...) {
        course->Semester = "ERROR";
    }


    // Course
    try {
        course->Course  = fields[map["Course"]];
    } catch (...) {
        course->Course = "ERROR";
    }

    // Discipline
    try {
        course->Discipline = fields[map["Discipline"]];
    } catch (...) {
        course->Discipline = "ERROR";
    }


    // Year
    try {
        if (map["Year"] == -1) {
            course->Year = stoi(fields[map["Semester"]].substr(fields[map["Semester"]].length() - 2));

            if (course->Year > 50) course->Year += 1900;
            else course->Year += 2000;
        }
        else {
            course->Year = stoi(fields[map["Year"]]);
        }
    } catch (...) {
        course->Year = -1;
    }

    // Marks
    try {
        course->Marks = stoi(fields[map["Marks"]]);

        if (course->Marks > 100 || course->Marks < 0) course->Marks = -1;
    } catch (...) {
        // Ignore exception
        course->Marks = -1;
    }

    course->sourceCity = map.city;

    return *course;
}

bool checkCourseForInvalidData(const Course& course, const CourseErrorFlag flag) {
    switch (flag) {
        case IDE:
            // Check for ID
            if (course.ID == -1) {
                return true;
            }

            break;
        case Degree:
            // Check for Degree
            if (course.Degree == "ERROR") {
                return true;
            }

            break;
        case Semester:
            // Check for Semester
            if (course.Semester == "ERROR") {
                return true;
            }

            break;
        case CourseE:
            // Check for Course
            if (course.Course == "ERROR") {
                return true;
            }

            break;
        case Discipline:
            // Check for Discipline
            if (course.Discipline == "ERROR") {
                return true;
            }

            break;
        case Year:
            // Check for Year
            if (course.Year == -1) {
                return true;
            }

            break;
        case Marks:
            // Check for Marks
            if (course.Marks == -1) {
                return true;
            }

            break;
    }

    return false;
}

void fixCourseWithInvalidData(Course& course, CourseErrorFlag flag) {
    switch (flag) {
        case IDE:
            course.ID = Flanky::getInput<int>("Enter a new ID (-1 to SKIP): ");
            if (course. ID == -1) return;
            while (!Flanky::prompt("Is this correct")) {
                course.ID = Flanky::getInput<int>("Enter a new ID (-1 to SKIP): ");
                if (course. ID == -1) return;
            }

            break;
        case Degree:
            course.Degree = Flanky::getInputString("Enter Degree (0 to SKIP): ");
            if (course.Degree == "0") {
                course.Degree = "ERROR";
                return;
            }

            while (!Flanky::prompt("Is this correct")) {
                course.Degree = Flanky::getInputString("Enter Degree (0 to SKIP): ");
                if (course.Degree == "0") {
                    course.Degree = "ERROR";
                    return;
                }
            }

            break;
        case Semester:
            course.Semester = Flanky::getInputString("Enter Semester (0 to SKIP): ");
            if (course.Semester == "0") {
                course.Semester = "ERROR";
                return;
            }

            while (!Flanky::prompt("Is this correct")) {
                course.Semester = Flanky::getInputString("Enter Semester (0 to SKIP): ");
                if (course.Semester == "0") {
                    course.Semester = "ERROR";
                    return;
                }
            }

            break;
        case CourseE:
            course.Course = Flanky::getInputString("Enter Course (0 to SKIP): ");
            if (course.Course == "0") {
                course.Course = "ERROR";
                return;
            }

            while (!Flanky::prompt("Is this correct")) {
                course.Course = Flanky::getInputString("Enter Course (0 to SKIP): ");
                if (course.Course == "0") {
                    course.Course = "ERROR";
                    return;
                }
            }

            break;
        case Discipline:
            course.Discipline = Flanky::getInputString("Enter Discipline (0 to SKIP): ");
            if (course.Discipline == "0") {
                course.Discipline = "ERROR";
                return;
            }

            while (!Flanky::prompt("Is this correct")) {
                course.Discipline = Flanky::getInputString("Enter Discipline (0 to SKIP): ");
                if (course.Discipline == "0") {
                    course.Discipline = "ERROR";
                    return;
                }
            }

            break;
        case Year:
            course.Year = Flanky::getInput<int>("Enter a new Year (-1 to SKIP): ");
            if (course. Year == -1) return;
            while (!Flanky::prompt("Is this correct")) {
                course.Year = Flanky::getInput<int>("Enter a new Year (-1 to SKIP): ");
                if (course. Year == -1) return;
            }

            break;
        case Marks:
            course.Marks = Flanky::getInput<int>("Enter a new Marks (-1 to SKIP): ");
            if (course. Marks == -1) return;
            while (!Flanky::prompt("Is this correct")) {
                course.Marks = Flanky::getInput<int>("Enter a new Marks (-1 to SKIP): ");
                if (course. Marks == -1) return;
            }

            break;
    }
}