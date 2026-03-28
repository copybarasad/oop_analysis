#include "save_system.h"
#include "game.h"
#include "constants.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h"
#include "player.h"
#include "enemy.h"
#include "tower.h"
#include "damage_tower.h"
#include <iostream>
#include <sstream>
#include <algorithm>

// Сохранение игры
void SaveSystem::saveGame(const Game& game) {
    std::ofstream file(saveFileName);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для сохранения: " + saveFileName);
    }

    try {
        file << "level: " << game.getCurrentLevel() << "\n";
        
        saveField(file, *game.getField());
        savePlayer(file, *game.getPlayer());
        saveEnemies(file, game.getEnemies());
        saveTowers(file, game.getTower(), game.getDamageTower());
        
        file << "end" << "\n";
        
    } catch (const std::exception& e) {
        throw std::runtime_error("Ошибка при записи в файл: " + std::string(e.what()));
    }
}

void SaveSystem::saveField(std::ofstream& file, const Field& field) {
    file << "field: " << field.getWidth() << " " << field.getLength() << "\n";
    file << "cells: ";
    
    for (int x = 0; x < field.getWidth(); x++) {
        for (int y = 0; y < field.getLength(); y++) {
            Field& nonConstField = const_cast<Field&>(field);
            Type cellType = nonConstField.getCell(x, y).getType();
            file << static_cast<int>(cellType) << " ";
        }
    }
    file << "\n";
}

void SaveSystem::savePlayer(std::ofstream& file, const Player& player) {
    file << "player: " << player.getname() << " "
         << player.getX() << " " << player.getY() << " "
         << player.getLives() << " " << player.getScore() << " "
         << static_cast<int>(player.getCombatType()) << " "
         << player.getPower() << " " << player.getRadius() << "\n";
    
    Player& nonConstPlayer = const_cast<Player&>(player);
    const Hand& hand = nonConstPlayer.getSpellHand();
    file << "spells: " << hand.getSpellCount() << " ";
    
    for (int i = 0; i < hand.getSpellCount(); i++) {
        Spell* spell = hand.getSpell(i);
        std::string spellName = spell->getName();
        std::replace(spellName.begin(), spellName.end(), ' ', '_');
        
        if (SpellBasis* spellBasis = dynamic_cast<SpellBasis*>(spell)) {
            file << spellName << ":" << spellBasis->getPower() << " ";
        } else {
            file << spellName << ":0 ";
        }
    }
    file << "\n";
}

void SaveSystem::saveEnemies(std::ofstream& file, const std::vector<Enemy*>& enemies) {
    int aliveCount = 0;
    for (Enemy* enemy : enemies) {
        if (enemy->isAlive()) {
            aliveCount++;
        }
    }
    
    file << "enemies: " << aliveCount << "\n";
    
    for (Enemy* enemy : enemies) {
        if (enemy->isAlive()) {
            file << "enemy: " << enemy->getname() << " "
                 << enemy->getX() << " " << enemy->getY() << " "
                 << enemy->getLives() << " " << enemy->getAttackPower() << "\n";
        }
    }
}

void SaveSystem::saveTowers(std::ofstream& file, const Tower* tower, const DamageTower* damageTower) {
    file << "towers: " << (tower != nullptr) << " " << (damageTower != nullptr && damageTower->isAlive()) << "\n";
    
    if (tower) {
        std::string towerName = tower->getName();
        std::replace(towerName.begin(), towerName.end(), ' ', '_');
        file << "tower: " << towerName << " "
             << tower->getX() << " " << tower->getY() << "\n";
    }
    
    if (damageTower && damageTower->isAlive()) {
        std::string damageTowerName = damageTower->getName();
        std::replace(damageTowerName.begin(), damageTowerName.end(), ' ', '_');
        file << "damage_tower: " << damageTowerName << " "
             << damageTower->getX() << " " << damageTower->getY() << " "
             << damageTower->getLives() << " " << damageTower->getCooldownTimer() << " "
             << damageTower->getSpellPower() << "\n";
    }
}

