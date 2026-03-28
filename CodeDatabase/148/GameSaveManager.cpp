#include "GameSaveManager.h"
#include "Game.h"
#include "SpellHand.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "BoostSpell.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include "Ally.h"
#include "FileManager.h"
#include "Exceptions.h"
#include <sstream>
#include <filesystem>

/**
 * @file GameSaveManager.cpp
 * @brief Реализация менеджера сохранений (ИСПРАВЛЕНО - сохраняет стены и точные заклинания)
 */

const std::string GameSaveManager::SAVE_DIRECTORY = "saves/";
const std::string GameSaveManager::SAVE_EXTENSION = ".sav";

/**
 * @brief Создать директорию для сохранений если не существует
 */
void GameSaveManager::ensureSaveDirectoryExists() {
    std::filesystem::create_directories(SAVE_DIRECTORY);
}

/**
 * @brief Получить полный путь к файлу сохранения
 * @param saveName Имя сохранения
 * @return Полный путь
 */
std::string GameSaveManager::getSavePath(const std::string& saveName) {
    return SAVE_DIRECTORY + saveName + SAVE_EXTENSION;
}

/**
 * @brief Проверить существование сохранения
 * @param saveName Имя сохранения
 * @return true если файл существует
 */
bool GameSaveManager::saveExists(const std::string& saveName) {
    return std::filesystem::exists(getSavePath(saveName));
}

/**
 * @brief Валидировать данные сохранения
 * @param data Строка с данными
 * @return true если данные корректны
 */
bool GameSaveManager::validateSaveData(const std::string& data) {
    return !data.empty() && data.find("LEVEL:") != std::string::npos;
}

/**
 * @brief Сохранить игру в файл (ИСПРАВЛЕНО)
 * @param game Указатель на игру
 * @param saveName Имя сохранения
 * @throws FileWriteException если не удалось записать
 * @throws InvalidGameStateException если состояние некорректно
 */
void GameSaveManager::saveGame(Game* game, const std::string& saveName) {
    if (!game) {
        throw InvalidGameStateException("Game pointer is null");
    }

    ensureSaveDirectoryExists();
    std::string savePath = getSavePath(saveName);
    
    std::ostringstream buffer;
    
    // Сохраняем уровень
    buffer << "LEVEL:" << game->getCurrentLevel() << "\n";
    
    // Сохраняем поле
    const GameField* field = game->getField();
    buffer << "FIELD:" << field->getWidth() << "," << field->getHeight() << "\n";
    
    // ИСПРАВЛЕНО: Сохраняем заблокированные клетки (стены)
    buffer << "BLOCKED_CELLS:\n";
    for (int y = 0; y < field->getHeight(); ++y) {
        for (int x = 0; x < field->getWidth(); ++x) {
            if (!field->isCellPassable(x, y)) {
                buffer << x << "," << y << "\n";
            }
        }
    }
    buffer << "END_BLOCKED_CELLS\n";
    
    // Сохраняем игрока
    const Player* player = game->getPlayer();
    auto [px, py] = player->getPosition();
    buffer << "PLAYER:" << player->getHealth() << "," 
           << player->getMaxHealth() << ","
           << player->getDamage() << ","
           << px << "," << py << ","
           << player->getScore() << ","
           << player->getEnemiesKilled() << ","
           << static_cast<int>(player->getAttackMode()) << "\n";
    
    // ИСПРАВЛЕНО: Сохраняем заклинания с точными именами
    SpellHand* hand = game->getPlayerSpellHand();
    if (hand) {
        buffer << "SPELLS:" << hand->getSpellCount() << ","
               << hand->getMaxHandSize() << ","
               << hand->getBoostStacks() << "\n";
        
        for (int i = 0; i < hand->getSpellCount(); ++i) {
            const Spell* spell = hand->getSpell(i);
            if (spell) {
                buffer << "SPELL_NAME:" << spell->getName() << "\n";
            }
        }
    }
    
    // Сохраняем врагов
    buffer << "ENEMIES:\n";
    for (const auto& enemy : game->getEnemies()) {
        if (enemy->isAlive()) {
            auto [ex, ey] = enemy->getPosition();
            buffer << ex << "," << ey << "," 
                   << enemy->getHealth() << "," 
                   << enemy->getDamage() << "\n";
        }
    }
    buffer << "END_ENEMIES\n";
    
    // Сохраняем башни
    buffer << "TOWERS:\n";
    for (const auto& tower : game->getTowers()) {
        if (tower->isAlive()) {
            auto [tx, ty] = tower->getPosition();
            buffer << tx << "," << ty << "," << tower->getHealth() << "\n";
        }
    }
    buffer << "END_TOWERS\n";
    
    // Сохраняем союзников
    buffer << "ALLIES:\n";
    for (const auto& ally : game->getAllies()) {
        if (ally->isAlive()) {
            auto [ax, ay] = ally->getPosition();
            buffer << ax << "," << ay << "," << ally->getHealth() << "\n";
        }
    }
    buffer << "END_ALLIES\n";
    
    std::string saveData = buffer.str();
    
    if (!validateSaveData(saveData)) {
        throw InvalidGameStateException("Generated save data is invalid");
    }
    
    // Запись в файл через FileManager (RAII)
    FileManager fileManager(savePath, std::ios::out);
    fileManager.getStream() << saveData;
}

