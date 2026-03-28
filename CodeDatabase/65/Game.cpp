#include "Game.h"
#include "Colors.h"
#include <cctype>

void  Game::showMainMenu() {
    renderSystem->clearScreen();
    showMainMenuScreen();
    handleMainMenuKb();
}

void Game::showMainMenuScreen() {
    std::cout << TermColors::MAGENTA;
    std::cout << "╔════════════════════════════════════╗\n";
    std::cout << "║            2D RPG GAME             ║\n";
    std::cout << "╚════════════════════════════════════╝\n";
    std::cout << "\033[0m";

    const char* asciiArt = R"(
               __
           _  |@@|
          / \ \--/ __
          ) O|----|  |   __
         / / \ }{ /\ )_ / _\
         )/  /\__/\ \__O (__
        |/  (--/\--)    \__/
        /   _)(  )(_
           `---''---`
        )";

    std::cout << asciiArt << '\n';
    std::cout << "1. New Game\n";

    if (saveSystem.saveExists()) {
        std::cout << "2. Load Game\n";
    }

    std::cout << "Q. Quit\n\n";
    std::cout << "Choose an option: ";
}

void Game::handleMainMenuKb() {
    char ch = inputManager.getChar();

    switch (ch) {
        case '1':
            initNewGame();
            break;
        case '2':
            if (saveSystem.saveExists()) {
                loadGame();
            }
            break;
        case 'q':
        case 'Q':
            state = GameState::QUIT;
            break;
        default:
            break;
    }
}

void Game::showUpgradeMenuScreen() {
    std::cout << TermColors::MAGENTA;
    std::cout << "╔════════════════════════════════════╗\n";
    std::cout << "║      UPGRADE YOUR CHARACTER        ║\n";
    std::cout << "╚════════════════════════════════════╝\n";
    std::cout << "\033[0m\n";

    std::cout << "Choose an upgrade:\n";
    std::cout << "1. Increase Max Health (+20)\n";
    std::cout << "2. Increase Melee Damage (+10)\n";
    std::cout << "3. Increase Ranged Damage (+15)\n";
    std::cout << "4. Increase Range (+1)\n";
    std::cout << "5. Upgrade Random Spell (if available)\n";
    std::cout << "\nYour choice: ";
}

bool Game::handleUpgradeMenuKb() {
    char ch = inputManager.getChar();

    switch (ch) {
        case '1':
            player->upgradeHealth(20);
            std::cout << "\n\033[32mHealth upgraded!\033[0m\n";
            return true;
        case '2':
            player->upgradeDamage(10);
            std::cout << "\n\033[32mMelee damage upgraded!\033[0m\n";
            return true;
        case '3':
            player->upgradeRangeDamage(15);
            std::cout << "\n\033[32mRanged damage upgraded!\033[0m\n";
            return true;
        case '4':
            player->upgradeRange(1);
            std::cout << "\n\033[32mRange upgraded!\033[0m\n";
            return true;
        case '5':
            upgradeRandomSpell();
            return true;
        default:
            std::cout << "\n\033[31mInvalid choice!\033[0m\n";
            sleep(1);
            return false;
    }

}

void Game::showUpgradeMenu() {
    renderSystem->clearScreen();
    showUpgradeMenuScreen();
    while (!handleUpgradeMenuKb()) {}
    sleep(1);
    initLevel();
    state = GameState::PLAYING;
}

void Game::showGameOverScreen() {
    std::cout << TermColors::RED;
    std::cout << "╔════════════════════════════════════╗\n";
    std::cout << "║          GAME OVER!                ║\n";
    std::cout << "╚════════════════════════════════════╝\n";
    std::cout << "\033[0m\n";
    std::cout << "You survived " << levelManager.getCurrentLevel() << " levels.\n";
    std::cout << "\n1. Restart\n";
    std::cout << "2. Main Menu\n";
    std::cout << "Q. Quit\n";
}

void Game::handleGameOverKb() {
    char ch = inputManager.getChar();

    switch (ch) {
        case '1':
            initNewGame();
            break;
        case '2':
            state = GameState::MAIN_MENU;
            break;
        case 'q':
        case 'Q':
            state = GameState::QUIT;
            running = false;
            break;
        default:
            state = GameState::MAIN_MENU;
            break;
    }
}

void Game::handleGameOver() {
    renderSystem->clearScreen();
    showGameOverScreen();
    handleGameOverKb();
}

void Game::handleGameWin() {
    renderSystem->clearScreen();
    showGameWinScreen();
    inputManager.getChar();
    state = GameState::MAIN_MENU;
}

void Game::showGameWinScreen() {
    std::cout << TermColors::GREEN;
    std::cout << "╔════════════════════════════════════╗\n";
    std::cout << "║       CONGRATULATIONS!             ║\n";
    std::cout << "║        YOU WIN!                    ║\n";
    std::cout << "╚════════════════════════════════════╝\n";
    std::cout << "\033[0m\n";

}

void Game::showError(const std::string& message) {
    renderSystem->clearScreen();
    showErrorScreen(message);
    inputManager.getChar();
}

void Game::showErrorScreen(const std::string message) {
    std::cout << TermColors::RED;
    std::cout << "╔════════════════════════════════════╗\n";
    std::cout << "║            ERROR!                  ║\n";
    std::cout << "╚════════════════════════════════════╝\n";
    std::cout << "\033[0m\n";
    std::cout << message << "\n\n";
    std::cout << "Press any key to continue...\n";
}

