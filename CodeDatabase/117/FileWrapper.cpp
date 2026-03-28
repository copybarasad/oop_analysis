#include "FileWrapper.h"

#include <stdexcept>

FileWrapper::FileWrapper(const std::string& filename, Mode mode)
    : mode(mode), fileName(filename) {
    std::ios::openmode openMode = std::ios::binary;
    if (mode == Mode::Read) {
        openMode |= std::ios::in;
    }
    else {
        openMode |= std::ios::out | std::ios::trunc;
    }

    fileStream.exceptions(std::ios::badbit);
    fileStream.open(fileName, openMode);

    if (!fileStream.is_open()) {
        throw std::runtime_error("Can't open file " + fileName);
    }
}

FileWrapper::~FileWrapper() {
    if (fileStream.is_open()) {
        try {
            fileStream.close();
        } catch (...) {}
    }
}

void FileWrapper::write(const nlohmann::json& output){
    if (this->mode != Mode::Write) {
        throw std::runtime_error("File opened in read mode, cannot write");
    }
    
    try {
        fileStream << output.dump(4);
    } catch (const std::exception& e) {
        throw std::runtime_error("Error writing JSON to file: " + std::string(e.what()));
    }
}

nlohmann::json FileWrapper::read(){
    if (this->mode != Mode::Read) {
        throw std::runtime_error("File opened in write mode, cannot read");
    }
    
    nlohmann::json input;
    fileStream >> input;
    
    if (fileStream.fail() && !fileStream.eof()) {
        throw std::runtime_error("Failed to parse JSON from file");
    }
    
    return input;
}