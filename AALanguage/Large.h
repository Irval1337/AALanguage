#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

class large {
public:
    large();
    large(std::string);
    large(long long value);
    large& operator+=(const large& value);
    large& operator-=(const large& value);
    large& operator*=(const large& value);
    large& operator/=(const large& value);
    large& operator%=(const large& value);
    large operator++(::int32_t d);
    large operator--(::int32_t d);
    large& operator++();
    large& operator--();
    large operator-();
    bool operator== (large val);
    bool operator!= (large val);
    bool operator<= (large val);
    bool operator>= (large val);
    bool operator> (large val);
    bool operator< (large val);
    large operator+ (large val);
    large operator- (large val);
    large operator* (large val);
    large operator/ (long long val);
    large operator/ (large val);
    large operator% (large val);
    std::string ToString();
    std::string GetNum();
    large pow(large val);
    friend std::istream& operator>>(std::istream& is, large& lg) {
        std::string s;
        is >> s;
        lg = s;
        return is;
    }
    friend std::ostream& operator<<(std::ostream& os, large lg) {
        return os << lg.ToString();
    }
private:
    enum Sign { Plus, Minus };

    std::string num;
    Sign sign;
};