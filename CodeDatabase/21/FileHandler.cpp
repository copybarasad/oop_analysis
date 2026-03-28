#include "FileHandler.hpp"


FileHandler::FileHandler(const std::string& filename, std::ios::openmode mode) : file_(filename, mode) {
    if (!file_.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
}

FileHandler::FileHandler(const std::string& filename) : file_(filename) {
    if (!file_.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
}


FileHandler::~FileHandler() {
    if (file_.is_open()) file_.close();
}


std::fstream& FileHandler::stream() {
    return file_;
}


void FileHandler::write(const std::string& string) {
    this->stream() << string;
}


std::string FileHandler::read() {
    std::string s;
    this->stream() >> s;
    return s;
}
