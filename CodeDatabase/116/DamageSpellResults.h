#pragma once

struct damageSpellResults{
    int woundedEnemies;
    int woundedTowers;
    int woundedFortresses;
    int woundedPlayers;
    int woundedAllies;
    int damage;
    damageSpellResults() : woundedEnemies(0), woundedTowers(0), woundedFortresses(0), woundedPlayers(0), woundedAllies(0) {};
};