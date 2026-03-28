#ifndef INPUT_UTILS_H
#define INPUT_UTILS_H

#include <cctype>

inline char normalizeKey(char c) {
    return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
}

#endif


