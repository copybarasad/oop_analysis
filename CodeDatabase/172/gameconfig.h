#pragma once

#include "pos.h"

#include <string>

namespace game {
    // ----------------- Game parameters ------------------
    struct GameConfig {

        // == Generation parameters ==
        int maxSize                 = 25;
        int minSize                 = 10;

        int defaultSize             = minSize;

        int wallCornerChance        = 40;

        double wallMaxDensity       = 0.09;
        double slimeMaxDensity      = 0.05;

        // == Enemy parameters ==
        int maxEnemies              = 5;

        int defaultSpawnInterval    = 16;
        int minSpawnInterval        = 8;

        int enemyDefaultDamage      = 5;
        int enemyDefaultHP          = 100;
        int enemyDespawnTurn        = 5;
        int enemiesPerSpawn         = 1;
        int enemyRandMoveChance     = 16;

        Pos eBuildingInitialPosition = Pos(defaultSize - 2, defaultSize - 2);
        Pos eTowerInitialPosition    = Pos(defaultSize - 2, 1);

        int eTowerRadius            = 3;
        int eTowerDamage            = 10;
        int eTowerCooldown          = 5;
        int eTowerMinCooldown       = 2;

        // == Player parameters ==
        int critChance              = 25;
        int critMultiplier          = 2;

        int playerDefaultHP         = 100;
        Pos playerInitialPosition   = Pos();
        int playerMeleeDamage       = 20;
        int playerRangedDamage      = 12;

        // == Spell parameters ==
        int playerHandCapacity       = 9;

        int playerSpellDirectDamage = playerMeleeDamage * 2;
        int playerSpellDirectRadius = (int)(defaultSize / 2);

        int playerSpellAreaDamage   = playerMeleeDamage * 2;
        int playerSpellAreaRadius   = (int)(defaultSize / 3);

        int trapSpellBaseDamage     = playerMeleeDamage * 4;

        int spellsRemovingCoef      = 2;    // per level

        // == Level up parameters ==
        int hpUpgradeValue          = 20;
        int damageUpgradeValue      = 10;

        int spellRadiusUpgradeValue = 2;
        int spellDamageUpgradeValue = 10;

        // == Save parameters ==
        std::string saveVersion     = "1.3.1";
        std::string saveName        = "save";
    };
}