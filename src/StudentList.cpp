#include "../include/StudentList.h"
#include "../include/StudentHelper.h"
#include "../include/CSV.h"
#include "../include/Miscellaneous.h"

void StudentList::addDataFromFile(const std::string& path) {
    Flanky::CSVFile file(path);

    auto map = buildStudentMap(file.getLine(0));

    for (int i = 1; i < file.getNumberOfLines(); i++) {
        this->list.push_back(buildStudentFromMap(*map, file.getLine(i)));
    }

    delete map;
}

void StudentList::addDataFromFiles(const std::vector<std::string>& paths) {
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
    auto map = buildStudentMap(firstFieldsFromFirstFile);

    // Get data from the first file
    for (int i = 1; i < file->getNumberOfLines(); i++) {
        this->list.push_back(buildStudentFromMap(*map, file->getLine(i)));
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
            this->list.push_back(buildStudentFromMap(*map, file->getLine(j)));
        }

        delete file;
    }

    delete map;

    // Add data from files which have different maps
    addDataFromFiles(differentMapPaths);
}

void StudentList::fillCityData(CityList& cityList) {
    for (auto& student : list) {
        // First, extract city using algorithm and look up file
        auto city = extractCityName(cityList, student.Address);

        if (city == "ERROR") {
            Flanky::print("Failed to extract city from this address: ");
            Flanky::print(student.Address + '\n', Flanky::Color::BLUE);

            if (Flanky::prompt("Do you want to add a new city")) {
                // Add new city
                city = Flanky::getInputString("Enter city name: ");
                while (!Flanky::prompt("Is this correct")) {
                    city = Flanky::getInputString("Enter city name: ");
                }

                std::vector<std::string> aliases;

                if (Flanky::prompt("Do you want to add an alias")) {
                    do {
                        auto input = Flanky::getInputString("Enter alias (0 to EXIT): ");
                        if (input == "0") break;
                        while (!Flanky::prompt("Is this correct")) {
                            if (input == "0") break;
                            input = Flanky::getInputString("Enter alias (0 to EXIT): ");
                        }
                        aliases.push_back(input);
                    }
                    while (Flanky::prompt("Do you want to add another alias"));
                }

                cityList.addCity(city, aliases);
            } else if (Flanky::prompt("Do you want to add an alias for an existing city")) {
                while (true) {
                    auto input = Flanky::getInputString("Please enter city name (0 to EXIT): ");
                    if (input == "0") break;
                    while (!Flanky::prompt("Is this correct")) {
                        if (input == "0") break;
                        input = Flanky::getInputString("Please enter city name (0 to EXIT): ");
                    }

                    auto pos = cityList.findCity(input);

                    // If still not found, try to search in aliases
                    if (pos == -1) {
                        pos = cityList.findAlias(input);
                    }

                    // If not found, try to enter again
                    if (pos == -1) {
                        Flanky::print("No city found matching input, please try again.\n", Flanky::Color::RED);
                        continue;
                    }

                    city = input;
                    break;
                }

                // Add aliases for existing city
                std::vector<std::string> aliases;
                do {
                    auto input = Flanky::getInputString("Enter alias (0 to EXIT): ");
                    if (input == "0") break;
                    while (!Flanky::prompt("Is this correct")) {
                        if (input == "0") break;
                        input = Flanky::getInputString("Enter alias (0 to EXIT): ");
                    }
                    aliases.push_back(input);
                }
                while (Flanky::prompt("Do you want to add another alias"));

                cityList.addCity(city, aliases);
            } else if (Flanky::prompt("Do you want to update the address")) {
                auto input = Flanky::getInputString("Enter address (0 to EXIT): ");
                if (input == "0") break;
                while (!Flanky::prompt("Is this correct")) {
                    if (input == "0") break;
                    input = Flanky::getInputString("Enter address (0 to EXIT): ");
                }

                student.Address = input;
            } else {
                Flanky::print("No changes were made to this address, skipping.\n", Flanky::Color::YELLOW);

                if (!Flanky::prompt("Do you want to move to next address")) break;
            }
        }

        student.City = city;

    }
}

