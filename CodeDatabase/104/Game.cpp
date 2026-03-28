#include "Game.h"
#include "GameSaveManager.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "MeleeSpell.h"
#include <iostream>
#include <random>

// Конструктор по умолчанию - начинает с уровня 1
Game::Game() : current_level_(1), game_running_(true), game_won_(false) {
    player_upgrade_ = std::make_unique<PlayerUpgrade>();
    save_manager_ = std::make_unique<GameSaveManager>();
    renderer_ = std::make_unique<GameRenderer>();
    input_handler_ = std::make_unique<InputHandler>();
    InitializeLevel(1);
}

// Конструктор для загрузки - начинает с указанного уровня
Game::Game(int startLevel) : current_level_(startLevel), game_running_(true), game_won_(false) {
    player_upgrade_ = std::make_unique<PlayerUpgrade>();
    save_manager_ = std::make_unique<GameSaveManager>();
    renderer_ = std::make_unique<GameRenderer>();
    input_handler_ = std::make_unique<InputHandler>();
    InitializeLevel(startLevel);
}

// Инициализация уровня
void Game::InitializeLevel(int level) {
    int field_size = 10 + level * 2;
    field_ = std::make_unique<GameField>(field_size, field_size, true);

    if (!player_) {
        // Создание нового игрока со всеми заклинаниями
        player_ = std::make_unique<Player>(field_size / 2, field_size / 2, 100, 50 + level * 10);

        player_->getHand().addSpell(std::make_unique<DirectDamageSpell>(25, 3, 15));
        player_->getHand().addSpell(std::make_unique<AreaDamageSpell>(20, 2, 20));
        player_->getHand().addSpell(std::make_unique<TrapSpell>(30, 5));
        player_->getHand().addSpell(std::make_unique<MeleeSpell>(35, 25));
    }
    else {
        // Перемещение и восстановление существующего игрока
        player_->setPosition(field_size / 2, field_size / 2);
        player_->restoreHealthToMax();
        player_->restoreManaToMax();
    }

    field_->setPlayer(player_.get());
    SpawnEnemiesForLevel(level);

    if (level >= 2) {
        SpawnTowersForLevel(level);
    }
}

// Запуск игры и отображение инструкций
void Game::StartGame() {
    std::cout << "=== MAGIC BATTLE ===\n";
    std::cout << "Defeat all enemies on 3 levels to win!\n";
    std::cout << "Commands:\n";
    std::cout << "  W - move up\n";
    std::cout << "  A - move left\n";
    std::cout << "  S - move down\n";
    std::cout << "  D - move right\n";
    std::cout << "  cast [number] - cast spell\n";
    std::cout << "  save - save game\n";
    std::cout << "  load - load game (restarts with saved state)\n";
    std::cout << "  quit - exit game\n";
    std::cout << "Map Legend: P=Player, E=Enemy, B=Tower, T=Trap, #=Obstacle\n";
    std::cout << "You restore 5 mana every turn!\n";
}

// Основной игровой цикл
void Game::ProcessGameLoop() {
    while (IsGameRunning()) {
        renderGameState();

        // Ход игрока
        std::cout << "\nEnter command: ";
        std::string input;
        std::getline(std::cin, input);

        CommandResult result = processPlayerTurn(input);

        if (result == CommandResult::RESTART_WITH_LOAD) {
            try {
                auto loadedGame = save_manager_->loadGame("autosave.txt");
                // Перемещаем состояние из загруженной игры в текущую
                this->player_ = std::move(loadedGame->player_);
                this->field_ = std::move(loadedGame->field_);
                this->current_level_ = loadedGame->getCurrentLevel();
                this->field_->setPlayer(this->player_.get());
                std::cout << "Game loaded successfully!\n";
                continue; // Продолжаем цикл с загруженной игрой
            }
            catch (const GameSaveManager::SaveException& e) {
                std::cout << "Load failed: " << e.what() << "\n";
            }
        }
        else if (result == CommandResult::QUIT) {
            game_running_ = false;
            std::cout << "Exiting game...\n";
            break;
        }

        if (!player_->isAlive()) {
            processGameOver();
            break;
        }

        // Ход врагов
        processEnemiesTurn();

        if (!game_won_) {
            CheckLevelCompletion();
        }
    }

    if (game_won_) {
        std::cout << "\n=== YOU WIN! ===\n";
        std::cout << "Congratulations! You defeated all levels!\n";
    }
}

