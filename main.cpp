#include <bits/stdc++.h>
using namespace std;

class Date {
private:
    int _year{1900};
    int _month{1};
    int _day{1};

    static bool isLeap(int y) {
        return (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
    }
    static int daysInMonth(int y, int m) {
        static const int mdays[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
        if (m == 2) return mdays[m] + (isLeap(y) ? 1 : 0);
        if (m >= 1 && m <= 12) return mdays[m];
        return 0;
    }
    static bool valid(int y, int m, int d) {
        if (m < 1 || m > 12) return false;
        if (d < 1 || d > daysInMonth(y, m)) return false;
        return true;
    }

    // Howard Hinnant's algorithms for civil date <-> days
    static long long days_from_civil(int y, unsigned m, unsigned d) {
        y -= m <= 2;
        const int era = (y >= 0 ? y : y - 399) / 400;
        const unsigned yoe = (unsigned)(y - era * 400);              // [0, 399]
        const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1; // [0, 365]
        const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;  // [0, 146096]
        return era * 146097LL + (long long)doe - 1;                  // days since 0000-12-31
    }
    static void civil_from_days(long long z, int &y, unsigned &m, unsigned &d) {
        z += 1; // now days since 0000-12-31 -> 0001-01-01 is 1
        const long long era = (z >= 0 ? z : z - 146096) / 146097;
        const unsigned doe = (unsigned)(z - era * 146097);                    // [0, 146096]
        const unsigned yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365; // [0, 399]
        y = (int)(yoe) + (int)era * 400;
        const unsigned doy = doe - (365 * yoe + yoe/4 - yoe/100);             // [0, 365]
        const unsigned mp = (5 * doy + 2) / 153;                              // [0, 11]
        d = doy - (153 * mp + 2) / 5 + 1;                                     // [1, 31]
        m = mp + (mp < 10 ? 3 : -9);                                          // [1, 12]
        y += (m <= 2);
    }

public:
    Date() = default; // 1900-1-1 by default
    Date(int yy, int mm, int dd) {
        if (valid(yy, mm, dd)) {
            _year = yy; _month = mm; _day = dd;
        } else {
            _year = 1900; _month = 1; _day = 1;
        }
    }

    // Output helper
    void out() const {
        cout << _year << '-' << _month << '-' << _day << '\n';
    }

    // Add/subtract days producing a new Date
    Date operator+(long long days) const {
        long long base = days_from_civil(_year, _month, _day);
        long long nd = base + days;
        int y; unsigned m, d;
        civil_from_days(nd, y, m, d);
        return Date(y, (int)m, (int)d);
    }
    Date operator-(long long days) const { return (*this) + (-days); }

    // Difference in days, absolute value
    long long operator-(const Date &other) const {
        long long a = days_from_civil(_year, _month, _day);
        long long b = days_from_civil(other._year, other._month, other._day);
        long long diff = a - b;
        if (diff < 0) diff = -diff;
        return diff;
    }

    // Comparisons
    bool operator<(const Date &rhs) const {
        if (_year != rhs._year) return _year < rhs._year;
        if (_month != rhs._month) return _month < rhs._month;
        return _day < rhs._day;
    }

    // Pre/post increment/decrement
    Date &operator++() { // ++d : next day
        *this = (*this) + 1;
        return *this;
    }
    Date operator++(int) { // d++
        Date tmp = *this;
        *this = (*this) + 1;
        return tmp;
    }
    Date &operator--() { // --d : previous day
        *this = (*this) - 1;
        return *this;
    }
    Date operator--(int) { // d--
        Date tmp = *this;
        *this = (*this) - 1;
        return tmp;
    }

    friend ostream &operator<<(ostream &os, const Date &dt) {
        os << dt._year << '-' << dt._month << '-' << dt._day;
        return os;
    }
};

static void Test() {
    int op;
    if (!(cin >> op)) return;
    int yy, mm, dd;
    if (op == 1 || op == 0) {
        Date d0;
        Date d1(2000, 2, 29);
        Date d2(1900, 2, 29);
        cout << d0 << '\n';
        cout << d1 << '\n';
        cout << d2 << '\n';
        // d0.out(); d1.out(); d2.out();
    }
    if (op == 2 || op == 0) {
        cin >> yy >> mm >> dd;
        Date d0(yy, mm, dd);
        for (int i = 0; i < 5; ++i) cout << ++d0 << '\n'; // (++d0).out();
        for (int i = 0; i < 5; ++i) cout << d0++ << '\n'; // (d0++).out();
        for (int i = 0; i < 5; ++i) cout << d0-- << '\n'; // (d0--).out();
        for (int i = 0; i < 2; ++i) cout << --d0 << '\n'; // (--d0).out();
        cout << d0 << '\n';
        // d0.out();
    }
    if (op == 3 || op == 0) {
        cin >> yy >> mm >> dd;
        Date d0(yy, mm, dd);
        cout << d0 + 100 << '\n';
        // (d0+100).out();
        cout << d0 - 1000 << '\n';
        // (d0-1000).out();
    }
    if (op == 4 || op == 0) {
        cin >> yy >> mm >> dd;
        Date d0(yy, mm, dd);
        Date d1(2020, 12, 21);
        cout << (d0 < d1) << '\n';
    }
    if (op == 5 || op == 0) {
        cin >> yy >> mm >> dd;
        Date d0(yy, mm, dd);
        Date d1(1912, 6, 23);
        cout << (d0 - d1) << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Test();
    return 0;
}

