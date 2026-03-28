#include "game.h"
#include "ally.h"
#include "trap.h"
#include "directdamagespell.h"
#include "areadamagespell.h"
#include "trapspell.h"
#include "summonspell.h"
#include "enhancementspell.h"
#include "gamerenderer.h"
#include <iostream>
#include <random>
#include <sstream>

Game::Game(int fieldWidth, int fieldHeight, ControlConfig controlConfig, std::shared_ptr<GameLogSink> sink)
    : field(fieldWidth, fieldHeight),
      player(),
      gameRunning(true),
      turnCounter(0),
      playerActionTaken(false),
      enemiesKilledSinceLastSpell(0),
      baseFieldWidth(fieldWidth),
      baseFieldHeight(fieldHeight),
      currentLevelIndex(1),
      awaitingUpgradeSelection(false),
      awaitingRestartDecision(false),
      campaignActive(true),
      levelInProgress(false),
      saveFilePath("savegame.dat"),
      currentEnemyHealth(30),
      currentEnemyDamage(10),
	  controls(std::move(controlConfig)),
	  eventBus(std::move(sink)) {
    // Устанавливаем callback для уведомлений о спавне врагов от зданий
    // Сущности только информируют, Game решает, логировать ли
    buildingManager.setEnemySpawnCallback([this](int buildingX, int buildingY, int spawnX, int spawnY) {
        std::ostringstream ss;
        ss << "Building at (" << buildingX << ", " << buildingY 
           << ") spawned new enemy at (" << spawnX << ", " << spawnY << ")";
        logEvent(GameEventType::System, ss.str());
    });
    
    initializeSpells();
    startLevel(currentLevelIndex);
}

void Game::spawnPlayer() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(1, field.getWidth() - 2);
    std::uniform_int_distribution<> disY(1, field.getHeight() - 2);
    
    int x, y;
    do {
        x = disX(gen);
        y = disY(gen);
    } while (!field.isCellPassable(x, y) || buildingManager.isCellOccupiedByBuilding(x, y));
    
    player.setPosition(x, y);
}

void Game::processCellEffects(int x, int y) {
    if (field.isSlowCell(x, y)) {
        player.setSlowed(true);
        std::cout << "You stepped on a slow cell! You'll miss next turn.\n";
    }
}

void Game::displayHelp() const {
	auto keyOrFallback = [&](CommandName name, char fallback) {
		auto value = controls.keyFor(name);
		return value.value_or(fallback);
	};

    std::cout << "Controls:\n";
    std::cout << "  " << keyOrFallback(CommandName::MoveUp, 'w')
			  << "/" << keyOrFallback(CommandName::MoveLeft, 'a')
			  << "/" << keyOrFallback(CommandName::MoveDown, 's')
			  << "/" << keyOrFallback(CommandName::MoveRight, 'd') << " - move\n";
    std::cout << "  " << keyOrFallback(CommandName::SwitchMode, 'm') << " - switch combat mode\n";
    std::cout << "  " << keyOrFallback(CommandName::RangedAttack, 'f') << " - ranged attack (ranged mode only)\n";
    std::cout << "  " << keyOrFallback(CommandName::CastSpell, 'c') << " - cast spell\n";
    std::cout << "  " << keyOrFallback(CommandName::BuySpell, 'b') << " - buy spell (cost: " << SPELL_COST << " score)\n";
    std::cout << "  " << keyOrFallback(CommandName::Save, 'p') << " - save game\n";
    std::cout << "  " << keyOrFallback(CommandName::Load, 'o') << " - load game\n";
    std::cout << "  " << keyOrFallback(CommandName::ShowInfo, 'i') << " - show spell information and stats\n";
    std::cout << "  " << keyOrFallback(CommandName::Help, 'h') << " - show this help\n";
    std::cout << "  " << keyOrFallback(CommandName::Quit, 'q') << " - quit\n";
	std::cout << "Mapping source: " << (controls.isUsingDefault() ? "default layout" : "custom layout") << "\n";
    std::cout << "Get new spells by killing " << ENEMIES_PER_SPELL << " enemies or buying them.\n";
}

bool Game::isGameRunning() const {
    return gameRunning && player.isAlive();
}

void Game::displayField() const {
    GameRenderer renderer(*this);
    renderer.displayField(turnCounter);
}