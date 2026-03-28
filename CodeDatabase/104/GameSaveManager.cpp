#include "GameSaveManager.h"
#include "Game.h"
#include "Player.h"
#include "GameField.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "MeleeSpell.h"
#include <iostream>
#include <sstream>
#include <memory>

// Сохранение игры в файл
void GameSaveManager::saveGame(const Game& game, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw SaveException("Cannot open file for writing: " + filename);
    }
    file << "MAGIC_BATTLE_SAVE" << std::endl;
    file << "LEVEL " << game.getCurrentLevel() << std::endl;
    savePlayer(file, game.getPlayer());
    saveGameField(file, game.getGameField());

    if (file.fail()) {
        throw SaveException("Failed to write to save file: " + filename);
    }

    std::cout << "Game saved successfully to " << filename << std::endl;
}

// Сохранение данных игрока
void GameSaveManager::savePlayer(std::ofstream& file, const Player& player) const {
    file << "PLAYER_STATS "
        << player.getHealth() << " "
        << player.getMaxHealth() << " "
        << player.getMana() << " "
        << player.getMaxMana() << " "
        << player.getPosition().getX() << " "
        << player.getPosition().getY() << std::endl;

    saveSpells(file, player.getHand());
}
void GameSaveManager::saveSpells(std::ofstream& file, const PlayerHand& hand) const {
    file << "SPELL_COUNT " << hand.getSpellCount() << std::endl;

    for (size_t i = 0; i < hand.getSpellCount(); ++i) {
        SpellCard* spell = hand.getSpell(i);
        if (!spell) continue;

        if (auto directSpell = dynamic_cast<DirectDamageSpell*>(spell)) {
            file << "SPELL DirectDamage "
                << directSpell->getDamage() << " "
                << directSpell->getCastRange() << " "
                << directSpell->getManaCost() << " "
                << (directSpell->isAreaSpell() ? "1" : "0") << std::endl;
        }
        else if (auto areaSpell = dynamic_cast<AreaDamageSpell*>(spell)) {
            file << "SPELL AreaDamage "
                << areaSpell->getDamage() << " "
                << areaSpell->getCastRange() << " "
                << areaSpell->getManaCost() << " "
                << (areaSpell->getCastRange() > 2 ? "1" : "0") << std::endl;
        }
        else if (auto trapSpell = dynamic_cast<TrapSpell*>(spell)) {
            file << "SPELL Trap "
                << trapSpell->getDamage() << " "
                << trapSpell->getManaCost() << " "
                << (trapSpell->isAreaSpell() ? "1" : "0") << std::endl;
        }
        else if (auto meleeSpell = dynamic_cast<MeleeSpell*>(spell)) {
            file << "SPELL Melee "
                << meleeSpell->getDamage() << " "
                << meleeSpell->getManaCost() << " "
                << (meleeSpell->getCastRange() > 0 ? "1" : "0") << std::endl;
        }
    }
}

// Сохранение игрового поля
void GameSaveManager::saveGameField(std::ofstream& file, const GameField& field) const {
    file << "FIELD_SIZE " << field.getWidth() << " " << field.getHeight() << std::endl;
    const auto& obstacles = field.getObstacles();
    file << "OBSTACLES " << obstacles.size() << std::endl;
    for (const auto& obstacle : obstacles) {
        file << "OBSTACLE " << obstacle.getX() << " " << obstacle.getY() << std::endl;
    }
    const auto& enemies = field.getEnemies();
    file << "ENEMIES " << enemies.size() << std::endl;
    for (const auto& enemy : enemies) {
        if (enemy->IsAlive()) {
            file << "ENEMY "
                << enemy->getPositionX() << " "
                << enemy->getPositionY() << " "
                << enemy->GetHealth() << " "
                << enemy->GetDamage() << std::endl;
        }
    }

    const auto& towers = field.getTowers();
    file << "TOWERS " << towers.size() << std::endl;
    for (const auto& tower : towers) {
        file << "TOWER "
            << tower->getPositionX() << " "
            << tower->getPositionY() << " "
            << tower->getAttackRange() << " "
            << tower->getDamage() << " "
            << (tower->canAttack() ? "1" : "0") << std::endl;
    }
    const auto& traps = field.getTraps();
    file << "TRAPS " << traps.size() << std::endl;
    for (const auto& trap : traps) {
        file << "TRAP " << trap.getX() << " " << trap.getY() << std::endl;
    }
}

