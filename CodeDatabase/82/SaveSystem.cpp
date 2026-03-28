#include "SaveSystem.h"
#include "GameState.h"
#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "Spell.h"
#include "SpellHand.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "GameException.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>

std::vector<std::string> SaveSystem::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<std::string> SaveSystem::readLines(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw SaveLoadException("Не удалось открыть файл для чтения: " + filename);
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    return lines;
}

void SaveSystem::writeValue(std::ofstream& file, const std::string& key, int value) {
    file << key << "=" << value << "\n";
}

void SaveSystem::writeValue(std::ofstream& file, const std::string& key, const std::string& value) {
    file << key << "=" << value << "\n";
}

void SaveSystem::writePosition(std::ofstream& file, const std::string& key, int x, int y) {
    file << key << "=" << x << "," << y << "\n";
}

void SaveSystem::saveGame(const GameState& gameState, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw SaveLoadException("Не удалось открыть файл для записи: " + filename);
    }

    const auto& field = gameState.getGameField();
    const auto& player = gameState.getPlayer();
    const auto& enemies = gameState.getEnemies();

    writeValue(file, "field_width", field.getWidth());
    writeValue(file, "field_height", field.getHeight());

    file << "field_grid=\n";
    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            char symbol = field.getCell(x, y).getSymbol();
            if (symbol == 'P') symbol = '.'; 
            if (symbol == 'E') symbol = '.';
            file << symbol;
        }
        file << "\n";
    }

    writePosition(file, "player_pos", player.getPosition().x, player.getPosition().y);
    writeValue(file, "player_health", player.getHealth());
    writeValue(file, "player_max_health", player.getMaxHealth());
    writeValue(file, "player_mana", player.getMana());
    writeValue(file, "player_max_mana", player.getMaxMana());
    writeValue(file, "player_score", player.getScore());
    writeValue(file, "player_melee_damage", player.getMeleeDamage());
    writeValue(file, "player_ranged_damage", player.getRangedDamage());
    writeValue(file, "player_combat_mode", static_cast<int>(player.getCombatMode()));

    writeValue(file, "player_spell_count", player.getSpellHand().getSpellCount());
    for (size_t i = 0; i < player.getSpellHand().getSpellCount(); ++i) {
        const Spell* spell = player.getSpellHand().getSpell(i);
        if (spell) {
            file << "player_spell_" << i << "_type=";
            if (dynamic_cast<const DirectDamageSpell*>(spell)) {
                file << "direct\n";
            } else if (dynamic_cast<const AreaDamageSpell*>(spell)) {
                file << "area\n";
            } else {
                file << "unknown\n";
            }
            writeValue(file, "player_spell_" + std::to_string(i) + "_name", spell->getName());
            writeValue(file, "player_spell_" + std::to_string(i) + "_damage", spell->getDamage());
            writeValue(file, "player_spell_" + std::to_string(i) + "_mana_cost", spell->getManaCost());
            writeValue(file, "player_spell_" + std::to_string(i) + "_range", spell->getRange());
            if (const auto* areaSpell = dynamic_cast<const AreaDamageSpell*>(spell)) {
                writeValue(file, "player_spell_" + std::to_string(i) + "_area_size", areaSpell->getAreaSize());
            }
        }
    }

    writeValue(file, "enemy_count", static_cast<int>(enemies.size()));
    for (size_t i = 0; i < enemies.size(); ++i) {
        const auto& enemy = enemies[i];
        if (enemy.isAlive()) {
            writePosition(file, "enemy_" + std::to_string(i) + "_pos", enemy.getPosition().x, enemy.getPosition().y);
            writeValue(file, "enemy_" + std::to_string(i) + "_health", enemy.getHealth());
            writeValue(file, "enemy_" + std::to_string(i) + "_damage", enemy.getDamage());
        } else {
            writeValue(file, "enemy_" + std::to_string(i) + "_alive", 0);
        }
    }

    file.close();
    std::cout << "Игра сохранена в " << filename << std::endl;
}

int SaveSystem::readIntValue(const std::vector<std::string>& lines, const std::string& key) {
    for (const auto& line : lines) {
        if (line.substr(0, key.length() + 1) == key + "=") {
            try {
                return std::stoi(line.substr(key.length() + 1));
            } catch (...) {
                throw SaveLoadException("Неверный формат числа для ключа: " + key);
            }
        }
    }
    throw SaveLoadException("Ключ не найден: " + key);
}

std::string SaveSystem::readStringValue(const std::vector<std::string>& lines, const std::string& key) {
    for (const auto& line : lines) {
        if (line.substr(0, key.length() + 1) == key + "=") {
            return line.substr(key.length() + 1);
        }
    }
    throw SaveLoadException("Ключ не найден: " + key);
}

