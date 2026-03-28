#include "SaveLoadManager.h"
#include "GameContext.h"
#include <fstream>
#include <iostream>
#include <algorithm>

std::string SaveLoadManager::getSavePath() const {
    return saveDirectory + "/" + defaultFilename;
}

bool SaveLoadManager::ensureSaveDirectory() const {
    try {
        if (std::filesystem::exists(saveDirectory)) {
            return true;
        }
        
        bool created = std::filesystem::create_directories(saveDirectory);
        if (!created) {
            throw DirectoryCreationException(saveDirectory);
        }
        
        return true;
        
    } catch (const std::filesystem::filesystem_error& e) {
        throw DirectoryCreationException(saveDirectory + " (" + e.what() + ")");
    } catch (const std::exception& e) {
        throw SaveLoadException(std::string("Неизвестная ошибка при создании директории: ") + e.what());
    }
}

bool SaveLoadManager::saveGame(GameContext& context) {
    try {
        if (!ensureSaveDirectory()) {
            throw DirectoryCreationException(saveDirectory);
        }
        
        std::string filepath = getSavePath();
        auto& world = context.getController().getWorld();
        auto& generator = context.getController().getGenerator();

        int currentLevel = context.getLevelManager().getCurrentLevelNumber();

        nlohmann::json saveData;
        saveData["game_world"] = Serializer::serializeGameWorld(world, generator);
        saveData["current_level"] = currentLevel;
        
        std::ofstream file(filepath);
        if (!file.is_open()) {
            throw FileOpenException(filepath);
        }
        
        file << saveData.dump(4);
        file.close();
        
        std::cout << "Игра успешно сохранена в файл: " << filepath << std::endl;
        std::cout << "Уровень: " << currentLevel << std::endl;
        
        return true;
        
    } catch (const SaveLoadException& e) {
        std::cout << "Ошибка сохранения: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cout << "Неизвестная ошибка сохранения: " << e.what() << std::endl;
        return false;
    }
}

bool SaveLoadManager::loadGame(GameContext& context) {
    try {
        std::string filepath = getSavePath();

        if (!std::filesystem::exists(filepath)) {
            throw FileNotFoundException(filepath);
        }
        
        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw FileOpenException(filepath);
        }

        nlohmann::json saveData;
        try {
            file >> saveData;
        } catch (const nlohmann::json::exception& e) {
            throw InvalidDataException("Ошибка парсинга JSON: " + std::string(e.what()));
        }
        file.close();
        
        context.initializeNewLevel();
        
        auto& generator = context.getController().getGenerator();

        auto loadedWorld = Serializer::deserializeGameWorld(saveData["game_world"]);
        
        if (!loadedWorld) {
            throw InvalidDataException("не удалось десериализовать игровой мир");
        }

        context.getController().setWorld(std::move(loadedWorld));

        if (saveData.contains("current_level")) {
            int savedLevel = saveData["current_level"];
            context.getLevelManager().setCurrentLevel(savedLevel - 1);
            std::cout << "Игра успешно загружена! Уровень: " << savedLevel << std::endl;
        } else {
            throw InvalidDataException("отсутствует поле current_level");
        }
        
        return true;
        
    } catch (const SaveLoadException& e) {
        std::cout << "Ошибка загрузки: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cout << "Неизвестная ошибка загрузки: " << e.what() << std::endl;
        return false;
    }
}

bool SaveLoadManager::saveExists() const {
    std::string filepath = getSavePath();
    return std::filesystem::exists(filepath);
}