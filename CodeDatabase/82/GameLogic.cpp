#include "GameLogic.h"
#include "MovementValidator.h"
#include "CombatSystem.h"
#include "SpellFactory.h"
#include <iostream>
#include <limits>

bool GameLogic::processPlayerAction(GameState& gameState, GameAction action) {
    Position direction(0, 0);
    bool isMovementAction = false;
    
    switch (action) {
        case GameAction::SWITCH_COMBAT_MODE:
            gameState.getPlayer().switchCombatMode();
            std::cout << "Режим боя изменен на: " << gameState.getPlayer().getCombatModeString() << std::endl;
            return true;
            
        case GameAction::RANGED_ATTACK:
            handleRangedAttack(gameState);
            return true;
            
        case GameAction::QUIT_GAME:
            gameState.setGameRunning(false);
            return true;

        case GameAction::CAST_SPELL_1:
            return handleSpellCast(gameState, 0);

        case GameAction::CAST_SPELL_2:
            return handleSpellCast(gameState, 1);
        
        case GameAction::CAST_SPELL_3:
            return handleSpellCast(gameState, 2);
            
        case GameAction::MOVE_UP:
        case GameAction::MOVE_DOWN:
        case GameAction::MOVE_LEFT:
        case GameAction::MOVE_RIGHT:
            direction = InputHandler().calculateMoveDirection(action);
            isMovementAction = true;
            break;
            
        case GameAction::INVALID_ACTION:
            std::cout << "Неверная команда! Используйте WASD для движения, F для атаки, SPACE для смены режима боя, 1-3 для заклинаний, G для сохранения, Q для выхода." << std::endl;
            return false;

        case GameAction::SAVE_GAME:
            return true;
    }
    
    if (isMovementAction) {
        handlePlayerMovement(gameState, direction);
        return true;
    }
    
    return false;
}

bool GameLogic::handleSpellCast(GameState& gameState, size_t spellIndex) {
    Player& player = gameState.getPlayer();
    
    if (spellIndex >= player.getSpellHand().getSpellCount()) {
        std::cout << "В этом слоте нет заклинания!" << std::endl;
        return false;
    }
    
    const Spell* spell = player.getSpellHand().getSpell(spellIndex);
    if (!spell) {
        std::cout << "Ошибка: заклинание не найдено!" << std::endl;
        return false;
    }
    
    std::cout << "Выбранное заклинание: " << spell->getName() << std::endl;
    std::cout << "Урон: " << spell->getDamage() << std::endl;
    std::cout << "Дальность: " << spell->getRange() << " клеток" << std::endl;
    std::cout << "Стоимость маны: " << spell->getManaCost() << std::endl;
    
    if (!player.canCastSpell(*spell)) {
        std::cout << "Недостаточно маны для заклинания " << spell->getName() << "!" << std::endl;
        std::cout << "Нужно: " << spell->getManaCost() << ", доступно: " << player.getMana() << std::endl;
        return false;
    }
    
    std::cout << "Введите координаты цели (x y): ";
    int targetX, targetY;
    std::cin >> targetX >> targetY;
    
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    Position target(targetX, targetY);
    
    if (!MovementValidator::isValidPosition(target, gameState.getGameField())) {
        std::cout << "Неверные координаты цели!" << std::endl;
        return false;
    }
    
    std::cout << "Ваша позиция: (" << player.getPosition().x << ", " << player.getPosition().y << ")" << std::endl;
    std::cout << "Цель: (" << target.x << ", " << target.y << ")" << std::endl;
    
    int distance = std::abs(player.getPosition().x - target.x) + std::abs(player.getPosition().y - target.y);
    std::cout << "Расстояние до цели: " << distance << " клеток" << std::endl;
    std::cout << "Максимальная дальность заклинания: " << spell->getRange() << " клеток" << std::endl;
    
    return processSpellCast(gameState, spellIndex, target);
}

void GameLogic::handlePlayerMovement(GameState& gameState, const Position& direction) {
    Position newPosition = gameState.getPlayer().getPosition() + direction;
    
    if (!isValidMovement(gameState, newPosition)) {
        std::cout << "Невозможно переместиться в эту клетку!" << std::endl;
        return;
    }
    
    auto& enemies = gameState.getEnemies();
    bool cellBlocked = false;
    
    for (const auto& enemy : enemies) {
        if (enemy.isAlive() && enemy.getPosition() == newPosition) {
            if (gameState.getPlayer().getCombatMode() == CombatMode::MELEE) {
                CombatSystem::performMeleeAttack(gameState.getPlayer(), newPosition, enemies);
            } else {
                std::cout << "Путь заблокирован врагом! Переключитесь на ближний бой для атаки." << std::endl;
            }
            cellBlocked = true;
            break;
        }
    }
    
    if (cellBlocked) {
        return;
    }
    
    gameState.setPlayerPosition(newPosition);
}

