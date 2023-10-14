#include "../include/Cities.h"
#include "../include/Miscellaneous.h"

using namespace std;

void CityList::loadCitiesFromFile(const std::string& path) {
    try {
        ifstream file;
        file.open(path);

        if (file.bad() || !file.is_open()) throw std::ifstream::failure("Failed to open file to read cities.");

        while (!file.eof()) {
            string line;
            getline(file, line, '\n');

            if (line.empty()) continue;

            auto strings = Flanky::splitString(line, '-');
            auto city = strings[0];
            strings.erase(strings.begin());

            bool alreadyHasLowerCaseCopy = false;
            string cityCopy = city;
            Flanky::toLowerCase(cityCopy);
            for (const auto &string: strings)
                if (string == cityCopy) {
                    alreadyHasLowerCaseCopy = true;
                    break;
                }
            if (!alreadyHasLowerCaseCopy) strings.push_back(cityCopy);

            this->cities.push_back(city);
            this->aliases.push_back(strings);
        }

        file.close();
    } catch (std::exception &e) {
        Flanky::print(e.what(), Flanky::Color::RED);
        exit(-1);
    }
}

void CityList::writeCitiesToFile(const string& path) {
    try {
        string str;

        for (int i = 0; i < this->cities.size(); i++) {
            str += this->cities[i];
            for (const auto &item: this->aliases[i]) {
                str += " - " + item;
            }
            str += '\n';
        }

        ofstream file;
        file.open(path);

        if (file.bad() || !file.is_open()) throw std::ifstream::failure("Failed to open file to write cities.");

        file << str;

        file.close();
    } catch (std::exception &e) {
        Flanky::print(e.what(), Flanky::Color::RED);
        exit(-1);
    }
}

int CityList::findCity(const string &name) {
    for (int i = 0; i < this->cities.size(); i++) {
        if (this->cities[i] == name) return i;
    }

    return -1;
}

int CityList::findAlias(const string& alias) {
    int pos = -1;

    for (int i = 0; i < this->aliases.size(); i++) {
        for (auto& item : this->aliases[i]) {
            if (item == alias) {
                pos = i;
                break;
            }
        }
    }

    return pos;
}

void CityList::addCity(string &name, vector<std::string>& aliasesList) {
    auto cityPos = findCity(name);

    // If city is not in the list, add it to the list
    if (cityPos == -1) {
        this->cities.push_back(name);

        // Make aliases lower case
        for (auto &item: aliasesList) {
            Flanky::toLowerCase(item);
        }

        // Add lowercase copy of the city name
        std::string cityCopy = name;
        Flanky::toLowerCase(cityCopy);
        aliasesList.push_back(cityCopy);

        this->aliases.push_back(aliasesList);
    }
    // Update the aliases of the city
    else {
        auto oldAliases = this->aliases[cityPos];
        vector<string> newAliases;

        for (const auto &item: oldAliases) {
            newAliases.push_back(item);
        }

        for (auto &item: aliasesList) {
            // Ignore duplicate aliases
            if (findAlias(item) != -1) continue;

            // Make alias lower case
            Flanky::toLowerCase(item);

            newAliases.push_back(item);
        }

        this->aliases[cityPos] = newAliases;
    }
}

std::string CityList::getCity(const int& pos) {
    if (pos < 0 || pos > (this->cities.size() - 1)) return "ERROR";

    return this->cities[pos];
}

bool CityList::isEmpty() const {
    return this->cities.empty();
}