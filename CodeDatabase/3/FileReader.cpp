#include "Serialization/FileReader.hpp"


FileReader::FileReader(const char* filename) : file(std::fopen(filename, "rb")), path(filename) {
    if (!file) {
        throw FileReadException(path);
    }
}

FileReader::~FileReader() {
    if (file) std::fclose(file);
}

std::string FileReader::read() {
    std::string data;
    char buf[512];
    while (true) {
        size_t n = std::fread(buf, 1, sizeof(buf), file);
        if (n == 0) {
            if (std::ferror(file)) {
                throw FileReadException(path);
            }
            break;
        }
        data.append(buf, n);
    }
    std::fseek(file, 0, 0);
    return data;
}