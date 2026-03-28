#include "file_raii.hpp"

FileOpenError::FileOpenError(const std::string& msg)
    : std::runtime_error(msg) {}

FileReadError::FileReadError(const std::string& msg)
    : std::runtime_error(msg) {}

InvalidSaveFormat::InvalidSaveFormat(const std::string& msg)
    : std::runtime_error(msg) {}

FileRAII::FileRAII(const std::string& filename, std::ios::openmode mode) {
    file_.open(filename, mode);
    if (!file_.is_open())
        throw FileOpenError("Ошибка: невозможно открыть файл: " + filename);
}

FileRAII::~FileRAII() {
    if (file_.is_open())
        file_.close();
}

std::fstream& FileRAII::get() {
    return file_;
}