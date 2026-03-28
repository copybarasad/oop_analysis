#ifndef FILERAII_H
#define FILERAII_H

#include <fstream>
#include <string>
#include "GameExceptions.hpp"

class FileRAII {
private:
    std::fstream file_;
    std::string filename_;
    std::ios_base::openmode mode_;

public:
    FileRAII(const std::string& filename, std::ios_base::openmode mode)
        : filename_(filename), mode_(mode) {
        file_.open(filename, mode);
        if (!file_.is_open()) {
            if (mode & std::ios::in) {
                throw GameLoadException("Cannot open file for reading: " + filename);
            } else {
                throw GameSaveException("Cannot open file for writing: " + filename);
            }
        }
    }

    ~FileRAII() {
        if (file_.is_open()) {
            file_.close();
        }
    }

    FileRAII(const FileRAII&) = delete;
    FileRAII& operator=(const FileRAII&) = delete;

    FileRAII(FileRAII&& other) noexcept 
        : file_(std::move(other.file_)), filename_(std::move(other.filename_)), mode_(other.mode_) {
    }

    FileRAII& operator=(FileRAII&& other) noexcept {
        if (this != &other) {
            file_ = std::move(other.file_);
            filename_ = std::move(other.filename_);
            mode_ = other.mode_;
        }
        return *this;
    }

    std::fstream& get() { return file_; }
    const std::fstream& get() const { return file_; }
    
    bool isOpen() const { return file_.is_open(); }
    void close() { file_.close(); }
};

#endif