/**
 * @brief Загрузить игру из файла (ИСПРАВЛЕНО)
 * @param game Указатель на игру
 * @param saveName Имя сохранения
 * @throws FileNotFoundException если файл не найден
 * @throws FileReadException если не удалось прочитать
 * @throws CorruptedSaveException если данные повреждены
 */
void GameSaveManager::loadGame(Game* game, const std::string& saveName) {
    if (!game) {
        throw InvalidGameStateException("Game pointer is null");
    }

    std::string savePath = getSavePath(saveName);
    
    if (!saveExists(saveName)) {
        throw FileNotFoundException(savePath);
    }

    try {
        // Чтение из файла через FileManager (RAII)
        FileManager fileManager(savePath, std::ios::in);
        std::stringstream buffer;
        buffer << fileManager.getStream().rdbuf();
        std::string content = buffer.str();
        
        if (!validateSaveData(content)) {
            throw CorruptedSaveException(savePath + " - Invalid save format");
        }
        
        // Очищаем текущее состояние
        game->getEnemies().clear();
        game->getTowers().clear();
        game->getAllies().clear();
        game->getTraps().clear();
        
        std::istringstream iss(content);
        std::string line;
        
        int level = 1;
        int fieldWidth = 15, fieldHeight = 15;
        int playerHealth = 0, playerMaxHealth = 0, playerDamage = 0;
        int playerX = 0, playerY = 0;
        int playerScore = 0, playerKills = 0;
        int attackModeInt = 0;
        
        int spellCount = 0, maxHandSize = 5, boostStacks = 0;
        
        bool readingBlockedCells = false;
        bool readingEnemies = false;
        bool readingTowers = false;
        bool readingAllies = false;
        
        SpellHand* hand = game->getPlayerSpellHand();
        GameField* field = nullptr;
        
        while (std::getline(iss, line)) {
            if (line.empty()) continue;
            
            // Парсинг уровня
            if (line.find("LEVEL:") == 0) {
                level = std::stoi(line.substr(6));
                game->setCurrentLevel(level);
            }
            // Парсинг поля
            else if (line.find("FIELD:") == 0) {
                std::string fieldData = line.substr(6);
                std::istringstream fss(fieldData);
                std::string token;
                std::getline(fss, token, ',');
                fieldWidth = std::stoi(token);
                std::getline(fss, token, ',');
                fieldHeight = std::stoi(token);
                
                if (fieldWidth < 10 || fieldWidth > 50 || fieldHeight < 10 || fieldHeight > 50) {
                    throw CorruptedSaveException(savePath + " - Invalid field dimensions");
                }
                
                game->recreateField(fieldWidth, fieldHeight);
                field = game->getField();
            }
            // ИСПРАВЛЕНО: Начало секции заблокированных клеток
            else if (line.find("BLOCKED_CELLS:") == 0) {
                readingBlockedCells = true;
                readingEnemies = false;
                readingTowers = false;
                readingAllies = false;
            }
            // ИСПРАВЛЕНО: Конец секции заблокированных клеток
            else if (line.find("END_BLOCKED_CELLS") == 0) {
                readingBlockedCells = false;
            }
            // ИСПРАВЛЕНО: Парсинг заблокированной клетки
            else if (readingBlockedCells && !line.empty() && field) {
                std::istringstream bss(line);
                std::string token;
                std::vector<std::string> values;
                
                while (std::getline(bss, token, ',')) {
                    values.push_back(token);
                }
                
                if (values.size() >= 2) {
                    int bx = std::stoi(values[0]);
                    int by = std::stoi(values[1]);
                    
                    if (bx >= 0 && bx < fieldWidth && by >= 0 && by < fieldHeight) {
                        field->setCellBlocked(bx, by, true);
                    }
                }
            }
            // Парсинг игрока
            else if (line.find("PLAYER:") == 0) {
                std::string playerData = line.substr(7);
                std::istringstream pss(playerData);
                std::string token;
                std::vector<std::string> values;
                
                while (std::getline(pss, token, ',')) {
                    values.push_back(token);
                }
                
                if (values.size() >= 8) {
                    playerHealth = std::stoi(values[0]);
                    playerMaxHealth = std::stoi(values[1]);
                    playerDamage = std::stoi(values[2]);
                    playerX = std::stoi(values[3]);
                    playerY = std::stoi(values[4]);
                    playerScore = std::stoi(values[5]);
                    playerKills = std::stoi(values[6]);
                    attackModeInt = std::stoi(values[7]);
                    
                    if (playerHealth < 0 || playerHealth > 500) {
                        throw CorruptedSaveException(savePath + " - Invalid player health");
                    }
                    
                    if (playerX < 0 || playerX >= fieldWidth || playerY < 0 || playerY >= fieldHeight) {
                        throw CorruptedSaveException(savePath + " - Player position out of bounds");
                    }
                    
                    AttackMode mode = (attackModeInt == 0) ? AttackMode::MELEE : AttackMode::RANGED;
                    
                    game->loadPlayerState(playerHealth, playerMaxHealth, playerDamage, 
                                         playerX, playerY, playerScore, playerKills, mode);
                }
            }
            // Парсинг информации о заклинаниях
            else if (line.find("SPELLS:") == 0) {
                std::string spellData = line.substr(7);
                std::istringstream sss(spellData);
                std::string token;
                std::vector<std::string> values;
                
                while (std::getline(sss, token, ',')) {
                    values.push_back(token);
                }

                if (values.size() >= 3) {
                    spellCount = std::stoi(values[0]);
                    maxHandSize = std::stoi(values[1]);
                    boostStacks = std::stoi(values[2]);
                    
                    if (spellCount < 0 || spellCount > 10) {
                        throw CorruptedSaveException(savePath + " - Invalid spell count");
                    }

                    if (maxHandSize < 5 || maxHandSize > 10) {
                        throw CorruptedSaveException(savePath + " - Invalid hand size");
                    }

                    // ИСПРАВЛЕНО: Очищаем руку перед загрузкой
                    if (hand) {
                        while (hand->getSpellCount() > 0) {
                            hand->removeSpell(0);
                        }
                        
                        while (hand->getMaxHandSize() < maxHandSize) {
                            hand->increaseMaxSize(1);
                        }
                    }
                }
            }
            // ИСПРАВЛЕНО: Парсинг конкретного заклинания (добавляем точное заклинание)
            else if (line.find("SPELL_NAME:") == 0) {
                std::string spellName = line.substr(11);
                
                if (!hand) continue;
                
                std::unique_ptr<Spell> spell;
                
                if (spellName == "Direct Damage") {
                    spell = std::make_unique<DirectDamageSpell>();
                }
                else if (spellName == "Area Damage") {
                    spell = std::make_unique<AreaDamageSpell>();
                }
                else if (spellName == "Trap") {
                    spell = std::make_unique<TrapSpell>();
                }
                else if (spellName == "Summon Ally") {
                    spell = std::make_unique<SummonSpell>();
                }
                else if (spellName == "Boost") {
                    spell = std::make_unique<BoostSpell>();
                }
                else {
                    throw CorruptedSaveException(savePath + " - Unknown spell: " + spellName);
                }

                if (spell) {
                    hand->addSpell(std::move(spell));
                }
            }
            // Начало секции врагов
            else if (line.find("ENEMIES:") == 0) {
                readingBlockedCells = false;
                readingEnemies = true;
                readingTowers = false;
                readingAllies = false;
            }
            // Конец секции врагов
            else if (line.find("END_ENEMIES") == 0) {
                readingEnemies = false;
            }
            // Начало секции башен
            else if (line.find("TOWERS:") == 0) {
                readingBlockedCells = false;
                readingEnemies = false;
                readingTowers = true;
                readingAllies = false;
            }
            // Конец секции башен
            else if (line.find("END_TOWERS") == 0) {
                readingTowers = false;
            }
            // Начало секции союзников
            else if (line.find("ALLIES:") == 0) {
                readingBlockedCells = false;
                readingEnemies = false;
                readingTowers = false;
                readingAllies = true;
            }
            // Конец секции союзников
            else if (line.find("END_ALLIES") == 0) {
                readingAllies = false;
            }
            // Парсинг данных врага
            else if (readingEnemies && !line.empty()) {
                std::istringstream ess(line);
                std::string token;
                std::vector<std::string> values;
                
                while (std::getline(ess, token, ',')) {
                    values.push_back(token);
                }

                if (values.size() >= 4) {
                    int ex = std::stoi(values[0]);
                    int ey = std::stoi(values[1]);
                    int health = std::stoi(values[2]);
                    int damage = std::stoi(values[3]);
                    
                    if (ex < 0 || ex >= fieldWidth || ey < 0 || ey >= fieldHeight) {
                        throw CorruptedSaveException(savePath + " - Enemy position out of bounds");
                    }

                    if (health < 1 || health > 200) {
                        throw CorruptedSaveException(savePath + " - Invalid enemy health");
                    }

                    if (damage < 5 || damage > 100) {
                        throw CorruptedSaveException(savePath + " - Invalid enemy damage");
                    }

                    auto enemy = std::make_unique<Enemy>(ex, ey);
                    enemy->setHealth(health);
                    enemy->setDamage(damage);
                    game->getEnemies().push_back(std::move(enemy));
                }
            }
            // Парсинг данных башни
            else if (readingTowers && !line.empty()) {
                std::istringstream tss(line);
                std::string token;
                std::vector<std::string> values;
                
                while (std::getline(tss, token, ',')) {
                    values.push_back(token);
                }

                if (values.size() >= 3) {
                    int tx = std::stoi(values[0]);
                    int ty = std::stoi(values[1]);
                    int health = std::stoi(values[2]);
                    
                    if (tx < 0 || tx >= fieldWidth || ty < 0 || ty >= fieldHeight) {
                        throw CorruptedSaveException(savePath + " - Tower position out of bounds");
                    }

                    if (health < 1 || health > 300) {
                        throw CorruptedSaveException(savePath + " - Invalid tower health");
                    }

                    auto tower = std::make_unique<EnemyTower>(tx, ty, health, 20, 4, 2);
                    game->getTowers().push_back(std::move(tower));
                }
            }
            // Парсинг данных союзника
            else if (readingAllies && !line.empty()) {
                std::istringstream ass(line);
                std::string token;
                std::vector<std::string> values;
                
                while (std::getline(ass, token, ',')) {
                    values.push_back(token);
                }

                if (values.size() >= 3) {
                    int ax = std::stoi(values[0]);
                    int ay = std::stoi(values[1]);
                    int health = std::stoi(values[2]);
                    
                    if (ax < 0 || ax >= fieldWidth || ay < 0 || ay >= fieldHeight) {
                        throw CorruptedSaveException(savePath + " - Ally position out of bounds");
                    }

                    if (health < 1 || health > 100) {
                        throw CorruptedSaveException(savePath + " - Invalid ally health");
                    }

                    auto ally = std::make_unique<Ally>(ax, ay);
                    ally->setHealth(health);
                    game->getAllies().push_back(std::move(ally));
                }
            }
        }

        // Восстановление boost стеков
        if (hand && boostStacks > 0) {
            hand->resetBoostStacks();
            for (int i = 0; i < boostStacks; ++i) {
                hand->addBoostStack();
            }
        }
        
    } catch (const FileNotFoundException& e) {
        throw;
    } catch (const CorruptedSaveException& e) {
        throw;
    } catch (const std::exception& e) {
        throw FileReadException(savePath, e.what());
    }
}


