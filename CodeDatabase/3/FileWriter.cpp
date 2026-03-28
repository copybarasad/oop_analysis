#include "Serialization/FileWriter.hpp"


FileWriter::FileWriter(const char* filename) : file(std::fopen(filename, "wb")), path(filename) {
    if (!file) {
        throw FileWriteException(path);
    }
}

FileWriter::~FileWriter() {
    if (file) std::fclose(file);
}

void FileWriter::write(std::string str) {
    size_t n = std::fwrite(str.data(), 1, str.size(), file);
    if (n != str.size()) {
        throw FileWriteException(path);
    }
}
