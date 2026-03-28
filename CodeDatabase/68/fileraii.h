#ifndef FILERAII_H
#define FILERAII_H

#include <fstream>
#include <string>
#include "gameexceptions.h"

class FileRAII {
private:
    std::fstream file;
    std::string filename;
    
public:
    FileRAII(const std::string& filename, std::ios::openmode mode) 
        : filename(filename) {
        file.open(filename, mode);
        if (!file.is_open()) {
            throw FileOpenException("Cannot open file: " + filename);
        }
    }
    
    ~FileRAII() {
        if (file.is_open()) {
            file.close();
        }
    }
    
    std::fstream& get() { return file; }
    operator bool() const { return file.good(); }
    
    // Prevent copying
    FileRAII(const FileRAII&) = delete;
    FileRAII& operator=(const FileRAII&) = delete;
    
    // Allow moving
    FileRAII(FileRAII&& other) noexcept 
        : file(std::move(other.file)), filename(std::move(other.filename)) {
    }
    
    FileRAII& operator=(FileRAII&& other) noexcept {
        if (this != &other) {
            if (file.is_open()) {
                file.close();
            }
            file = std::move(other.file);
            filename = std::move(other.filename);
        }
        return *this;
    }
};

#endif