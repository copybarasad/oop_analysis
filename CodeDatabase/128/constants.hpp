#pragma once

namespace configs {
    //Конструктор в gamemodel
    constexpr int kEnemyCount = 2;
    constexpr int kBuildingSpawnDelay = 6;
    //в gamecontroller
    constexpr int kHealAmount = 10;
    constexpr int kHealCooldown = 7;
    constexpr int kAmountWithoutAttack = 3;
    //в gamemove
    constexpr int kScore = 10;
    constexpr int kNumberSides = 4;
    //в  direct damage
    constexpr int kDirectDamage = 12;
    constexpr int kDirectRadius = 3;
    constexpr int kAreaDamage = 9;
    constexpr int kAreaRadius = 3;
    constexpr int kAreaSide = 2;
    //Ally
    constexpr int kBaseAllyHealth = 30;
    constexpr int kBaseAllyDamage = 7;
    constexpr int kSummonCount = 1; 
    //enemy_building
    constexpr int kBaseHealth = 30;
    constexpr int kBaseDamage = 0;
    //enemy_tower
    constexpr int kBaseHealthTower = 50;
    constexpr int kBaseDamageTower = 10;
    constexpr int kAttackRadiusTower = 3;
    constexpr int kTowerDelay = 5;
    //enhancement
    constexpr int kEnhancementValue = 1;

    constexpr int kTrapDamage = 10;   
    constexpr int kTrapDamagePerStack = 5; 

    constexpr int kCostMaxHealthPlayer = 30;
    constexpr int kCostDamagePlayer = 20;
    constexpr int kOnceUpgradeHealth = 15;
    constexpr int kOnceUpgradeDamage = 5;
}