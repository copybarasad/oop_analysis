#ifndef ACTION_SYSTEM_H
#define ACTION_SYSTEM_H

#include "LevelController.h"
#include "CombatSystem.h"
#include "TextSystem.h"
#include "Command.h"
#include "SpellHand.h"
#include <memory>

class ActionSystem {
private:
    std::shared_ptr<TextSystem> textSystem;
    CombatSystem combatSystem;

public:
    ActionSystem(std::shared_ptr<TextSystem> textSystem);
    
    bool processCommand(Command command, LevelController& level);
    bool movePlayer(LevelController& level, int deltaX, int deltaY);
    bool playerAttack(LevelController& level);
    bool useSpell(LevelController& level, int spellIndex);
    void showSpells(LevelController& level);
    
private:
    void giveRandomSpell(SpellHand& hand);
};

#endif