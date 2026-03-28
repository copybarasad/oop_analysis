#include "InputHandler.hpp"
#include <conio.h>
#include <thread>
#include <chrono>

InputHandler::InputHandler(GameInterface* interface) 
    : gameInterface(interface) {}

bool InputHandler::handleMovement(GameState& state, GamePhase& currentPhase, bool& gameRunning) {
    char input = _getch();
    Direction dir = Direction::None;
    
    switch (tolower(input)) {
        case 'w': dir = Direction::Up; break;
        case 's': dir = Direction::Down; break;
        case 'a': dir = Direction::Left; break;
        case 'd': dir = Direction::Right; break;
        case 'q': 
            gameRunning = false;
            return true;
        case 'r':
            state.resetForNewLevel(state.currentLevel);
            gameInterface->showMessage("Уровень перезагружен!");
            return true;
        case 't':
            state.player.switchAttackType();
            {
                std::string attackType = (state.player.getAttackType() == AttackType::Melee) ? "Ближний" : "Дальний";
                gameInterface->showMessage("Тип атаки изменен на: " + attackType);
            }
            return true;
        case 'f':
            if (state.player.getAttackType() == AttackType::Ranged) {
                return handleRangedAttack(state);
            } else {
                gameInterface->showMessage("Сначала переключитесь на дальний тип атаки (T)!");
                return false;
            }
        case 'b':
            return handleBuySpell(state);
        case 'p':
            gameInterface->showMessage("Ход пропущен!");
            return true;
        case 'o':
            currentPhase = GamePhase::SaveGame;
            return false;
        case 'l':
            currentPhase = GamePhase::LoadGame;
            return false;
        default:
            if (input >= '1' && input <= '9') {
                int spellIndex = input - '1';
                if (spellIndex < state.player.getSpellHand().getSpellCount()) {
                    return handleSpellCast(state, spellIndex);
                }
            }
            gameInterface->showMessage("Неизвестная команда! Попробуйте снова.");
            return false;
    }
    
    if (dir != Direction::None) {
        return movePlayer(state, dir);
    }
    
    return false;
}

bool InputHandler::handleSpellCast(GameState& state, int spellIndex) {
    ISpell* spell = state.player.getSpellHand().getSpell(spellIndex);
    if (!spell) {
        gameInterface->showMessage("Заклинание не найдено!");
        return false;
    }
    
    if (state.player.getMana() < spell->getManaCost()) {
        gameInterface->showMessage("Недостаточно маны!");
        return false;
    }
    
    if (spell->getType() == SpellType::Enhancement) {
        int playerX = state.player.getPositionX();
        int playerY = state.player.getPositionY();
        return castSpell(state, spellIndex, playerX, playerY);
    }
    
    int targetX = state.player.getPositionX();
    int targetY = state.player.getPositionY();
    bool selecting = true;
    
    while (selecting) {
        gameInterface->showSpellTargetSelection(spell, targetX, targetY);
        
        char input = _getch();
        switch (tolower(input)) {
            case 'w': if (targetY > 0) targetY--; break;
            case 's': if (targetY < state.field.getHeight() - 1) targetY++; break;
            case 'a': if (targetX > 0) targetX--; break;
            case 'd': if (targetX < state.field.getWidth() - 1) targetX++; break;
            case 13:
                if (castSpell(state, spellIndex, targetX, targetY)) {
                    gameInterface->showMessage("Заклинание применено успешно!");
                    selecting = false;
                } else {
                    gameInterface->showMessage("Не удалось применить заклинание!");
                }
                break;
            case 27:
                gameInterface->showMessage("Отмена применения заклинания.");
                return false;
        }
    }
    
    return true;
}

bool InputHandler::handleRangedAttack(GameState& state) {
    if (state.player.getAttackType() != AttackType::Ranged) {
        gameInterface->showMessage("Сначала переключитесь на дальний тип атаки (T)!");
        return false;
    }
    
    int targetX = state.player.getPositionX();
    int targetY = state.player.getPositionY();
    bool selecting = true;
    
    while (selecting) {
        gameInterface->showRangedAttackTarget(targetX, targetY);
        
        char input = _getch();
        switch (tolower(input)) {
            case 'w': 
                if (targetY > 0) targetY--; 
                break;
            case 's': 
                if (targetY < state.field.getHeight() - 1) targetY++; 
                break;
            case 'a': 
                if (targetX > 0) targetX--; 
                break;
            case 'd': 
                if (targetX < state.field.getWidth() - 1) targetX++; 
                break;
            case 13:
                if (playerRangedAttack(state, targetX, targetY)) {
                    gameInterface->showMessage("Дальнобойная атака успешна!");
                    selecting = false;
                } else {
                    gameInterface->showMessage("Не удалось атаковать цель!");
                }
                break;
            case 27:
                gameInterface->showMessage("Отмена атаки.");
                return false;
        }
    }
    
    return true;
}

