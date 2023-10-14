#include "../include/StudentHelper.h"
#include "../include/Miscellaneous.h"
#include "../include/Cities.h"

#include <string>

using namespace std;

void splitStudentName(const std::string& studentName, std::string& fatherName, string& firstName, string& lastName) {
// Splitting student and father name into parts
    auto studentNameParts = Flanky::splitString(studentName, ' ');
    auto fatherNameParts = Flanky::splitString(fatherName, ' ');


// Putting strings in first if they are not found in father name
    for (auto &studentNamePart: studentNameParts) {
        bool isNotInFatherName = true;

        for (auto &fatherNamePart: fatherNameParts) {
            if (studentNamePart == fatherNamePart) {
                isNotInFatherName = false;
                continue;
            }
        }

        if (isNotInFatherName) firstName += studentNamePart + ' ';
        else lastName += studentNamePart + ' ';
    }

    Flanky::trimString(firstName);

// If the names are the same, will be marked as ERROR.
    if (firstName.empty()) {
        firstName = "ERROR";
        lastName = "ERROR";
    }
}

std::string extractCityName(CityList& cityList,std::string& address) {
    string addressLowerCase = address;
    Flanky::toLowerCase(addressLowerCase);
    auto addressParts = Flanky::splitString(addressLowerCase, ' ');
    auto size = addressParts.size();
    string str = addressParts[size - 1];
    int pos;

    // We do not check the city names because we are matching lower case strings only found in the aliases

    // Check the last part
    pos = cityList.findAlias(str);
    if (pos != -1) {
        return cityList.getCity(pos);
    }

    // Check the last two parts
    str = addressParts[size - 2] + ' ' + addressParts[size - 1];
    // Check aliases
    pos = cityList.findAlias(str);
    if (pos != -1) {
        return cityList.getCity(pos);
    }

    // Check the last three parts
    // Check city names
    str = addressParts[size - 3] + ' ' + addressParts[size - 2] + ' ' + addressParts[size - 1];
    // Check aliases
    pos = cityList.findAlias(str);
    if (pos != -1) {
        return cityList.getCity(pos);
    }

    return "ERROR";
}

