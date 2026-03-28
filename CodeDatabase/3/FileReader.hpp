#pragma once
#include <stdexcept>
#include <string>
#include "Serialization/Exceptions.hpp"


class FileReader {
private:
    std::FILE* file;
    const char* path;

public:
    FileReader(const char* filename);
    ~FileReader();
    std::string read();
};

