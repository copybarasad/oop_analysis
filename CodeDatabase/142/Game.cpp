#include "Game.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "Spell.h"
#include "InputReader.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <cctype>
#include <string>
#include <optional>

const std::string Game::SAVE_FILE = "game_save.dat";
const std::string Game::CONTROLS_FILE = "controls.cfg";

Game::Game() : field(10, 10), running(true), currentLevel(1), 
               initialEnemyCount(0), enemiesKilled(0), currentPhase(GamePhase::Menu) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    setDefaultControls();
    loadControls();
}

void Game::run() {
    while (running && currentPhase != GamePhase::Exit) {
        switch (currentPhase) {
            case GamePhase::Menu:
                processMenu();
                break;
            case GamePhase::GameStart:
                processGameStart();
                break;
            case GamePhase::LevelStart:
                processLevelStart();
                break;
            case GamePhase::PlayerTurn:
                processPlayerTurn();
                break;
            case GamePhase::AlliesTurn:
                processAlliesTurn();
                break;
            case GamePhase::EnemiesTurn:
                processEnemiesTurn();
                break;
            case GamePhase::EnemyBaseTurn:
                processEnemyBaseTurn();
                break;
            case GamePhase::LevelComplete:
                processLevelComplete();
                break;
            case GamePhase::GameOver:
                processGameOver();
                break;
            default:
                running = false;
                break;
        }
    }
}

void Game::processMenu() {
    system("cls");
    std::cout << "=== ИГРА ===" << std::endl;
    std::cout << "1. Новая игра" << std::endl;
    std::cout << "2. Загрузить игру" << std::endl;
    std::cout << "3. Выход" << std::endl;
    std::cout << "Выберите действие: ";
    
    int choice;
    std::cin >> choice;
    std::cin.ignore();
    
    switch (choice) {
        case 1:
            currentPhase = GamePhase::GameStart;
            break;
        case 2:
            if (loadGame()) {
                currentPhase = GamePhase::LevelStart;
            } else {
                pause("Не удалось загрузить игру. Нажмите Enter для продолжения...");
            }
            break;
        case 3:
            currentPhase = GamePhase::Exit;
            break;
        default:
            pause("Неверный выбор. Нажмите Enter для продолжения...");
            break;
    }
}

void Game::processGameStart() {
    initializeNewGame();
    currentPhase = GamePhase::LevelStart;
}

void Game::processLevelStart() {
    system("cls");
    std::cout << "=== УРОВЕНЬ " << currentLevel << " ===" << std::endl;
    pause("Нажмите Enter, чтобы начать уровень...");
    currentPhase = GamePhase::PlayerTurn;
}

void Game::processPlayerTurn() {
    if (!field.getPlayer() || !field.getPlayer()->isAlive()) {
        currentPhase = GamePhase::GameOver;
        return;
    }
    
    if (checkLevelComplete()) {
        currentPhase = GamePhase::LevelComplete;
        return;
    }
    
    displayGameScreen();
    
    ConsoleCommandReader reader;
    Command cmd = reader.readCommand(*this);
    bool turnUsed = applyCommand(cmd);
    handlePostPlayerAction(turnUsed);
}

void Game::processAlliesTurn() {
    // Placeholder for future allies implementation
    // For now, just move to enemies turn
    currentPhase = GamePhase::EnemiesTurn;
}

void Game::processEnemiesTurn() {
    for (const auto& enemy : field.getEnemies()) {
        int dx = (std::rand() % 3) - 1; // -1, 0, 1
        int dy = (std::rand() % 3) - 1;
        
        if (dx != 0 || dy != 0) {
            enemy->move(dx, dy, field);
        }
        
        if (!field.getPlayer() || !field.getPlayer()->isAlive()) {
            currentPhase = GamePhase::GameOver;
            return;
        }
    }
    
    currentPhase = GamePhase::EnemyBaseTurn;
}

void Game::processEnemyBaseTurn() {
    // Placeholder for future enemy bases/towers implementation
    // For now, just move back to player turn
    currentPhase = GamePhase::PlayerTurn;
}

