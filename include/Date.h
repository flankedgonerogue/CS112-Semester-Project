#ifndef PROJECT_DATE_H
#define PROJECT_DATE_H

#include <string>

namespace Flanky {
    class Date {
    private:
        int Year{};
        int Month{};
        int Day{};
        bool isUnknown = false;

        [[nodiscard]] bool isLeapYear() const;
    public:
        Date() = default;
        explicit Date(int Y, int M, int D);
        explicit Date(const std::string& str);

        [[nodiscard]] bool isValidDate() const;

        [[nodiscard]] std::string toFormattedString() const;

        bool operator>(const Date& other) const;

        bool operator<(const Date& other) const;

        bool operator==(const Date& other) const;
    };
}
#endif //PROJECT_DATE_H