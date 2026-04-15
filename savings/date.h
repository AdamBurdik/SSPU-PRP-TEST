#pragma once

bool is_leap_year(int year);
int get_days_in_year(int year);
int get_last_day(int month, int year);

#ifdef DATE_IMPLEMENTATION

inline bool is_leap_year(const int year) {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

inline int get_days_in_year(const int year) {
    return is_leap_year(year) ? 366 : 365;
}

inline int get_last_day(const int month, const int year) {
    switch (month) {
        case 2: return is_leap_year(year) ? 29 : 28;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        default:
            return 31;
    }
}

#endif
