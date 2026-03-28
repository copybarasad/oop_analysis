#pragma once
#include <fstream>
#include <string>
#include "SaveLoadException.h"

class FileHandler {
private:
    std::fstream file;
    std::string filename;
    bool is_open_flag;

public:
    FileHandler(const std::string& fname, std::ios::openmode mode)
        : filename(fname), is_open_flag(false) {
        file.open(filename, mode);

        if (!file.is_open()) {
            throw FileOpenException(filename);
        }
        is_open_flag = true;
    }

    ~FileHandler() {
        if (is_open_flag && file.is_open()) {
            file.close();
        }
    }

    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;

    FileHandler(FileHandler&& other) noexcept
        : file(std::move(other.file)),
        filename(std::move(other.filename)),
        is_open_flag(other.is_open_flag) {
        other.is_open_flag = false;
    }

    std::fstream& get_stream() { return file; }

    bool is_open() const { return is_open_flag; }

    const std::string& get_filename() const { return filename; }
};
