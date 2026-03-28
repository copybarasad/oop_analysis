#ifndef RPG_SPELLSELECTCOMMAND_H
#define RPG_SPELLSELECTCOMMAND_H

#include "Command.h"
#include "../Game.h"
#include "../Player.h"
#include "../UpgradeSpell.h"

class SpellSelectCommand : public Command {
    int index;
    std::shared_ptr<Game>& game;
public:
    SpellSelectCommand(int index, std::shared_ptr<Game>& game) : index(index), game(game) {}

    std::pair<bool, std::string> execute() override {
        auto& player = game->getPlayer();
        auto& shop = game->getShop();
        auto& spellsHand = player.getSpellsHand();
        auto mode = player.getMode();
        if (mode ==  PlayerModes::SHOP) {
            shop.buyItem(index, player);
            return {false, ""};
        } if (mode ==  PlayerModes::UPGRADE) {
            player.upgradeSpell(index);
            return {false, ""};
        }if (auto* upgrade = dynamic_cast<UpgradeSpell*>(spellsHand.getSelectedSpell())) {
            upgrade->setTarget(index);
            return {false, ""};
        }
        auto spell = spellsHand.selectSpell(index);
        if (!spell)
            return {false, ""};
        player.setMode( PlayerModes::SPELL);
        player.setTarget(player.getPosition());
        return {false, ""};
    };

};

#endif