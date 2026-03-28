#pragma once
#include <iostream>
#include <vector>
#include <limits>
#include "GameField.h"
#include "Player.h"
#include "PlayerHand.h"

class BattleSystem{
public:
    int countAliveEnemies(Field& field);
    int playerAttack(Field& field, Player& player, PlayerHand& hand);
    int castSpell(Field& field, Player& player, PlayerHand& hand);
    void rewardForKill(PlayerHand& hand);
    void updateTraps(Field& field, PlayerHand& hand);

};