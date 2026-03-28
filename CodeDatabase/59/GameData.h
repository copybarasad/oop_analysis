#pragma once

#include <vector>
#include "Player.h"
#include "Field.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include "SpellShop.h"
#include "PlayerHand.h"
#include "TrapSpell.h"
#include "json.hpp"
using json = nlohmann::json;
struct GameData {
    Player player;
    Field field;
    std::vector<Enemy> enemies;
    std::vector<EnemyTower> towers;
    SpellShop spellShop;
    PlayerHand playerHand;
    std::vector<TrapSpell> active_traps;
    int current_level = 1;

    GameData();

    GameData(bool isNewGame);

    void goToNextLevel();
    void generateLevel(int level);

    json toJson() const;
    void fromJson(const json& j);
};