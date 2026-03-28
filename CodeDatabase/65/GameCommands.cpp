#include "Game.h"
#include "commands/MoveCommand.h"

// Реализация методов для обработки команд

void Game::handleMove(Direction direction) {
    if (targettingSystem.getIsActive()) {
        // Режим прицеливания - двигаем курсор
        int dx = 0, dy = 0;
        switch (direction) {
            case Direction::UP: dy = -1; break;
            case Direction::DOWN: dy = 1; break;
            case Direction::LEFT: dx = -1; break;
            case Direction::RIGHT: dx = 1; break;
            default: break;
        }
        targettingSystem.moveCursor(dx, dy, *field);
        return;
    }

    // Обычное движение игрока
    if (player->getCanMove() != 0 && player->getCanMove() < 2) {
        player->setCanMove(player->getCanMove() + 1);
    } else {
        player->setCanMove(0);
    }

    int dx = 0, dy = 0;
    switch (direction) {
        case Direction::UP: dy = -1; break;
        case Direction::DOWN: dy = 1; break;
        case Direction::LEFT: dx = -1; break;
        case Direction::RIGHT: dx = 1; break;
        default: return;
    }

    collisionSystem.tryMovePlayer(*field, dx, dy);
}

void Game::handleAttack() {
    if (targettingSystem.getIsActive()) {
        return; // В режиме прицеливания атака не работает
    }

    if (player->getAttackMode() == AttackMode::CLOSE_COMBAT) {
        // Ближний бой - получаем следующую команду направления через gameController
        auto dirCmd = getGameController()->getNextCommand();
        if (dirCmd) {
            dirCmd->execute(*this);
        }
    } else {
        // Дальний бой - активируем прицеливание
        attackSystem->setAttackType(AttackType::RANGE);
        targettingSystem.startRangedAttack(player.get());
    }
}

void Game::handleChangeAttackMode() {
    player->toggleAttackMode();
}

void Game::handleQuit() {
    state = GameState::QUIT;
    running = false;
}

void Game::handleConfirm() {
    if (targettingSystem.getIsActive()) {
        if (targettingSystem.confirm()) {
            attackSystem->executeAttack(
                targettingSystem.getCursorX(),
                targettingSystem.getCursorY()
            );
        }
    }
}

void Game::handleCancel() {
    if (targettingSystem.getIsActive()) {
        targettingSystem.cancel();
    }
}

void Game::handleCastSpell() {
    if (targettingSystem.getIsActive()) {
        return; // Нельзя применять заклинание во время прицеливания
    }

    Hand& hand = player->getHand();
    int spellIndex = hand.getSize();
    char ch;
    
    while (spellIndex >= hand.getSize()) {
        ch = inputManager.getChar();
        if (ch == 27) return; // ESC
        if (std::isdigit(ch)) {
            spellIndex = ch - '0';
        }
    }
    
    ISpell* spell = hand.getSpell(spellIndex);
    attackSystem->setActiveSpellIndex(spellIndex);
    attackSystem->setAttackType(AttackType::SPELL);
    targettingSystem.startCastSpell(player.get(), spell);
}

void Game::handleSaveGame() {
    saveGame();
}

void Game::handleLoadGame() {
    loadGame();
}
