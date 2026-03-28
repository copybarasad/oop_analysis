#ifndef FILEWRAPPER_H
#define FILEWRAPPER_H

#include <fstream>
#include <string>
#include "../Enums.h"
#include "../libs/json.hpp"

class FileWrapper {
private:
    std::fstream fileStream;
    Mode mode;
    std::string fileName;

public:
    FileWrapper(const std::string& filename, Mode mode);
    ~FileWrapper();

    FileWrapper(const FileWrapper&) = delete;
    FileWrapper& operator=(const FileWrapper&) = delete;
    FileWrapper(FileWrapper&&) = default;
    FileWrapper& operator=(FileWrapper&&) = default;
    void write(const nlohmann::json& output);
    nlohmann::json read();
};

#endif