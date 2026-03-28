#include "BuySpellCommand.h"
#include "../Game.h"
#include "../Player.h"
#include "../Hand.h"
#include <iostream>

CommandResult BuySpellCommand::execute(Game& game) {
    Player* player = game.GetPlayer();
    if (!player) return {false, "No player."};

    const int cost = 5;

    if (player->GetHand().Size() >= player->GetHand().Capacity()) {
        return {false, "Hand is full. Can't buy."};
    } else if (!player->SpendCoins(cost)) {
        return {false, "Not enough coins to buy. Need " + std::to_string(cost) + "."};
    } else {
        auto s = Hand::CreateRandomSpell();
        player->GetHand().AddSpell(std::move(s));
        return {true, "Bought random spell and added to hand."};
    }
}


