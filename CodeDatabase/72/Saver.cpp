//
// Created by Mac on 10.11.2025.
//

#include "Saver.h"
#include <sys/stat.h>

#define ERROR_OPEN_FILE_FOR_WRITE "Ошибка открытия файла для записи!"
#define ERROR_NO_WRITE_PERMISSION "Нет прав для записи в файл!"

Saver::Saver(const std::string &filename) {

    struct stat fileStat{};
    if (stat(filename.c_str(), &fileStat) == 0) {
        if (!(fileStat.st_mode & S_IWUSR)) {
            throw std::runtime_error(ERROR_NO_WRITE_PERMISSION);
        }
    }

    file.open(filename, std::ios::binary);

    if (!file)
        throw std::runtime_error(ERROR_OPEN_FILE_FOR_WRITE);
}

Saver::~Saver() {
    file.close();
}