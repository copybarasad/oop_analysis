#include "FileController.h"
#include <sstream>
#include <iomanip>
#include <functional>

std::string computeHash(const std::string& data) {
    std::size_t hash = std::hash<std::string>{}(data);
    return std::to_string(hash);
}

void FileController::saveJson(const std::string& filename, const nlohmann::json& data) const {
    try {
        // Преобразуем данные в строку JSON
        std::string jsonString = data.dump(4);
        
        // Вычисляем хэш содержимого
        std::string contentHash = computeHash(jsonString);
        
        FileModel file(filename, FileModel::Mode::Write);
        auto& stream = file.stream();
        
        // Записываем данные в файл
        stream << jsonString;
        if (!stream.good()) {
            throw SaveException("Ошибка при записи в файл: " + filename);
        }
        
        // Сохраняем хэш в отдельный файл
        std::string hashFilename = filename + ".hash";
        FileModel hashFile(hashFilename, FileModel::Mode::Write);
        auto& hashStream = hashFile.stream();
        hashStream << contentHash;
        if (!hashStream.good()) {
            throw SaveException("Ошибка при записи хэш-файла: " + hashFilename);
        }
        
    } catch (const SaveLoadException&) {
        throw;
    } catch (const std::exception& e) {
        throw SaveException(std::string("Ошибка сохранения: ") + e.what());
    }
}

nlohmann::json FileController::loadJson(const std::string& filename, bool checkHash) const {
    try {
        FileModel file(filename, FileModel::Mode::Read);
        auto& stream = file.stream();
        
        // Читаем всё содержимое файла
        std::stringstream buffer;
        buffer << stream.rdbuf();
        std::string fileContent = buffer.str();
        
        // Вычисляем хэш загруженного содержимого
        std::string loadedHash = computeHash(fileContent);
        
        // Загружаем ожидаемый хэш из файла
        std::string hashFilename = filename + ".hash";
        FileModel hashFile(hashFilename, FileModel::Mode::Read);
        auto& hashStream = hashFile.stream();
        std::string expectedHash;
        hashStream >> expectedHash;
        
        if (!hashStream.good() && !hashStream.eof()) {
            throw LoadException("Ошибка чтения хэш-файла: " + hashFilename);
        }

        if (checkHash && loadedHash != expectedHash) {
            throw LoadException("Неверный хэш в файле: " + hashFilename);
        }
        
        // Парсим JSON из загруженного содержимого
        nlohmann::json data = nlohmann::json::parse(fileContent);
        return data;
        
    } catch (const nlohmann::json::exception& e) {
        throw LoadException(std::string("Ошибка парсинга JSON: ") + e.what());
    } catch (const SaveLoadException&) {
        throw;
    } catch (const std::exception& e) {
        throw LoadException(std::string("Ошибка загрузки: ") + e.what());
    }
}