// Проверка активности игры
bool Game::IsGameRunning() const {
    return game_running_ && player_->isAlive() && !game_won_;
}

// Обработка ввода игрока
Game::CommandResult Game::ProcessInput(const std::string& input) {
    return processPlayerTurn(input);
}

// Обработка хода игрока
Game::CommandResult Game::processPlayerTurn(const std::string& input) {
    if (input == "save") {
        try {
            save_manager_->saveGame(*this, "autosave.txt");
            std::cout << "Game saved!\n";
        }
        catch (const GameSaveManager::SaveException& e) {
            std::cout << "Save error: " << e.what() << "\n";
        }
        return CommandResult::CONTINUE;
    }
    else if (input == "load") {
        std::cout << "Loading game...\n";
        return CommandResult::RESTART_WITH_LOAD;
    }

    InputHandler::PlayerInput parsedInput = input_handler_->parseInput(input);

    switch (parsedInput.type) {
    case InputHandler::PlayerInput::Type::MOVE: {
        Position currentPos = player_->getPosition();
        int newX = currentPos.getX() + parsedInput.move_dx;
        int newY = currentPos.getY() + parsedInput.move_dy;

        if (player_->move(newX, newY, *field_)) {
            std::cout << "Move successful!\n";
        }
        else {
            std::cout << "Cannot move there!\n";
        }
        break;
    }

    case InputHandler::PlayerInput::Type::CAST_SPELL: {
        SpellCard* spell = player_->getHand().getSpell(parsedInput.spell_index);
        if (dynamic_cast<MeleeSpell*>(spell)) {
            SpellCastInfo cast_info(Position(0, 0));
            if (player_->castSpell(parsedInput.spell_index, *field_, cast_info)) {
                std::cout << "Melee spell cast successfully around you!\n";
            }
            else {
                std::cout << "Failed to cast melee spell!\n";
            }
        }
        else {
            // Обычные заклинания требуют выбора цели
            SpellCastInfo cast_info = input_handler_->getSpellTarget(*player_, *field_);
            if (player_->castSpell(parsedInput.spell_index, *field_, cast_info)) {
                std::cout << "Spell cast successfully!\n";
            }
            else {
                std::cout << "Failed to cast spell!\n";
            }
        }
        break;
    }

    case InputHandler::PlayerInput::Type::QUIT:
        return CommandResult::QUIT;

    case InputHandler::PlayerInput::Type::INVALID:
        std::cout << "Invalid command! Try again.\n";
        break;
    }

    player_->restoreMana(5);
    std::cout << "+5 mana restored!\n";
    return CommandResult::CONTINUE;
}

// Обработка проигрыша
void Game::processGameOver() {
    std::cout << "\n=== GAME OVER ===\n";
    std::cout << "You have been defeated!\n";

    std::cout << "Choose an option:\n";
    std::cout << "1. Restart game\n";
    std::cout << "2. Quit\n";

    std::string choice;
    std::getline(std::cin, choice);

    if (choice == "1") {
        // Перезапуск игры
        current_level_ = 1;
        game_running_ = true;
        game_won_ = false;
        InitializeLevel(1);
        std::cout << "Game restarted!\n";
    }
    else {
        game_running_ = false;
        std::cout << "Thanks for playing!\n";
    }
}

// Создание врагов для уровня
void Game::SpawnEnemiesForLevel(int level) {
    int enemy_count = 3;
    if (level == 2) enemy_count = 5;
    else if (level == 3) enemy_count = 8;

    // Усиление врагов с каждым уровнем
    int base_health = 30 + (level - 1) * 15;
    int base_damage = 15 + (level - 1) * 10;

    field_->spawnEnemies(enemy_count, base_health, base_damage);
}

// Создание башен для уровня
void Game::SpawnTowersForLevel(int level) {
    int tower_count = level - 1;
    field_->spawnTowers(tower_count);
}

// Проверка завершения уровня
void Game::CheckLevelCompletion() {
    if (field_->areAllEnemiesDefeated()) {
        if (current_level_ < 3) {
            std::cout << "\n=== LEVEL " << current_level_ << " COMPLETED! ===\n";
            removeHalfOfSpells();

            // Выбор замены удаленных заклинаний
            if (player_->getHand().getSpellCount() < 4) {
                showSpellReplacementMenu();
            }

            // Прокачка характеристик
            applyLevelUpgrades();

            current_level_++;
            InitializeLevel(current_level_);
            std::cout << "Starting Level " << current_level_ << "...\n";
        }
        else {
            game_won_ = true;
        }
    }
}

