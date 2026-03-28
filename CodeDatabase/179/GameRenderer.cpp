#include "GameRenderer.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"

void ConsoleRenderer::renderField(const GameField& field) {
    int width = field.getWidth();
    int height = field.getHeight();
    
    // Верхняя граница
    std::cout << "┌";
    for (int x = 0; x < width; ++x) {
        std::cout << "──";
    }
    std::cout << "┐\n";
    
    for (int y = 0; y < height; ++y) {
        std::cout << "│";
        for (int x = 0; x < width; ++x) {
            Position pos(x, y);
            const Cell& cell = field.getCell(pos);
            std::cout << getCellSymbol(cell, pos, field) << " ";
        }
        std::cout << "│\n";
    }
    
    // Нижняя граница
    std::cout << "└";
    for (int x = 0; x < width; ++x) {
        std::cout << "──";
    }
    std::cout << "┘\n";
}

char ConsoleRenderer::getCellSymbol(const Cell& cell, const Position& pos, const GameField& field) {
    if (cell.hasObject()) {
        auto obj = cell.getObject();
        std::string type = obj->getType();
        
        if (type == "Player") return 'P';
        if (type == "Enemy") return 'E';
        if (type == "EnemyBuilding") return 'B';
        if (type == "EnemyTower") return 'T';
        if (type == "Ally") return 'A';
        if (type == "Trap") return 'X';
    }
    
    switch (cell.getType()) {
        case CellType::IMPASSABLE: return '#';
        case CellType::SLOWING: return '~';
        case CellType::NORMAL: return '.';
    }
    
    return '?';
}

void ConsoleRenderer::renderGameState(const GameField& field, int level, int score) {
    auto player = field.getPlayer();
    if (!player) return;
    
    std::cout << "\n=== УРОВЕНЬ " << level << " ===\n";
    std::cout << "Очки: " << score << " | ";
    std::cout << "Здоровье: " << player->getHealth() << "/" << player->getMaxHealth() << " | ";
    std::cout << "Мана: " << player->getMana() << "/" << player->getMaxMana() << "\n";
    
    auto allies = field.getAllies();
    auto enemies = field.getEnemies();
    auto buildings = field.getBuildings();
    auto towers = field.getTowers();
    
    std::cout << "Союзники: " << allies.size()
              << " | Враги: " << enemies.size()
              << " | Здания: " << buildings.size()
              << " | Башни: " << towers.size() << "\n";
    
    if (player->getFightMode() == FightMode::RANGED) {
        std::cout << "Режим: ДАЛЬНИЙ БОЙ (Урон: " << player->getRangedDamage()
                  << ", Радиус: " << player->getRangedAttackRadius() << ")\n";
    } else {
        std::cout << "Режим: БЛИЖНИЙ БОЙ (Урон: " << player->getMeleeDamage() << ")\n";
    }
    
    std::cout << "Позиция: (" << field.getPlayerPosition().x << ", "
              << field.getPlayerPosition().y << ")\n\n";
    
    std::cout << "Легенда: P-Игрок  A-Союзник  E-Враг  B-Здание  T-Башня  X-Ловушка  #-Стена  ~-Болото  .-Пусто\n";
}

void ConsoleRenderer::renderHelp() {
    std::cout << "\n=== УПРАВЛЕНИЕ ===\n";
    std::cout << "W - Вверх    A - Влево    S - Вниз    D - Вправо\n";
    std::cout << "M - Сменить режим боя    C - Применить заклинание\n";
    std::cout << "R - Дальняя атака (только в RANGED режиме)    P - Показать заклинания\n";
    std::cout << "X - Выйти из игры\n";
}

void ConsoleRenderer::renderMessage(const std::string& message) {
    std::cout << message << std::endl;
}

void ConsoleRenderer::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

std::string ConsoleRenderer::getHealthBar(int health, int maxHealth) {
    int bars = (health * 10) / maxHealth;
    std::string bar = "[";
    for (int i = 0; i < 10; ++i) {
        if (i < bars) bar += "█";
        else bar += " ";
    }
    bar += "]";
    return bar;
}
