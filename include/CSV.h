#ifndef PROJECT_CSV_H
#define PROJECT_CSV_H

#include <fstream>
#include <exception>
#include <vector>
#include <list>

namespace Flanky {
    class CSVFile {
    private:
        std::vector<std::vector<std::string>> CSVLines;
    public:
        explicit CSVFile(const std::string &path);

        [[nodiscard]] std::vector<std::string>& getLine(int i);

        [[nodiscard]] int getNumberOfLines() const;
    };
}

#endif //PROJECT_CSV_H