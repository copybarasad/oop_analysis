//
// Created by Mac on 10.11.2025.
//

#include "Reader.h"
#include <sys/stat.h>
#define ERROR_OPEN_FILE_FOR_READ "Ошибка открытия файла для чтения!"
#define ERROR_FILE_NOT_EXIST "Файл не существует!"
#define ERROR_NO_READ_PERMISSION "Нет прав для чтения из файла!"

Reader::Reader(const std::string &filename) {

    struct stat fileStat{};
    if (stat(filename.c_str(), &fileStat) != 0) {
        throw std::runtime_error(ERROR_FILE_NOT_EXIST);
    }

    if (!(fileStat.st_mode & S_IRUSR)) {
        throw std::runtime_error(ERROR_NO_READ_PERMISSION);
    }

    file.open(filename, std::ios::binary);

    if (!file)
        throw std::runtime_error(ERROR_OPEN_FILE_FOR_READ);
}

Reader::~Reader() {
    file.close();
}