// Загрузка игры
bool SaveSystem::loadGame(Game& game) {
    std::ifstream file(saveFileName);
    if (!file.is_open()) {
        throw std::runtime_error("Файл сохранения не найден: " + saveFileName);
    }

    try {
        std::string token;
        
        file >> token;
        if (token != "level:") {
            throw std::runtime_error("Некорректный формат файла: ожидался level");
        }
        int level;
        file >> level;
        game.setCurrentLevel(level);
        
        loadField(file, game);
        loadPlayer(file, game);
        loadEnemies(file, game);
        loadTowers(file, game);
        
        file >> token;
        if (token != "end") {
            throw std::runtime_error("Некорректный конец файла сохранения");
        }
        
        game.setGameRunning(true);
        return true;
        
    } catch (const std::exception& e) {
        throw std::runtime_error("Ошибка при чтении файла: " + std::string(e.what()));
    }
}

void SaveSystem::loadField(std::ifstream& file, Game& game) {
    std::string token;
    file >> token;
    if (token != "field:") {
        throw std::runtime_error("Ошибка формата: ожидалось FIELD:");
    }
    
    int width, length;
    file >> width >> length;
    
    Field* newField = new Field(width, length, 0);
    
    file >> token;
    if (token != "cells:") {
        delete newField;
        throw std::runtime_error("Ошибка формата: ожидалось cells:");
    }
    
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < length; y++) {
            int cellType;
            if (!(file >> cellType)) {
                delete newField;
                throw std::runtime_error("Ошибка чтения данных клетки");
            }
            
            Cell& cell = newField->getCell(x, y);
            switch (static_cast<Type>(cellType)) {
                case Type::EMPTY:
                    cell.setEmpty();
                    break;
                case Type::OBSTACLE:
                    cell.setObstacle();
                    break;
                case Type::TRAP:
                    cell.setTrap();
                    break;
                case Type::DAMAGE_TRAP:
                    cell.setDamageTrap();
                    break;
                case Type::TOWER:
                    cell.setTower();
                    break;
                case Type::DAMAGE_TOWER:
                    cell.setDamageTower();
                    break;
                case Type::PLAYER:
                case Type::ENEMY:
                    cell.setEmpty();
                    break;
                default:
                    cell.setEmpty();
                    break;
            }
        }
    }
    
    game.setField(newField);
}

void SaveSystem::loadPlayer(std::ifstream& file, Game& game) {
    std::string token;
    file >> token;
    if (token != "player:") {
        throw std::runtime_error("Ошибка формата: ожидалось player:");
    }
    
    std::string name;
    int x, y, lives, score, combatType, power, radius;
    
    if (!(file >> name >> x >> y >> lives >> score >> combatType >> power >> radius)) {
        throw std::runtime_error("Ошибка чтения данных игрока");
    }
    
    Player* newPlayer = new Player(name, *game.getField(), x, y, constants::HAND_SIZE);
    newPlayer->setLives(lives);
    newPlayer->setScore(score);
    newPlayer->setCombatType(static_cast<Player::CombatStyle>(combatType));
    newPlayer->upgradeDamage(power - constants::PLAYER_NEAR_COMBAT_POWER);
    
    if (game.getField()->isValidPosition(x, y)) {
        game.getField()->getCell(x, y).setPlayer();
    }
    
    file >> token;
    if (token != "spells:") {
        delete newPlayer;
        throw std::runtime_error("Ошибка формата: ожидалось SPELLS:");
    }
    
    int spellCount;
    if (!(file >> spellCount)) {
        delete newPlayer;
        throw std::runtime_error("Ошибка чтения количества заклинаний");
    }
    
    Hand& hand = newPlayer->getSpellHand();
    while (hand.getSpellCount() > 0) {
        hand.removeSpell(0);
    }
    
    for (int i = 0; i < spellCount; i++) {
        std::string spellData;
        if (!(file >> spellData)) {
            delete newPlayer;
            throw std::runtime_error("Ошибка чтения данных заклинания");
        }
        
        size_t colonPos = spellData.find(':');
        if (colonPos == std::string::npos) {
            delete newPlayer;
            throw std::runtime_error("Некорректный формат данных заклинания");
        }
        
        std::string spellName = spellData.substr(0, colonPos);
        int spellPower = std::stoi(spellData.substr(colonPos + 1));
        
        std::replace(spellName.begin(), spellName.end(), '_', ' ');
        
        Spell* spell = createSpellByName(spellName, spellPower);
        if (spell) {
            hand.addSpell(spell);
        }
    }
    
    game.setPlayer(newPlayer);
}

