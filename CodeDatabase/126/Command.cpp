#include "../../headers/commands/Command.h"
#include "../../headers/gamelogic/Game.h"
#include "../../headers/objects/Player.h"

MoveCommand::MoveCommand(Direction dir) : direction(dir) {}

void MoveCommand::execute(Game& game) {
    game.movePlayer(direction);
}

std::string MoveCommand::getName() const { return "Move"; }

void SwitchWeaponCommand::execute(Game& game) {
    game.switchWeapon();
}

std::string SwitchWeaponCommand::getName() const { return "SwitchWeapon"; }

RangedAttackCommand::RangedAttackCommand(Direction dir) : direction(dir) {}

void RangedAttackCommand::execute(Game& game) {
    game.performRangedAttack(direction);
}

std::string RangedAttackCommand::getName() const { return "RangedAttack"; }

CastSpellCommand::CastSpellCommand() : spellIndex(-1), direction(Direction::UP), needsInput(true) {}

CastSpellCommand::CastSpellCommand(int idx, Direction dir) 
    : spellIndex(idx), direction(dir), needsInput(false) {}

void CastSpellCommand::execute(Game& game) {
    if (needsInput) {
        auto player = game.getPlayer();
        int spellCount = player->getSpellHand().getSpellCount();
        
        if (spellCount == 0) {
            std::cout << "No spells available!\n";
            return;
        }
        
        player->getSpellHand().displaySpells();
        
        std::cout << "Choose spell index (0-" << spellCount - 1 << "): ";
        std::cin >> spellIndex;
        
        if (spellIndex < 0 || spellIndex >= spellCount) {
            std::cout << "Invalid spell index!\n";
            return;
        }
        
        char dirChar;
        std::cout << "Enter direction (w-up, s-down, a-left, d-right): ";
        std::cin >> dirChar;
        
        switch(dirChar){
            case 'w': case 'W': direction = Direction::UP; break;
            case 's': case 'S': direction = Direction::DOWN; break;
            case 'a': case 'A': direction = Direction::LEFT; break;
            case 'd': case 'D': direction = Direction::RIGHT; break;
            default: 
                std::cout << "Invalid direction!\n"; 
                direction = Direction::UP;
        }
    }
    
    game.castSpell(spellIndex, direction);
}

std::string CastSpellCommand::getName() const { return "CastSpell"; }

void BuySpellCommand::execute(Game& game) {
    game.buySpell();
}

std::string BuySpellCommand::getName() const { return "BuySpell"; }

void SaveGameCommand::execute(Game& game) {
    game.saveGame();
}

std::string SaveGameCommand::getName() const { return "SaveGame"; }

void LoadGameCommand::execute(Game& game) {
    game.loadGame();
}

std::string LoadGameCommand::getName() const { return "LoadGame"; }

void QuitCommand::execute(Game& game) {
    game.quitGame();
}

std::string QuitCommand::getName() const { return "Quit"; }

void NextLevelCommand::execute(Game& game) {
    game.nextLevel();
}

std::string NextLevelCommand::getName() const { return "NextLevel"; }

void RestartCommand::execute(Game& game) {
    game.restartGame();
}

std::string RestartCommand::getName() const { return "Restart"; }