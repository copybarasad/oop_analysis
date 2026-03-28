#include "game.hpp"

#include "sys/ai_sys.hpp"
#include "sys/move_sys.hpp"
#include "sys/fight_sys.hpp"
#include "game_objects/spells/damage_spell.hpp"
#include "game_objects/spells/aoe_spell.hpp"
#include "game_objects/spells/trap_spell.hpp"

#include <iostream>
#include <algorithm>

Game::Game(int startLevel) : gameState() {
    levelManager.createLevel(gameState.getEntityManager(), startLevel);
}

void Game::processCommand(GameCommand& cmd) {
    if (gameState.getTurnCounter() % 7 == 0) {
        gameState.getEntityManager().GiveRandomSpellToPlayer();
    }

    auto& manager = gameState.getEntityManager();
    
    CLIRenderer& uiRenderer = gameRenderer.getRenderer();
    switch (cmd.action) {
        case GameAction::MOVE: {
            int dx = 0, dy = 0;
            if (cmd.dir == MoveDir::UP) dy = -1;
            else if (cmd.dir == MoveDir::DOWN) dy = 1;
            else if (cmd.dir == MoveDir::LEFT) dx = -1;
            else if (cmd.dir == MoveDir::RIGHT) dx = 1;
            
            auto free_cells = manager.getMap()->freeCells();
            auto positions = manager.getPositions();
            Mover::Move(manager.getPlayer(), dx, dy, free_cells, positions);
            Fighter::HandAttack(*manager.getPlayer(), manager);
            break;
        }
            
        case GameAction::CAST_SPELL: {
            SpellHand* hand = manager.getPlayerHand();
            if (!hand || hand->getSize() == 0) {
                uiRenderer.renderMessage("Рука пуста!");
                break;
            }
            
            if (cmd.spellIndex.has_value()) {
                size_t idx = cmd.spellIndex.value();
                SpellCard* spell = hand->getSpell(idx);
                if (!spell) {
                    uiRenderer.renderMessage("Некорректный индекс!");
                    break;
                }
                
                Entity* player = manager.getPlayer();
                bool applied = false;
                
                if (auto dmgSpell = dynamic_cast<DirectDamageSpell*>(spell)) {
                    applied = Fighter::CastDirectDamage(*player, manager, *dmgSpell);
                } else if (auto aoeSpell = dynamic_cast<AoeDamageSpell*>(spell)) {
                    if (cmd.targetX.has_value() && cmd.targetY.has_value()) {
                        applied = Fighter::CastAoeDamage(*player, manager, *aoeSpell, 
                                                        cmd.targetX.value(), cmd.targetY.value());
                    } else {
                        uiRenderer.renderMessage("Для этого заклинания нужны координаты!");
                        uiRenderer.renderMessage("Введите верхний левый угол квадрата 2x2 (x y): ");
                        int x, y;
                        std::cin >> x >> y;
                        applied = Fighter::CastAoeDamage(*player, manager, *aoeSpell, x, y);
                    }
                } else if (auto trapSpell = dynamic_cast<TrapSpell*>(spell)) {
                    if (cmd.targetX.has_value() && cmd.targetY.has_value()) {
                        applied = Fighter::CastTrap(*player, manager, *trapSpell, 
                                                   cmd.targetX.value(), cmd.targetY.value());
                    } else {
                        uiRenderer.renderMessage("Для этого заклинания нужны координаты!");
                        uiRenderer.renderMessage("Введите верхний левый угол квадрата 2x2 (x y): ");
                        int x, y;
                        std::cin >> x >> y;
                        applied = Fighter::CastTrap(*player, manager, *trapSpell, x, y);
                    }
                }
                
                if (applied) {
                    hand->removeSpell(idx);
                }
            } else {
                hand->listSpells();
                uiRenderer.renderMessage("Выберите заклинание по индексу: ");
                size_t idx;
                std::cin >> idx;
                
                SpellCard* spell = hand->getSpell(idx);
                if (!spell) {
                    uiRenderer.renderError("Некорректный индекс!");
                    break;
                }
                
                Entity* player = manager.getPlayer();
                bool applied = false;
                
                if (auto dmgSpell = dynamic_cast<DirectDamageSpell*>(spell)) {
                    applied = Fighter::CastDirectDamage(*player, manager, *dmgSpell);
                } else if (auto aoeSpell = dynamic_cast<AoeDamageSpell*>(spell)) {
                    int x, y;
                    uiRenderer.renderMessage("Введите координаты для установки ловушки (x y): ");
                    std::cin >> x >> y;
                    applied = Fighter::CastAoeDamage(*player, manager, *aoeSpell, x, y);
                } else if (auto trapSpell = dynamic_cast<TrapSpell*>(spell)) {
                    int x, y;
                    uiRenderer.renderMessage("Введите координаты для установки ловушки (x y): ");
                    std::cin >> x >> y;
                    applied = Fighter::CastTrap(*player, manager, *trapSpell, x, y);
                }
                
                if (applied) {
                    hand->removeSpell(idx);
                }
            }
            break;
        }
            
        case GameAction::PAUSE:
            gameState.setStatus(GameStatus::Paused);
            break;
            
        default:
            break;
    }
}

void Game::processEnemiesTurn() {
    AISystem::EnemyTurn(gameState.getEntityManager());
}

void Game::processTowerTurn() {
    AISystem::TowerTurn(gameState.getEntityManager(), gameState.getTurnCounter());
}

void Game::checkGameState() {
    auto& manager = gameState.getEntityManager();
    
    if (!manager.getPlayer()->isAlive()) {
        gameState.setStatus(GameStatus::Lose);
        return;
    }

    bool anyEnemyAlive = false;
    for (auto e : manager.getEnemies()) {
        if (e->isAlive()) {
            anyEnemyAlive = true;
            break;
        }
    }

    if (!anyEnemyAlive && !manager.getTower()->isAlive()) {
        gameState.setStatus(GameStatus::Win);
    }
}

bool Game::proceedToNextLevel() {
    if (gameState.getStatus() == GameStatus::Win) {
        levelManager.nextLevel();
        int nextLevel = levelManager.getCurrentLevel();
        
        levelManager.createLevel(gameState.getEntityManager(), nextLevel);
        
        gameState.setStatus(GameStatus::Running);
        gameRenderer.getRenderer().renderNextLevel(nextLevel);
        return true;
    }
    return false;
}

GameStatus Game::run(TerminalInputHandler& input) {
    gameRenderer.getRenderer().renderMessage("Игра началась!");
    gameRenderer.render(gameState.getEntityManager());

    while (gameState.isRunning()) {
        auto cmd = input.getNextCommand(GameMode::IN_GAME);
        if (auto* gameCmd = dynamic_cast<GameCommand*>(cmd.get())) {
            processCommand(*gameCmd);
        }
        if (!gameState.isRunning()) break;
        
        processEnemiesTurn();
        if (!gameState.isRunning()) break;
        
        processTowerTurn();
        if (!gameState.isRunning()) break;
        
        checkGameState();
        if (!gameState.isRunning()) break;

        gameState.incrementTurnCounter();
        gameRenderer.render(gameState.getEntityManager());
    }

    gameState.setTurnCounter(0);
    return gameState.getStatus();
}

GameState& Game::getGameState() {
    return gameState;
}

const GameState& Game::getGameState() const {
    return gameState;
}
