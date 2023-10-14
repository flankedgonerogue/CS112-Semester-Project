#ifndef PROJECT_V2_MISCELLANEOUS_H
#define PROJECT_V2_MISCELLANEOUS_H

#include <iostream>
#include <vector>
#include <limits>

namespace Flanky {

    // Colors for print function
    enum Color {
        DEFAULT = 0, RED = 1, GREEN = 2, YELLOW = 3, BLUE = 4, MAGENTA = 5, CYAN = 6
    };

    // Prints to std::cout according to the color defined.
    void print(const std::string &str, Color color = DEFAULT);

    // Gets input from std::cin in boolean format
    bool prompt(const std::string &prompt);

    // Gets input from std::cin in required type and handles error
    template<typename T> T getInput(const std::string &prompt) {
        while (true) {
            std::cout << prompt;
            T input;
            std::cin >> input;
            std::cin.ignore(100, '\n');

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(100, '\n');
                Flanky::print("Invalid input!, Please try again.\n", RED);
                continue;
            } else {
                return input;
            }
        }

    }

    // Gets line input from std::cin in required type and handles error
    std::string getInputString(const std::string &prompt);

    // Removes space from front and back
    void trimString(std::string &str);

    // Returns all the file paths from the folder of the required extension type
    std::vector<std::string> getFilePathsFromFolder(const std::string &path, const std::string &ext = ".txt");

    // Converts the string to lowercase
    void toLowerCase(std::string &str);

    // Returns a vector of strings split according to the delimiter
    std::vector<std::string> splitString(const std::string& str, const char& delim);

    // Get the path of current working directory
    std::string getCurrentDirectory();

    [[nodiscard]] bool checkIfFileExists(const std::string&, const std::string& = ".txt");
}

#endif //PROJECT_V2_MISCELLANEOUS_H