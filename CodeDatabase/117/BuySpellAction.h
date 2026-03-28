#ifndef BUYSPELLACTION_H
#define BUYSPELLACTION_H

#include "Action.h"
#include "../Spells/DirDMGSpell.h"
#include "../Spells/AreaDMGSpell.h"
#include "../Spells/SummonSpell.h"
#include "../Spells/UpgradeSpell.h"
#include "../Spells/TrapSpell.h"

#include "../Output/ConsoleRenderer.h"
#include "../Output/GameVisualizer.h"

#include "../LogSystem/Notifier.h"
#include "../LogSystem/SpellBuyEvent.h"

#include <map>

template <class InputHandler>
class BuySpellAction : public Action {
private:
    InputHandler& handler;
    Hand& hand;
    Player& player;
    GameVisualizer<ConsoleRenderer>& view;
public:
    BuySpellAction(InputHandler& handler, GameVisualizer<ConsoleRenderer>& view, Player& player, Hand& hand) : 
        handler(handler), view(view), player(player), hand(hand) {}

    bool execute() override{
        int spellList[5] = {
            DIR_SPELL_COST, 
            AREA_SPELL_COST, 
            SUMMON_SPELL_COST, 
            UPGRADE_SPELL_COST, 
            TRAP_SPELL_COST
        };

        std::map<SpellType, std::string> spellTypeToString = {
            {SpellType::DirDMGSpell, "Direct damage spell"},
            {SpellType::AreaDMGSpell, "Area damage spell"},
            {SpellType::SummonSpell, "Summon spell"},
            {SpellType::TrapSpell, "Trap spell"},
            {SpellType::UpgradeSpell, "Upgrade spell"},
        };

        this->view.renderShop();

        int choice = handler.getNum();
        
        if (choice > 0 && choice <= 5) {
            if (spellList[choice - 1] > this->player.getEXP()) {
                this->view.renderMessage("Come back when you're a little mmm richer");
                return false;
            }
            
            SpellType newSpell;
            newSpell = (SpellType)(choice - 1);
            if (this->hand.addSpell(newSpell)) {
                this->player.subEXP(spellList[choice - 1]);
                this->view.renderMessage(spellTypeToString[newSpell] + " was bought");
                SpellBuyEvent event = SpellBuyEvent(spellTypeToString[newSpell], spellList[choice - 1]);
                Notifier::getInstance().notify(event);
                return true;
            }
            else {
                this->view.renderMessage("Your hand is full");
            }
        }
        else{
            this->view.renderErrorMessage("Invalid choice");
        }
        
        return false;
    }

    std::string getName() const override{
        return "Buy spell";
    }
};

#endif