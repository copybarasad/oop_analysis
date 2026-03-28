#pragma once
#include <string>
#include <vector>
#include "Field.hpp"

// Информация о сохранении для отображения в меню загрузки
struct SaveInfo {
    std::string filename;
    std::string timestamp;
    int level;
    int playerScore;
};

// Класс для управления сохранениями и загрузками игры
class SaveManager {
public:
    // Сохранение текущего состояния игры в файл
    static bool saveGame(const std::string& filename, const Field& field, int level);
    
    // Загрузка состояния игры из файла
    static bool loadGame(const std::string& filename, Field& field, int& level);
    
    // Получение списка всех доступных сохранений
    static std::vector<SaveInfo> getSaveFiles();
    
    // Генерация уникального имени для нового сохранения
    static std::string generateSaveName();

private:
    // Вспомогательные методы для работы с файлами
    static bool ensureSaveDirectoryExists();
    static std::string getSaveDirectoryPath();
};
