#pragma once
#include "../game_objects/Player.h"
#include "../game_objects/GameField.h"
#include "../game_objects/Character.h"
#include "../game_main/GameState.h"
#include "../game_objects/PlayerHand.h"
#include <vector>
#include <sstream>
#include <iostream>

class SpellManager {
public:
    // Есть ли враги рядом 
    bool showTargetsInRange(const Player& player, const GameState& state, int range);

    // Процесс обработки заклинания 
    void spellProcessing(Player& player, const PlayerHand& hand, GameState& state);
};