void Game::processLevelComplete() {
    system("cls");
    field.draw();
    std::cout << "\n=== УРОВЕНЬ " << currentLevel << " ПРОЙДЕН! ===" << std::endl;
    showStats();
    
    currentLevel++;
    pause("Нажмите Enter для продолжения...");
    
    // Initialize next level
    initializeLevel(currentLevel);
    currentPhase = GamePhase::LevelStart;
}

void Game::processGameOver() {
    system("cls");
    field.draw();
    std::cout << "\n=== ИГРА ОКОНЧЕНА ===" << std::endl;
    showStats();
    
    std::cout << "\n1. Начать заново" << std::endl;
    std::cout << "2. Выход в меню" << std::endl;
    std::cout << "Выберите действие: ";
    
    int choice;
    std::cin >> choice;
    std::cin.ignore();
    
    if (choice == 1) {
        currentPhase = GamePhase::GameStart;
    } else {
        currentPhase = GamePhase::Menu;
    }
}

void Game::initializeNewGame() {
    currentLevel = 1;
    enemiesKilled = 0;
    initializeLevel(currentLevel);
}

void Game::initializeLevel(int level) {
    // Clear field
    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            field.clearCell(x, y);
        }
    }
    
    // Initialize walls
    for (int i = 0; i < field.getHeight(); i++) {
        for (int j = 0; j < field.getWidth(); j++) {
            if (std::rand() % 10 == 0) {
                // Walls are handled by Field's initializeWalls, but we need to clear first
            }
        }
    }
    
    // Create new field with walls
    field = Field(field.getWidth(), field.getHeight());
    
    // Place player
    auto player = std::make_unique<Player>(1, 1);
    field.setPlayer(std::move(player));
    
    // Place enemies (more enemies per level)
    int enemyCount = 3 + (level - 1) * 2;
    initialEnemyCount = enemyCount;
    int width = field.getWidth();
    int height = field.getHeight();
    
    for (int i = 0; i < enemyCount; i++) {
        int x, y;
        int attempts = 0;
        do {
            x = std::rand() % width;
            y = std::rand() % height;
            attempts++;
        } while ((!field.isEmpty(x, y) || 
                 (x == field.getPlayer()->getX() && y == field.getPlayer()->getY())) &&
                 attempts < 100);
        
        if (attempts < 100) {
            field.addEnemy(std::make_unique<Enemy>(x, y));
        }
    }
    
    enemiesKilled = 0;
}

bool Game::checkLevelComplete() const {
    return field.getEnemies().empty();
}

bool Game::checkGameOver() const {
    return !field.getPlayer() || !field.getPlayer()->isAlive();
}

void Game::tryGiveNewSpell() {
    if (!field.getPlayer()) return;
    
    Hand& hand = field.getPlayer()->getHand();
    if (hand.isFull()) {
        std::cout << "Рука заклинаний полна! Новое заклинание не получено.\n";
        return;
    }
    
    std::unique_ptr<Spell> newSpell;
    int spellType = std::rand() % 2;
    
    if (spellType == 0) {
        newSpell = std::make_unique<DirectDamageSpell>(3, 20);
    } else {
        newSpell = std::make_unique<AreaDamageSpell>(3, 15);
    }
    
    if (hand.addSpell(std::move(newSpell))) {
        std::cout << "Вы получили новое заклинание: " 
                  << hand.getSpell(hand.getSpellCount() - 1)->getName() << "!\n";
        pause();
    }
}

void Game::showStats() const {
    if (field.getPlayer()) {
        std::cout << "\nУровень: " << currentLevel
                  << " | Здоровье: " << field.getPlayer()->getHP()
                  << " | Урон: " << field.getPlayer()->getDamage()
                  << " | Очки: " << field.getPlayer()->getScore()
                  << " | Позиция: (" << field.getPlayer()->getX() << ", " << field.getPlayer()->getY() << ")\n";
    }
}

