#pragma once
#include <iostream>
#include <string>

class Logger {
public:
    void header(int turn);
    void info(const std::string& msg);
    void action(const std::string& msg);
    void warning(const std::string& msg);
    void separator();
};
