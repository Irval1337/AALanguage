#include "Large.h"

large::large() {
    num = "0";
    sign = Sign::Plus;
}
large::large(std::string value) {
    bool ok = true;
    num = value;
    sign = value[0] == '-' ? Sign::Minus : Sign::Plus;
    if (value[0] == '-' || value[0] == '+')
        num.erase(0, 1);
    for (int i = 0; i < num.size(); ++i) {
        ok &= num[i] >= '0' && num[i] <= '9';
    }
    if (!ok)
        throw std::exception("Cannot cast not an integer std::string to large");
}
large::large(long long value) {
    num = std::to_string(value);
    sign = value < 0 ? Sign::Minus : Sign::Plus;
    if (sign == Sign::Minus)
        num.erase(0, 1);
}
std::string large::GetNum() {
    return num;
}
large& large::operator+=(const large& value) {
    return *this = (*this + value);
}
large& large::operator-=(const large& value) {
    return *this = (*this - value);
}
large& large::operator*=(const large& value) {
    return *this = (*this * value);
}
large& large::operator/=(const large& value) {
    return *this = (*this / value);
}
large& large::operator%=(const large& value) {
    return *this = (*this % value);
}
large large::operator++(::int32_t d) {
    auto tmp = *this;
    *this += large(1);
    return tmp;
}
large large::operator--(::int32_t d) {
    auto tmp = *this;
    *this -= large(1);
    return tmp;
}
large& large::operator++() {
    return *this += large(1);
}
large& large::operator--() {
    return *this -= large(1);
}
large large::operator-() {
    return large(0) - *this;
}
bool large::operator== (large val) {
    std::string num1 = num, num2 = val.GetNum();
    return num1 == num2;
}
bool large::operator!= (large val) {
    return !(*this == val);
}
bool large::operator<= (large val) {
    return *this < val || *this == val;
}
bool large::operator>= (large val) {
    return *this < val == false;
}
bool large::operator> (large val) {
    return *this >= val && *this != val;
}
bool large::operator< (large val) {
    if (val.sign != sign)
        return sign == Sign::Minus;
    bool isMinus = val.sign == Sign::Minus;
    std::string num1 = num, num2 = val.GetNum();
    if (num1.length() != num2.length())
        return isMinus ? num1.length() > num2.length() : num1.length() < num2.length();
    else if (num1 == num2)
        return false;
    for (int i = 0; i < num1.length(); i++) {
        if (num1[i] != num2[i])
            return isMinus ? num1[i] > num2[i] : num1[i] < num2[i];
    }
}
large large::operator+ (large val) {
    if (val.sign != sign)
        return val.sign == Sign::Plus ? val - large(num) : large(num) - large(val.GetNum());
    bool isMinus = val.sign == Sign::Minus;
    std::string num2 = val.GetNum(), num1 = num;;
    reverse(num1.begin(), num1.end());
    reverse(num2.begin(), num2.end());
    int carry = 0, n = std::max(num1.length(), num2.length()) + 1;
    num1 += std::string(n - num1.length(), '0');
    num2 += std::string(n - num2.length(), '0');
    std::string ans = std::string(n, '0');
    for (int i = 0; i < n; i++) {
        ans[i] += num1[i] + num2[i] - 2 * '0';
        if (ans[i] > '9') {
            ans[i + 1]++;
            ans[i] -= 10;
        }
    }
    if (ans[n - 1] == '0') ans.erase(n - 1, 1);
    reverse(ans.begin(), ans.end());
    if (isMinus)
        ans = "-" + ans;
    return large(ans);
}
large large::operator- (large val) {
    if (sign == Sign::Minus && val.sign == Sign::Plus)
        return large("-" + val.GetNum()) + *this;
    else if (sign == Sign::Plus && val.sign == Sign::Minus)
        return *this + large(val.GetNum());
    else if (sign == Sign::Minus && val.sign == Sign::Minus)
        return large(val.GetNum()) - large(num);
    std::string num1 = num, num2 = val.GetNum();
    if (num1 == num2) {
        return large(0);
    }
    bool isMunus = val > *this;
    if (isMunus)
        swap(num1, num2);
    reverse(num1.begin(), num1.end());
    reverse(num2.begin(), num2.end());
    std::string ans = std::string(num1.length(), '0');
    num2 += std::string(num1.length() - num2.length(), '0');
    for (int i = 0; i < num1.length(); i++) {
        ans[i] += num1[i] - num2[i];
        if (ans[i] < '0') {
            ans[i] += 10;
            ans[i + 1]--;
        }
    }
    reverse(ans.begin(), ans.end());
    int i = 0;
    while (ans[i] == '0') i++;
    return large((isMunus ? "-" : "") + ans.substr(i));
}
large large::operator* (large val) {
    bool isMinus = val.sign != sign;
    std::string num1 = num, num2 = val.GetNum();
    large ans;
    if (num1 == "0" || num2 == "0")
        return large(0);
    if (num2.length() > 1) {
        std::vector<large> nums;
        for (int i = num2.length() - 1; i >= 0; i--) {
            nums.push_back(*this * large(num2.substr(i, 1)));
            if (nums[num2.length() - 1 - i] != large(0))
                nums[num2.length() - 1 - i] = large(nums[num2.length() - 1 - i].GetNum() + std::string(num2.length() - 1 - i, '0'));
        }
        for (int i = 0; i < nums.size(); i++) ans += nums[i];
        return large((isMinus ? "-" : "") + ans.GetNum());
    } else {
        for (int i = 0; i < atoi(num2.c_str()); i++)
            ans = ans + *this;
        return large((isMinus ? "-" : "") + ans.GetNum());
    }
}
large large::operator/ (long long val) {
    bool isMinus = val >= 0 != sign == Sign::Plus;
    std::string num1 = num, num2 = std::to_string(val), ans = "";
    val = abs(val);
    long long tmp, i = 1;
    for (; (i < num1.length()) && (tmp = atoi(num1.substr(0, i).c_str()), tmp < val); i++) {}
    tmp = atoi(num1.substr(0, i).c_str());
    bool moved = false;
    if (val > tmp)
        return large(0);
    else {
        ans += std::to_string(tmp / val);
        tmp = tmp % val;
        num1.erase(0, i);
        while (num1.length() > 0) {
            if (tmp == 0 && num1 == std::string(num1.length(), '0'))
                return large(ans + num1);
            if (tmp != 0) {
                tmp *= 10;
                tmp += atoi(num1.substr(0, 1).c_str());
                num1.erase(0, 1);
                ans += std::to_string(tmp / val);
                tmp = tmp % val;
            } else {
                ans += std::to_string(atoi(num1.substr(0, 1).c_str()) / val);
                tmp = atoi(num1.substr(0, 1).c_str()) % val;
                num1.erase(0, 1);
            }
        }
    }
    return large((isMinus ? "-" : "") + ans);
}
large large::operator/ (large val) {
    if (val.GetNum() == "0")
        throw std::exception("Division by zero");
    bool isMinus = val.sign != sign;
    std::string num1 = num, num2 = val.GetNum();
    large L = large(0), R = large(num1), m;
    while (L < R - large(1)) {
        m = L + (R - L) / 2;
        if (m * large(num2) >= large(this->num)) R = m;
        else L = m;
    }
    if (R * large(num2) > large(this->num))
        R -= large(1);
    return large((isMinus && R != large(0) ? "-" : "") + R.GetNum());
}
large large::operator% (large val) {
    bool isMinus = val.sign != sign;
    val = large(val.GetNum());
    auto ans = large(this->num) - large(this->num) / val * val;
    return (isMinus && ans != large(0) ? "-" : "") + ans.GetNum();
}
std::string large::ToString() {
    return (sign == Sign::Minus ? "-" : "") + num;
}
large large::pow(large val) {
    large res = 1, a = *this;
    while (val != 0) {
        if ((val.num.back() - '0') % 2)
            res *= a;
        a *= a;
        val /= 2;
    }
    return res;
}