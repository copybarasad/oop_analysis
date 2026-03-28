#include "SimpleFileSystem.h"
#include <string>
#include <vector>
#include <direct.h>   
#include <windows.h>   
#include <sys/stat.h>   

bool SimpleFileSystem::createDirectory(const std::string& path) {
    return _mkdir(path.c_str()) == 0 || errno == EEXIST;
}

bool SimpleFileSystem::exists(const std::string& path) {
    struct stat info;
    return stat(path.c_str(), &info) == 0;
}

bool SimpleFileSystem::removeFile(const std::string& path) {
    return std::remove(path.c_str()) == 0;
}

std::vector<std::string> SimpleFileSystem::getFilesInDirectory(const std::string& path, const std::string& extension) {
    std::vector<std::string> files;
    
    if (!exists(path)) {
        return files;
    }
    
    std::string searchPath = path + "\\*" + extension;
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findFileData);
    
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                std::string filename = findFileData.cFileName;
                size_t lastdot = filename.find_last_of(".");
                if (lastdot != std::string::npos) {
                    filename = filename.substr(0, lastdot);
                }
                files.push_back(filename);
            }
        } while (FindNextFileA(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
    
    return files;
}
