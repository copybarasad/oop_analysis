#include "fileguard.h"

FileGuard::FileGuard(std::string name) :
    filename(name), commited(false)
{
    // Сразу открываем основной файл, без временных файлов
    file.open(filename, std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create save file: " + filename);
    }
}

FileGuard::~FileGuard(){
    if (!commited && file.is_open()) {
        file.close();
        // Не удаляем файл, так как он основной
    }
}

void FileGuard::commit() {
    if (!file.is_open()) {
        throw std::runtime_error("File not opened");
    }

    // Просто закрываем файл
    file.close();
    commited = true;
    // Без переименования, так как файл уже основной
}

std::fstream& FileGuard::get_file()
{
    return file;
}

// Остальные методы можно оставить как есть