void Game::showHand() const {
    if (field.getPlayer()) {
        const Hand& hand = field.getPlayer()->getHand();
        std::cout << "\n=== Рука заклинаний (" << hand.getSpellCount() 
                  << "/" << hand.getMaxSize() << ") ===\n";
        for (int i = 0; i < hand.getSpellCount(); i++) {
            Spell* spell = hand.getSpell(i);
            if (spell) {
                std::cout << "[" << i << "] " << spell->getName() 
                          << " - " << spell->getDescription()
                          << " (Урон: " << spell->getDamage() 
                          << ", Радиус: " << spell->getRadius() << ")\n";
            }
        }
        std::cout << "========================\n";
    }
}

void Game::displayGameScreen() const {
    system("cls");
    field.draw();
    showStats();
    showHand();
}

void Game::pause(const std::string& message) const {
    std::cout << message;
    std::cin.ignore();
    std::cin.get();
}

bool Game::applyCommand(const Command& cmd) {
    bool turnUsed = false;
    switch (cmd.type) {
        case CommandType::Move:
            if (field.getPlayer()) {
                field.getPlayer()->move(cmd.dx, cmd.dy, field);
                turnUsed = true;
            }
            break;
        case CommandType::SwitchMode:
            if (field.getPlayer()) {
                field.getPlayer()->switchCombatMode();
            }
            break;
        case CommandType::CastSpell:
            if (field.getPlayer()) {
                if (field.getPlayer()->castSpell(cmd.spellIndex, cmd.targetX, cmd.targetY, field)) {
                    turnUsed = true;
                    pause();
                } else {
                    pause("Заклинание не применено. Нажмите Enter для продолжения...");
                }
            }
            break;
        case CommandType::SaveGame:
            if (saveGame()) {
                pause("Игра сохранена! Нажмите Enter для продолжения...");
            } else {
                pause("Ошибка сохранения! Нажмите Enter для продолжения...");
            }
            break;
        case CommandType::QuitToMenu:
            currentPhase = GamePhase::Menu;
            break;
        case CommandType::Invalid:
            pause("Неверная команда! Нажмите Enter для продолжения...");
            break;
        default:
            break;
    }
    return turnUsed;
}

void Game::handlePostPlayerAction(bool turnUsed) {
    if (!field.getPlayer() || !field.getPlayer()->isAlive()) {
        currentPhase = GamePhase::GameOver;
        return;
    }

    // Check if enemy was killed and give new spell
    int currentEnemyCount = static_cast<int>(field.getEnemies().size());
    int newKills = initialEnemyCount - currentEnemyCount - enemiesKilled;
    if (newKills > 0) {
        enemiesKilled += newKills;
        tryGiveNewSpell();
    }
    
    if (checkLevelComplete()) {
        currentPhase = GamePhase::LevelComplete;
        return;
    }

    if (turnUsed) {
        currentPhase = GamePhase::AlliesTurn;
    }
}

void Game::render() const {
    displayGameScreen();
}

void Game::setDefaultControls() {
    controls = ControlSettings{};
}

void Game::loadControls() {
    ControlSettings parsed = controls;
    std::string error;

    if (!parseControlsFile(parsed, error)) {
        if (!error.empty()) {
            std::cout << "Ошибка чтения файла управления: " << error 
                      << ". Используются настройки по умолчанию.\n";
        }
        setDefaultControls();
        return;
    }

    if (!validateControls(parsed, error)) {
        std::cout << "Ошибка в конфигурации управления: " << error 
                  << ". Используются настройки по умолчанию.\n";
        setDefaultControls();
        return;
    }

    controls = parsed;
}

