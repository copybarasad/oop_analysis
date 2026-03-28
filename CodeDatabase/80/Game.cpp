#include "Game.h"
#include "ConsoleRenderer.h"
#include "GameVisualizer.h"
#include "GameRenderData.h"
#include "GameEventManager.h"
#include <iostream>
#include <limits>

Game::Game(Map& gameMap)
    : map(gameMap), hero(), tower(3), attack_tower(), counter(0),
    gameRunning(false), levelCompleted(false), levelManager(), currentLevel(1),
    inMainMenu(true), inLoadMenu(false), inSaveMenu(false),
    visualizer(std::make_unique<GameVisualizer<ConsoleRenderer>>(80)) {

    LOG_INFO("Game object created");
}

void Game::InitializeGame() {
    LOG_INFO("Initializing new game");

    gameRunning = true;
    levelCompleted = false;
    counter = 0;
    inMainMenu = false;

    levelManager.ResetToFirstLevel();
    currentLevel = 1;

    LOG_DEBUG("Game state reset: level=1, counter=0");

    InitializeLevel();
}

void Game::StartNewGame() {
    LOG_INFO("=== STARTING NEW GAME ===");
    InitializeGame();

    GameEventManager::getInstance().logGameStarted();
    LOG_INFO("New game started successfully");
}

void Game::DisplayGameState() {
    LOG_DEBUG("Displaying game state");
    visualizer->render(GetRenderData());
}

bool Game::loadGame(const std::string& saveName) {
    LOG_INFO("Attempting to load game: " + saveName);

    CleanupGame();

    if (saveSystem.LoadGame(saveName, map, hero, tower, attack_tower, counter, currentLevel)) {
        gameRunning = true;
        levelCompleted = false;
        inMainMenu = false;

        GameEventManager::getInstance().logGameLoaded(saveName);
        LOG_INFO("Game loaded successfully: " + saveName);
        LOG_INFO("Loaded level: " + std::to_string(currentLevel) +
            ", turn: " + std::to_string(counter));
        LOG_INFO("Player HP: " + std::to_string(hero.heallpoints) +
            "/" + std::to_string(hero.GetMaxHealth()));

        return true;
    }

    LOG_ERROR("Failed to load game: " + saveName);
    return false;
}

std::vector<std::string> Game::getSaveList() {
    auto saves = saveSystem.GetSaveList();
    LOG_DEBUG("Retrieved save list, count: " + std::to_string(saves.size()));
    return saves;
}

void Game::ProcessEnemiesTurn() {
    LOG_DEBUG("Processing enemies turn");
    manag.MoveEnemies(map);
}

void Game::ProcessStructuresTurn() {
    LOG_DEBUG("Processing structures turn");
    tower.Move(map);
    attack_tower.Move(map);
}

bool Game::CheckWinCondition() {
    bool victory = attack_tower.heallpoints <= 0;
    if (victory) {
        LOG_INFO("WIN CONDITION MET: Attack tower destroyed!");
    }
    return victory;
}

bool Game::CheckLoseCondition() {
    bool defeat = hero.heallpoints <= 0;
    if (defeat) {
        LOG_CRITICAL("LOSE CONDITION MET: Player defeated!");
    }
    return defeat;
}

