#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>
#include <fstream>
#include <string>

class Logger {
public:
    bool useConsole = true;
    bool useFile = false;
    std::ofstream fileStream;

    template <typename T>
    Logger& operator<<(const T& data) {
        if (useConsole) std::cout << data;
        if (useFile && fileStream.is_open()) fileStream << data;
        return *this;
    }
    // Перегрузка для манипуляторов потока, таких как std::endl.
    Logger& operator<<(std::ostream& (*manip)(std::ostream&)) {
        if (useConsole) std::cout << manip;
        if (useFile && fileStream.is_open()) fileStream << manip;
        return *this;
    }

    void flush() {
        if (useConsole) std::cout.flush();
        if (useFile && fileStream.is_open()) fileStream.flush();
    }
};
#endif