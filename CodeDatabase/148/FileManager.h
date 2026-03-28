#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <fstream>
#include <string>
#include "Exceptions.h"

/**
 * @file FileManager.h
 * @brief RAII-обёртка для работы с файлами
 * 
 * Использует идиому RAII для автоматического закрытия файлов.
 */

/**
 * @class FileManager
 * @brief Менеджер файлов с RAII
 * 
 * Автоматически открывает и закрывает файлы, обрабатывает ошибки.
 */
class FileManager {
private:
    std::fstream fileStream;  ///< Файловый поток
    std::string filename;     ///< Имя файла
    bool isOpen;              ///< Флаг: файл открыт
    
public:
    /**
     * @brief Конструктор для записи
     * @param fname Имя файла
     * @param mode Режим открытия (по умолчанию запись)
     * @throws FileWriteException если не удалось открыть для записи
     */
    FileManager(const std::string& fname, std::ios::openmode mode = std::ios::out) 
        : filename(fname), isOpen(false) {
        fileStream.open(filename, mode);
        
        if (!fileStream.is_open()) {
            if (mode & std::ios::out) {
                throw FileWriteException(filename);
            } else {
                throw FileNotFoundException(filename);
            }
        }
        
        isOpen = true;
    }
    
    /**
     * @brief Деструктор - автоматически закрывает файл (RAII)
     */
    ~FileManager() {
        if (isOpen && fileStream.is_open()) {
            fileStream.close();
        }
    }
    
    // Запрещаем копирование
    FileManager(const FileManager&) = delete;
    FileManager& operator=(const FileManager&) = delete;
    
    /**
     * @brief Получить ссылку на поток
     * @return Ссылка на fstream
     */
    std::fstream& getStream() {
        return fileStream;
    }
    
    /**
     * @brief Проверить, открыт ли файл
     * @return true если открыт
     */
    bool isFileOpen() const {
        return isOpen && fileStream.is_open();
    }
};

#endif // FILEMANAGER_H