void StudentList::fillGenderData() {
    std::string pathToFolder = Flanky::getCurrentDirectory() + "\\GenderIdentification";

    // Check if required files are present
    if (!Flanky::checkIfFileExists(pathToFolder + "\\script.py", ".py")) {
        Flanky::print("Unable to find python script.\n", Flanky::Color::RED);
        return;
    }
    if (!Flanky::checkIfFileExists(pathToFolder + "\\GenderClassifier.pickle", ".pickle")) {
        Flanky::print("Unable to find pickle file.\n", Flanky::Color::RED);
        return;
    }

    std::ofstream file(pathToFolder + "\\output.txt", std::ios::out);

    std::string outData;
    std::vector<int> posData;

    for (int i = 0; i < list.size(); i++) {
        if (this->list[i].Gender != 'U') continue;

        posData.push_back(i);

        // Write all the students with unknown gender to file GenderIdentification/input.txt
        outData += this->list[i].FirstName + '\n';
    }

    std::ofstream ofile;
    ofile.open(pathToFolder + "\\input.txt");

    // Check if the file has opened
    if (ofile.bad() || !ofile.is_open()) throw std::fstream::failure("Failed to open file for gender identification");

    ofile << outData;
    ofile.flush();
    ofile.close();

    // Execute python script
    std::string command = pathToFolder + "\\script.py " + pathToFolder;
    system(command.c_str());

    // Now read the data
    std::vector<char> genderData;

    std::ifstream ifile;
    ifile.open(pathToFolder + "\\output.txt");

    // Check if the file has opened
    if (ifile.bad() || !ifile.is_open()) throw std::fstream::failure("Failed to open file for gender identification");

    while (!ifile.eof()) {
        std::string line;
        getline(ifile, line, '\n');

        if (line.empty()) continue;

        Flanky::trimString(line);

        if (line == "male") genderData.push_back('M');
        else genderData.push_back('F');
    }
    ifile.close();

    // Set the gender data
    for (int& i : posData) {
        this->list[i].Gender = genderData[i];
    }
}

std::list<Student*>* StudentList::getStudentsWithInvalidData(StudentErrorFlag flag) {
    auto subList = new std::list<Student*>;

    for (auto& student : this->list) {
        if (checkStudentForInvalidData(student, flag)) subList->push_back(&student);
    }

    return subList;
}

void StudentList::writeToFile(const std::string& path) const {
    std::ofstream file(path);

    if (!file.is_open()) throw std::fstream::failure("Failed to open file to write student data");

    file << this->toFormattedString();

    file.flush();

    file.close();
}

