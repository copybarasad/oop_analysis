#include "Game.h"
#include "ConsoleView.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "EnhancementSpell.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <algorithm>
#include <random>
#include <cstring>

Game::Game() 
    : currentLevel(nullptr), currentLevelNumber(1), 
      fieldWidth(15), fieldHeight(15),
      state(GameState::MENU),
      playerMaxHealth(100), playerMeleeDamage(20), playerRangedDamage(15) {
}

void Game::startNewGame() {
    currentLevelNumber = 1;
    playerMaxHealth = 100;
    playerMeleeDamage = 20;
    playerRangedDamage = 15;
    state = GameState::LEVEL_PROGRESS;
    initializeLevel(1);
}

void Game::initializeLevel(int levelNumber) {
    fieldWidth = 15 + (levelNumber - 1) * 2;
    fieldHeight = 15 + (levelNumber - 1) * 2;
    
    currentLevel = std::make_unique<GameController>(
        fieldWidth, fieldHeight, 5);
    
    currentLevel->clearAllEntities();
    
    currentLevel->setPlayerStats(playerMaxHealth, playerMeleeDamage, playerRangedDamage);
    
    if (levelNumber > 1) {
        currentLevel->restorePlayerHealth();
        
        removeHalfSpells();
    }
    
    spawnLevelEnemies(levelNumber);
}

void Game::spawnLevelEnemies(int levelNumber) {
    int enemyCount = 3 + levelNumber;
    int enemyHealth = calculateEnemyHealth(levelNumber);
    int enemyDamage = calculateEnemyDamage(levelNumber);
    
    int buildingCount = 2 + (levelNumber / 2);
    int buildingHealth = calculateBuildingHealth(levelNumber);
    
    int towerCount = 1 + (levelNumber / 3);
    int towerHealth = calculateTowerHealth(levelNumber);
    int towerDamage = calculateTowerDamage(levelNumber);
    
    currentLevel->spawnEnemiesWithStats(enemyCount, enemyHealth, enemyDamage);
    currentLevel->spawnBuildingsWithStats(buildingCount, buildingHealth);
    currentLevel->spawnTowersWithStats(towerCount, towerHealth, towerDamage);
}

int Game::calculateEnemyHealth(int level) const {
    return 30 + (level - 1) * 10;
}

int Game::calculateEnemyDamage(int level) const {
    return 10 + (level - 1) * 3;
}

int Game::calculateBuildingHealth(int level) const {
    return 50 + (level - 1) * 15;
}

int Game::calculateTowerHealth(int level) const {
    return 40 + (level - 1) * 12;
}

int Game::calculateTowerDamage(int level) const {
    return 10 + (level - 1) * 2;
}

void Game::processPlayerTurn(char command) {
    if (!currentLevel || state != GameState::LEVEL_PROGRESS) return;
    
    if (currentLevel->getPlayer().get_IsSlowed()) {
        currentLevel->getPlayerMutable().remove_Slow();
        currentLevel->setLastMessage("Эффект замедления снят! Ход пропущен.");
        processAlliesTurn();
        processEnemiesTurn();
        processBuildingsAndTowersTurn();
        checkLevelConditions();
        return;
    }
    
    bool turnUsed = false;
    
    switch (std::toupper(command)) {
        case 'W': turnUsed = currentLevel->movePlayerUp(); break;
        case 'A': turnUsed = currentLevel->movePlayerLeft(); break;
        case 'S': turnUsed = currentLevel->movePlayerDown(); break;
        case 'D': turnUsed = currentLevel->movePlayerRight(); break;
        case 'M': 
            currentLevel->switchPlayerCombatMode();
            break;
    }
    
    if (turnUsed && std::toupper(command) != 'M') {
        processAlliesTurn();
        processEnemiesTurn();
        processBuildingsAndTowersTurn();
        checkLevelConditions();
    }
}

void Game::processAlliesTurn() {
}

void Game::processEnemiesTurn() {
}

void Game::processBuildingsAndTowersTurn() {
}

void Game::checkLevelConditions() {
    if (!currentLevel) return;
    
    if (currentLevel->isGameOver()) {
        if (currentLevel->isGameWon()) {
            completeLevel();
        } else {
            failLevel();
        }
    }
}