void Game::handleLevelComplete() {
    renderSystem->clearScreen();
    showLevelCompleteScreen();
    inputManager.getChar();

    levelManager.nextLevel();
    player->restoreHealth();
    player->removeHalfSpells();

    state = GameState::UPGRADE_MENU;
}

void Game::showLevelCompleteScreen() {
    std::cout << TermColors::GREEN;
    std::cout << "╔════════════════════════════════════╗\n";
    std::cout << "║      LEVEL COMPLETED!              ║\n";
    std::cout << "╚════════════════════════════════════╝\n";
    std::cout << "\033[0m\n";
    std::cout << "Level " << levelManager.getCurrentLevel() << " completed!\n";
    std::cout << "\nPress any key to continue...\n";

}

void Game::showSpellChoice() {
    showSpellChoiceScreen();
    handleSpellChoiceKb();
    state = GameState::PLAYING;
}


void Game::showSpellChoiceScreen() {
    renderSystem->clearScreen();
    spellChoices = SpellFactory::createThreeRandomSpells();

    std::cout << TermColors::YELLOW;
    std::cout << "╔════════════════════════════════════╗\n";
    std::cout << "║          LEVEL UP!                 ║\n";
    std::cout << "║      Choose a new spell:           ║\n";
    std::cout << "╚════════════════════════════════════╝\n";
    std::cout << "\033[0m\n";

    for (size_t i = 0; i < spellChoices.size(); i++) {
        std::cout << i << ". " << spellChoices[i]->getName() 
            << " (Range: " << spellChoices[i]->getRange()
            << ", Damage: " << spellChoices[i]->getDamage() << ")\n";
    }

    std::cout << "\nPress number to choose\n";
}

void Game::handleSpellChoiceKb() {
    int ch, choiceIndex;
    do {
        ch = inputManager.getChar();
        choiceIndex = ch - '0';
    } while (!std::isdigit(ch) && 
            (choiceIndex < 0 || 
             choiceIndex >= static_cast<int>(spellChoices.size())));

    Hand& hand = player->getHand();
    if (!hand.isFull()) {
        hand.addSpell(std::move(spellChoices[choiceIndex]));
    }

    spellChoices.clear();
}

void Game::displayGameHint() {
    renderSystem->moveCursor(0, field->getHeight() + 1);

    // status bar
    std::cout << '\n' << TermColors::GRAY_MAGENTA; 
    std::cout << "Level: " << levelManager.getCurrentLevel() 
        << " | HP: " << player->getHealth() << "/" << player->getMaxHealth()
        << " | XP: " << player->getXP() << "/" << player->getXPToNextLevel()
        << " | Player Level: " << player->getLevel() << std::endl;
    if (player->getAttackMode() == AttackMode::CLOSE_COMBAT) {
        std::cout << "| Damage: " << player->getDamage() << std::endl;
    } else {
        std::cout << "| Range Damage: " << player->getRangeDamage() << std::endl;
    }

    // spells
    std::cout << "Spells: \n";
    Hand& hand = player->getHand();
    int spellIndex = 0;
    for (const auto& spell : hand.getSpells()) {
        std::cout << spellIndex << ". " << spell->getName() << std::endl;
        spellIndex++;
    }

    // hint
    std::cout << "\n[W/A/S/D] Move | [Space] Attack | [E] Toggle Attack Mode | [C] Cast Spell";
    std::cout << "\n[F] Save | [L] Load | [Q] Quit\033[0m\n";
}

void Game::initTerminal() {
    tcgetattr(STDIN_FILENO, &oldSettings);
    struct termios newSettings = oldSettings;
    newSettings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);
}

void Game::restoreTerminal() {
    renderSystem->clearScreen();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
}

void Game::updateGameWorld() {
    aiSystem.updateEnemies(*field, collisionSystem);
    updateTowers();
    field->removeDeadEnemies();
}

void Game::updateTowers() {
    for (auto& obj : field->getObjects()) {
        if (obj->getTags() & ObjectTag::TAG_TOWER) {
            Entity* tower = dynamic_cast<Entity*>(obj.get());
            if (tower && tower->isAlive()) {
                int dx = player->getX() - tower->getX();
                int dy = player->getY() - tower->getY();
                int distSq = dx*dx + dy*dy;

                // Tower range is about 5 tiles
                if (distSq <= 25) {
                    player->takeDamage(tower->getDamage());
                }
            }
        }
    }
}

void Game::gameLoop() {
    while (running && state == GameState::PLAYING) {
        if (player->canLevelUp()) {
            player->levelUp();
            state = GameState::SPELL_CHOICE;
            continue;
        }

        renderSystem->draw();
        displayGameHint();

        if (!player->isAlive()) {
            state = GameState::GAME_OVER;
            break;
        }

        if (levelManager.isLevelCompleted(*field)) {
            state = GameState::LEVEL_COMPLETE;
            break;
        }

        // Используем новую систему команд
        auto command = gameController->getNextCommand();
        
        if (command) {
            bool wasTargetting = targettingSystem.getIsActive();
            command->execute(*this);
            
            // Обновляем мир если:
            // 1. Игрок совершил движение
            // 2. Была атака (вышли из режима прицеливания)
            bool isMove = dynamic_cast<MoveCommand*>(command.get()) != nullptr;
            bool wasAttack = wasTargetting && !targettingSystem.getIsActive();
            
            if (!targettingSystem.getIsActive() && (isMove || wasAttack)) {
                updateGameWorld();
            }
        }
    }
}