bool Game::parseControlsFile(ControlSettings& outSettings, std::string& error) const {
    std::ifstream file(CONTROLS_FILE);
    if (!file.is_open()) {
        error = "файл не найден";
        return false;
    }

    auto trim = [](const std::string& s) -> std::string {
        const char* whitespace = " \t\n\r";
        size_t start = s.find_first_not_of(whitespace);
        if (start == std::string::npos) return "";
        size_t end = s.find_last_not_of(whitespace);
        return s.substr(start, end - start + 1);
    };

    std::unordered_map<std::string, char*> commandMap = {
        {"MOVE_UP", &outSettings.moveUp},
        {"MOVE_DOWN", &outSettings.moveDown},
        {"MOVE_LEFT", &outSettings.moveLeft},
        {"MOVE_RIGHT", &outSettings.moveRight},
        {"SWITCH_MODE", &outSettings.switchMode},
        {"CAST_SPELL", &outSettings.castSpell},
        {"SAVE_GAME", &outSettings.saveGame},
        {"QUIT", &outSettings.quitToMenu}
    };

    std::unordered_map<std::string, bool> seenCommands;
    std::unordered_map<char, std::string> usedKeys;

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        line = trim(line);
        if (line.empty() || line[0] == '#') {
            continue;
        }

        size_t eqPos = line.find('=');
        if (eqPos == std::string::npos) {
            error = "некорректный формат в строке " + std::to_string(lineNumber);
            return false;
        }

        std::string commandName = trim(line.substr(0, eqPos));
        std::string value = trim(line.substr(eqPos + 1));

        if (commandName.empty() || value.empty()) {
            error = "отсутствует команда или клавиша в строке " + std::to_string(lineNumber);
            return false;
        }

        std::transform(commandName.begin(), commandName.end(), commandName.begin(), ::toupper);

        if (value.size() != 1) {
            error = "клавиша должна быть одним символом в строке " + std::to_string(lineNumber);
            return false;
        }

        if (commandMap.find(commandName) == commandMap.end()) {
            error = "неизвестная команда '" + commandName + "' в строке " + std::to_string(lineNumber);
            return false;
        }

        if (seenCommands[commandName]) {
            error = "команда '" + commandName + "' указана несколько раз";
            return false;
        }

        char key = value[0];
        char lowerKey = static_cast<char>(std::tolower(static_cast<unsigned char>(key)));
        if (usedKeys.find(lowerKey) != usedKeys.end() && usedKeys[lowerKey] != commandName) {
            error = "клавиша '" + std::string(1, key) + "' назначена для нескольких команд";
            return false;
        }

        *commandMap[commandName] = key;
        seenCommands[commandName] = true;
        usedKeys[lowerKey] = commandName;
    }

    // Ensure all commands are configured
    if (seenCommands.size() != commandMap.size()) {
        error = "не все команды описаны в файле управления";
        return false;
    }

    return true;
}

bool Game::validateControls(const ControlSettings& settings, std::string& error) const {
    std::unordered_map<char, std::string> usedKeys;

    auto checkKey = [&](char key, const std::string& commandName) -> bool {
        char lowerKey = static_cast<char>(std::tolower(static_cast<unsigned char>(key)));
        if (usedKeys.find(lowerKey) != usedKeys.end()) {
            error = "клавиша '" + std::string(1, key) + "' используется для команд '" + 
                    usedKeys[lowerKey] + "' и '" + commandName + "'";
            return false;
        }
        usedKeys[lowerKey] = commandName;
        return true;
    };

    bool ok = true;
    ok = ok && checkKey(settings.moveUp, "MOVE_UP");
    ok = ok && checkKey(settings.moveDown, "MOVE_DOWN");
    ok = ok && checkKey(settings.moveLeft, "MOVE_LEFT");
    ok = ok && checkKey(settings.moveRight, "MOVE_RIGHT");
    ok = ok && checkKey(settings.switchMode, "SWITCH_MODE");
    ok = ok && checkKey(settings.castSpell, "CAST_SPELL");
    ok = ok && checkKey(settings.saveGame, "SAVE_GAME");
    ok = ok && checkKey(settings.quitToMenu, "QUIT");

    return ok;
}

std::unordered_map<std::string, char> Game::getControlsForTest() const {
    return {
        {"MOVE_UP", controls.moveUp},
        {"MOVE_DOWN", controls.moveDown},
        {"MOVE_LEFT", controls.moveLeft},
        {"MOVE_RIGHT", controls.moveRight},
        {"SWITCH_MODE", controls.switchMode},
        {"CAST_SPELL", controls.castSpell},
        {"SAVE_GAME", controls.saveGame},
        {"QUIT", controls.quitToMenu}
    };
}

