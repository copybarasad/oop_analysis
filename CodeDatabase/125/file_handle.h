#pragma once
#include <fstream>
#include <string>
#include <stdexcept>

class FileHandle {
public:
    FileHandle(const std::string& filename, std::ios::openmode mode);

    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    std::fstream& get() { return file_; }
    const std::fstream& get() const { return file_; }

    explicit operator bool() const { return file_.is_open(); }

private:
    std::fstream file_;
};