#ifndef FILE_RAII_H
#define FILE_RAII_H

#include <fstream>

class FileRAII {
private:
    std::fstream& file;
public:
    explicit FileRAII(std::fstream& f) : file(f) {}
    ~FileRAII() { 
        if(file.is_open()) {
            file.close(); 
        }
    }
    // Запрещаем копирование
    FileRAII(const FileRAII&) = delete;
    FileRAII& operator=(const FileRAII&) = delete;
};

#endif