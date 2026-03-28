#include "CombatSystem.h"
#include "../interface/ConsoleRenderer.h"
#include <sstream>
#include <iostream>

// Конструктор
CombatSystem::CombatSystem(GameState& state) : gameState(state) { }

// Ход и логика игрока
void CombatSystem::processPlayerCombat() {
    Player& player = gameState.getPlayer();

    // Првоеряются все возможные враги, если враг и игрок в одной клетке -> performAttack + проверка на ближний / дальний бой
    for (const auto& character : gameState.getAllCharacters()) {
        if (character != & player && character->isAlive()) {
            if (player.getCombatMode() == CombatMode::Melee) {
                if (player.getX() == character->getX() && player.getY() == character->getY()) {  
                    performAttack(character, "Enemy", 10, 15, "is died!");                    
                    break;
                }
            } else {    
                if (checkRangedAttack(character, "Enemy", 10, 15, "is died!")) { break; } 
            }
        } 
    } 

    // Првоеряются все возможные вражеские здания, если здание и игрок в одной клетке -> performAttack + проверка на ближний / дальний бой
    for (const auto& building : gameState.getAllBuildings()) {
        if (building->isAlive()) {
            if (player.getCombatMode() == CombatMode::Melee) {
                if (player.getX() == building->getX() && player.getY() == building->getY()) {
                    performAttack(building, "Enemy's Building", 20, 30, "is destroyed!");                    
                    break;
                }
            } else {
                if (checkRangedAttack(building, "Enemy's Building", 20, 30, "is destroyed!")) { break; }
            }
        } 
    }

    // Проверка на башни
    for (const auto& tower : gameState.getAllTowers()) {
        if (tower->isAlive()) {
            if (player.getCombatMode() == CombatMode::Melee) {
                if (player.getX() == tower->getX() && player.getY() == tower->getY()) {
                    performAttack(tower, "Enemy Tower", 25, 50, "is destroyed!");                    
                    break;
                }
            } else {
                if (checkRangedAttack(tower, "Enemy Tower", 25, 50, "is destroyed!")) { break; }
            }
        } 
    }

    // Проверка на союзников
    for (const auto& ally : gameState.getAllAllies()) {
        if (ally->isAlive()) {
            if (player.getX() == ally->getX() && player.getY() == ally->getY()) {
                player.revertToPreviousPosition();
            }
        }
    }
}

// Передвжиение соююзников
void CombatSystem::processAllyMovement() {
    for (auto& ally : gameState.getAllAllies()) {
        if (ally->isAlive()) {
            moveSingleAlly(ally);               // Двигаем каждого  
            processGameLogicAllyMove(ally);     // Логика хода союзника
        }
    }
}

// Передвижение одного союзника: идёт сначала до здания, потом до башни, потом за врагами
void CombatSystem::moveSingleAlly(Ally* ally) {
    Player& player = gameState.getPlayer();
    const GameField& field = gameState.getField();

    int prevX = ally->getX();
    int prevY = ally->getY();
    
    bool has_target = false;

    for (const auto& building : gameState.getAllBuildings()) {
        if (building->isAlive()) {
            ally->move(building->getX(), building->getY(), field);
            has_target = true;
            return;
        } else {
            has_target = false;
        }
    } 

    if (!has_target) {
        for (const auto& tower : gameState.getAllTowers()) {
            if (tower->isAlive()) {
                ally->move(tower->getX(), tower->getY(), field);
                has_target = true;
                return;
            } else {
                has_target = false;
            }
        } 
    }

    if (!has_target) {
        for (const auto& character : gameState.getAllCharacters()) {
            if (character->isAlive() && character != &player) {
                ally->move(character->getX(), character->getY(), field);
                has_target = true;
                return;
            } 
        }
    }
    
}

// Логика хода союзника
void CombatSystem::processGameLogicAllyMove(Ally* ally) {
    Player& player = gameState.getPlayer();
    bool is_fight = false;

    for (const auto& character : gameState.getAllCharacters()) {
        if (character->isAlive() && character != &player) {
            if (ally->getX() == character->getX() && ally->getY() == character->getY()) {  
                handleAllyAttack(character, "Enemy", "is died!", ally);     // Боёвка + логирование атаки
                is_fight = true;               
                break;
            }
        } 
    } 

    for (const auto& building : gameState.getAllBuildings()) {
        if (building->isAlive()) {
            if (ally->getX() == building->getX() && ally->getY() == building->getY()) {
                handleAllyAttack(building, "Enemy's Building", "is destroyed!", ally);     // Боёвка + логирование атаки
                is_fight = true;                 
                break;
            }
            
        } 
    }

    for (const auto& tower : gameState.getAllTowers()) {
        if (tower->isAlive()) {
            if (ally->getX() == tower->getX() && ally->getY() == tower->getY()) {
                handleAllyAttack(tower, "Enemy Tower", "is destroyed!", ally);    // Боёвка + логирование атаки      
                is_fight = true;             
                break;
            }
        } 
    }

    for (const auto& other : gameState.getAllAllies()) {
        if (ally != other && other->isAlive() && ally->getX() == other->getX() && ally->getY() == other->getY()) {
            ally->revertToPreviousPosition();   // Если свой - просто отходим обратно
            break;
        } 
    }

    if (ally->isAlive() && ally->getX() == player.getX() && ally->getY() == player.getY()) {
        ally->revertToPreviousPosition();   // Если свой - просто отходим обратно
    }

    if (!is_fight) handleAllyMovementResult(ally);  // Иначе если не было сражения, то логируем ход союзника
}