void SaveSystem::loadEnemies(std::ifstream& file, Game& game) {
    std::string token;
    file >> token;
    if (token != "enemies:") {
        throw std::runtime_error("Ошибка формата: ожидалось enemies:");
    }
    
    int enemyCount;
    if (!(file >> enemyCount)) {
        throw std::runtime_error("Ошибка чтения количества врагов");
    }
    
    game.clearEnemies();
    
    for (int i = 0; i < enemyCount; i++) {
        file >> token;
        if (token != "enemy:") {
            throw std::runtime_error("Ошибка формата: ожидалось enemy:");
        }
        
        std::string name;
        int x, y, lives, attackPower;
        
        if (!(file >> name >> x >> y >> lives >> attackPower)) {
            throw std::runtime_error("Ошибка чтения данных врага");
        }
        
        Enemy* enemy = new Enemy(name, *game.getField(), x, y);
        enemy->setLives(lives);
        enemy->upgradeDamage(attackPower - constants::ENEMY_DAMAGE_POWER);
        game.addEnemy(enemy);
        
        if (game.getField()->isValidPosition(x, y)) {
            game.getField()->getCell(x, y).setEnemy();
        }
    }
}

void SaveSystem::loadTowers(std::ifstream& file, Game& game) {
    std::string token;
    file >> token;
    if (token != "towers:") {
        throw std::runtime_error("Ошибка формата: ожидалось towers:");
    }
    
    int hasTower, hasDamageTower;
    if (!(file >> hasTower >> hasDamageTower)) {
        throw std::runtime_error("Ошибка чтения данных башен");
    }
    
    if (hasTower) {
        file >> token;
        if (token != "tower:") {
            throw std::runtime_error("Ошибка формата: ожидалось tower:");
        }
        
        std::string name;
        int x, y;
        if (!(file >> name >> x >> y)) {
            throw std::runtime_error("Ошибка чтения данных башни");
        }
        
        std::replace(name.begin(), name.end(), '_', ' ');
        
        Tower* tower = new Tower(name, *game.getField(), x, y);
        game.setTower(tower);
    }
    
    if (hasDamageTower) {
        file >> token;
        if (token != "damage_tower:") {
            throw std::runtime_error("Ошибка формата: ожидалось damage_tower:");
        }
        
        std::string name;
        int x, y, lives, cooldown, spellPower;
        if (!(file >> name >> x >> y >> lives >> cooldown >> spellPower)) {
            throw std::runtime_error("Ошибка чтения данных башни урона");
        }
        
        std::replace(name.begin(), name.end(), '_', ' ');
        
        DamageTower* damageTower = new DamageTower(name, *game.getField(), x, y);
        damageTower->setLives(lives);
        damageTower->setCooldown(cooldown);
        damageTower->upgradeDamage(spellPower - constants::TOWER_SPELL_POWER);
        game.setDamageTower(damageTower);
        
        if (game.getField()->isValidPosition(x, y)) {
            game.getField()->getCell(x, y).setDamageTower();
        }
    }
}

Spell* SaveSystem::createSpellByName(const std::string& name, int power) {
    if (name == "Магическая стрела") {
        DirectDamageSpell* spell = new DirectDamageSpell();
        int upgrades = power - constants::DIRECT_SPELL_POWER;
        spell->upgrade(upgrades);
        return spell;
    } else if (name == "Огненный шар") {
        AreaDamageSpell* spell = new AreaDamageSpell();
        int upgrades = power - constants::AREA_SPELL_POWER;
        spell->upgrade(upgrades);
        return spell;
    } else if (name == "Магическая ловушка") {
        TrapSpell* spell = new TrapSpell();
        int upgrades = power - constants::TRAP_SPELL_POWER;
        spell->upgrade(upgrades);
        return spell;
    }
    return nullptr;
}