std::string StudentList::getUniqueValuesString() const {
    std::vector<int> sIDs;
    std::vector<std::string> sFirstNames, sFatherNames, sLastNames, sAddresses, sCities, sLastDegrees;
    std::vector<char> sGenders, sRegistrationStatuses, sDegreeStatuses;
    std::vector<Flanky::Date> sDateOfBirths, sRegistrationDates;
    for (auto& item : this->list) {
        bool isUnique1 = true, isUnique2 = true, isUnique3 = true, isUnique4 = true, isUnique5 = true, isUnique6 = true, isUnique7 = true, isUnique8 = true, isUnique9 = true, isUnique10 = true, isUnique11 = true, isUnique12 = true;
        for (const auto& item1 : sIDs) {
            if (item.ID == item1) {
                isUnique1 = false;
                break;
            }
        }
        for (const auto& item1 : sFirstNames) {
            if (item.FirstName == item1) {
                isUnique2 = false;
                break;
            }
        }
        for (const auto& item1 : sLastNames) {
            if (item.LastName == item1) {
                isUnique3 = false;
                break;
            }
        }
        for (const auto& item1 : sGenders) {
            if (item.Gender == item1) {
                isUnique4 = false;
                break;
            }
        }
        for (const auto& item1 : sAddresses) {
            if (item.Address == item1) {
                isUnique5 = false;
                break;
            }
        }
        for (const auto& item1 : sCities) {
            if (item.City == item1) {
                isUnique6 = false;
                break;
            }
        }
        for (const auto& item1 : sDateOfBirths) {
            if (item.DateOfBirth == item1) {
                isUnique7 = false;
                break;
            }
        }
        for (const auto& item1 : sRegistrationDates) {
            if (item.RegistrationDate == item1) {
                isUnique8 = false;
                break;
            }
        }
        for (const auto& item1 : sRegistrationStatuses) {
            if (item.RegistrationStatus == item1) {
                isUnique9 = false;
                break;
            }
        }
        for (const auto& item1 : sDegreeStatuses) {
            if (item.DegreeStatus == item1) {
                isUnique10 = false;
                break;
            }
        }
        for (const auto& item1 : sLastDegrees) {
            if (item.LastDegree == item1) {
                isUnique11 = false;
                break;
            }
        }
        for (const auto& item1 : sFatherNames) {
            if (item.FatherName == item1) {
                isUnique12 = false;
                break;
            }
        }

        if (isUnique1) sIDs.push_back(item.ID);
        if (isUnique2) sFirstNames.push_back(item.FirstName);
        if (isUnique3) sLastNames.push_back(item.LastName);
        if (isUnique4) sGenders.push_back(item.Gender);
        if (isUnique5) sAddresses.push_back(item.Address);
        if (isUnique6) sCities.push_back(item.City);
        if (isUnique7) sDateOfBirths.push_back(item.DateOfBirth);
        if (isUnique8) sRegistrationDates.push_back(item.RegistrationDate);
        if (isUnique9) sRegistrationStatuses.push_back(item.RegistrationStatus);
        if (isUnique10) sDegreeStatuses.push_back(item.DegreeStatus);
        if (isUnique11) sLastDegrees.push_back(item.LastDegree);
        if (isUnique12) sFatherNames.push_back(item.FatherName);
    }

    std::string str;

    str.append("Unique values for student field ID: " + std::to_string(sIDs.size()) + '\n');
    str.append("Unique values for student field FirstName: " + std::to_string(sFirstNames.size()) + '\n');
    str.append("Unique values for student field LastName: " + std::to_string(sLastNames.size()) + '\n');
    str.append("Unique values for student field FatherName: " + std::to_string(sFatherNames.size()) + '\n');
    str.append("Unique values for student field Gender: " + std::to_string(sGenders.size()) + '\n');
    str.append("Unique values for student field Address: " + std::to_string(sAddresses.size()) + '\n');
    str.append("Unique values for student field City: " + std::to_string(sCities.size()) + '\n');
    str.append("Unique values for student field DOB: " + std::to_string(sDateOfBirths.size()) + '\n');
    str.append("Unique values for student field RegDate: " + std::to_string(sRegistrationDates.size()) + '\n');
    str.append("Unique values for student field RegistrationStatus: " + std::to_string(sRegistrationStatuses.size()) + '\n');
    str.append("Unique values for student field DegreeStatus: " + std::to_string(sDegreeStatuses.size()) + '\n');
    str.append("Unique values for student field LastDegree: " + std::to_string(sLastDegrees.size()) + '\n');

    return str;
}

