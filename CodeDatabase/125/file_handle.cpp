#include "file_handle.h"

FileHandle::FileHandle(const std::string& filename, std::ios::openmode mode)
    : file_(filename, mode) {
    if (!file_.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: \"" + filename + "\". "
                                 "Проверьте права доступа, путь и наличие диска.");
    }
}