// Загрузка игры из файла
std::unique_ptr<Game> GameSaveManager::loadGame(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw SaveException("Save file not found: " + filename);
    }

    std::string header;
    std::getline(file, header);

    if (header != "MAGIC_BATTLE_SAVE") {
        throw SaveException("Invalid save file format: " + filename);
    }

    // Читаем уровень
    std::string levelLine;
    std::getline(file, levelLine);
    int level = 1;
    if (levelLine.find("LEVEL") != std::string::npos) {
        level = std::stoi(levelLine.substr(6));
    }
    auto game = std::make_unique<Game>(level);
    auto player = loadPlayer(file);
    auto gameField = loadGameField(file, player.get());

    // Устанавливаем загруженные объекты в игру
    game->setPlayer(std::move(player));
    game->setGameField(std::move(gameField));

    std::cout << "Game loaded successfully! Level: " << level << std::endl;

    return game;
}

// Загрузка данных игрока
std::unique_ptr<Player> GameSaveManager::loadPlayer(std::ifstream& file) {
    std::string line;
    std::getline(file, line);
    std::stringstream ss(line);

    std::string tag;
    int health, maxHealth, mana, maxMana, posX, posY;

    ss >> tag >> health >> maxHealth >> mana >> maxMana >> posX >> posY;

    if (tag != "PLAYER_STATS") {
        throw SaveException("Invalid save file: missing player stats");
    }
    auto player = std::make_unique<Player>(posX, posY, maxHealth, maxMana);
    player->setHealth(health);
    player->setMana(mana);
    loadSpells(file, *player);

    return player;
}

// Загрузка заклинаний
void GameSaveManager::loadSpells(std::ifstream& file, Player& player) {
    std::string line;
    std::getline(file, line);
    std::stringstream ss(line);

    std::string tag;
    size_t spellCount;
    ss >> tag >> spellCount;

    if (tag != "SPELL_COUNT") {
        throw SaveException("Invalid save file: missing spell count");
    }

    player.clearHand();

    for (size_t i = 0; i < spellCount; ++i) {
        std::getline(file, line);
        std::stringstream spellSS(line);

        std::string spellTag, spellType;
        spellSS >> spellTag >> spellType;

        if (spellTag != "SPELL") continue;

        if (spellType == "DirectDamage") {
            int damage, range, manaCost, isEnhanced;
            spellSS >> damage >> range >> manaCost >> isEnhanced;

            auto spell = std::make_unique<DirectDamageSpell>(damage, range, manaCost);
            if (isEnhanced) {
                spell->increaseRange(1);
            }
            player.getHand().addSpell(std::move(spell));
        }
        else if (spellType == "AreaDamage") {
            int damage, range, manaCost, isEnhanced;
            spellSS >> damage >> range >> manaCost >> isEnhanced;

            auto spell = std::make_unique<AreaDamageSpell>(damage, range, manaCost);
            if (isEnhanced) {
                spell->increaseRange(1);
            }
            player.getHand().addSpell(std::move(spell));
        }
        else if (spellType == "Trap") {
            int damage, manaCost, isEnhanced;
            spellSS >> damage >> manaCost >> isEnhanced;

            auto spell = std::make_unique<TrapSpell>(damage, manaCost);
            if (isEnhanced) {
                spell->increaseRange(1); 
            }
            player.getHand().addSpell(std::move(spell));
        }
        else if (spellType == "Melee") {
            int damage, manaCost, isEnhanced;
            spellSS >> damage >> manaCost >> isEnhanced;

            auto spell = std::make_unique<MeleeSpell>(damage, manaCost);
            if (isEnhanced) {
                spell->increaseRange(1);
            }
            player.getHand().addSpell(std::move(spell));
        }
    }
}