void Game::HandleGameEnd() {
    if (CheckWinCondition()) {
        GameRenderData data = GetRenderData();
        data.victory = true;
        data.statusMessage = "VICTORY! You destroyed the enemy tower!";
        visualizer->render(data);

        GameEventManager::getInstance().logVictory();
        LOG_INFO("=== VICTORY ===");
        LOG_INFO("Player destroyed enemy tower on level " + std::to_string(currentLevel));
        LOG_INFO("Total turns: " + std::to_string(counter));
        LOG_INFO("Enemies defeated: " + std::to_string(hero.GetDefeatedCount()));
    }
    else if (CheckLoseCondition()) {
        GameRenderData data = GetRenderData();
        data.gameOver = true;
        data.statusMessage = "GAME OVER! Your hero has fallen.";
        visualizer->render(data);

        GameEventManager::getInstance().logGameOver();
        LOG_CRITICAL("=== GAME OVER ===");
        LOG_CRITICAL("Player defeated on level " + std::to_string(currentLevel));
        LOG_CRITICAL("Turn: " + std::to_string(counter));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    LOG_INFO("Returning to main menu after game end");
    returnToMainMenu();
}

GameRenderData Game::GetRenderData() {
    GameRenderData data;
    data.currentLevel = currentLevel;
    data.turnCounter = counter;
    data.gameRunning = gameRunning;

    data.inMainMenu = inMainMenu;
    data.inLoadMenu = inLoadMenu;
    data.inSaveMenu = inSaveMenu;
    data.levelCompleted = levelCompleted;
    data.gameOver = CheckLoseCondition();
    data.victory = CheckWinCondition();

    data.playerHP = hero.heallpoints;
    data.playerMaxHP = hero.GetMaxHealth();
    data.playerDamage = hero.GetBaseDamage();
    data.playerSpellCount = hero.GetSpellCount();
    data.enemiesDefeated = hero.GetDefeatedCount();
    data.playerSlowed = hero.IsSlowed();
    data.playerX = hero.x;
    data.playerY = hero.y;

    data.gameMap = &map;
    data.mapWidth = map.GetWeight();
    data.mapHeight = map.GetHeight();

    LOG_DEBUG("Render data created: Level=" + std::to_string(currentLevel) +
        ", Turn=" + std::to_string(counter) +
        ", PlayerHP=" + std::to_string(data.playerHP));

    return data;
}

std::string Game::getGameStats() {
    std::string stats = "Level: " + std::to_string(currentLevel) +
        ", Turn: " + std::to_string(counter) +
        ", HP: " + std::to_string(hero.heallpoints) + "/" + std::to_string(hero.GetMaxHealth()) +
        ", Enemies defeated: " + std::to_string(hero.GetDefeatedCount());

    LOG_DEBUG("Game stats requested: " + stats);

    return stats;
}

void Game::SaveGameMenu() {
    LOG_INFO("Entering save game menu");

    GameRenderData data;
    data.inSaveMenu = true;
    data.statusMessage = "=== SAVE GAME ===";
    data.inputPrompt = "Enter save name (or 'cancel' to return): ";
    visualizer->render(data);

    std::string saveName;
    std::getline(std::cin, saveName);

    if (saveName == "cancel" || saveName.empty()) {
        LOG_INFO("Save cancelled by user");
        return;
    }

    if (saveSystem.SaveExists(saveName)) {
        LOG_WARNING("Save '" + saveName + "' already exists");
        data.inputPrompt = "Save '" + saveName + "' already exists. Overwrite? (y/n): ";
        visualizer->showPrompt(data.inputPrompt);

        char choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice != 'y' && choice != 'Y') {
            data.statusMessage = "Save cancelled.";
            visualizer->renderWithMessage(GetRenderData(), data.statusMessage);
            LOG_INFO("Save overwrite cancelled by user");
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            return;
        }
    }

    if (saveSystem.SaveGame(saveName, map, hero, tower, attack_tower, counter, currentLevel)) {
        data.statusMessage = "Game saved successfully as: " + saveName + " (Level " +
            std::to_string(currentLevel) + ")";
        visualizer->renderWithMessage(GetRenderData(), data.statusMessage);

        GameEventManager::getInstance().logGameSaved(saveName);
        LOG_INFO("Game saved successfully: " + saveName);
        LOG_INFO("Save details: Level " + std::to_string(currentLevel) +
            ", Turn " + std::to_string(counter));
    }
    else {
        data.statusMessage = "Failed to save game!";
        visualizer->renderWithMessage(GetRenderData(), data.statusMessage);
        LOG_ERROR("Failed to save game: " + saveName);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
}

void Game::InitializeLevel() {
    LOG_INFO("=== INITIALIZING LEVEL " + std::to_string(currentLevel) + " ===");

    Level currentLevelObj = levelManager.GetCurrentLevel();
    map = currentLevelObj.CreateMap();

    LOG_DEBUG("Map created: " + std::to_string(map.GetWeight()) + "x" +
        std::to_string(map.GetHeight()));

    gameRunning = true;
    levelCompleted = false;
    counter = 0;

    hero.heallpoints = hero.GetMaxHealth();
    LOG_DEBUG("Player health restored to: " + std::to_string(hero.heallpoints));

    RemoveHalfOfSpells();

    tower = Tower(3);
    tower.SetEnemyStats(currentLevelObj.GetEnemyHealth(), currentLevelObj.GetEnemyDamage());
    attack_tower = AttackTower();
    tower.heallpoints = currentLevelObj.GetTowerHealth();
    attack_tower.heallpoints = currentLevelObj.GetAttackTowerHealth();

    LOG_DEBUG("Tower stats: HP=" + std::to_string(tower.heallpoints) +
        ", EnemyHP=" + std::to_string(currentLevelObj.GetEnemyHealth()) +
        ", EnemyDamage=" + std::to_string(currentLevelObj.GetEnemyDamage()));
    LOG_DEBUG("Attack tower HP: " + std::to_string(attack_tower.heallpoints));

    if (currentLevel == 1) {
        tower.Spawn(2, 2, map);
        hero.Spawn(0, 0, map);
        attack_tower.Spawn(5, 5, map);
        LOG_INFO("Level 1: Tutorial placement");
    }
    else if (currentLevel == 2) {
        tower.Spawn(4, 5, map);
        hero.Spawn(0, 0, map);
        attack_tower.Spawn(8, 8, map);
        LOG_INFO("Level 2: Intermediate placement");
    }
    else {
        tower.Spawn(5, 5, map);
        hero.Spawn(0, 0, map);
        attack_tower.Spawn(12, 12, map);
        LOG_INFO("Level " + std::to_string(currentLevel) + ": Advanced placement");
    }

    manag = GameManager();
    LOG_DEBUG("GameManager initialized");

    GameEventManager::getInstance().logLevelStarted(currentLevel, currentLevelObj.GetLevelName());

    LOG_INFO("Level started: " + currentLevelObj.GetLevelName());
    LOG_INFO("Enemy stats: HP=" + std::to_string(currentLevelObj.GetEnemyHealth()) +
        ", Damage=" + std::to_string(currentLevelObj.GetEnemyDamage()));

    GameRenderData data;
    data.statusMessage = "Level " + std::to_string(currentLevel) +
        " - " + currentLevelObj.GetLevelName() +
        "\nEnemy stats: HP=" + std::to_string(currentLevelObj.GetEnemyHealth()) +
        ", Damage=" + std::to_string(currentLevelObj.GetEnemyDamage());
    visualizer->renderWithMessage(data, data.statusMessage);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    LOG_INFO("Level initialization complete");
}

void Game::CleanupGame() {
    LOG_DEBUG("Cleaning up game state");

    auto& objects = map.GetObjects();
    int enemyCount = 0;

    for (auto it = objects.begin(); it != objects.end(); ) {
        if (it->second && it->second->ind_type == 'e') {
            delete it->second;
            it = objects.erase(it);
            enemyCount++;
        }
        else {
            ++it;
        }
    }

    if (enemyCount > 0) {
        LOG_INFO("Cleaned up " + std::to_string(enemyCount) + " enemies from map");
    }
}

void Game::RemoveHalfOfSpells() {
    int currentSpells = hero.GetSpellCount();

    LOG_DEBUG("Removing half of spells. Current count: " + std::to_string(currentSpells));

    if (currentSpells > 0) {
        int spellsToRemove = currentSpells / 2;
        LOG_INFO("Removing " + std::to_string(spellsToRemove) + " out of " +
            std::to_string(currentSpells) + " spells");

        for (int i = 0; i < spellsToRemove; i++) {
            hero.RemoveRandomSpell();
        }

        int remaining = hero.GetSpellCount();
        LOG_INFO("Spells after removal: " + std::to_string(remaining));
    }
    else {
        LOG_DEBUG("No spells to remove");
    }
}

void Game::showMainMenu() {
    LOG_DEBUG("Showing main menu");

    inMainMenu = true;
    inLoadMenu = false;
    inSaveMenu = false;

    GameRenderData data = GetRenderData();
    data.inMainMenu = true;
    data.inputPrompt = "Choose option (1-3): ";

    visualizer->render(data);
}

void Game::returnToMainMenu() {
    LOG_INFO("Returning to main menu");

    gameRunning = false;
    inMainMenu = true;
    inLoadMenu = false;
    inSaveMenu = false;
    levelCompleted = false;

    CleanupGame();

    showMainMenu();
}

void Game::showLoadMenu() {
    LOG_DEBUG("Showing load menu");

    inLoadMenu = true;
    inMainMenu = false;

    GameRenderData data = GetRenderData();
    data.inLoadMenu = true;
    data.inMainMenu = false;

    auto saves = saveSystem.GetSaveList();
    if (saves.empty()) {
        data.statusMessage = "No save games found!";
        data.inputPrompt = "Press any key to continue...";
        LOG_WARNING("No save games found when showing load menu");
    }
    else {
        data.saveList = saves;
        data.inputPrompt = "Choose save to load (0 to cancel): ";
        LOG_DEBUG("Found " + std::to_string(saves.size()) + " save games");
    }

    visualizer->render(data);
}

void Game::LoadGameMenu() {
    LOG_INFO("Entering load game menu");

    auto saves = saveSystem.GetSaveList();
    GameRenderData data;
    data.inLoadMenu = true;

    if (saves.empty()) {
        data.statusMessage = "No save games found!";
        data.inputPrompt = "Press any key to continue...";
        visualizer->render(data);
        LOG_WARNING("No save games available to load");

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return;
    }

    data.saveList = saves;
    data.menuOptions.clear();
    for (size_t i = 0; i < saves.size(); i++) {
        data.menuOptions.push_back(std::to_string(i + 1) + ". " + saves[i]);
    }
    data.menuOptions.push_back("0. Back to main menu");
    data.inputPrompt = "Choose save to load: ";

    visualizer->render(data);

    int choice;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        LOG_WARNING("Invalid input in load menu");
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice > 0 && choice <= static_cast<int>(saves.size())) {
        std::string saveName = saves[choice - 1];
        LOG_INFO("User selected save: " + saveName + " (choice " + std::to_string(choice) + ")");

        if (loadGame(saveName)) {
            data.statusMessage = "Game loaded successfully!";
            visualizer->renderWithMessage(GetRenderData(), data.statusMessage);
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        }
        else {
            LOG_ERROR("Failed to load selected game: " + saveName);
        }
    }
    else if (choice == 0) {
        LOG_INFO("Load cancelled, returning to main menu");
    }
    else {
        LOG_WARNING("Invalid choice in load menu: " + std::to_string(choice));
    }
}

