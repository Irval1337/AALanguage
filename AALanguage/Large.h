#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

//! A class for AALanguage variables of type large that implement long arithmetic.
class large {
public:
    //! Default constructor, absolute number value is set to 0.
    large();
    /**
    * @brief Constructor from a string representation of a number.
    *
    * @param str A string containing a number in simplified mathematical form
    */
    large(std::string str);
    /**
    * @brief Constructor from a long long equivalent of a number.
    *
    * @param value A long long number equivalent
    */
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
    //! A plus operation that uses columnar addition.
    large operator+ (large val);
    //! A minus operation that uses subtraction in a column.
    large operator- (large val);
    //! A multiplication operation that uses column multiplication.
    large operator* (large val);
    //! An operation of division by long long long using a columnar calculation.
    large operator/ (long long val);
    //! An operation of integer division by a large number. Uses the binary search algorithm and multiplication to check for equality. O(logN) asymptotics.
    large operator/ (large val);
    large operator% (large val);
    /**
    * @brief Converting a large number into a string.
    *
    * @return String representation.
    */
    std::string ToString();
private:
    enum Sign { Plus, Minus };

    std::string num;
    Sign sign;

    std::string GetNum();
    large pow(large val);
};