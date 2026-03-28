#include "file_manager.h"
#include "exceptions.h" 

FileManager::FileManager(const std::string& filename, std::ios_base::openmode mode) : filename_(filename) {
    file_stream_.open(filename, mode);
    if (!file_stream_.is_open()) {
        throw IOException("Failed to open file: " + filename);
    }
}

FileManager::~FileManager() {
    if (file_stream_.is_open()) {
        file_stream_.close();
    }
}

std::fstream& FileManager::getStream() {
    return file_stream_;
}