bool Game::saveGame() const {
    GameState state = captureGameState();
    std::string error;
    if (!state.saveToFile(SAVE_FILE, error)) {
        std::cout << "Ошибка сохранения: " << error << "\n";
        return false;
    }
    return true;
}

bool Game::loadGame() {
    std::string error;
    auto loaded = GameState::loadFromFile(SAVE_FILE, error);
    if (!loaded.has_value()) {
        std::cout << "Ошибка загрузки: " << error << "\n";
        return false;
    }
    restoreGameState(*loaded);
    return true;
}

GameState Game::captureGameState() const {
    GameState state;
    
    if (field.getPlayer()) {
        Player* player = field.getPlayer();
        state.setPlayer(player->getX(),
                        player->getY(),
                        player->getHP(),
                        player->getMaxHP(),
                        player->getScore(),
                        player->getMeleeDamage(),
                        player->getRangedDamage(),
                        (player->getCombatMode() == CombatMode::Melee) ? 0 : 1);
        
        const Hand& hand = player->getHand();
        state.setHandMaxSize(hand.getMaxSize());
        for (int i = 0; i < hand.getSpellCount(); ++i) {
            Spell* spell = hand.getSpell(i);
            if (spell) {
                GameState::SpellData spellData;
                spellData.type = (spell->getName() == "Прямой урон") ? 0 : 1;
                spellData.radius = spell->getRadius();
                spellData.damage = spell->getDamage();
                state.addSpell(spellData);
            }
        }
    }
    
    state.setFieldSize(field.getWidth(), field.getHeight());
    
    // Save walls
    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            if (field.isWall(x, y)) {
                state.addWall(x, y);
            }
        }
    }
    
    // Save enemies
    for (const auto& enemy : field.getEnemies()) {
        state.addEnemy(enemy->getX(), enemy->getY(), enemy->getHP(), enemy->getDamage());
    }
    
    state.setProgress(currentLevel, initialEnemyCount, enemiesKilled);
    
    return state;
}

void Game::restoreGameState(const GameState& state) {
    currentLevel = state.getCurrentLevel();
    initialEnemyCount = state.getInitialEnemyCount();
    enemiesKilled = state.getEnemiesKilled();
    
    // Recreate field without walls first
    field = Field(state.getFieldWidth(), state.getFieldHeight());
    
    // Clear all cells first
    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            field.clearCell(x, y);
        }
    }
    
    // Restore walls
    for (const auto& wall : state.getWalls()) {
        field.setWall(wall.first, wall.second);
    }
    
    // Restore player
    auto player = std::make_unique<Player>(state.getPlayerX(), state.getPlayerY(), 
                                           state.getPlayerMaxHP(), state.getMeleeDamage(), state.getRangedDamage());
    player->setPosition(state.getPlayerX(), state.getPlayerY());
    player->setHP(state.getPlayerHP());
    player->setScore(state.getPlayerScore());
    
    if (state.getCombatMode() == 1 && player->getCombatMode() == CombatMode::Melee) {
        player->switchCombatMode();
    }
    
    // Restore hand
    Hand& hand = player->getHand();
    // Clear existing spells and add saved ones
    while (hand.getSpellCount() > 0) {
        hand.removeSpell(0);
    }
    
    for (const auto& spellData : state.getSpells()) {
        std::unique_ptr<Spell> spell;
        if (spellData.type == 0) {
            spell = std::make_unique<DirectDamageSpell>(spellData.radius, spellData.damage);
        } else {
            spell = std::make_unique<AreaDamageSpell>(spellData.radius, spellData.damage);
        }
        hand.addSpell(std::move(spell));
    }
    
    field.setPlayer(std::move(player));
    
    // Restore enemies
    const auto& enemies = state.getEnemies();
    const auto& hp = state.getEnemyHP();
    const auto& dmg = state.getEnemyDamage();
    for (size_t i = 0; i < enemies.size(); ++i) {
        auto enemy = std::make_unique<Enemy>(enemies[i].first, enemies[i].second,
                                              hp[i], dmg[i]);
        field.addEnemy(std::move(enemy));
    }
}
