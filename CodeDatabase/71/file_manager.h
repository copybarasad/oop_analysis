#pragma once
#include <string>
#include <fstream>

class FileManager {
public:
    FileManager(const std::string& filename, std::ios_base::openmode mode);
    ~FileManager();
    std::fstream& getStream();

private:
    std::fstream file_stream_;
    std::string filename_;
};