void Game::completeLevel() {
    state = GameState::LEVEL_COMPLETE;
    currentLevelNumber++;
    
    if (currentLevelNumber > 10) {
        state = GameState::GAME_WON;
        ConsoleView::clearScreen();
        std::cout << "\n  ╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "  ║                    ПОЗДРАВЛЯЕМ!                                ║\n";
        std::cout << "  ║              ВЫ ПРОШЛИ ВСЕ 10 УРОВНЕЙ!                        ║\n";
        std::cout << "  ║                    ИГРА ЗАВЕРШЕНА!                            ║\n";
        std::cout << "  ╚════════════════════════════════════════════════════════════╝\n\n";
        return;
    }
    
    state = GameState::UPGRADE_PHASE;
    showUpgradeMenu();
    
    initializeLevel(currentLevelNumber);
    state = GameState::LEVEL_PROGRESS;
}

void Game::failLevel() {
    state = GameState::LEVEL_FAILED;
    if (askRestartOrExit()) {
        startNewGame();
    } else {
        state = GameState::GAME_OVER;
    }
}

bool Game::askRestartOrExit() {
    std::cout << "\n  Игра окончена! Начать заново? (Y/N): ";
    char choice = _getch();
    std::cout << choice << "\n";
    return std::toupper(choice) == 'Y';
}

void Game::removeHalfSpells() {
    if (!currentLevel) return;
    
    PlayerHand& hand = currentLevel->getPlayerHandMutable();
    int spellCount = hand.getSpellCount();
    int toRemove = spellCount / 2;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    
    for (int i = 0; i < toRemove && hand.getSpellCount() > 0; ++i) {
        std::uniform_int_distribution<> dis(0, hand.getSpellCount() - 1);
        int indexToRemove = dis(gen);
        hand.removeSpell(indexToRemove);
    }
}

void Game::showUpgradeMenu() {
    ConsoleView::clearScreen();
    std::cout << "\n  ===== УРОВЕНЬ " << (currentLevelNumber - 1) << " ПРОЙДЕН! =====\n\n";
    std::cout << "  Выберите улучшение:\n";
    std::cout << "  1. Увеличить максимальное здоровье (+20)\n";
    std::cout << "  2. Увеличить ближний урон (+5)\n";
    std::cout << "  3. Увеличить дальний урон (+3)\n";
    std::cout << "  4. Улучшить выбранное заклинание\n";
    
    UpgradeChoice choice = getUpgradeChoice();
    applyPlayerUpgrade(choice);
}

UpgradeChoice Game::getUpgradeChoice() {
    while (true) {
        char input = _getch();
        switch (input) {
            case '1': return UpgradeChoice::HEALTH;
            case '2': return UpgradeChoice::MELEE_DAMAGE;
            case '3': return UpgradeChoice::RANGED_DAMAGE;
            case '4': return UpgradeChoice::UPGRADE_SPELL;
            default: continue;
        }
    }
}

void Game::applyPlayerUpgrade(UpgradeChoice choice) {
    switch (choice) {
        case UpgradeChoice::HEALTH:
            playerMaxHealth += 20;
            std::cout << "  Здоровье увеличено до " << playerMaxHealth << "\n";
            break;
        case UpgradeChoice::MELEE_DAMAGE:
            playerMeleeDamage += 5;
            std::cout << "  Ближний урон увеличен до " << playerMeleeDamage << "\n";
            break;
        case UpgradeChoice::RANGED_DAMAGE:
            playerRangedDamage += 3;
            std::cout << "  Дальний урон увеличен до " << playerRangedDamage << "\n";
            break;
        case UpgradeChoice::UPGRADE_SPELL:
            upgradeSelectedSpell();
            break;
        default:
            break;
    }
    
    std::cout << "  Нажмите любую клавишу для продолжения...\n";
    _getch();
}