void GameLogic::handleRangedAttack(GameState& gameState) {
    if (!CombatSystem::performRangedAttack(
        gameState.getPlayer(),
        gameState.getEnemies(),
        gameState.getGameField()
    )) {
        std::cout << "Нет подходящих целей для дальней атаки!" << std::endl;
    }
}

void GameLogic::updateEnemies(GameState& gameState) {
    std::vector<std::vector<bool>> passableMap(
        gameState.getGameField().getHeight(),
        std::vector<bool>(gameState.getGameField().getWidth())
    );
    
    for (int y = 0; y < gameState.getGameField().getHeight(); ++y) {
        for (int x = 0; x < gameState.getGameField().getWidth(); ++x) {
            passableMap[y][x] = gameState.getGameField().isCellPassable(Position(x, y));
        }
    }

    auto& enemies = gameState.getEnemies();
    
    int defeatedCount = 0;
    
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            
            if (MovementValidator::arePositionsAdjacent(
                enemy.getPosition(), gameState.getPlayer().getPosition())) {
                CombatSystem::performEnemyAttack(enemy, gameState.getPlayer());
                std::cout << "Враг атаковал вас! Потеряно здоровья: " << enemy.getDamage() << std::endl;
                
                if (!gameState.getPlayer().isAlive()) {
                    gameState.setGameRunning(false);
                    std::cout << "Вы погибли!" << std::endl;
                    return; 
                }
            }

            handleEnemyMovement(gameState, enemy, enemies);
        }
    }

    for (const auto& enemy : enemies) {
        if (!enemy.isAlive()) {
            defeatedCount++;
        }
    }

    gameState.removeDeadEnemies();

    for (int i = 0; i < defeatedCount; ++i) {
        handleEnemyDefeated(gameState);
    }
}

void GameLogic::handleEnemyMovement(GameState& gameState, Enemy& enemy, std::vector<Enemy>& allEnemies) {
    std::vector<std::vector<bool>> passableMap(
        gameState.getGameField().getHeight(),
        std::vector<bool>(gameState.getGameField().getWidth())
    );
    
    for (int y = 0; y < gameState.getGameField().getHeight(); ++y) {
        for (int x = 0; x < gameState.getGameField().getWidth(); ++x) {
            passableMap[y][x] = gameState.getGameField().isCellPassable(Position(x, y));
        }
    }
    
    Position playerPos = gameState.getPlayer().getPosition();
    Position oldPos = enemy.getPosition();
    
    enemy.move(playerPos, passableMap, allEnemies);
    
    if (enemy.getPosition() == playerPos) {
        enemy.setPosition(oldPos);
    }
}

bool GameLogic::isValidMovement(const GameState& gameState, const Position& position) {
    return MovementValidator::isValidPosition(position, gameState.getGameField()) &&
           MovementValidator::isCellPassable(position, gameState.getGameField());
}

bool GameLogic::checkGameOver(const GameState& gameState) {
    return !gameState.getPlayer().isAlive();
}

bool GameLogic::processSpellCast(GameState& gameState, size_t spellIndex, const Position& target) {
    Player& player = gameState.getPlayer();
    const Spell* spell = player.getSpellHand().getSpell(spellIndex);
    
    if (!spell) {
        std::cout << "Ошибка: заклинание не найдено!" << std::endl;
        return false;
    }
    
    if (!player.canCastSpell(*spell)) {
        std::cout << "Недостаточно маны для заклинания " << spell->getName() << "!" << std::endl;
        return false;
    }
    
    if (!spell->canCast(player.getPosition(), target, gameState)) {
        std::cout << "Цель вне радиуса действия заклинания!" << std::endl;
        return false;
    }
    
    bool castSuccess = player.getSpellHand().castSpell(spellIndex, target, gameState, player);
    if (castSuccess) {
        player.spendMana(spell->getManaCost());
        std::cout << "Мана потрачена. Осталось: " << player.getMana() << "/" << player.getMaxMana() << std::endl;
    } else {
        std::cout << "Заклинание не сработало!" << std::endl;
    }
    
    return castSuccess;
}


void GameLogic::handleEnemyDefeated(GameState& gameState) {
    gameState.getPlayer().getSpellHand().onEnemyDefeated();
}