std::unordered_map<std::string, int> StudentList::getUniqueValues() const {
    std::vector<int> sIDs;
    std::vector<std::string> sFirstNames, sFatherNames, sLastNames, sAddresses, sCities, sLastDegrees;
    std::vector<char> sGenders, sRegistrationStatuses, sDegreeStatuses;
    std::vector<Flanky::Date> sDateOfBirths, sRegistrationDates;
    for (auto& item : this->list) {
        bool isUnique1 = true, isUnique2 = true, isUnique3 = true, isUnique4 = true, isUnique5 = true, isUnique6 = true, isUnique7 = true, isUnique8 = true, isUnique9 = true, isUnique10 = true, isUnique11 = true, isUnique12 = true;
        for (const auto& item1 : sIDs) {
            if (item.ID == item1) {
                isUnique1 = false;
                break;
            }
        }
        for (const auto& item1 : sFirstNames) {
            if (item.FirstName == item1) {
                isUnique2 = false;
                break;
            }
        }
        for (const auto& item1 : sLastNames) {
            if (item.LastName == item1) {
                isUnique3 = false;
                break;
            }
        }
        for (const auto& item1 : sGenders) {
            if (item.Gender == item1) {
                isUnique4 = false;
                break;
            }
        }
        for (const auto& item1 : sAddresses) {
            if (item.Address == item1) {
                isUnique5 = false;
                break;
            }
        }
        for (const auto& item1 : sCities) {
            if (item.City == item1) {
                isUnique6 = false;
                break;
            }
        }
        for (const auto& item1 : sDateOfBirths) {
            if (item.DateOfBirth == item1) {
                isUnique7 = false;
                break;
            }
        }
        for (const auto& item1 : sRegistrationDates) {
            if (item.RegistrationDate == item1) {
                isUnique8 = false;
                break;
            }
        }
        for (const auto& item1 : sRegistrationStatuses) {
            if (item.RegistrationStatus == item1) {
                isUnique9 = false;
                break;
            }
        }
        for (const auto& item1 : sDegreeStatuses) {
            if (item.DegreeStatus == item1) {
                isUnique10 = false;
                break;
            }
        }
        for (const auto& item1 : sLastDegrees) {
            if (item.LastDegree == item1) {
                isUnique11 = false;
                break;
            }
        }
        for (const auto& item1 : sFatherNames) {
            if (item.FatherName == item1) {
                isUnique12 = false;
                break;
            }
        }

        if (isUnique1) sIDs.push_back(item.ID);
        if (isUnique2) sFirstNames.push_back(item.FirstName);
        if (isUnique3) sLastNames.push_back(item.LastName);
        if (isUnique4) sGenders.push_back(item.Gender);
        if (isUnique5) sAddresses.push_back(item.Address);
        if (isUnique6) sCities.push_back(item.City);
        if (isUnique7) sDateOfBirths.push_back(item.DateOfBirth);
        if (isUnique8) sRegistrationDates.push_back(item.RegistrationDate);
        if (isUnique9) sRegistrationStatuses.push_back(item.RegistrationStatus);
        if (isUnique10) sDegreeStatuses.push_back(item.DegreeStatus);
        if (isUnique11) sLastDegrees.push_back(item.LastDegree);
        if (isUnique12) sFatherNames.push_back(item.FatherName);
    }

    std::unordered_map<std::string, int> map;

    map["ID"] = sIDs.size();
    map["FirstName"] = sFirstNames.size();
    map["LastName"] = sLastNames.size();
    map["FatherName"] = sFatherNames.size();
    map["Gender"] = sGenders.size();
    map["Address"] = sAddresses.size();
    map["City"] = sCities.size();
    map["DateOfBirth"] = sDateOfBirths.size();
    map["RegistrationDate"] = sRegistrationDates.size();
    map["RegistrationStatus"] = sRegistrationStatuses.size();
    map["DegreeStatus"] = sDegreeStatuses.size();
    map["LastDegree"] = sLastDegrees.size();

    return map;
}


std::string StudentList::getInvalidValuesString() {
    std::string str;

    for (int i = 0; i != StudentErrorFlag::LastDegree; i++ )
    {
        auto flag = static_cast<StudentErrorFlag>(i);
        auto data = this->getStudentsWithInvalidData(flag);

        str.append("Invalid data for student field " + StudentErrorFlagToString(flag) + ": " + std::to_string(data->size()) + '\n');

        delete data;
    }

    return str;
}

std::string StudentList::toFormattedString() const {
    std::string str;
    for (const auto& item : this->list) {
        str += item.toFormattedString() + '\n';
    }

    return str;
}

int StudentList::getSize() const {
    return (int) this->list.size();
}

std::vector<Student>& StudentList::getList() {
    return this->list;
}

std::string StudentList::getMaleVsFemaleString() const {
    int maleCount = 0, femaleCount = 0;
    for (const auto& item : this->list) {
        if (item.Gender == 'M') maleCount++;
        else if (item.Gender == 'F') femaleCount++;
    }

    return ("Female : Male = " + std::to_string(femaleCount) + " : " + std::to_string(maleCount) + '\n');
}
