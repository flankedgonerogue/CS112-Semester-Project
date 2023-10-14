#include <string>
#include "../include/CSV.h"
#include "../include/Miscellaneous.h"

namespace Flanky {
    using namespace std;

    CSVFile::CSVFile(const string &path) {
        ifstream file;
        file.open(path);

        if (file.bad() || !file.is_open()) throw std::ifstream::failure("Failed to open CSV file.");

        while (!file.eof()) {
            string line;
            std::getline(file, line, '\n');

            if (line.empty()) continue;

            this->CSVLines.push_back(Flanky::splitString(line, ','));
        }

        file.close();
    }

    int CSVFile::getNumberOfLines() const {
        return (int) this->CSVLines.size();
    }

    vector<string> &CSVFile::getLine(int i) {
        return this->CSVLines[i];
    }
}