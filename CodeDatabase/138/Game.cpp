#include "Game.h"
#include "GameExceptions.h"
#include "GameState.h"
#include "SpellFactory.h"
#include "MathUtils.h"
#include <iostream>
#include <limits>
#include <vector>

Game::Game()
    : player(nullptr)
    , field(nullptr)
    , levelManager(nullptr)
    , saveLoadSystem(nullptr)
    , currentPhase(GamePhase::MENU)
    , currentTurn(0) {
    
    saveLoadSystem = std::make_unique<SaveLoadSystem>();
}

void Game::Run() {
    std::cout << "╔══════════════════════════════════════════╗" << std::endl;
    std::cout << "║       OOP GAME PROJECT                   ║" << std::endl;
    std::cout << "║       Добро пожаловать!                  ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════╝\n" << std::endl;
    
    while (currentPhase != GamePhase::EXITING) {
        try {
            switch (currentPhase) {
                case GamePhase::MENU:
                    showMainMenu();
                    break;
                case GamePhase::PLAYING:
                    gameLoop();
                    break;
                case GamePhase::LEVEL_COMPLETE:
                    showLevelCompleteScreen();
                    break;
                case GamePhase::GAME_OVER:
                    showGameOverMenu();
                    break;
                case GamePhase::EXITING:
                    break;
            }
        } catch (const GameException& error) {
            std::cerr << "\n✗ Ошибка игры: " << error.what() << std::endl;
            std::cout << "Нажмите Enter для продолжения..." << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } catch (const std::exception& error) {
            std::cerr << "\n✗ Неожиданная ошибка: " << error.what() << std::endl;
            std::cout << "Нажмите Enter для продолжения..." << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    
    std::cout << "\nСпасибо за игру!" << std::endl;
}

void Game::initializeGame() {
    player = std::make_unique<Player>(100, 20, 15, 5);
    field = std::make_unique<GameField>(15, 15);
    levelManager = std::make_unique<LevelManager>();
    currentTurn = 0;
}

void Game::showMainMenu() {
    std::cout << "\n═══════════════════════════════════════════" << std::endl;
    std::cout << "ГЛАВНОЕ МЕНЮ" << std::endl;
    std::cout << "═══════════════════════════════════════════" << std::endl;
    std::cout << "1. Новая игра" << std::endl;
    std::cout << "2. Загрузить игру" << std::endl;
    std::cout << "3. Выход" << std::endl;
    std::cout << "═══════════════════════════════════════════" << std::endl;
    
    int choice = 0;
    while (choice < 1 || choice > 3) {
        std::cout << "Ваш выбор (1-3): ";
        std::cin >> choice;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = 0;
            continue;
        }
        
        if (choice < 1 || choice > 3) {
            std::cout << "Выберите число от 1 до 3!" << std::endl;
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    switch (choice) {
        case 1:
            startNewGame();
            break;
        case 2:
            loadGame();
            break;
        case 3:
            currentPhase = GamePhase::EXITING;
            break;
    }
}

void Game::startNewGame() {
    try {
        initializeGame();
        
        auto initialSpell = SpellFactory::CreateRandomSpell(field->GetWidth(), field->GetHeight());
        player->AddSpell(std::move(initialSpell));
        
        levelManager->StartLevel(1, *field);
        
        currentPhase = GamePhase::PLAYING;
    } catch (const std::exception& error) {
        std::cerr << "Ошибка при запуске новой игры: " << error.what() << std::endl;
        currentPhase = GamePhase::MENU;
    }
}

void Game::loadGame() {
    try {
        if (!saveLoadSystem->SaveExists(SaveLoadSystem::DEFAULT_SAVE_FILE)) {
            std::cout << "\n✗ Файл сохранения не найден!" << std::endl;
            std::cout << "Нажмите Enter для продолжения..." << std::endl;
            std::cin.get();
            currentPhase = GamePhase::MENU;
            return;
        }
        
        // Загружаем полное состояние игры
        GameState savedState = GameState::LoadFromFile(SaveLoadSystem::DEFAULT_SAVE_FILE);
        
        // Создаём новое поле с правильным размером
        initializeGame();
        field = std::make_unique<GameField>(savedState.fieldWidth, savedState.fieldHeight);
        
        // Применяем сохранённое состояние
        savedState.ApplyToGame(*player, *field, *levelManager, currentTurn);
        
        std::cout << "\n✓ Игра полностью загружена!" << std::endl;
        std::cout << "Уровень: " << levelManager->GetCurrentLevelNumber() << std::endl;
        std::cout << "Ход: " << currentTurn << std::endl;
        std::cout << "Врагов: " << field->GetEnemyCount() << std::endl;
        std::cout << "Союзников: " << field->GetAllyCount() << std::endl;
        
        currentPhase = GamePhase::PLAYING;
    } catch (const SaveFileException& error) {
        std::cerr << "\n✗ " << error.what() << std::endl;
        std::cout << "Нажмите Enter для продолжения..." << std::endl;
        std::cin.get();
        currentPhase = GamePhase::MENU;
    } catch (const std::exception& error) {
        std::cerr << "\n✗ Ошибка загрузки: " << error.what() << std::endl;
        std::cout << "Нажмите Enter для продолжения..." << std::endl;
        std::cin.get();
        currentPhase = GamePhase::MENU;
    }
}

void Game::gameLoop() {
    while (currentPhase == GamePhase::PLAYING && player->IsAlive()) {
        processTurn();
        
        if (currentPhase != GamePhase::PLAYING) {
            break;
        }
    }
}

void Game::processTurn() {
    currentTurn++;
    
    std::cout << "\n╔══════════════════════════════════════════╗" << std::endl;
    std::cout << "║ ХОД " << currentTurn << std::endl;
    std::cout << "╚══════════════════════════════════════════╝" << std::endl;
    
    displayGameState();
    displaySpells();
    
    if (!player->CanMove()) {
        player->RemoveSlowEffect();
        std::cout << "\n⚠ Игрок пропускает ход из-за эффекта замедления!" << std::endl;
        std::cout << "Нажмите Enter для продолжения..." << std::endl;
        std::cin.get();
    } else {
        handlePlayerInput();
    }
    
    if (currentPhase != GamePhase::PLAYING) {
        return;
    }
    
    std::cout << "\n--- Ход союзников ---" << std::endl;
    updateAllies();
    
    std::cout << "\n--- Ход врагов ---" << std::endl;
    updateEnemies();
    
    std::cout << "\n--- Ход зданий ---" << std::endl;
    updateBuildings();
    
    std::cout << "\n--- Ход башен ---" << std::endl;
    updateTowers();
    
    checkTraps();
    removeDeadEntities();
    checkLevelCompletion();
    checkGameOver();
}

void Game::displayGameState() {
    field->Display();
    std::cout << "\nЗдоровье: " << player->GetHealth() << std::endl;
    std::cout << "Очки: " << player->GetScore() << std::endl;
    std::cout << "Режим боя: " << (player->GetCombatMode() == CombatMode::MELEE ? "Ближний" : "Дальний") << std::endl;
    std::cout << "Уровень: " << levelManager->GetCurrentLevelNumber() << std::endl;
}

void Game::displaySpells() {
    std::cout << "\nЗаклинания:" << std::endl;
    if (player->GetSpellCount() == 0) {
        std::cout << "  (нет заклинаний)" << std::endl;
    } else {
        for (size_t i = 0; i < player->GetSpellCount(); ++i) {
            std::cout << "  " << i << ": " << player->GetSpellNameAt(i) << std::endl;
        }
    }
}

void Game::handlePlayerInput() {
    std::cout << "\n═══════════════════════════════════════════" << std::endl;
    std::cout << "Команды:" << std::endl;
    std::cout << "  w/s/a/d   - движение" << std::endl;
    std::cout << "  [пробел]  - атака" << std::endl;
    std::cout << "  c         - сменить режим боя" << std::endl;
    std::cout << "  z[номер]  - использовать заклинание" << std::endl;
    std::cout << "  b         - купить заклинание (30 очков)" << std::endl;
    std::cout << "  save      - сохранить игру" << std::endl;
    std::cout << "  q         - выход в меню" << std::endl;
    std::cout << "═══════════════════════════════════════════" << std::endl;
    
    std::string input;
    std::getline(std::cin, input);
    
    if (input.empty()) {
        return;
    }
    
    // Проверяем многосимвольные команды СНАЧАЛА
    if (input == "save") {
        handleSaveGame();
        return;
    }
    
    // Затем односимвольные
    char command = input[0];
    
    switch (command) {
        case 'w':
        case 's':
        case 'a':
        case 'd':
            handleMovement(command);
            break;
        case ' ':
            handleAttack();
            break;
        case 'c':
            handleSwitchMode();
            break;
        case 'z':
            handleSpellCast(input);
            break;
        case 'b':
            handleBuySpell();
            break;
        case 'q':
            if (askYesNo("Вернуться в главное меню? Несохраненный прогресс будет потерян!")) {
                currentPhase = GamePhase::MENU;
            }
            break;
        default:
            std::cout << "✗ Неизвестная команда!" << std::endl;
            break;
    }
}

void Game::handleMovement(char direction) {
    int deltaX = 0, deltaY = 0;
    
    switch (direction) {
        case 'w': deltaY = -1; break;
        case 's': deltaY = 1; break;
        case 'a': deltaX = -1; break;
        case 'd': deltaX = 1; break;
    }
    
    if (field->MovePlayer(deltaX, deltaY)) {
        std::cout << "✓ Игрок переместился" << std::endl;
        
        if (field->IsCellSlowing(field->GetPlayerX(), field->GetPlayerY())) {
            player->ApplySlowEffect();
            std::cout << "⚠ Игрок наступил на замедляющую клетку! Следующий ход будет пропущен." << std::endl;
        }
    } else {
        std::cout << "✗ Невозможно переместиться в эту клетку!" << std::endl;
    }
}

void Game::handleAttack() {
    int playerX = field->GetPlayerX();
    int playerY = field->GetPlayerY();
    int damage = player->GetCurrentDamage();
    bool attacked = false;
    
    if (player->GetCombatMode() == CombatMode::MELEE) {
        for (int dy = -1; dy <= 1 && !attacked; ++dy) {
            for (int dx = -1; dx <= 1 && !attacked; ++dx) {
                if (dx == 0 && dy == 0) continue;
                
                int targetX = playerX + dx;
                int targetY = playerY + dy;
                
                if (field->IsEnemyAt(targetX, targetY)) {
                    field->AttackEnemyAt(targetX, targetY, damage);
                    std::cout << "✓ Игрок атаковал врага в ближнем бою! Урон: " << damage << std::endl;
                    attacked = true;
                } else if (field->IsBuildingAt(targetX, targetY)) {
                    field->AttackBuildingAt(targetX, targetY, damage);
                    std::cout << "✓ Игрок атаковал здание в ближнем бою! Урон: " << damage << std::endl;
                    attacked = true;
                }
            }
        }
    } else {
        int attackRadius = 3;
        for (int dy = -attackRadius; dy <= attackRadius && !attacked; ++dy) {
            for (int dx = -attackRadius; dx <= attackRadius && !attacked; ++dx) {
                if (dx == 0 && dy == 0) continue;
                
                int distance = MathUtils::CalculateDistance(0, 0, dx, dy);
                if (distance > attackRadius) continue;
                
                int targetX = playerX + dx;
                int targetY = playerY + dy;
                
                if (field->IsEnemyAt(targetX, targetY)) {
                    field->AttackEnemyAt(targetX, targetY, damage);
                    std::cout << "✓ Игрок атаковал врага в дальнем бою! Урон: " << damage << std::endl;
                    attacked = true;
                } else if (field->IsBuildingAt(targetX, targetY)) {
                    field->AttackBuildingAt(targetX, targetY, damage);
                    std::cout << "✓ Игрок атаковал здание в дальнем бою! Урон: " << damage << std::endl;
                    attacked = true;
                }
            }
        }
    }
    
    if (!attacked) {
        std::cout << "✗ Нет целей в радиусе атаки!" << std::endl;
    }
}

void Game::handleSpellCast(const std::string& input) {
    int spellIndex = -1;
    
    if (input.length() > 1) {
        try {
            spellIndex = std::stoi(input.substr(1));
        } catch (...) {
            spellIndex = -1;
        }
    }
    
    if (spellIndex < 0 || spellIndex >= static_cast<int>(player->GetSpellCount())) {
        std::cout << "Введите номер заклинания: ";
        std::cin >> spellIndex;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    if (spellIndex < 0 || spellIndex >= static_cast<int>(player->GetSpellCount())) {
        std::cout << "✗ Неверный номер заклинания!" << std::endl;
        return;
    }
    
    Spell* spell = player->GetSpellAt(spellIndex);
    if (!spell) {
        std::cout << "✗ Заклинание не найдено!" << std::endl;
        return;
    }
    
    if (spell->IsEnhancement()) {
        std::cout << "Введите номер заклинания для улучшения: ";
        int targetSpellIndex;
        std::cin >> targetSpellIndex;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (targetSpellIndex < 0 || targetSpellIndex >= static_cast<int>(player->GetSpellCount())) {
            std::cout << "✗ Неверный номер заклинания!" << std::endl;
            return;
        }
        
        if (targetSpellIndex == spellIndex) {
            std::cout << "✗ Нельзя улучшить заклинание улучшения само собой!" << std::endl;
            return;
        }
        
        Spell* targetSpell = player->GetSpellAt(targetSpellIndex);
        if (targetSpell && targetSpell->IsEnhancement()) {
            std::cout << "✗ Нельзя улучшить заклинание улучшения!" << std::endl;
            return;
        }
        
        if (targetSpell) {
            targetSpell->Enhance();
            std::cout << "✓ Заклинание '" << targetSpell->GetName() << "' улучшено!" << std::endl;
            player->RemoveSpellAt(spellIndex);
        }
    } else {
        std::cout << "Введите координаты цели (x y): ";
        int targetX, targetY;
        std::cin >> targetX >> targetY;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (player->UseSpell(spellIndex, *field, targetX, targetY)) {
            std::cout << "✓ Заклинание '" << spell->GetName() << "' успешно применено!" << std::endl;
            player->RemoveSpellAt(spellIndex);
        } else {
            std::cout << "✗ Не удалось применить заклинание (цель вне радиуса или не найдена)!" << std::endl;
        }
    }
}

void Game::handleSwitchMode() {
    player->SwitchCombatMode();
    std::cout << "✓ Режим боя изменен на: " 
              << (player->GetCombatMode() == CombatMode::MELEE ? "Ближний" : "Дальний") << std::endl;
}

void Game::handleBuySpell() {
    if (player->TryBuySpell()) {
        auto newSpell = SpellFactory::CreateRandomSpell(field->GetWidth(), field->GetHeight());
        player->AddSpell(std::move(newSpell));
        std::cout << "✓ Заклинание куплено за " << Player::SPELL_COST << " очков!" << std::endl;
    } else {
        std::cout << "✗ Недостаточно очков или рука заполнена!" << std::endl;
    }
}

void Game::handleSaveGame() {
    try {
        // Захватываем полное состояние игры
        GameState currentState = GameState::CaptureFromGame(*player, *field, *levelManager, currentTurn);
        currentState.SaveToFile(SaveLoadSystem::DEFAULT_SAVE_FILE);
        std::cout << "\n✓ Игра полностью сохранена!" << std::endl;
    } catch (const SaveFileException& error) {
        std::cerr << "✗ " << error.what() << std::endl;
    } catch (const std::exception& error) {
        std::cerr << "✗ Ошибка сохранения: " << error.what() << std::endl;
    }
}

void Game::updateAllies() {
    std::vector<std::pair<int, int>> allyAttacks = field->UpdateAllies();
    for (const auto& attack : allyAttacks) {
        std::cout << "  Союзник в (" << attack.first << ", " << attack.second << ") атаковал врага!" << std::endl;
    }
    
    if (allyAttacks.empty()) {
        std::cout << "  (нет союзников или активности)" << std::endl;
    }
}

void Game::updateEnemies() {
    std::vector<int> attackingEnemies = field->MoveEnemies();
    
    for (int enemyIndex : attackingEnemies) {
        int damage = field->GetEnemyDamageAt(enemyIndex);
        player->TakeDamage(damage);
        std::cout << "  ⚠ Враг атаковал игрока! Урон: " << damage << std::endl;
    }
    
    if (attackingEnemies.empty()) {
        std::cout << "  (враги передвигаются)" << std::endl;
    }
}

void Game::updateBuildings() {
    field->UpdateBuildings();
    std::cout << "  (здания производят врагов)" << std::endl;
}

void Game::updateTowers() {
    field->UpdateTowers();
    
    for (size_t i = 0; i < field->GetTowerCount(); ++i) {
        int towerX = field->GetTowerX(i);
        int towerY = field->GetTowerY(i);
        int damage = field->GetTowerDamageAt(towerX, towerY);
        
        if (damage > 0) {
            player->TakeDamage(damage);
            std::cout << "  ⚠ Башня атаковала игрока! Урон: " << damage << std::endl;
        }
    }
}

void Game::checkTraps() {
    field->CheckTraps();
}

void Game::removeDeadEntities() {
	size_t enemiesBefore = field->GetEnemyCount();
	field->RemoveDeadEnemies();
	size_t enemiesAfter = field->GetEnemyCount();
	
	int killedEnemies = static_cast<int>(enemiesBefore - enemiesAfter);
	if (killedEnemies > 0) {
		int pointsPerEnemy = 10;
		player->AddScore(killedEnemies * pointsPerEnemy);
		std::cout << "\n✓ Убито врагов: " << killedEnemies << " (+" << (killedEnemies * pointsPerEnemy) << " очков)" << std::endl;
	}
	
	size_t buildingsBefore = field->GetBuildingCount();
	field->RemoveDeadBuildings();
	size_t buildingsAfter = field->GetBuildingCount();
	
	int destroyedBuildings = static_cast<int>(buildingsBefore - buildingsAfter);
	if (destroyedBuildings > 0) {
		int pointsPerBuilding = 50;
		player->AddScore(destroyedBuildings * pointsPerBuilding);
		std::cout << "✓ Уничтожено зданий: " << destroyedBuildings << " (+" << (destroyedBuildings * pointsPerBuilding) << " очков)" << std::endl;
	}
	
	field->RemoveDeadAllies();
}

void Game::checkLevelCompletion() {
    if (levelManager->GetCurrentLevel() && 
        levelManager->GetCurrentLevel()->IsCompleted(*field)) {
        currentPhase = GamePhase::LEVEL_COMPLETE;
    }
}

void Game::checkGameOver() {
    if (!player->IsAlive()) {
        currentPhase = GamePhase::GAME_OVER;
    }
}

void Game::showGameOverMenu() {
    std::cout << "\n╔══════════════════════════════════════════╗" << std::endl;
    std::cout << "║           ИГРА ОКОНЧЕНА                  ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════╝" << std::endl;
    std::cout << "Финальные очки: " << player->GetScore() << std::endl;
    std::cout << "Достигнутый уровень: " << levelManager->GetCurrentLevelNumber() << std::endl;
    std::cout << "\n1. Начать заново" << std::endl;
    std::cout << "2. Выход в меню" << std::endl;
    
    int choice = 0;
    while (choice < 1 || choice > 2) {
        std::cout << "Ваш выбор (1-2): ";
        std::cin >> choice;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = 0;
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (choice == 1) {
        startNewGame();
    } else {
        currentPhase = GamePhase::MENU;
    }
}

void Game::showLevelCompleteScreen() {
    levelManager->CompleteLevel(*player);
    
    if (askYesNo("\nПерейти на следующий уровень?")) {
        transitionToNextLevel();
    } else {
        currentPhase = GamePhase::MENU;
    }
}

void Game::transitionToNextLevel() {
    levelManager->ApplyLevelTransition(*player);
    
    levelManager->ShowUpgradeMenu(*player);
    
    int nextLevel = levelManager->GetCurrentLevelNumber() + 1;
    
    int newWidth = 15 + (nextLevel - 1) * 3;
    int newHeight = 15 + (nextLevel - 1) * 3;
    if (newWidth > 25) newWidth = 25;
    if (newHeight > 25) newHeight = 25;
    
    field = std::make_unique<GameField>(newWidth, newHeight);
    
    levelManager->StartLevel(nextLevel, *field);
    
    currentPhase = GamePhase::PLAYING;
}

bool Game::askYesNo(const std::string& question) {
    std::cout << question << " (y/n): ";
    char answer;
    std::cin >> answer;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return (answer == 'y' || answer == 'Y');
}