// Удаление половины заклинаний
void Game::removeHalfOfSpells() {
    PlayerHand& hand = player_->getHand();
    size_t currentSpellCount = hand.getSpellCount();

    if (currentSpellCount <= 1) {
        return;
    }

    size_t spellsToKeep = (currentSpellCount + 1) / 2;
    size_t spellsToRemove = currentSpellCount - spellsToKeep;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::cout << "Removing " << spellsToRemove << " spell(s) randomly...\n";

    for (size_t i = 0; i < spellsToRemove; ++i) {
        if (hand.getSpellCount() > spellsToKeep) {
            std::uniform_int_distribution<size_t> dist(0, hand.getSpellCount() - 1);
            size_t indexToRemove = dist(gen);
            hand.removeSpell(indexToRemove);
            std::cout << "Spell " << (indexToRemove + 1) << " removed.\n";
        }
    }
}

// Меню выбора замены заклинаний
void Game::showSpellReplacementMenu() {
    std::cout << "\n=== CHOOSE A SPELL TO REPLACE LOST ONE ===\n";

    std::vector<std::string> missingSpells = getMissingSpells();

    for (size_t i = 0; i < missingSpells.size(); ++i) {
        std::cout << (i + 1) << ". " << missingSpells[i] << "\n";
    }

    int choice;
    std::cout << "Enter your choice (1-" << missingSpells.size() << "): ";
    std::cin >> choice;
    std::cin.ignore();

    if (choice >= 1 && choice <= static_cast<int>(missingSpells.size())) {
        addSpellByName(missingSpells[choice - 1]);
        std::cout << missingSpells[choice - 1] << " added to your hand!\n";
    }
    else {
        std::cout << "Invalid choice. No spell added.\n";
    }
}

// Получение списка отсутствующих заклинаний
std::vector<std::string> Game::getMissingSpells() const {
    std::vector<std::string> allSpells = { "Direct Damage", "Area Damage", "Trap", "Melee" };
    std::vector<std::string> missingSpells;

    const PlayerHand& hand = player_->getHand();
    bool hasDirect = false, hasArea = false, hasTrap = false, hasMelee = false;

    for (size_t i = 0; i < hand.getSpellCount(); ++i) {
        SpellCard* spell = hand.getSpell(i);
        if (dynamic_cast<DirectDamageSpell*>(spell)) hasDirect = true;
        else if (dynamic_cast<AreaDamageSpell*>(spell)) hasArea = true;
        else if (dynamic_cast<TrapSpell*>(spell)) hasTrap = true;
        else if (dynamic_cast<MeleeSpell*>(spell)) hasMelee = true;
    }

    if (!hasDirect) missingSpells.push_back("Direct Damage");
    if (!hasArea) missingSpells.push_back("Area Damage");
    if (!hasTrap) missingSpells.push_back("Trap");
    if (!hasMelee) missingSpells.push_back("Melee");

    return missingSpells;
}

// Добавление заклинания по имени
void Game::addSpellByName(const std::string& spellName) {
    if (spellName == "Direct Damage") {
        player_->getHand().addSpell(std::make_unique<DirectDamageSpell>(25, 3, 15));
    }
    else if (spellName == "Area Damage") {
        player_->getHand().addSpell(std::make_unique<AreaDamageSpell>(20, 2, 20));
    }
    else if (spellName == "Trap") {
        player_->getHand().addSpell(std::make_unique<TrapSpell>(30, 5));
    }
    else if (spellName == "Melee") {
        player_->getHand().addSpell(std::make_unique<MeleeSpell>(35, 25));
    }
}

// Применение улучшений уровня
void Game::applyLevelUpgrades() {
    player_upgrade_->showUpgradeMenu(*player_);
}

// Отрисовка состояния игры
void Game::renderGameState() {
    renderer_->renderGameState(*field_, *player_);
    renderer_->renderPlayerStats(*player_);
    renderer_->renderPlayerHand(*player_);
    std::cout << "Current Level: " << current_level_ << "\n";
    std::cout << "Enemies remaining: " << field_->getEnemyCount() << "\n";

    if (current_level_ >= 2) {
        std::cout << "Towers remaining: " << field_->getTowerCount() << "\n";
    }
}

void Game::processEnemiesTurn() {
    field_->updateEnemies();
}
