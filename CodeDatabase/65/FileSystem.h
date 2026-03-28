#pragma once

#include <fstream>
#include <stdexcept>
#include <vector>

class FileSystem {
    std::string filename;
    std::fstream file;
public:
    FileSystem(std::string _filename) : filename(_filename) {
        file.open(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Error: `" + filename + "` is not opened.");
        }
    }

    void write(std::string content) {
        file << content;
    }

    std::vector<std::string> read() {
        std::vector<std::string> result;
        std::string line;
        while (getline(file, line)) {
            result.push_back(line);
        }
        return result;
    }

    ~FileSystem() {
        if (file.is_open()) {
            file.close();
        }
    }
};
