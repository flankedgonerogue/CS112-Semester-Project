#ifndef PROJECT_CITIES_H
#define PROJECT_CITIES_H

#include <string>
#include <vector>
#include <fstream>

class CityList {
private:
    std::vector<std::string> cities;
    std::vector<std::vector<std::string>> aliases;
public:
    void loadCitiesFromFile(const std::string&);
    void writeCitiesToFile(const std::string&);
    int findCity(const std::string&);
    int findAlias(const std::string&);
    std::string getCity(const int&);
    void addCity(std::string&, std::vector<std::string>&);

    [[nodiscard]] bool isEmpty() const;
};

#endif //PROJECT_CITIES_H