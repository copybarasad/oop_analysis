#include "../include/FileGuard.h"
#include "../include/GameExceptions.h"

FileGuard::FileGuard(const std::string& filename, std::ios::openmode mode)
    : filename(filename), isOpen(false) {
    file.open(filename, mode);
    if (file.is_open()) {
        isOpen = true;
    }
}

FileGuard::~FileGuard() {
    if (isOpen && file.is_open()) {
        file.close();
    }
}

FileGuard::FileGuard(FileGuard&& other) noexcept
    : file(std::move(other.file)), 
      filename(std::move(other.filename)),
      isOpen(other.isOpen) {
    other.isOpen = false;
}

FileGuard& FileGuard::operator=(FileGuard&& other) noexcept {
    if (this != &other) {
        if (isOpen && file.is_open()) {
            file.close();
        }
        
        file = std::move(other.file);
        filename = std::move(other.filename);
        isOpen = other.isOpen;
        
        other.isOpen = false;
    }
    return *this;
}

std::fstream& FileGuard::getStream() {
    return file;
}

bool FileGuard::isFileOpen() const {
    return isOpen && file.is_open();
}

void FileGuard::close() {
    if (isOpen && file.is_open()) {
        file.close();
        isOpen = false;
    }
}
