#ifndef SIMPLEFILESYSTEM_H
#define SIMPLEFILESYSTEM_H

#include <stdexcept>
#include <string>
#include <vector>

class SimpleFileSystem {
public:
    static bool createDirectory(const std::string& path);
    static bool exists(const std::string& path);
    static bool removeFile(const std::string& path);
    static std::vector<std::string> getFilesInDirectory(const std::string& path, const std::string& extension = "");
};

#endif