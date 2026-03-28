#ifndef FILEGUARD_H
#define FILEGUARD_H

#include <fstream>
#include <string>

class FileGuard {
private:
    std::fstream file;
    std::string filename;
    bool isOpen;

public:
    FileGuard(const std::string& filename, std::ios::openmode mode);
    ~FileGuard();
    
    FileGuard(const FileGuard&) = delete;
    FileGuard& operator=(const FileGuard&) = delete;
    
    FileGuard(FileGuard&& other) noexcept;
    FileGuard& operator=(FileGuard&& other) noexcept;
    
    std::fstream& getStream();
    bool isFileOpen() const;
    void close();
};

#endif