void CombatSystem::handleAllyMovementResult(Ally* ally) {
    std::ostringstream ss;
    if ((ally->getPrevX() != ally->getX() || ally->getPrevY() != ally->getY()) && ally->isAlive()) {
        ss << Colors::GRAY 
            << "Ally moved from (" << ally->getPrevX() << ", " << ally->getPrevY() 
        << ") to (" << ally->getX() << ", " << ally->getY() << ")"
            << Colors::RESET;
    } else if (ally->isAlive()) {
        ss << Colors::GRAY << "Ally stayed on the same position: ("
        << ally->getX() << ", " << ally->getY() << ")" << Colors::RESET;
    }

    gameState.addMessage(ss.str());
}




// Передвижение врагов
void CombatSystem::processEnemyMovement() {
    Player& player = gameState.getPlayer();
    for (auto& character : gameState.getAllCharacters()) {
        if (character != &player && character->isAlive()) {
            moveSingleEnemy(character);              // Двигаем каждого врага
            processGameLogicEnemyMove(character);    // Логика хода врага
        }
    }
}

// Двигаем каждого врага
void CombatSystem::moveSingleEnemy(Character* enemy) {
    Player& player = gameState.getPlayer();
    const GameField& field = gameState.getField();

    int prevX = enemy->getX();
    int prevY = enemy->getY();
    
    enemy->move(player.getX(), player.getY(), field);
    
    // Проверяем, не занята ли новая клетка другим персонажем или зданием
    if (isEnemyCellOccupied(enemy)) {
        enemy->revertToPreviousPosition();  // Если клетка занята, возвращаем врага на предыдущую позицию
    }
    checkTrapAndApplyDamage(enemy);         // Если клетка - ловушка, применяем
}

// Проверка на ловушку
void CombatSystem::checkTrapAndApplyDamage(Character* enemy) {
    const GameField& field = gameState.getField();
    int x = enemy->getX();
    int y = enemy->getY();

    const Cell& cell = gameState.getField().getCell(x, y);
    if (cell.getHasTrap()) {
        gameState.setCellTrap(x, y, false);            // одноразовая ловушка
        int trapDamage = gameState.getLastTrapDamage();  
        enemy->takeDamage(trapDamage);
        
        std::ostringstream ss;
        if (!enemy->isAlive()) {
            ss << Colors::GREEN << "Enemy was killed by the Trap Spell!" << Colors::RESET;
        } else {
            ss << Colors::ORANGE << "Enemy stepped on a trap at (" << x << ", " << y << ") and took " 
            << trapDamage << " damage! Enemy health: " << enemy->getHealth() << Colors::RESET;
        }

        gameState.addMessage(ss.str());
    }
}

// Занята ли уже другим врагом клетка
bool CombatSystem::isEnemyCellOccupied(Character* enemy) const {
    Player& player = gameState.getPlayer();
    for (auto& other : gameState.getAllCharacters()) {
        if (other != &player && other != enemy && other->isAlive() && 
            enemy->getX() == other->getX() && enemy->getY() == other->getY()) {
            return true;
        }
    }
    for (const auto& building : gameState.getAllBuildings()) {
        if (building->isAlive() && enemy->getX() == building->getX() && enemy->getY() == building->getY()) {
            return true;
        }
    }
    for (const auto& tower : gameState.getAllTowers()) {
        if (tower->isAlive() && enemy->getX() == tower->getX() && enemy->getY() == tower->getY()) {
            return true;
        }
    }
    return false;
}

// Логика хода врага
void CombatSystem::processGameLogicEnemyMove(Character* enemy) {
    Player& player = gameState.getPlayer();
    if (player.getX() == enemy->getX() && player.getY() == enemy->getY()) {
        handleEnemyAttack(enemy);  // Если атакует игрока -> лоигурем атаку
    } else if (isAllyInCell(enemy->getX(), enemy->getY())) {
        enemyAttackAlly(enemy);  // Если атакует союзника -> лоигурем атаку
    } else {
        handleEnemyMovementResult(enemy);  // иначе логируем реузльтат движения
    }
}

// Есть ли союзник в клетке
bool CombatSystem::isAllyInCell(int x, int y) {
    for (const auto& ally : gameState.getAllAllies()) {
        if (ally->isAlive() && ally->getX() == x && ally->getY() == y) {
            return true;
        }
    }
    return false;
}

