#include "save_manager.h"
#include "save_exceptions.h"
#include <iostream>
#include <fstream>
#include <iomanip>

bool SaveManager::SaveExists() {
    std::ifstream file(SAVE_FILE);
    bool exists = file.is_open();
    file.close();
    return exists;
}

bool SaveManager::SaveGame(int level, const GameField& field, 
                          const Player& player, const EnemyManager& em) {
    try {
        FileHandle file(SAVE_FILE, std::ios::out);
        auto& f = file.get();

        json save_data;
        save_data["version"] = "1.0";
        save_data["level"] = level;
        save_data["field"] = GameSerializer::GameFieldToJson(field);
        save_data["player"] = GameSerializer::PlayerToJson(player);
        save_data["enemy_manager"] = GameSerializer::EnemyManagerToJson(em);
        
        f << std::setw(4) << save_data;
        
        std::cout << "Игра успешно сохранена в " << SAVE_FILE << "!\n";
        return true;

    } catch (const SaveFileOpenError& e) {
        std::cout << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cout << "Ошибка при сохранении: " << e.what() << "\n";
    }
    return false;
}

bool SaveManager::LoadGame(int& level, GameField& field, 
                          Player& player, EnemyManager& em) {
    try {
        FileHandle file(SAVE_FILE, std::ios::in);
        auto& f = file.get();

        json save_data;
        f >> save_data;

        GameSerializer::ValidateGameState(save_data);
        level = save_data["level"].get<int>();
        GameSerializer::JsonToGameField(save_data["field"], field);
        GameSerializer::JsonToPlayer(save_data["player"], player);
        GameSerializer::JsonToEnemyManager(save_data["enemy_manager"], em);

        return true;

    } catch (const SaveFileOpenError& e) {
        std::cout << e.what() << "\n";
    } catch (const SaveFileCorrupted& e) {
        std::cout << e.what() << "\n";
    } catch (const json::exception& e) {
        std::cout << "Ошибка формата JSON: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cout << "Неизвестная ошибка при загрузке: " << e.what() << "\n";
    }
    
    std::cout << "Начинается новая игра.\n";
    return false;
}