void Game::upgradeSelectedSpell() {
    if (!currentLevel) return;
    
    PlayerHand& hand = currentLevel->getPlayerHandMutable();
    Spell* spell = hand.getSelectedSpell();
    
    if (!spell) {
        std::cout << "  Нет выбранного заклинания!\n";
        return;
    }
    
    std::string name = spell->getName();
    if (name == "Прямой урон") {
        DirectDamageSpell* dd = dynamic_cast<DirectDamageSpell*>(spell);
        if (dd) {
            dd->setDamage(dd->getDamage() + 5);
            dd->setRange(dd->getRange() + 1);
            std::cout << "  Заклинание 'Прямой урон' улучшено! (+5 урона, +1 радиус)\n";
        }
    } else if (name == "Урон по площади") {
        AreaDamageSpell* ad = dynamic_cast<AreaDamageSpell*>(spell);
        if (ad) {
            ad->setDamage(ad->getDamage() + 3);
            ad->setAreaSize(ad->getAreaSize() + 1);
            std::cout << "  Заклинание 'Урон по площади' улучшено! (+3 урона, +1 размер области)\n";
        }
    } else if (name == "Ловушка") {
        TrapSpell* ts = dynamic_cast<TrapSpell*>(spell);
        if (ts) {
            ts->setDamage(ts->getDamage() + 5);
            ts->setRange(ts->getRange() + 1);
            std::cout << "  Заклинание 'Ловушка' улучшено! (+5 урона, +1 радиус)\n";
        }
    } else if (name == "Призыв") {
        SummonSpell* ss = dynamic_cast<SummonSpell*>(spell);
        if (ss) {
            ss->setSummonCount(ss->getSummonCount() + 1);
            std::cout << "  Заклинание 'Призыв' улучшено! (+1 союзник)\n";
        }
    } else {
        std::cout << "  Это заклинание нельзя улучшить таким образом.\n";
    }
}

void Game::saveGame() {
    try {
        std::ofstream file(SAVE_FILE, std::ios::binary);
        if (!file.is_open()) {
            throw FileWriteException(SAVE_FILE, "Game::saveGame");
        }
        
        saveGameData(file);
        
        if (file.fail()) {
            throw FileWriteException(SAVE_FILE, "Game::saveGame");
        }
        
        std::cout << "  Игра успешно сохранена!\n";
    } catch (const SaveException& e) {
        std::cerr << "Ошибка сохранения: " << e.what() << std::endl;
        throw;
    } catch (const std::exception& e) {
        throw SaveException(std::string("Неожиданная ошибка: ") + e.what(), 
                           "Game::saveGame");
    }
}

