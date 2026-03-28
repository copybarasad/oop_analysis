#include "renderer.hpp"
#include "game_state.hpp"
#include "player.hpp"
#include "map.hpp"
#include <iostream>
#include <iomanip>

void Renderer::renderGameState(const GameState& gameState) const {
    std::cout << "\033[2J\033[1;1H";
    std::cout << "=== Jujutsu kaisen - Ход " << gameState.getCurrentTurn() << " ===\n\n";
    
    renderPlayerInfo(*gameState.getPlayer());
    renderSpellInfo(*gameState.getPlayer());
    renderGameStats(gameState);
    std::cout << "\n";
    renderMap(gameState);
    std::cout << "\n";
    
    std::cout << "УПРАВЛЕНИЕ: W/A/S/D=Движение | ПРОБЕЛ+W/A/S/D=Атака | Q=Смена оружия\n";
    std::cout << "            1-9=Заклинание | B=Купить заклинание | E=Пропуск | 0=Выход | M=Меню\n";
}

void Renderer::renderMap(const GameState& gameState) const {
    const Map* map = gameState.getMap();
    if (!map) return;
    
    std::cout << "Карта:\n  ";
    for (int x = 0; x < map->getLength(); x++) {
        std::cout << std::setw(2) << x % 10 << " ";
    }
    std::cout << "\n";
    
    for (int y = 0; y < map->getWidth(); y++) {
        std::cout << std::setw(2) << y << " ";
        for (int x = 0; x < map->getLength(); x++) {
            std::cout << getCellSymbol(gameState, x, y) << "  ";
        }
        std::cout << "\n";
    }
    
    std::cout << "\nОбозначения: P=Игрок, E=Враг, B=База, #=Стена, ~=Ловушка замедления\n";
}

void Renderer::renderPlayerInfo(const Player& player) const {
    std::cout << "Игрок: Здоровье " << player.getHealthPoints() << "/" 
              << player.getMaxHealthPoints() << " в (" 
              << player.getPositionX() << ", " << player.getPositionY() << ")\n";
    std::cout << "Оружие: " << (player.getCurrentWeapon() == WeaponType::SWORD ? "Меч" : "Лук") 
              << " | Может двигаться: " << (player.canMove() ? "Да" : "Нет")
              << " | Может атаковать: " << (player.canAttack() ? "Да" : "Нет") << "\n";
}

void Renderer::renderSpellInfo(const Player& player) const {
    const PlayerHand* hand = player.getSpellHand();
    if (hand) {
        std::cout << "Заклинания: " << hand->getSize() << "/" << hand->getMaxSize() 
                  << " | Очки заклинаний: " << hand->getSpellPoints() << "\n";
    }
}

void Renderer::renderGameStats(const GameState& gameState) const {
    int aliveEnemies = 0;
    for (const auto& enemy : gameState.getEnemies()) {
        if (enemy && enemy->isAlive()) {
            aliveEnemies++;
        }
    }
    
    int aliveBuildings = 0;
    for (const auto& building : gameState.getBuildings()) {
        if (building && building->isAlive()) {
            aliveBuildings++;
        }
    }
    
    std::cout << "Враги: " << aliveEnemies 
              << " | Базы: " << aliveBuildings 
              << " | Очки: " << gameState.getScore() << "\n";
}

char Renderer::getCellSymbol(const GameState& gameState, int x, int y) const {
    const Map* map = gameState.getMap();
    const Player* player = gameState.getPlayer();
    
    if (!map || !player) return '?';
    
    if (player->isAlive() && player->getPositionX() == x && player->getPositionY() == y) {
        return 'P';
    }
    
    for (const auto& enemy : gameState.getEnemies()) {
        if (enemy && enemy->isAlive() && 
            enemy->getPositionX() == x && enemy->getPositionY() == y) {
            return 'E';
        }
    }
    
    for (const auto& building : gameState.getBuildings()) {
        if (building && building->isAlive() && 
            building->getPositionX() == x && building->getPositionY() == y) {
            return 'B';
        }
    }
    
    const Cell& cell = map->getCell(x, y);
    
    if (!cell.isPassable()) {
        return '#';
    }
    
    if (!cell.isEmptyCell()) {
        return 'X';
    }
    
    switch (cell.getCellType()) {
        case Cell::Type::SLOWING: return '~';
        case Cell::Type::BLOCKING: return '#';
        default: return '.';
    }
}