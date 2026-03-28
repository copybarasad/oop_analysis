#include "SaveManager.h"
#include "Game.h"
#include <fstream>
#include <iostream>

void SaveManager::saveGameToFile(const Game& game, const std::string& filename) {
    std::ofstream file(filename, std::ios::out);
    if (!file.is_open()) {
        throw LoadException("Не удалось открыть файл для сохранения");
    }

    game.saveToStream(file); // Сохраняем данные игры

    file.close();
    std::cout << "Игра сохранена в файл " << filename << std::endl;
}

void SaveManager::loadGameFromFile(Game& game, const std::string& filename) {
    std::ifstream file(filename, std::ios::in);
    if (!file.is_open()) {
        throw LoadException("Не удалось открыть файл для загрузки");
    }

    game.loadFromStream(file); // Загружаем данные игры

    file.close();
    std::cout << "Игра загружена из файла " << filename << std::endl;
}