void Game::saveGameData(std::ofstream& file) const {
    file.write(SAVE_MAGIC, strlen(SAVE_MAGIC));
    
    file.write(reinterpret_cast<const char*>(&currentLevelNumber), sizeof(currentLevelNumber));
    file.write(reinterpret_cast<const char*>(&fieldWidth), sizeof(fieldWidth));
    file.write(reinterpret_cast<const char*>(&fieldHeight), sizeof(fieldHeight));
    file.write(reinterpret_cast<const char*>(&playerMaxHealth), sizeof(playerMaxHealth));
    file.write(reinterpret_cast<const char*>(&playerMeleeDamage), sizeof(playerMeleeDamage));
    file.write(reinterpret_cast<const char*>(&playerRangedDamage), sizeof(playerRangedDamage));
    
    if (!currentLevel) {
        throw SaveException("Нет текущего уровня для сохранения", "Game::saveGameData");
    }
    
    const Player& player = currentLevel->getPlayer();
    int health = player.get_Health();
    int maxHealth = player.get_MaxHealth();
    int meleeDamage = player.get_MeleeDamage();
    int rangedDamage = player.get_RangedDamage();
    int score = player.get_Score();
    CombatMode combatMode = player.get_CombatMode();
    bool isSlowed = player.get_IsSlowed();
    Position pos = player.get_Position();
    
    file.write(reinterpret_cast<const char*>(&health), sizeof(health));
    file.write(reinterpret_cast<const char*>(&maxHealth), sizeof(maxHealth));
    file.write(reinterpret_cast<const char*>(&meleeDamage), sizeof(meleeDamage));
    file.write(reinterpret_cast<const char*>(&rangedDamage), sizeof(rangedDamage));
    file.write(reinterpret_cast<const char*>(&score), sizeof(score));
    file.write(reinterpret_cast<const char*>(&combatMode), sizeof(combatMode));
    file.write(reinterpret_cast<const char*>(&isSlowed), sizeof(isSlowed));
    file.write(reinterpret_cast<const char*>(&pos), sizeof(pos));
    
    const PlayerHand& hand = currentLevel->getPlayerHand();
    int spellCount = hand.getSpellCount();
    int selectedIndex = hand.getSelectedIndex();
    int killCount = hand.getKillCount();
    
    file.write(reinterpret_cast<const char*>(&spellCount), sizeof(spellCount));
    file.write(reinterpret_cast<const char*>(&selectedIndex), sizeof(selectedIndex));
    file.write(reinterpret_cast<const char*>(&killCount), sizeof(killCount));
    
    for (int i = 0; i < spellCount; ++i) {
        const Spell* spell = hand.getSpell(i);
        if (!spell) {
            throw SaveException("Пустое заклинание в руке", "Game::saveGameData");
        }
        saveSpell(file, spell);
    }
    
    const std::vector<Enemy>& enemies = currentLevel->getEnemies();
    int enemyCount = static_cast<int>(enemies.size());
    file.write(reinterpret_cast<const char*>(&enemyCount), sizeof(enemyCount));
    for (const auto& enemy : enemies) {
        if (!enemy.isAlive()) continue;
        int health = enemy.get_Health();
        int damage = enemy.get_Damage();
        Position pos = enemy.get_Position();
        file.write(reinterpret_cast<const char*>(&health), sizeof(health));
        file.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
        file.write(reinterpret_cast<const char*>(&pos), sizeof(pos));
    }
    
    const std::vector<EnemyBuilding>& buildings = currentLevel->getBuildings();
    int buildingCount = static_cast<int>(buildings.size());
    file.write(reinterpret_cast<const char*>(&buildingCount), sizeof(buildingCount));
    for (const auto& building : buildings) {
        if (!building.isAlive()) continue;
        int health = building.get_Health();
        int spawnCounter = building.get_SpawnCounter();
        int spawnRange = building.get_SpawnRange();
        Position pos = building.get_Position();
        file.write(reinterpret_cast<const char*>(&health), sizeof(health));
        file.write(reinterpret_cast<const char*>(&spawnCounter), sizeof(spawnCounter));
        file.write(reinterpret_cast<const char*>(&spawnRange), sizeof(spawnRange));
        file.write(reinterpret_cast<const char*>(&pos), sizeof(pos));
    }
    
    const std::vector<EnemyTower>& towers = currentLevel->getTowers();
    int towerCount = static_cast<int>(towers.size());
    file.write(reinterpret_cast<const char*>(&towerCount), sizeof(towerCount));
    for (const auto& tower : towers) {
        if (!tower.isAlive()) continue;
        int health = tower.get_Health();
        int damage = tower.get_Damage();
        int attackRange = tower.get_AttackRange();
        int cooldown = tower.get_Cooldown();
        int maxCooldown = tower.get_MaxCooldown();
        Position pos = tower.get_Position();
        file.write(reinterpret_cast<const char*>(&health), sizeof(health));
        file.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
        file.write(reinterpret_cast<const char*>(&attackRange), sizeof(attackRange));
        file.write(reinterpret_cast<const char*>(&cooldown), sizeof(cooldown));
        file.write(reinterpret_cast<const char*>(&maxCooldown), sizeof(maxCooldown));
        file.write(reinterpret_cast<const char*>(&pos), sizeof(pos));
    }
}

void Game::saveSpell(std::ofstream& file, const Spell* spell) const {
    std::string name = spell->getName();
    
    if (name == "Прямой урон") {
        SpellType type = SpellType::DIRECT_DAMAGE;
        const DirectDamageSpell* dd = dynamic_cast<const DirectDamageSpell*>(spell);
        if (!dd) {
            throw SaveException("Ошибка приведения типа заклинания", "Game::saveSpell");
        }
        int damage = dd->getDamage();
        int range = dd->getRange();
        file.write(reinterpret_cast<const char*>(&type), sizeof(type));
        file.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
        file.write(reinterpret_cast<const char*>(&range), sizeof(range));
    } else if (name == "Урон по площади") {
        SpellType type = SpellType::AREA_DAMAGE;
        const AreaDamageSpell* ad = dynamic_cast<const AreaDamageSpell*>(spell);
        if (!ad) {
            throw SaveException("Ошибка приведения типа заклинания", "Game::saveSpell");
        }
        int damage = ad->getDamage();
        int range = ad->getRange();
        int areaSize = ad->getAreaSize();
        file.write(reinterpret_cast<const char*>(&type), sizeof(type));
        file.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
        file.write(reinterpret_cast<const char*>(&range), sizeof(range));
        file.write(reinterpret_cast<const char*>(&areaSize), sizeof(areaSize));
    } else if (name == "Ловушка") {
        SpellType type = SpellType::TRAP;
        const TrapSpell* ts = dynamic_cast<const TrapSpell*>(spell);
        if (!ts) {
            throw SaveException("Ошибка приведения типа заклинания", "Game::saveSpell");
        }
        int damage = ts->getDamage();
        int range = ts->getRange();
        file.write(reinterpret_cast<const char*>(&type), sizeof(type));
        file.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
        file.write(reinterpret_cast<const char*>(&range), sizeof(range));
    } else if (name == "Призыв") {
        SpellType type = SpellType::SUMMON;
        const SummonSpell* ss = dynamic_cast<const SummonSpell*>(spell);
        if (!ss) {
            throw SaveException("Ошибка приведения типа заклинания", "Game::saveSpell");
        }
        int summonCount = ss->getSummonCount();
        int range = ss->getRange();
        file.write(reinterpret_cast<const char*>(&type), sizeof(type));
        file.write(reinterpret_cast<const char*>(&summonCount), sizeof(summonCount));
        file.write(reinterpret_cast<const char*>(&range), sizeof(range));
    } else if (name == "Улучшение") {
        SpellType type = SpellType::ENHANCEMENT;
        file.write(reinterpret_cast<const char*>(&type), sizeof(type));
    } else {
        throw SaveException("Неизвестный тип заклинания: " + name, "Game::saveSpell");
    }
}

