#include "logger/FileLogger.h"
#include <filesystem>
#include <iostream>

FileLogger::FileLogger(const std::string &filename) {

    std::filesystem::path path(filename);
    std::filesystem::create_directories(path.parent_path());

    file.open(filename);
    if (!file.is_open())
    {
        std::cerr << "[ERROR] Cannot open log file: " << filename << std::endl;
    }
    else
    {
        std::cout << "[INFO] Log file opened successfully: " << filename << std::endl;
    }
}

void FileLogger::log(const Event &event)
{
    if (file.is_open())
    {
        file << "[LOG] " << event.message << std::endl;
    }
}