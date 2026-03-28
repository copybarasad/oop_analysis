#include "console_render.h"
#include <cstdlib>   
#include <cmath>
#include <iostream>

void ConsoleRenderer::drawGame(const GameMap& map, const EntityManager& entities, int level) {
    system("clear");
    std::cout << "=== LEVEL " << level << " ===\n";

    for (int y = 0; y < map.getHeight(); y++) {
        for (int x = 0; x < map.getWidth(); x++) {
            Position pos(x, y);
            const MapCell& cell = map.getCell(pos);
            char symbol = '.';

            if (cell.getType() == MapCell::Type::WALL) symbol = '#';
            else if (entities.getPlayer().getPosition().equals(pos)) symbol = 'P';
            else if (cell.getType() == MapCell::Type::LOW) symbol = '~';
            else if (cell.getType() == MapCell::Type::TRAP) symbol = '^';

            for (Enemy* enemy : entities.getEnemies()) {
                if (enemy->getPosition().equals(pos)) { symbol = 'E'; break; }
            }

            for (EnemyTower* tower : entities.getTowers()) {
                if (tower->getPosition().equals(pos)) { symbol = 'I'; break; }
            }

            for (EnemyBuilding* building : entities.getBuildings()) {
                if (building->getPosition().equals(pos)) { symbol = 'B'; break; }
            }

            for (Follower* follower : entities.getFollowers()) {
                if (follower->getPosition().equals(pos)) { symbol = 'F'; break; }
            }

            std::cout << symbol << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n=== Игрок ===\n";
    std::cout << entities.getPlayer().getStatusString() << "\n";

    std::cout << "\n=== Враги ===\n";
    int enemyCount = 0;
    for (Enemy* enemy : entities.getEnemies()) {
        if (enemy->isAlive()) {
            enemyCount++;
            std::cout << "Враг " << enemyCount << ": " << enemy->getStatusString() << "\n";
        }
    }

    std::cout << "\n=== Союзники ===\n";
    int follCount = 0;
    for (Follower* f : entities.getFollowers()) {
        if (f->isAlive()) {
            follCount++;
            std::cout << "Союзник " << follCount << ": " << f->getStatusString() << "\n";
        }
    }

    std::cout << "\n=== Башни ===\n";
    for (EnemyTower* tower : entities.getTowers()) {
        if (tower->isAlive()) std::cout << tower->getStatusString() << "\n";
    }

    std::cout << "\nУправление: " << getControlsString() << "\n";
}

std::string ConsoleRenderer::getControlsString() const {
    if (!inputHandler) {
        return "wasd-движение, t-режим боя, c-заклинание, v-сохранение, q-выход";
    }
    
    std::string controls;
    const auto& keyBindings = inputHandler->getKeyBindings();
    
    std::map<GameCommand::Type, char> reverseMap;
    for (const auto& pair : keyBindings) {
        reverseMap[pair.second] = pair.first;
    }
    
    auto addControl = [&](GameCommand::Type cmd, const std::string& desc) {
        auto it = reverseMap.find(cmd);
        if (it != reverseMap.end()) {
            if (!controls.empty()) controls += ", ";
            controls += char(std::toupper(it->second));
            controls += "-";
            controls += desc;
        }
    };
    
    addControl(GameCommand::Type::MOVE_UP, "вверх");
    addControl(GameCommand::Type::MOVE_DOWN, "вниз");
    addControl(GameCommand::Type::MOVE_LEFT, "влево");
    addControl(GameCommand::Type::MOVE_RIGHT, "вправо");
    addControl(GameCommand::Type::TOGGLE_COMBAT, "режим боя");
    addControl(GameCommand::Type::CAST_SPELL, "заклинание");
    addControl(GameCommand::Type::SAVE_GAME, "сохранение");
    

    auto quitIt = reverseMap.find(GameCommand::Type::QUIT);
    if (quitIt != reverseMap.end()) {
        if (!controls.empty()) controls += ", ";
        controls += char(std::toupper(quitIt->second));
        controls += "-выход";
    }
    
    return controls;
}

void ConsoleRenderer::drawTarget(const GameMap& map, const EntityManager& entities, const Position& cursor, int radius) {
    system("clear");
    for (int y = 0; y < map.getHeight(); y++) {
        for (int x = 0; x < map.getWidth(); x++) {
            Position pos(x, y);
            char symbol = '.';
            bool inRange = false;

            const MapCell& cell = map.getCell(pos);
            if (cell.getType() == MapCell::Type::WALL) symbol = '#';
            else if (cell.getType() == MapCell::Type::LOW) symbol = '~';
            else if (cell.getType() == MapCell::Type::TRAP) symbol = '^';

            int dist = std::abs(entities.getPlayer().getPosition().getX() - x) +
                       std::abs(entities.getPlayer().getPosition().getY() - y);
            if (dist <= radius) inRange = true;

            if (entities.getPlayer().getPosition().equals(pos)) symbol = 'P';
            for (Enemy* enemy : entities.getEnemies())
                if (enemy->getPosition().equals(pos)) symbol = 'E';
            for (Follower* f : entities.getFollowers())
                if (f->getPosition().equals(pos)) symbol = 'F';
            for (EnemyTower* tower : entities.getTowers())
                if (tower->getPosition().equals(pos)) symbol = 'I';

            if (cursor.equals(pos)) std::cout << "\033[41m" << symbol << "\033[0m ";
            else if (inRange) std::cout << "\033[43m" << symbol << "\033[0m ";
            else std::cout << symbol << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\nУправление: " << getControlsString() << "\n";
    std::cout << "\nВыберите цель: ";
}

void ConsoleRenderer::drawMainMenu() {
    system("clear");
    std::cout << "=== ГЛАВНОЕ МЕНЮ ===\n";
    std::cout << "1. Новая игра\n";
    std::cout << "2. Загрузить игру\n";
    std::cout << "3. Выйти\n";
    std::cout << "Выберите действие: ";
    std::cout << std::flush;
}

void ConsoleRenderer::drawLevelUpMenu() {
    system("clear");
    std::cout << "=== УРОВЕНЬ ПОВЫШЕН ===\n";
    std::cout << "1. Увеличить здоровье (+50 HP)\n";
    std::cout << "2. Увеличить урон (+30 урона)\n";
    std::cout << "3. Увеличить уровень заклинаний\n";
    std::cout << "Ваш выбор: ";
}

void ConsoleRenderer::showMessage(const std::string& msg) {
    std::cout << msg << "\n";
}

void ConsoleRenderer::deadPlayer() {
    system("clear");
    std::cout << "Нажмите 1, если хотите начать игру занаво";
    std::cout << "Нажмите любую другую клавишу, чтобы выйти";
}

void ConsoleRenderer::showSpells(const PlayerHand& hand) {
    std::cout << "=== Заклинания в руке ===\n";
    for (int i = 0; i < hand.getSpellCount(); i++) {
        ISpell* spell = hand.getSpell(i);
        std::cout << i << ". " << spell->getName() << "\n";
    }
    std::cout << "Выберите заклинание: ";
}


