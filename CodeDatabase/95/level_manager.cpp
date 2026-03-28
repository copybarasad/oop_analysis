#include "level_manager.hpp"
#include "game_objects/player.hpp"
#include "game_objects/enemy.hpp"
#include "game_objects/tower.hpp"
#include "game_objects/spells/damage_spell.hpp"
#include "game_objects/spells/aoe_spell.hpp"
#include "game_objects/spells/trap_spell.hpp"
#include <iostream>

LevelManager::LevelManager() : currentLevel(1) {}

void LevelManager::createLevel(eManager& manager, int level) {
    manager.clearEnemies();
    
    int baseSize = 15;
    int width = baseSize + (level - 1) * 2;
    int height = baseSize + (level - 1) * 2;
    
    setupMap(manager, width, height);
    setupPlayer(manager, level);
    setupEnemies(manager, level);
    setupTower(manager, level);
    
    std::cout << "УРОВЕНЬ " << level << "\n";
    std::cout << "Размер карты: " << width << "x" << height << "\n";
}

void LevelManager::setupMap(eManager& manager, int w, int h) {
    Map* map = new Map(w, h);

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            if (x == 0 || x == w-1 || y == 0 || y == h-1) {
                map->setTile(x, y, TileType::Wall);
            } else {
                map->setTile(x, y, TileType::Grass);
            }
        }
    }

    std::uniform_real_distribution<> chance(0.0, 1.0);
    double rock_density = 0.07;
    for (int y = 1; y < h - 1; ++y) {
        for (int x = 1; x < w - 1; ++x) {
            if (chance(gen) < rock_density) {
                map->setTile(x, y, TileType::Wall);
            }
        }
    }

    manager.setMap(map);
}

Position LevelManager::getRandomFreePosition(const eManager& manager) {
    const Map* map = manager.getMap();
    if (!map) {
        return {1, 1};
    }
    
    auto free_cells = map->freeCells();
    if (free_cells.empty()) {
        return {1, 1};
    }
    
    std::uniform_int_distribution<> dist(0, free_cells.size() - 1);
    return free_cells[dist(gen)];
}

void LevelManager::setupPlayer(eManager& manager, int level) {
    Entity* player = manager.getPlayer();
    
    if (!player) {
        Position pos = getRandomFreePosition(manager);
        player = new Player(pos.x, pos.y, 100, 40);
        manager.setPlayer(player);
        
        if (!manager.getPlayerHand()) {
            manager.setPlayerHand(new SpellHand());
        }
    } else {
        Position pos = getRandomFreePosition(manager);
        player->setPos(pos.x, pos.y);
    }

    if (level == 1) {
        player->setHP(100);
        player->setDamage(60);
    }

    if (level > 1) {
        std::cout << "Выберите улучшение для игрока: \n(1) Здоровье +15\n(2) Урон +10\n";
        int choice;
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            std::cout << "Здоровье игрока увеличино на 15 единиц\n";
            player->setHP(player->getHP() + 15);
            break;
        case 2:
            std::cout << "Урон игрока увеличен на 10 единиц\n";
            player->setDamage(player->getDamage() + 10);
            break;
        default:
            break;
        }
    }
    
    auto hand = manager.getPlayerHand();
    if (hand && level > 1) {
        if (hand->getSize() > 1) {
            int cardsToRemove = hand->getSize() / 2;
            std::cout << "Удалено " << cardsToRemove << " заклинаний при переходе на уровень " << level << "\n";
            
            for (int i = 0; i < cardsToRemove; ++i) {
                size_t randomIndex = rand() % hand->getSize();
                hand->removeSpell(randomIndex);
            }
        }
    }
}

void LevelManager::setupEnemies(eManager& manager, int level) {
    int baseEnemyCount = 1;
    int enemyCount = baseEnemyCount + level;

    for (int i = 0; i < enemyCount; ++i) {
        Position pos = getRandomFreePosition(manager);
        Entity* enemy = new Enemy(pos.x, pos.y);
        
        int baseHealth = 40;
        int baseDamage = 5;
        
        enemy->setHP(baseHealth + level * 15);
        enemy->setDamage(baseDamage + level * 3);
        
        manager.setEnemy(enemy);
    }
    
    std::cout << "Создано врагов: " << enemyCount << "\n";
}

void LevelManager::setupTower(eManager& manager, int level) {
    Entity* tower = manager.getTower();
    
    if (!tower) {
        Position pos = getRandomFreePosition(manager);
        tower = new Tower(pos.x, pos.y);
        manager.setTower(tower);
    } else {
        Position pos = getRandomFreePosition(manager);
        tower->setPos(pos.x, pos.y);
    }
    
    int baseHealth = 120;
    tower->setHP(baseHealth + level * 25);
}

void LevelManager::nextLevel() {
    currentLevel++;
}

int LevelManager::getCurrentLevel() {
    return currentLevel;
}
