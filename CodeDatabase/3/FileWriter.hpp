#pragma once
#include <stdexcept>
#include <string>
#include "Serialization/Exceptions.hpp"


class FileWriter {
private:
    std::FILE* file;
    const char* path;

public:
    FileWriter(const char* filename);
    ~FileWriter();
    void write(std::string str);
};