bool InputHandler::handleBuySpell(GameState& state) {
    const int SPELL_COST = 30;
    Hero& player = state.player;
    
    if (player.getScore() < SPELL_COST) {
        gameInterface->showMessage("Недостаточно очков!");
        return false;
    }
    
    if (player.getSpellHand().getSpellCount() >= player.getSpellHand().getMaxSize()) {
        gameInterface->showMessage("Рука заклинаний полна!");
        return false;
    }
    
    if (player.canBuySpell(SPELL_COST)) {
        player.addScore(-SPELL_COST);
        player.getSpellHand().fillWithRandomSpells(1);
        gameInterface->showMessage("Приобретено новое заклинание!");
        return true;
    }
    
    return false;
}

bool InputHandler::movePlayer(GameState& state, Direction direction) {
    if (state.player.isSlowed()) {
        state.player.removeSlowness();
        return false;
    }
    
    int currentX = state.player.getPositionX();
    int currentY = state.player.getPositionY();
    int newX = currentX;
    int newY = currentY;
    
    switch (direction) {
        case Direction::Up: newY--; break;
        case Direction::Down: newY++; break;
        case Direction::Left: newX--; break;
        case Direction::Right: newX++; break;
        case Direction::None: return false;
    }
    
    if (!state.field.isValidPosition(newX, newY)) {
        return false;
    }
    
    Cell& targetCell = state.field.getCell(newX, newY);
    
    if (targetCell.hasEnemy()) {
        if (EntityManager::damageEnemyAt(state, newX, newY, state.player.getDamage())) {
            gameInterface->showMessage("Игрок атаковал врага в ближнем бою! Урон: " + std::to_string(state.player.getDamage()));
            return true;
        }
        return false;
    }
    
    if (targetCell.hasBuilding()) {
        if (EntityManager::damageBuildingAt(state, newX, newY, state.player.getDamage())) {
            gameInterface->showMessage("Игрок атаковал здание в ближнем бою! Урон: " + std::to_string(state.player.getDamage()));
            return true;
        }
        return false;
    }
    
    if (targetCell.isPassable() && targetCell.isEmpty()) {
        state.field.getCell(currentX, currentY).setHasPlayer(false);
        state.player.setPosition(newX, newY);
        targetCell.setHasPlayer(true);
        
        if (targetCell.isSlowing()) {
            state.player.applySlowness();
        }
        return true;
    }
    
    return false;
}

bool InputHandler::castSpell(GameState& state, int spellIndex, int targetX, int targetY) {
    return state.player.castSpell(spellIndex, targetX, targetY, state);
}

bool InputHandler::playerRangedAttack(GameState& state, int targetX, int targetY) {
    int playerX = state.player.getPositionX();
    int playerY = state.player.getPositionY();
    
    int distance = std::abs(playerX - targetX) + std::abs(playerY - targetY);
    int rangedRange = state.player.getRangedRange();

    if (distance > rangedRange || distance == 0) {
        gameInterface->showMessage("Цель слишком далеко или неверная позиция!");
        return false;
    }
    
    if (!state.field.isValidPosition(targetX, targetY)) {
        gameInterface->showMessage("Неверная позиция цели!");
        return false;
    }
    
    bool attacked = false;
    
    if (EntityManager::damageEnemyAt(state, targetX, targetY, state.player.getDamage())) {
        gameInterface->showMessage("Дальнобойная атака по врагу! Урон: " + std::to_string(state.player.getDamage()));
        attacked = true;
    }
    
    if (EntityManager::damageBuildingAt(state, targetX, targetY, state.player.getDamage())) {
        gameInterface->showMessage("Дальнобойная атака по зданию! Урон: " + std::to_string(state.player.getDamage()));
        attacked = true;
    }
    
    if (!attacked) {
        gameInterface->showMessage("Атака не достигла цели!");
    }
    
    return attacked;
}