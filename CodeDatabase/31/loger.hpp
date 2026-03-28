#ifndef LOGER_HPP
#define LOGER_HPP

#include <iostream>

#include <cstdio>


enum class LogMode { Terminal, File };

class Loger {
    FILE *file {stdout};
public:
    Loger(LogMode mode) {
        FILE * file = (mode == LogMode::File) ? std::fopen("logs.txt", "w") : stdout;
    }
    void take_info(std::string log) {
        fprintf(file, "%s", log.c_str());
    }   
};

#endif 