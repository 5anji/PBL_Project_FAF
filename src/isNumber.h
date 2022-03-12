#pragma once
#include <string>

// Check if string is a number
bool isNumber(std::string& str) {
    for (char const& c : str) {
        if (std::isdigit(c) == 0)
            return false;
    }
    return true;
}