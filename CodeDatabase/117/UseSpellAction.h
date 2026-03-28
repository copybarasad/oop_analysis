#ifndef USESPELLACTION_H
#define USESPELLACTION_H

#include "Action.h"
#include "../SpellsStrategies/ISpellStr.h"
#include <map>
#include <memory>
#include "../Spells/DirDMGSpell.h"
#include "../Spells/AreaDMGSpell.h"
#include "../Spells/SummonSpell.h"
#include "../Spells/UpgradeSpell.h"
#include "../Spells/TrapSpell.h"
#include "../SpellsStrategies/AreaDMGStr.h"
#include "../SpellsStrategies/DirDMGStr.h"
#include "../SpellsStrategies/TrapStr.h"
#include "../SpellsStrategies/SummonStr.h"
#include "../SpellsStrategies/UpgradeStr.h"
#include "../Output/ConsoleRenderer.h"
#include "../Output/GameVisualizer.h"
#include <vector>

#include "../LogSystem/Notifier.h"
#include "../LogSystem/SpellCastEvent.h"

template <class InputHandler>
class UseSpellAction : public Action {
private:
    std::map<SpellType, std::unique_ptr<ISpellStr>> strategies;
    GameVisualizer<ConsoleRenderer>& view;
    InputHandler& handler;
    GameState& gameState;

    void initializeStr(){
        this->strategies[SpellType::DirDMGSpell] = std::make_unique<DirDMGStr<InputHandler>>(handler, view);
        this->strategies[SpellType::AreaDMGSpell] = std::make_unique<AreaDMGStr<InputHandler>>(handler, view);
        this->strategies[SpellType::SummonSpell] = std::make_unique<SummonStr>();
        this->strategies[SpellType::TrapSpell] = std::make_unique<TrapStr<InputHandler>>(handler, view);
        this->strategies[SpellType::UpgradeSpell] = std::make_unique<UpgradeStr>();
    }

public:
    UseSpellAction(InputHandler& handler, GameVisualizer<ConsoleRenderer>& view, GameState& gameState):
        handler(handler), view(view), gameState(gameState) {
        this->initializeStr();
    }

    bool execute() override{
        std::map<SpellType, std::string> spellTypeToString = {
            {SpellType::DirDMGSpell, "Direct damage spell"},
            {SpellType::AreaDMGSpell, "Area damage spell"},
            {SpellType::SummonSpell, "Summon spell"},
            {SpellType::TrapSpell, "Trap spell"},
            {SpellType::UpgradeSpell, "Upgrade spell"},
        };

        this->view.renderHand();

        int spellCount = this->gameState.getHand().getCount();

        if (spellCount == 1) {
            this->view.renderMessage("Choose spell (1) or 0 to cancel");
        } else {
            this->view.renderMessage("Choose spell (1-" + std::to_string(spellCount) + ") or 0 to cancel");
        }

        int choice = this->handler.getNum();

        if (choice > spellCount || choice < 0) {
            this->view.renderErrorMessage("Invalid choice");
            return false;
        }

        if (choice == 0) {
            return false;
        }

        SpellType spell = this->gameState.getHand().getSpell(choice - 1);
        if (this->strategies[spell]->execute(this->gameState)){
            this->view.renderMessage("You casted spell");
            this->gameState.getHand().removeSpell(choice - 1);
            SpellCastEvent event = SpellCastEvent(spellTypeToString[spell], "Player");
            Notifier::getInstance().notify(event);
            return true;
        }
        return false;
    }

    std::string getName() const {
        return "Use spell";
    }
};

#endif