void Game::ShowLevelUpMenu() {
    LOG_INFO("=== LEVEL UP MENU ===");
    LOG_INFO("Player leveled up to level " + std::to_string(currentLevel));

    GameRenderData data;
    data.statusMessage = "==================================\n" +
        std::string("        LEVEL UP! LEVEL ") + std::to_string(currentLevel) + "\n" +
        std::string("==================================\n") +
        hero.GetStats() + "\n" +
        std::string("Choose an upgrade:\n") +
        std::string("1. +10 Max Health (Current: ") + std::to_string(hero.GetMaxHealth()) + ")\n" +
        std::string("2. +5 Base Damage (Current: ") + std::to_string(hero.GetBaseDamage()) + ")\n" +
        std::string("3. Upgrade Random Spell (Spells: ") + std::to_string(hero.GetSpellCount()) + ")\n" +
        std::string("4. Add New Random Spell\n") +
        std::string("==================================");
    data.inputPrompt = "Enter your choice (1-4): ";
    visualizer->render(data);
}

void Game::ApplyPlayerUpgrade(int choice) {
    LOG_INFO("Applying player upgrade, choice: " + std::to_string(choice));

    GameRenderData data;

    switch (choice) {
    case 1:
        hero.UpgradeMaxHealth();
        data.statusMessage = "Max Health upgraded!";
        LOG_INFO("Player chose: +10 Max Health");
        break;
    case 2:
        hero.UpgradeBaseDamage();
        data.statusMessage = "Base Damage upgraded!";
        LOG_INFO("Player chose: +5 Base Damage");
        break;
    case 3:
        hero.UpgradeRandomSpell();
        data.statusMessage = "Random Spell upgraded!";
        LOG_INFO("Player chose: Upgrade Random Spell");
        break;
    case 4:
        if (hero.GetSpellCount() < 5) {
            hero.AddRandomSpell();
            data.statusMessage = "Spell arsenal upgraded! New spell added.";
            LOG_INFO("Player chose: Add New Random Spell");
        }
        else {
            data.statusMessage = "Spell book is full! Can't add more spells.";
            LOG_WARNING("Player tried to add spell but book is full");
        }
        break;
    default:
        data.statusMessage = "Invalid choice! No upgrade applied.";
        LOG_WARNING("Invalid upgrade choice: " + std::to_string(choice));
        break;
    }

    visualizer->renderWithMessage(GetRenderData(), data.statusMessage);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}