bool Game::loadGame() {
    try {
        std::ifstream file(SAVE_FILE, std::ios::binary);
        if (!file.is_open()) {
            return false;
        }
        
        loadGameData(file);
        
        if (file.fail()) {
            throw InvalidSaveDataException("Ошибка чтения данных", "Game::loadGame");
        }
        
        state = GameState::LEVEL_PROGRESS;
        return true;
    } catch (const LoadException& e) {
        std::cerr << "Ошибка загрузки: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        throw LoadException(std::string("Неожиданная ошибка: ") + e.what(),
                           "Game::loadGame");
    }
}

void Game::loadGameData(std::ifstream& file) {
    constexpr size_t magicSize = 13;
    char magic[magicSize];
    size_t magicLen = strlen(SAVE_MAGIC);
    file.read(magic, magicLen);
    magic[magicLen] = '\0';
    
    if (file.gcount() != static_cast<std::streamsize>(magicLen) || 
        strcmp(magic, SAVE_MAGIC) != 0) {
        throw InvalidSaveDataException("Неверный формат файла сохранения", 
                                      "Game::loadGameData");
    }
    
    file.read(reinterpret_cast<char*>(&currentLevelNumber), sizeof(currentLevelNumber));
    file.read(reinterpret_cast<char*>(&fieldWidth), sizeof(fieldWidth));
    file.read(reinterpret_cast<char*>(&fieldHeight), sizeof(fieldHeight));
    file.read(reinterpret_cast<char*>(&playerMaxHealth), sizeof(playerMaxHealth));
    file.read(reinterpret_cast<char*>(&playerMeleeDamage), sizeof(playerMeleeDamage));
    file.read(reinterpret_cast<char*>(&playerRangedDamage), sizeof(playerRangedDamage));
    
    if (file.fail()) {
        throw InvalidSaveDataException("Ошибка чтения общих данных", "Game::loadGameData");
    }
    
    if (currentLevelNumber < 1 || fieldWidth < 5 || fieldHeight < 5 || 
        playerMaxHealth < 1 || playerMeleeDamage < 1 || playerRangedDamage < 1) {
        throw InvalidSaveDataException("Некорректные значения параметров", 
                                      "Game::loadGameData");
    }
    
    currentLevel = std::make_unique<GameController>(fieldWidth, fieldHeight, 5);
    currentLevel->clearAllEntities();
    
    int health, maxHealth, meleeDamage, rangedDamage, score;
    CombatMode combatMode;
    bool isSlowed;
    Position pos;
    
    file.read(reinterpret_cast<char*>(&health), sizeof(health));
    file.read(reinterpret_cast<char*>(&maxHealth), sizeof(maxHealth));
    file.read(reinterpret_cast<char*>(&meleeDamage), sizeof(meleeDamage));
    file.read(reinterpret_cast<char*>(&rangedDamage), sizeof(rangedDamage));
    file.read(reinterpret_cast<char*>(&score), sizeof(score));
    file.read(reinterpret_cast<char*>(&combatMode), sizeof(combatMode));
    file.read(reinterpret_cast<char*>(&isSlowed), sizeof(isSlowed));
    file.read(reinterpret_cast<char*>(&pos), sizeof(pos));
    
    if (file.fail()) {
        throw InvalidSaveDataException("Ошибка чтения данных игрока", "Game::loadGameData");
    }
    
    Player& player = currentLevel->getPlayerMutable();
    player.setMaxHealth(maxHealth);
    player.setMeleeDamage(meleeDamage);
    player.setRangedDamage(rangedDamage);
    player.setHealth(health);
    player.setScore(score);
    player.setCombatMode(combatMode);
    player.setSlowState(isSlowed);
    player.set_Position(pos);
    
    currentLevel->getFieldMutable().clearPlayerPosition(Position(0, 0));
    currentLevel->getFieldMutable().setPlayerPosition(pos);
    
    int spellCount, selectedIndex, killCount;
    file.read(reinterpret_cast<char*>(&spellCount), sizeof(spellCount));
    file.read(reinterpret_cast<char*>(&selectedIndex), sizeof(selectedIndex));
    file.read(reinterpret_cast<char*>(&killCount), sizeof(killCount));
    
    if (file.fail()) {
        throw InvalidSaveDataException("Ошибка чтения данных заклинаний", "Game::loadGameData");
    }
    
    if (spellCount < 0 || spellCount > 10 || selectedIndex < 0 || 
        (spellCount > 0 && selectedIndex >= spellCount)) {
        throw InvalidSaveDataException("Некорректные данные заклинаний", "Game::loadGameData");
    }
    
    PlayerHand& hand = currentLevel->getPlayerHandMutable();
    hand.clearAllSpells();
    
    for (int i = 0; i < spellCount; ++i) {
        std::unique_ptr<Spell> spell = loadSpell(file);
        if (spell) {
            hand.addSpell(std::move(spell));
        } else {
            throw InvalidSaveDataException("Не удалось загрузить заклинание " + 
                                          std::to_string(i), "Game::loadGameData");
        }
    }
    
    if (hand.getSpellCount() > 0) {
        hand.setSelectedIndex(selectedIndex);
    }
    hand.setKillCount(killCount);
    
    int enemyCount;
    file.read(reinterpret_cast<char*>(&enemyCount), sizeof(enemyCount));
    if (file.fail()) {
        throw InvalidSaveDataException("Ошибка чтения количества врагов", "Game::loadGameData");
    }
    if (enemyCount < 0 || enemyCount > 100) {
        throw InvalidSaveDataException("Некорректное количество врагов", "Game::loadGameData");
    }
    
    std::vector<Enemy>& enemies = currentLevel->getEnemiesMutable();
    enemies.clear();
    for (int i = 0; i < enemyCount; ++i) {
        int health, damage;
        Position pos;
        file.read(reinterpret_cast<char*>(&health), sizeof(health));
        file.read(reinterpret_cast<char*>(&damage), sizeof(damage));
        file.read(reinterpret_cast<char*>(&pos), sizeof(pos));
        if (file.fail()) {
            throw InvalidSaveDataException("Ошибка чтения данных врага " + std::to_string(i), "Game::loadGameData");
        }
        Enemy enemy(pos.x, pos.y, health, damage);
        enemy.setHealth(health);
        enemies.push_back(enemy);
        currentLevel->getFieldMutable().setEnemyPosition(pos);
    }
    
    int buildingCount;
    file.read(reinterpret_cast<char*>(&buildingCount), sizeof(buildingCount));
    if (file.fail()) {
        throw InvalidSaveDataException("Ошибка чтения количества зданий", "Game::loadGameData");
    }
    if (buildingCount < 0 || buildingCount > 50) {
        throw InvalidSaveDataException("Некорректное количество зданий", "Game::loadGameData");
    }
    
    std::vector<EnemyBuilding>& buildings = currentLevel->getBuildingsMutable();
    buildings.clear();
    for (int i = 0; i < buildingCount; ++i) {
        int health, spawnCounter, spawnRange;
        Position pos;
        file.read(reinterpret_cast<char*>(&health), sizeof(health));
        file.read(reinterpret_cast<char*>(&spawnCounter), sizeof(spawnCounter));
        file.read(reinterpret_cast<char*>(&spawnRange), sizeof(spawnRange));
        file.read(reinterpret_cast<char*>(&pos), sizeof(pos));
        if (file.fail()) {
            throw InvalidSaveDataException("Ошибка чтения данных здания " + std::to_string(i), "Game::loadGameData");
        }
        EnemyBuilding building(pos.x, pos.y, spawnRange, health);
        building.setHealth(health);
        building.setSpawnCounter(spawnCounter);
        buildings.push_back(building);
        currentLevel->getFieldMutable().setBuildingPosition(pos);
    }
    
    int towerCount;
    file.read(reinterpret_cast<char*>(&towerCount), sizeof(towerCount));
    if (file.fail()) {
        throw InvalidSaveDataException("Ошибка чтения количества башен", "Game::loadGameData");
    }
    if (towerCount < 0 || towerCount > 50) {
        throw InvalidSaveDataException("Некорректное количество башен", "Game::loadGameData");
    }
    
    std::vector<EnemyTower>& towers = currentLevel->getTowersMutable();
    towers.clear();
    for (int i = 0; i < towerCount; ++i) {
        int health, damage, attackRange, cooldown, maxCooldown;
        Position pos;
        file.read(reinterpret_cast<char*>(&health), sizeof(health));
        file.read(reinterpret_cast<char*>(&damage), sizeof(damage));
        file.read(reinterpret_cast<char*>(&attackRange), sizeof(attackRange));
        file.read(reinterpret_cast<char*>(&cooldown), sizeof(cooldown));
        file.read(reinterpret_cast<char*>(&maxCooldown), sizeof(maxCooldown));
        file.read(reinterpret_cast<char*>(&pos), sizeof(pos));
        if (file.fail()) {
            throw InvalidSaveDataException("Ошибка чтения данных башни " + std::to_string(i), "Game::loadGameData");
        }
        EnemyTower tower(pos.x, pos.y, health, damage, attackRange, maxCooldown);
        tower.setHealth(health);
        tower.setCooldown(cooldown);
        towers.push_back(tower);
        currentLevel->getFieldMutable().setTowerPosition(pos);
    }
}