// Враг атакует союзника
void CombatSystem::enemyAttackAlly(Character* enemy) {
    for (const auto& ally : gameState.getAllAllies()) {
        if (ally->isAlive() && ally->getX() == enemy->getX() && ally->getY() == enemy->getY()) {
            enemy->revertToPreviousPosition();
            ally->takeDamage(enemy->getDamage());
            
            std::ostringstream ss;
            if (!ally->isAlive()) {
                ss << Colors::RED << "Enemy killed Ally!" << Colors::RESET;
            } else {
                ss << Colors::RED << "Enemy attacks Ally! Ally health: " << ally->getHealth() << Colors::RESET;
            }
            gameState.addMessage(ss.str());
            return;
        }
    }
}

// Логирование атаки врага
void CombatSystem::handleEnemyAttack(Character* enemy) {
    Player& player = gameState.getPlayer();
    enemy->revertToPreviousPosition();
    player.takeDamage(enemy->getDamage());
        
    // Вывод информации о событиях
    std::ostringstream ss;
    ss << Colors::RED
        << "Enemy attacks Player! Player health: " << player.getHealth()
        << Colors::RESET;
   gameState.addMessage(ss.str());
}

// Логирвоание хода врага
void CombatSystem::handleEnemyMovementResult(Character* enemy) {
    std::ostringstream ss;
    if ((enemy->getPrevX() != enemy->getX() || enemy->getPrevY() != enemy->getY()) && enemy->isAlive()) {
        ss << Colors::GRAY 
            << "Enemy moved from (" << enemy->getPrevX() << ", " << enemy->getPrevY() 
        << ") to (" << enemy->getX() << ", " << enemy->getY() << ")"
            << Colors::RESET;
    } else if (enemy->isAlive()) {
        ss << Colors::GRAY << "Enemy stayed on the same position: ("
        << enemy->getX() << ", " << enemy->getY() << ")" << Colors::RESET;
    }

    gameState.addMessage(ss.str());
}




// Выполнить атаку (т.е. если игрок и враг / здание на одной клетке -> отктатить игрока обратно, враг / здание - нанести урон, добавить очки)
template<typename T>
void CombatSystem::performAttack(T* target, const std::string& targetName, int attackScore, int deathScore, const std::string& deathMessage) {
    Player& player = gameState.getPlayer();

    player.revertToPreviousPosition();
    target->takeDamage(player.getCurrentDamage());
    player.addScore(attackScore);

    std::ostringstream ss;

    if (!target->isAlive()) {
        player.addScore(deathScore);
        ss << Colors::GREEN << "\n" << targetName << " " << deathMessage << " " << Colors::RESET;
    } else {
        ss << Colors::ORANGE
        << "Player attacks " << targetName << "! " << targetName << " health: " << target->getHealth()
        << Colors::RESET;
    }
    
    gameState.addMessage(ss.str());
}

// Првоерка на дальнию атаку (если игрок и атакуемый обьект находятся на расстояние 1 клетки -> performAttack)
template<typename T>
bool CombatSystem::checkRangedAttack(T* target, const std::string& targetName, int attackScore, int deathScore, const std::string& deathMessage) {
    Player& player = gameState.getPlayer();

    // Если при дальнем бое игрок вступает на одну клетку с врагом (как при ближнем) -> откат игрока, урон не наносится
    if (player.getX() == target->getX() && player.getY() == target->getY()) { 
        player.revertToPreviousPosition(); 
        return false;
    }

    int playerPrevX = player.getPrevX();
    int playerPrevY = player.getPrevY();
    int playerX = player.getX();
    int playerY = player.getY();
    
    int moveX = playerX - playerPrevX;
    int moveY = playerY - playerPrevY;

    bool canAttack = false;

    if (moveX > 0) { // Движение вправо
        canAttack = (target->getY() == playerY) && (target->getX() == playerX + 1);
    } else if (moveX < 0) { // Движение влево
        canAttack = (target->getY() == playerY) && (target->getX() == playerX - 1);
    } else if (moveY > 0) { // Движение вниз
        canAttack = (target->getX() == playerX) && (target->getY() == playerY + 1);
    } else if (moveY < 0) { // Движение вверх
        canAttack = (target->getX() == playerX) && (target->getY() == playerY - 1);
    }

    if (canAttack) {
        performAttack(target, targetName, attackScore, deathScore, deathMessage);
        return true;
    }

    return false;
}

// Атака союзника
template<typename T>
void CombatSystem::handleAllyAttack(T* target, const std::string& targetName, const std::string& deathMessage, Ally* ally) {
    ally->revertToPreviousPosition();
    target->takeDamage(ally->getDamage());
        
    std::ostringstream ss;
    if (!target->isAlive()) {
        ss << Colors::GREEN << "\n" << targetName << " " << deathMessage << " " << Colors::RESET;
    } else {
        ss << Colors::ORANGE
        << "Ally attacks " << targetName << "! " << targetName << " health: " << target->getHealth()
        << Colors::RESET;
    }

   gameState.addMessage(ss.str());
}