void Game::AdvanceToNextLevel() {
    LOG_INFO("Advancing to next level. Current level: " + std::to_string(currentLevel));

    if (!hasNextLevel()) {
        GameRenderData data;
        data.victory = true;
        data.statusMessage = "==================================\n" +
            std::string("     CONGRATULATIONS!\n") +
            std::string("  You completed all levels!\n") +
            std::string("==================================");
        visualizer->render(data);

        LOG_INFO("=== GAME COMPLETED ===");
        LOG_INFO("All levels completed!");
        LOG_INFO("Final level: " + std::to_string(currentLevel));
        LOG_INFO("Total enemies defeated: " + std::to_string(hero.GetDefeatedCount()));

        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        levelCompleted = true;
        gameRunning = false;
        return;
    }

    GameEventManager::getInstance().logLevelCompleted(currentLevel);
    LOG_INFO("Level " + std::to_string(currentLevel) + " completed successfully");

    ShowLevelUpMenu();

    int choice = 1;
    ApplyPlayerUpgrade(choice);

    currentLevel++;
    levelManager.AdvanceToNextLevel();

    LOG_INFO("Advancing to level " + std::to_string(currentLevel));

    InitializeLevel();

    GameRenderData data;
    data.statusMessage = "=== LEVEL " + std::to_string(currentLevel) + " ===\n" +
        levelManager.GetCurrentLevel().GetLevelName() +
        "\nGet ready for the next challenge!";
    visualizer->renderWithMessage(data, data.statusMessage);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

void Game::ProcessLevelUpChoice(const std::string& input) {
    LOG_INFO("Processing level up choice: " + input);

    if (input == "1" || input == "2" || input == "3" || input == "4") {
        int choice = std::stoi(input);
        ApplyPlayerUpgrade(choice);

        GameEventManager::getInstance().logLevelCompleted(currentLevel);

        currentLevel++;
        levelManager.AdvanceToNextLevel();

        LOG_INFO("Advanced to level " + std::to_string(currentLevel));

        InitializeLevel();

        GameRenderData data;
        data.statusMessage = "=== LEVEL " + std::to_string(currentLevel) + " ===\n" +
            levelManager.GetCurrentLevel().GetLevelName() +
            "\nGet ready for the next challenge!";
        visualizer->renderWithMessage(data, data.statusMessage);
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }
    else {
        LOG_WARNING("Invalid level up choice: " + input + ", using default upgrade");

        ApplyPlayerUpgrade(1);

        GameEventManager::getInstance().logLevelCompleted(currentLevel);

        currentLevel++;
        levelManager.AdvanceToNextLevel();

        LOG_INFO("Advanced to level " + std::to_string(currentLevel) + " with default upgrade");

        InitializeLevel();

        GameRenderData data;
        data.statusMessage = "Invalid choice! Default upgrade applied.\n" +
            std::string("=== LEVEL ") + std::to_string(currentLevel) + " ===\n" +
            levelManager.GetCurrentLevel().GetLevelName() +
            "\nGet ready for the next challenge!";
        visualizer->renderWithMessage(data, data.statusMessage);
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }
}