std::pair<int, int> SaveSystem::readPositionValue(const std::vector<std::string>& lines, const std::string& key) {
    std::string posStr = readStringValue(lines, key);
    auto parts = split(posStr, ',');
    if (parts.size() != 2) {
        throw SaveLoadException("Неверный формат позиции для ключа: " + key);
    }
    try {
        int x = std::stoi(parts[0]);
        int y = std::stoi(parts[1]);
        return {x, y};
    } catch (...) {
        throw SaveLoadException("Неверный формат координат для ключа: " + key);
    }
}

std::unique_ptr<GameState> SaveSystem::loadGame(const std::string& filename) {
    auto lines = readLines(filename);

    int width = readIntValue(lines, "field_width");
    int height = readIntValue(lines, "field_height");

    auto gameState = GameState::createEmptyGame(width, height);
    auto& field = const_cast<GameField&>(gameState->getGameField());

    int gridStartLine = -1;
    for (size_t i = 0; i < lines.size(); ++i) {
        if (lines[i] == "field_grid=") {
            gridStartLine = i + 1;
            break;
        }
    }
    if (gridStartLine == -1) {
        throw SaveLoadException("Не найдена сетка поля в файле: " + filename);
    }

    for (int y = 0; y < height && gridStartLine + y < static_cast<int>(lines.size()); ++y) {
        const std::string& row = lines[gridStartLine + y];
        for (int x = 0; x < width && x < static_cast<int>(row.length()); ++x) {
            char c = row[x];
            CellType type = (c == '#') ? CellType::WALL : CellType::EMPTY;
            field.setCellType(Position(x, y), type);
        }
    }

    std::pair<int, int> playerPosPair = readPositionValue(lines, "player_pos");
    int playerX = playerPosPair.first;
    int playerY = playerPosPair.second;

    int playerHealth = readIntValue(lines, "player_health");
    int playerMaxHealth = readIntValue(lines, "player_max_health");
    int playerMana = readIntValue(lines, "player_mana");
    int playerMaxMana = readIntValue(lines, "player_max_mana");
    int playerScore = readIntValue(lines, "player_score");
    int playerMeleeDamage = readIntValue(lines, "player_melee_damage");
    int playerRangedDamage = readIntValue(lines, "player_ranged_damage");
    int combatModeInt = readIntValue(lines, "player_combat_mode");
    CombatMode playerCombatMode = (combatModeInt == 0) ? CombatMode::MELEE : CombatMode::RANGED;

    auto& player = const_cast<Player&>(gameState->getPlayer());
    player.setPosition(Position(playerX, playerY));
    player.setHealth(playerHealth);
    player.setMaxHealth(playerMaxHealth);
    player.setMana(playerMana);
    player.setMaxMana(playerMaxMana);
    player.setScore(playerScore);
    player.setMeleeDamage(playerMeleeDamage);
    player.setRangedDamage(playerRangedDamage);
    player.setCombatMode(playerCombatMode);

    auto& spellHand = player.getSpellHand();
    while (spellHand.getSpellCount() > 0) {
        spellHand.removeSpell(0);
    }

    int spellCount = readIntValue(lines, "player_spell_count");
    for (int i = 0; i < spellCount; ++i) {
        std::string type = readStringValue(lines, "player_spell_" + std::to_string(i) + "_type");
        std::string name = readStringValue(lines, "player_spell_" + std::to_string(i) + "_name");
        int damage = readIntValue(lines, "player_spell_" + std::to_string(i) + "_damage");
        int manaCost = readIntValue(lines, "player_spell_" + std::to_string(i) + "_mana_cost");
        int range = readIntValue(lines, "player_spell_" + std::to_string(i) + "_range");

        std::unique_ptr<Spell> spell;
        if (type == "direct") {
            spell = std::make_unique<DirectDamageSpell>(name, damage, manaCost, range);
        } else if (type == "area") {
            int areaSize = readIntValue(lines, "player_spell_" + std::to_string(i) + "_area_size");
            spell = std::make_unique<AreaDamageSpell>(name, damage, manaCost, range, areaSize);
        } else {
            throw SaveLoadException("Неизвестный тип заклинания: " + type);
        }
        spellHand.addSpell(std::move(spell));
    }

    int enemyCount = readIntValue(lines, "enemy_count");
    gameState->getEnemies().clear();
    for (int i = 0; i < enemyCount; ++i) {
        try {
            int aliveCheck = readIntValue(lines, "enemy_" + std::to_string(i) + "_alive");
            if (aliveCheck == 0) continue;
        } catch (...) {
        }
        std::pair<int, int> enemyPosPair = readPositionValue(lines, "enemy_" + std::to_string(i) + "_pos");
        int enemyX = enemyPosPair.first;
        int enemyY = enemyPosPair.second;
        int enemyHealth = readIntValue(lines, "enemy_" + std::to_string(i) + "_health");
        int enemyDamage = readIntValue(lines, "enemy_" + std::to_string(i) + "_damage");

        Enemy enemy(enemyX, enemyY);
        enemy.setHealth(enemyHealth);
        enemy.setDamage(enemyDamage);
        gameState->addEnemy(enemy);
    }

    std::cout << "Игра загружена из " << filename << std::endl;
    return gameState;
}