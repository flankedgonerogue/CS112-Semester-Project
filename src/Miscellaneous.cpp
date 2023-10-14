#include <string>
#include <iostream>
#include <filesystem>
#include <windows.h>
#include "../include/Miscellaneous.h"

namespace Flanky {
    using namespace std;

    void print(const string &str, Color color) {
        switch (color) {
            case DEFAULT:
                cout << str;
                break;
            case RED:
                cout << "\033[31m" << str << "\033[0m";
                break;
            case GREEN:
                cout << "\033[32m" << str << "\033[0m";
                break;
            case YELLOW:
                cout << "\033[33m" << str << "\033[0m";
                break;
            case BLUE:
                cout << "\033[34m" << str << "\033[0m";
                break;
            case MAGENTA:
                cout << "\033[35m" << str << "\033[0m";
                break;
            case CYAN:
                cout << "\033[36m" << str << "\033[0m";
        }

        cout.flush();
    }

    std::string getInputString(const std::string &prompt) {
        std::cout << prompt;
        std::string input;
        getline(std::cin, input);
        trimString(input);
        return input;
    }

    bool prompt(const std::string &prompt) {
        auto input = Flanky::getInput<char>(prompt + " (Y/n)? ");
        input = static_cast<char>(tolower(input));

        while (true) {
            if (input == 'n' || input == 'y') break;
            Flanky::print("Invalid choice. Please try again.\n", Color::RED);
            input = static_cast<char>(tolower(getInput<char>(prompt + " (Y/n)? ")));
        }

        return input == 'y';
    }

    void trimString(std::string &str) {
        // Removes leading space
        while (!str.empty() && isspace(str.front())) {
            str.erase(0, 1);
        }

        // Removes trailing space
        while (!str.empty() && isspace(str.back())) {
            str.pop_back();
        }
    }

    vector<string> getFilePathsFromFolder(const string &path, const string &ext) {
        vector<string> paths;

        // Iterate over each entry in the directory
        for (const auto &entry: std::filesystem::directory_iterator(path)) {
            // Check if the current entry is a regular file
            if (entry.path().extension() == ext) {
                paths.push_back(entry.path().string());
            }
        }

        return paths;
    }

    void toLowerCase(string &str) {
        for (auto &c: str) {
            c = static_cast<char>(tolower(c));
        }
    }

    vector<string> splitString(const string& str, const char& delim) {
        vector<string> strings;

        int i, j = 0;
        while (true) {
            i = static_cast<int>(str.find_first_of(delim, j));

            // No more delimiter was found so gets final substring and breaks loop
            if (i == -1) {
                string _str = str.substr(j);
                trimString(_str);
                strings.push_back(_str);

                break;
            }

            // Gets substring between the last delimiter position and the next delimiter position
            string _str = str.substr(j, i - j);
            trimString(_str);
            strings.push_back(_str);

            j = i + 1;
        }

        return strings;
    }

    std::string getCurrentDirectory() {
        char buffer[MAX_PATH];
        GetModuleFileNameA(nullptr, buffer, MAX_PATH);
        std::string::size_type pos = std::string(buffer).find_last_of("\\/");

        return std::string(buffer).substr(0, pos);
    }

    bool checkIfFileExists(const std::string& path, const std::string& ext) {
        std::filesystem::path file(path);
        if (!(std::filesystem::exists(file) && file.extension() == ext)) {
            return false;
        }
        return true;
    }

}