// Загрузка игрового поля
std::unique_ptr<GameField> GameSaveManager::loadGameField(std::ifstream& file, Player* player) {
    std::string line;
    std::getline(file, line);
    std::stringstream ss(line);

    std::string tag;
    int width, height;
    ss >> tag >> width >> height;

    if (tag != "FIELD_SIZE") {
        throw SaveException("Invalid save file: missing field size");
    }

    auto field = std::make_unique<GameField>(width, height, false);
    field->setPlayer(player);
    field->clearEnemies();
    field->clearTowers();
    field->clearTraps();
    field->clearObstacles(); // Очищаем препятствия

    // ЗАГРУЗКА ПРЕПЯТСТВИЙ (ДОБАВИТЬ ЭТО)
    std::getline(file, line);
    std::stringstream obstaclesSS(line);
    std::string obstaclesTag;
    size_t obstacleCount;
    obstaclesSS >> obstaclesTag >> obstacleCount;

    if (obstaclesTag == "OBSTACLES") {
        for (size_t i = 0; i < obstacleCount; ++i) {
            std::getline(file, line);
            std::stringstream obstacleSS(line);

            std::string obstacleTag;
            int posX, posY;
            obstacleSS >> obstacleTag >> posX >> posY;

            if (obstacleTag == "OBSTACLE") {
                field->addObstacle(Position(posX, posY));
            }
        }
        // После загрузки препятствий, читаем следующую строку для врагов
        std::getline(file, line);
    }

    // Теперь обрабатываем врагов из строки, которую мы прочитали
    std::stringstream enemiesSS(line);
    std::string enemiesTag;
    size_t enemyCount;
    enemiesSS >> enemiesTag >> enemyCount;

    if (enemiesTag == "ENEMIES") {
        for (size_t i = 0; i < enemyCount; ++i) {
            std::getline(file, line);
            std::stringstream enemySS(line);

            std::string enemyTag;
            int posX, posY, health, damage;
            enemySS >> enemyTag >> posX >> posY >> health >> damage;

            if (enemyTag == "ENEMY") {
                auto enemy = std::make_unique<Enemy>(health, damage);
                enemy->SetPosition(posX, posY);
                enemy->setHealth(health);
                field->addEnemy(std::move(enemy));
            }
        }
    }

    // Загружаем башни
    std::getline(file, line);
    std::stringstream towersSS(line);
    std::string towersTag;
    size_t towerCount;
    towersSS >> towersTag >> towerCount;

    if (towersTag == "TOWERS") {
        for (size_t i = 0; i < towerCount; ++i) {
            std::getline(file, line);
            std::stringstream towerSS(line);

            std::string towerTag;
            int posX, posY, range, damage, canAttack;
            towerSS >> towerTag >> posX >> posY >> range >> damage >> canAttack;

            if (towerTag == "TOWER") {
                auto tower = std::make_unique<EnemyTower>(posX, posY, range, damage);
                tower->setCanAttack(canAttack != 0);
                field->addTower(std::move(tower));
            }
        }
    }

    // Загружаем ловушки
    std::getline(file, line);
    std::stringstream trapsSS(line);
    std::string trapsTag;
    size_t trapCount;
    trapsSS >> trapsTag >> trapCount;

    if (trapsTag == "TRAPS") {
        for (size_t i = 0; i < trapCount; ++i) {
            std::getline(file, line);
            std::stringstream trapSS(line);

            std::string trapTag;
            int posX, posY;
            trapSS >> trapTag >> posX >> posY;

            if (trapTag == "TRAP") {
                field->addTrap(Position(posX, posY));
            }
        }
    }

    return field;
}

// Проверка существования файла сохранения
bool GameSaveManager::saveExists(const std::string& filename) const {
    std::ifstream file(filename);
    return file.good();
}