StudentMap* buildStudentMap(std::vector<std::string>& fields) {
    auto map = new StudentMap;

    Flanky::print("Please map the columns for standardization.\n");

    // Fill the map
    for (int i = 0; i < fields.size(); i++) {
        // Print list
        Flanky::print("Map " + fields[i] + " to:\n");
        if ((*map)[1] == -1) Flanky::print("    1. ID\n");
        if ((*map)[2] == -1) Flanky::print("    2. Student Name\n");
        if ((*map)[3] == -1) Flanky::print("    3. Father Name\n");
        if ((*map)[4] == -1) Flanky::print("    4. Gender\n");
        if ((*map)[5] == -1) Flanky::print("    5. Address\n");
        if ((*map)[6] == -1) Flanky::print("    6. Date Of Birth\n");
        if ((*map)[7] == -1) Flanky::print("    7. Registration Date\n");
        if ((*map)[8] == -1) Flanky::print("    8. Registration Status\n");
        if ((*map)[9] == -1) Flanky::print("    9. Degree Status\n");
        if ((*map)[10] == -1) Flanky::print("    10. Last Degree\n");

        // Get input
        int input = Flanky::getInput<int>("Enter number: ");

        // Handle incorrect input
        if (input < 1 || input > 10) {
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

    Flanky::print(
            "Mapping " + (((*map)[1] == -1) ? "MISSING" : fields[(*map)[1]]) + " -> ID\n" +
            "Mapping " + (((*map)[2] == -1) ? "MISSING" : fields[(*map)[2]]) + " -> Student Name\n" +
            "Mapping " + (((*map)[3] == -1) ? "MISSING" : fields[(*map)[3]]) + " -> Father Name\n" +
            "Mapping " + (((*map)[4] == -1) ? "MISSING" : fields[(*map)[4]]) + " -> Gender\n" +
            "Mapping " + (((*map)[5] == -1) ? "MISSING" : fields[(*map)[5]]) + " -> Address\n" +
            "Mapping " + (((*map)[6] == -1) ? "MISSING" : fields[(*map)[6]]) + " -> Date Of Birth\n" +
            "Mapping " + (((*map)[7] == -1) ? "MISSING" : fields[(*map)[7]]) + " -> Registration Date\n" +
            "Mapping " + (((*map)[8] == -1) ? "MISSING" : fields[(*map)[8]]) + " -> Registration Status\n" +
            "Mapping " + (((*map)[9] == -1) ? "MISSING" : fields[(*map)[9]]) + " -> Degree Status\n" +
            "Mapping " + (((*map)[10] == -1) ? "MISSING" : fields[(*map)[10]]) + " -> Last Degree\n",
            Flanky::Color::CYAN);

    // Calls the function again if the user entered incorrect mapping
    if (!Flanky::prompt("Is this mapping correct")) {
        delete map;
        return buildStudentMap(fields);
    }

    return map;
}

[[nodiscard]] Student& buildStudentFromMap(StudentMap& map, std::vector<std::string>& fields) {
    auto student = new Student;

    // ID
    try {
        if (map[1] == -1) throw std::exception();

        if (fields[map[1]].empty()) throw std::exception();

        student->ID = stoi(fields[map[1]]);
    } catch (...) {
        // Ignore exception
        student->ID = -1;
    }

    // First and Last Name
    try {
        if (map[2] == -1) throw std::exception();
        if (map[3] == -1) throw std::exception();

        splitStudentName(fields[map[2]], fields[map[3]], student->FirstName, student->LastName);
    } catch (...) {
        student->FirstName = "ERROR";
        student->LastName = "ERROR";
    }


    // Father Name
    try {
        if (map[3] == -1) throw std::exception();

        student->FatherName = fields[map[3]];
    } catch (...) {
        student->FatherName = "ERROR";
    }

    // Gender
    try {
        if (map[4] == -1) throw std::exception();

        if (fields[map[4]] == "0") student->Gender = 'M';
        else if (fields[map[4]] == "1") student->Gender = 'F';
        else student->Gender = 'U';
    } catch (...) {
        student->Gender = 'U';
    }

    // Address
    try {
        if (map[5] == -1) throw std::exception();

        if (fields[map[5]].empty()) student->Address = "ERROR";
        else student->Address = fields[map[5]];
    } catch (...) {
        student->Address = "ERROR";
    }

    student->City = "ERROR";

    // Date of birth
    try {
        if (map[6] == -1) throw std::exception();

        student->DateOfBirth = Flanky::Date(fields[map[6]]);
    } catch (...) {}

    // Registration Date
    try {
        if (map[7] == -1) throw std::exception();

        student->RegistrationDate = Flanky::Date(fields[map[7]]);
    } catch (...) {}

    // Registration status
    try {
        if (map[8] == -1) throw std::exception();

        if (fields[map[8]] == "A") student->RegistrationStatus = 'A';
        else if (fields[map[8]] == "T") student->RegistrationStatus = 'T';
        else student->RegistrationStatus = 'U';
    } catch (...) {
        student->RegistrationStatus = 'U';
    }


    // Degree status
    try {
        if (map[9] == -1) throw std::exception();

        if (fields[map[9]] == "C") student->DegreeStatus = 'C';
        else if (fields[map[9]] == "I") student->DegreeStatus = 'I';
        else student->DegreeStatus = 'U';
    } catch (...) {
        student->RegistrationStatus = 'U';
    }


    // Last Degree
    try {
        if (map[10] == -1) throw std::exception();

        if (fields[map[10]].empty()) student->LastDegree = "ERROR";
        else student->LastDegree = fields[map[10]];
    } catch (...) {
        student->LastDegree = "ERROR";
    }

    student->sourceCity = map.city;

    return *student;
}

bool checkStudentForInvalidData(Student& student, StudentErrorFlag flag) {
    switch (flag) {
        case ID:
            // Check for ID
            if (student.ID == -1) {
                return true;
            }
            break;
        case FirstName:
            // Check for FirstName
            if (student.FirstName == "ERROR") {
                return true;
            }
            break;
        case LastName:
            // Check for LastName
            if (student.LastName == "ERROR") {
                return true;
            }
            break;
        case FatherName:
            // Check for FatherName
            if (student.FatherName == "ERROR") {
                return true;
            }
            break;
        case Gender:
            // Check for Gender
            if (student.Gender == 'U') {
                return true;
            }
            break;
        case Address:
            // Check for Address
            if (student.Address == "ERROR") {
                return true;
            }
            break;
        case City:
            // Check for City
            if (student.City == "ERROR") {
                return true;
            }
            break;
        case DateOfBirth:
        // Check for DateOfBirth
            if (!student.DateOfBirth.isValidDate()) {
                return true;
            }
            break;
        case RegistrationDate:
            // Check for RegistrationDate
            if (!student.RegistrationDate.isValidDate()) {
                return true;
            }
            break;
        case RegistrationStatus:
            // Check for RegistrationStatus
            if (student.RegistrationStatus == 'U') {
                return true;
            }
            break;
        case DegreeStatus:
            // Check for DegreeStatus
            if (student.DegreeStatus == 'U') {
                return true;
            }
            break;
        case LastDegree:
            // Check for LastDegree
            if (student.LastDegree == "ERROR") {
                return true;
            }
            break;
        case Date:
            // Check for Date issues
            if (student.DateOfBirth > student.RegistrationDate) {
                return true;
            }
            break;
    }

    return false;
}

void fixStudentWithInvalidData(Student& student, StudentErrorFlag flag) {
    string str;
    switch (flag) {
        case ID:
            student.ID = Flanky::getInput<int>("Enter a new ID (-1 to SKIP): ");
            if (student. ID == -1) return;
            while (!Flanky::prompt("Is this correct")) {
                student.ID = Flanky::getInput<int>("Enter a new ID (-1 to SKIP): ");
                if (student. ID == -1) return;
            }

            break;
        case FirstName:
            student.FirstName = Flanky::getInputString("Enter first name (0 to SKIP): ");
            if (student.FirstName == "0") {
                student.FirstName = "ERROR";
                return;
            }

            while (!Flanky::prompt("Is this correct")) {
                student.FirstName = Flanky::getInputString("Enter first name (0 to SKIP): ");
                if (student.FirstName == "0") {
                    student.FirstName = "ERROR";
                    return;
                }
            }

            break;
        case LastName:
            student.LastName = Flanky::getInputString("Enter last name (0 to SKIP): ");
            if (student.LastName == "0") {
                student.LastName = "ERROR";
                return;
            }

            while (!Flanky::prompt("Is this correct")) {
                student.LastName = Flanky::getInputString("Enter last name (0 to SKIP): ");
                if (student.LastName == "0") {
                    student.LastName = "ERROR";
                    return;
                }
            }

            break;
        case FatherName:
            student.FatherName = Flanky::getInputString("Enter father name (0 to SKIP): ");
            if (student.FatherName == "0") {
                student.FatherName = "ERROR";
                return;
            }

            while (!Flanky::prompt("Is this correct")) {
                student.FatherName = Flanky::getInputString("Enter father name (0 to SKIP): ");
                if (student.FatherName == "0") {
                    student.FatherName = "ERROR";
                    return;
                }
            }

            break;
        case Gender:
            student.Gender = Flanky::getInput<char>("Enter gender (M/F/0 to SKIP): ");
            if (student.Gender == '0') {
                student.Gender = 'U';
                return;
            }

            while (!Flanky::prompt("Is this correct") && !(student.Gender == 'M' || student.Gender == 'F')) {
                student.Gender = Flanky::getInput<char>("Enter gender (M/F/0 to SKIP): ");
                if (student.Gender == '0') {
                    student.Gender = 'U';
                    return;
                }
            }

            break;
        case Address:
            student.Address = Flanky::getInputString("Enter Address (0 to SKIP): ");
            if (student.Address == "0") {
                student.Address = "ERROR";
                return;
            }

            while (!Flanky::prompt("Is this correct")) {
                student.Address = Flanky::getInputString("Enter Address (0 to SKIP): ");
                if (student.Address == "0") {
                    student.Address = "ERROR";
                    return;
                }
            }

            break;
        case City:
            student.City = Flanky::getInputString("Enter City (0 to SKIP): ");
            if (student.City == "0") {
                student.City = "ERROR";
                return;
            }

            while (!Flanky::prompt("Is this correct")) {
                student.City = Flanky::getInputString("Enter City (0 to SKIP): ");
                if (student.City == "0") {
                    student.City = "ERROR";
                    return;
                }
            }

            break;
        case DateOfBirth:
            str = Flanky::getInputString("Enter date of birth (Format: 01-Jan-2023/0 to SKIP): ");
            if (str == "0") {
                return;
            }
            student.DateOfBirth = Flanky::Date(str);

            while (!Flanky::prompt("Is this correct") && !student.DateOfBirth.isValidDate()) {
                str = Flanky::getInputString("Enter date of birth (Format: 01-Jan-2023/0 to SKIP): ");
                if (str == "0") {
                    return;
                }
                student.DateOfBirth = Flanky::Date(str);
            }

            break;
        case RegistrationDate:
            str = Flanky::getInputString("Enter registration date (Format: 01-Jan-2023/0 to SKIP): ");
            if (str == "0") {
                return;
            }
            student.RegistrationDate = Flanky::Date(str);

            while (!Flanky::prompt("Is this correct") && !student.RegistrationDate.isValidDate()) {
                str = Flanky::getInputString("Enter registration date (Format: 01-Jan-2023/0 to SKIP): ");
                if (str == "0") {
                    return;
                }
                student.RegistrationDate = Flanky::Date(str);
            }

            break;
        case RegistrationStatus:
            student.RegistrationStatus = Flanky::getInput<char>("Enter gender (A/T/0 to SKIP): ");
            if (student.RegistrationStatus == '0') {
                student.RegistrationStatus = 'U';
                return;
            }

            while (!Flanky::prompt("Is this correct") && !(student.RegistrationStatus == 'A' || student.RegistrationStatus == 'T')) {
                student.RegistrationStatus = Flanky::getInput<char>("Enter gender (A/T/0 to SKIP): ");
                if (student.RegistrationStatus == '0') {
                    student.RegistrationStatus = 'U';
                    return;
                }
            }

            break;
        case DegreeStatus:
            student.DegreeStatus = Flanky::getInput<char>("Enter gender (C/I/0 to SKIP): ");
            if (student.DegreeStatus == '0') {
                student.DegreeStatus = 'U';
                return;
            }

            while (!Flanky::prompt("Is this correct") && !(student.DegreeStatus == 'C' || student.DegreeStatus == 'I')) {
                student.DegreeStatus = Flanky::getInput<char>("Enter gender (C/I/0 to SKIP): ");
                if (student.DegreeStatus == '0') {
                    student.DegreeStatus = 'U';
                    return;
                }
            }
            break;
        case LastDegree:
            student.LastDegree = Flanky::getInputString("Enter City (0 to SKIP): ");
            if (student.LastDegree == "0") {
                student.LastDegree = "ERROR";
                return;
            }

            while (!Flanky::prompt("Is this correct")) {
                student.LastDegree = Flanky::getInputString("Enter City (0 to SKIP): ");
                if (student.LastDegree == "0") {
                    student.LastDegree = "ERROR";
                    return;
                }
            }

            break;
        case Date:
            str = Flanky::getInput<string>("Which date do you want to change (Registration/DateOfBirth/0 to SKIP): ");
            if (str[0] == 'R') {
                fixStudentWithInvalidData(student, StudentErrorFlag::RegistrationDate);
            } else if (str[0] == 'D') {
                fixStudentWithInvalidData(student, StudentErrorFlag::DateOfBirth);
            }

            break;
    }
}
