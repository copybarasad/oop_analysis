#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "GameField.h"
#include "Wanderer.h"
#include "CombatManager.h"
#include "BeastManager.h"
#include "SpellFactory.h"

class InputHandler {
private:
    Wanderer& wanderer;
    GameField& field;
    Position& wandererPosition;
    CombatManager& combatManager;
    BeastManager& beastManager;
    bool& wandererCanMove;
    bool& wandererOnSlowCell;
    bool& gameRunning;
    bool selectingSpellTarget;
    int selectedSpellIndex;

public:
    InputHandler(Wanderer& wanderer, GameField& field, Position& wandererPos,
                 CombatManager& combatManager, BeastManager& beastManager, bool& canMove, bool& onSlowCell, bool& running);
                 
    void processInput(char input);
    void applyCellEffects(int x, int y);
    void processSpellSelection(char input);
    void processSpellTargetSelection(char input);
    void processSpellPurchase();
    bool isSelectingSpellTarget() const;
    void startSpellTargetSelection(int spellIndex);
    void cancelSpellTargetSelection();
    void updateAllManagersPosition(const Position& newPos);
};

#endif