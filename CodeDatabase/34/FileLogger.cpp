#include "FileLogger.h"
#include "../Exceptions.h"

namespace Game {

FileLogger::FileLogger(const std::string& filename) {
    // Открываем файл в режиме дозаписи
    fileStream.open(filename, std::ios::out | std::ios::app);
    if (!fileStream.is_open()) {
        // В конструкторе лучше бросать исключение, если что-то пошло не так
        throw FileOpenException(filename);
    }
}

void FileLogger::log(const std::string& message) {
    if (fileStream.is_open()) {
        fileStream << "[LOG]: " << message << std::endl;
    }
}

}