std::unique_ptr<Spell> Game::loadSpell(std::ifstream& file) {
    SpellType type;
    file.read(reinterpret_cast<char*>(&type), sizeof(type));
    
    if (file.fail()) {
        throw InvalidSaveDataException("Ошибка чтения типа заклинания", "Game::loadSpell");
    }
    
    switch (type) {
        case SpellType::DIRECT_DAMAGE: {
            int damage, range;
            file.read(reinterpret_cast<char*>(&damage), sizeof(damage));
            file.read(reinterpret_cast<char*>(&range), sizeof(range));
            if (file.fail()) {
                throw InvalidSaveDataException("Ошибка чтения параметров DirectDamageSpell", 
                                              "Game::loadSpell");
            }
            return std::make_unique<DirectDamageSpell>(damage, range);
        }
        case SpellType::AREA_DAMAGE: {
            int damage, range, areaSize;
            file.read(reinterpret_cast<char*>(&damage), sizeof(damage));
            file.read(reinterpret_cast<char*>(&range), sizeof(range));
            file.read(reinterpret_cast<char*>(&areaSize), sizeof(areaSize));
            if (file.fail()) {
                throw InvalidSaveDataException("Ошибка чтения параметров AreaDamageSpell", 
                                              "Game::loadSpell");
            }
            return std::make_unique<AreaDamageSpell>(damage, range, areaSize);
        }
        case SpellType::TRAP: {
            int damage, range;
            file.read(reinterpret_cast<char*>(&damage), sizeof(damage));
            file.read(reinterpret_cast<char*>(&range), sizeof(range));
            if (file.fail()) {
                throw InvalidSaveDataException("Ошибка чтения параметров TrapSpell", 
                                              "Game::loadSpell");
            }
            return std::make_unique<TrapSpell>(damage, range);
        }
        case SpellType::SUMMON: {
            int summonCount, range;
            file.read(reinterpret_cast<char*>(&summonCount), sizeof(summonCount));
            file.read(reinterpret_cast<char*>(&range), sizeof(range));
            if (file.fail()) {
                throw InvalidSaveDataException("Ошибка чтения параметров SummonSpell", 
                                              "Game::loadSpell");
            }
            return std::make_unique<SummonSpell>(summonCount, range);
        }
        case SpellType::ENHANCEMENT: {
            return std::make_unique<EnhancementSpell>();
        }
        default:
            throw InvalidSaveDataException("Неизвестный тип заклинания: " + 
                                          std::to_string(static_cast<int>(type)), 
                                          "Game::loadSpell");
    }
}

void Game::run() {
}

void Game::startGame() {
    startNewGame();
}

