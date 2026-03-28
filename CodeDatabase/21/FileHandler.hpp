#pragma once
#include <fstream>
#include <string>

class FileHandler {
private:
    std::fstream file_;
public:
    FileHandler(const std::string& filename);
    FileHandler(const std::string& filename, std::ios::openmode mode);
    ~FileHandler();
    void write(const std::string& string);
    std::string read();
    std::fstream& stream();
};
