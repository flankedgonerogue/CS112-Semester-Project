#include "../include/Date.h"
#include "../include/Miscellaneous.h"

namespace Flanky {
    using namespace std;

    Date::Date(int Y, int M, int D): Year(Y), Month(M), Day(D) {}
    Date::Date(const string& str) {
        if (str.empty()) {
            this->isUnknown = true;
            return;
        }

        try {
            auto strings = Flanky::splitString(str, '-');

            this->Day = stoi(strings[0]);

            if (strings[1].length() == 3) {
                if (strings[1] == "Jan") this->Month = 1;
                else if (strings[1] == "Feb") this->Month = 2;
                else if (strings[1] == "Mar") this->Month = 3;
                else if (strings[1] == "Apr") this->Month = 4;
                else if (strings[1] == "May") this->Month = 5;
                else if (strings[1] == "Jun") this->Month = 6;
                else if (strings[1] == "Jul") this->Month = 7;
                else if (strings[1] == "Aug") this->Month = 8;
                else if (strings[1] == "Sep") this->Month = 9;
                else if (strings[1] == "Oct") this->Month = 10;
                else if (strings[1] == "Nov") this->Month = 11;
                else if (strings[1] == "Dec") this->Month = 12;
            } else {
                this->Month = stoi(strings[1]);
            }

            this->Year = stoi(strings[2]);
        } catch (...) {
            // Ignore exception mark date as unknown.
            this->isUnknown = true;
        }
    }

    string Date::toFormattedString() const {
        if (this->isUnknown) return "ERROR";
        return to_string(Day) + "-" + to_string(Month) + "-" + to_string(Year);
    }

    bool Date::isLeapYear() const {
        if (this->Year % 4 != 0) return false;
        if (this->Year % 100 != 0) return true;
        if (this->Year % 400 != 0) return false;
        return true;
    }

    bool Date::isValidDate() const {
        if (isUnknown) return false;

        if (this->Year < 1 || this->Month < 1 || this->Month > 12 || this->Day < 1) {
            return false;
        }

        int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        if (isLeapYear()) {
            daysInMonth[1] = 29; // February has 29 days in a leap year
        }

        if (this->Day > daysInMonth[this->Month - 1]) {
            return false;
        }

        return true;
    }

    bool Date::operator>(const Date& other) const {
        if (this->Year != other.Year)
            return this->Year > other.Year;
        if (this->Month != other.Month)
            return this->Month > other.Month;
        return this->Day > other.Day;
    }

    bool Date::operator<(const Date& other) const {
        if (this->Year != other.Year)
            return this->Year < other.Year;
        if (this->Month != other.Month)
            return this->Month < other.Month;
        return this->Day < other.Day;
    }

    bool Date::operator==(const Date &other) const {
        if (this->Year != other.Year)
            return false;
        if (this->Month != other.Month)
            return false;
        if (this->Day != other.Day)
            return false;

        return true;
    }
}