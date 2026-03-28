#include "FileModel.h"

#include <stdexcept>

FileModel::FileModel(const std::string& filename, Mode mode)
    : mode(mode), fileName(filename) {
    std::ios::openmode openMode = std::ios::binary;
    if (mode == Mode::Read) {
        openMode |= std::ios::in;
    } else {
        openMode |= std::ios::out | std::ios::trunc;
    }

    fileStream.exceptions(std::ios::badbit);
    fileStream.open(fileName, openMode);

    if (!fileStream.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + fileName);
    }
}

FileModel::~FileModel() {
    if (fileStream.is_open()) {
        try {
            fileStream.close();
        } catch (...) {
            // suppress exceptions in destructor
        }
    }
}

std::fstream& FileModel::stream() {
    return fileStream;
}

