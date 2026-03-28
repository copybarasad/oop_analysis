#include "GameLogic.h"
#include "Logger.h"
#include <iostream>
#include <filesystem>
#include <fstream>

GameLogic::GameLogic()
    : currentState(GameState::Menu),
      currentLevel(1),
      player_turn(true),
      running(true)
{
    EventManager::getInstance().notify(EventType::GameFlow, "GameLogic initialized");
}

GameLogic::~GameLogic() {
}

const Field* GameLogic::getField() const { return field.get(); }
Player* GameLogic::getPlayer() const { return player; }
GameState GameLogic::getState() const { return currentState; }
bool GameLogic::isRunning() const { return running; }
bool GameLogic::isPlayerTurn() const { return player_turn; }
int GameLogic::getCurrentLevel() const { return currentLevel; }

void GameLogic::exitGame() {
    running = false;
    EventManager::getInstance().notify(EventType::GameFlow, "Exit requested");
}

void GameLogic::startNewGame() {
    currentLevel = 1;

    Player* newPlayer = new Player(100, 25, 15, 3);
    newPlayer->getHand().addSpell(spellFactory.createRandomSpell());

    player = newPlayer;

    field = std::make_unique<Field>(25, 25);

    FieldContent content = field->generate_random_content(20, 30, 3, 5, 5, 25, 25, currentLevel);
    field->initialize(player, content);

    currentState = GameState::Playing;
    player_turn = true;
    EventManager::getInstance().notify(EventType::GameFlow, "New Game Started. Level 1");
}

void GameLogic::nextLevel() {
    currentLevel++;
    EventManager::getInstance().notify(EventType::GameFlow, "Transition to Level " + std::to_string(currentLevel));

    if (player->get_score() >= 40) {
        player->change_score(-40);
    }

    player->prepareForNextLevel();

    int rows = field->get_rows();
    int cols = field->get_cols();

    Player* playerCopy = new Player(*player);

    field = std::make_unique<Field>(rows, cols);
    player = playerCopy;

    int blocks = 10 + currentLevel * 2;
    int slows = 15 + currentLevel * 2;
    int buildings = 2 + (currentLevel / 2);
    int enemies = 3 + currentLevel;
    int towers = 3 + currentLevel;

    FieldContent content = field->generate_random_content(
        blocks, slows, buildings, enemies, towers, cols, rows, currentLevel
    );

    field->initialize(player, content);
    player_turn = true;
    currentState = GameState::Playing;
}

void GameLogic::upgradeHealth() {
    if (player) {
        player->upgradeMaxHealth(20);
        nextLevel();
    }
}

void GameLogic::upgradeDamage() {
    if (player) {
        player->upgradeDamage(5);
        nextLevel();
    }
}

void GameLogic::upgradeSpell() {
    if (player) {
        player->upgradeRandomSpellInHand();
        nextLevel();
    }
}

void GameLogic::movePlayer(sf::Vector2i dir) {
    if (currentState != GameState::Playing || !player_turn || !field || !player) return;

    if (player->is_slowed()) {
        player->decrement_slow();
        player_turn = false;
        EventManager::getInstance().notify(EventType::Movement, "Player is slowed, turn skipped");
        return;
    }

    if (field->move_player(dir)) {
        player_turn = false;
        EventManager::getInstance().notify(EventType::Movement, "Player moved");
    }
}

void GameLogic::castSpell(int targetX, int targetY, int spellIndex) {
    if (currentState != GameState::Playing || !player_turn || !field || !player) return;
    if (spellIndex < 0 || spellIndex >= player->getHand().getSpellCount()) return;

    ISpell* spell = player->getHand().getSpell(spellIndex);
    if (!spell) return;

    if (!field->is_valid_position({targetX, targetY})) return;

    if (!spell->isBuffSpell()) {
        player->applyBuffToSpell(spell);
    }

    if (spell->use(*player, *field, {targetX, targetY})) {
        if (!spell->isBuffSpell()) player->resetBuffCharges();
        player_turn = false;
        EventManager::getInstance().notify(EventType::Combat, "Spell cast: " + spell->getName());
    }
}

void GameLogic::buySpell() {
    if (currentState != GameState::Playing || !player) return;
    if (player->get_score() >= 50) {
        if(player->getHand().addSpell(spellFactory.createRandomSpell())) {
            player->change_score(-50);
            player_turn = false;
            EventManager::getInstance().notify(EventType::GameFlow, "Spell bought");
        }
    }
}

void GameLogic::toggleCombatMode() {
    if (player) player->toggle_combat_mode();
}

void GameLogic::returnToMenu() {
    currentState = GameState::Menu;
}

void GameLogic::continueGame() {
    if (field && player) currentState = GameState::Playing;
}

void GameLogic::saveGame(const std::string& filename) {
    try {
        std::ofstream ofs(filename, std::ios::binary);
        if (!ofs) throw std::runtime_error("Cannot open file for writing");

        int magic = 0xDEADBEEF;
        ofs.write(reinterpret_cast<const char*>(&magic), sizeof(magic));
        ofs.write(reinterpret_cast<const char*>(&player_turn), sizeof(player_turn));

        if(field) field->serialize(ofs);

        EventManager::getInstance().notify(EventType::GameFlow, "Game Saved");
    } catch (const std::exception& e) {
        EventManager::getInstance().notify(EventType::Error, std::string("Save Error: ") + e.what());
    }
}

void GameLogic::loadGame(const std::string& filename) {
    try {
        if (!std::filesystem::exists(filename)) {
            throw std::runtime_error("Save file not found");
        }

        std::ifstream ifs(filename, std::ios::binary);
        if (!ifs) throw std::runtime_error("Cannot open file");

        int magic;
        ifs.read(reinterpret_cast<char*>(&magic), sizeof(magic));
        if (magic != 0xDEADBEEF) throw std::runtime_error("Corrupted save file");

        ifs.read(reinterpret_cast<char*>(&player_turn), sizeof(player_turn));

        field = std::make_unique<Field>(25, 25);
        field->deserialize(ifs, spellFactory);
        player = field->getPlayer();

        if (!player) throw std::runtime_error("Player failed to load");

        currentState = GameState::Playing;
        EventManager::getInstance().notify(EventType::GameFlow, "Game Loaded");

    } catch (const std::exception& e) {
        EventManager::getInstance().notify(EventType::Error, std::string("Load Error: ") + e.what());
        currentState = GameState::Menu;
    }
}

void GameLogic::update() {
    if (currentState != GameState::Playing || !field || !player) return;

    if (!player_turn) {
        EventManager::getInstance().notify(EventType::GameFlow, "--- Enemy Turn ---");
        field->move_allies();
        field->move_enemies();
        field->process_buildings();
        field->process_towers();
        player->decrement_slow();

        if (player->getKillCount() >= 3) {
            player->resetKillCount();
            if(player->getHand().addSpell(spellFactory.createRandomSpell()))
                EventManager::getInstance().notify(EventType::Info, "Bonus Spell earned");
        }
        player_turn = true;
    }

    if (field->is_game_over()) {
        currentState = GameState::GameOver;
        EventManager::getInstance().notify(EventType::GameFlow, "Game Over");
    }
    else if (!field->has_buildings()) {
        currentState = GameState::LevelUpMenu;
        EventManager::getInstance().notify(EventType::GameFlow, "Level Complete (All buildings destroyed)");
    }
    else if (player->get_score() >= 40) {
        currentState = GameState::LevelUpMenu;
        EventManager::getInstance().notify(EventType::GameFlow, "Level Complete (Score limit reached)");
    }
}