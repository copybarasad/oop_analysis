#include "spellcard.h"
#include <iostream>
#include <cmath>

// ==================== SpellCard ====================
SpellCard::SpellCard(const std::string& spellName, const std::string& spellDesc, 
                     int cost, int effectValue, int effectRadius, int cd)
    : name(spellName), description(spellDesc), manaCost(cost), 
      value(effectValue), radius(effectRadius), cooldown(cd), currentCooldown(0) 
{}

bool SpellCard::checkBaseConditions(const SpellContext& context) const {
    if (!isReady()) {
        std::cout << name << " перезаряжается! Осталось: " << currentCooldown << "\n";
        return false;
    }
    
    int playerX, playerY;
    if (!context.field.findPlayerPosition(playerX, playerY)) {
        std::cout << "Не найден игрок!\n";
        return false;
    }
    
    if (!context.field.isValidPosition(context.targetX, context.targetY) || 
        !isInRange(context)) {
        std::cout << "Цель вне радиуса!\n";
        return false;
    }
    
    return true;
}

bool SpellCard::isInRange(const SpellContext& context) const {
    int playerX, playerY;
    context.field.findPlayerPosition(playerX, playerY);
    int distance = std::max(std::abs(playerX - context.targetX), 
                           std::abs(playerY - context.targetY));
    return distance <= radius;
}

// ==================== DirectDamageSpell ====================
DirectDamageSpell::DirectDamageSpell(const std::string& spellName, const std::string& spellDesc, 
                                   int cost, int damage, int range, int cd)
    : SpellCard(spellName, spellDesc, cost, damage, range, cd) 
{}

bool DirectDamageSpell::cast(SpellContext& context) {
    if (!checkBaseConditions(context)) return false;
    
    if (context.player.getEconomy().getMana() < manaCost) {
        std::cout << "Недостаточно маны!\n";
        return false;
    }
    
    std::cout << "Применяем " << name << "!\n";
    applyDirectDamage(context);
    
    context.player.getEconomy().spendMana(manaCost);
    currentCooldown = cooldown;
    return true;
}

void DirectDamageSpell::applyDirectDamage(SpellContext& context) {
    int enemyX, enemyY;
     
    if (context.field.findEnemyPosition(enemyX, enemyY) && 
        enemyX == context.targetX && enemyY == context.targetY) {
        context.enemy.takeDamage(value);
        std::cout << "Прямой урон наносит " << value << " урона врагу!\n";
        if (!context.enemy.isAlive()) {
            std::cout << "Враг побежден магией!\n";
        }
        return;
    }
     
    if (context.field.hasTowerAt(context.targetX, context.targetY)) {
        for (auto& tower : context.field.getTowers()) {
            int towerX, towerY;
            tower.getPosition(towerX, towerY);
            if (towerX == context.targetX && towerY == context.targetY && tower.getIsAlive()) {
                tower.takeDamage(value);
                std::cout << "Прямой урон наносит " << value << " урона башне!\n";
                return;
            }
        }
    }
    
    std::cout << "Цель не найдена!\n";
}

// ==================== AreaDamageSpell ====================
AreaDamageSpell::AreaDamageSpell(const std::string& spellName, const std::string& spellDesc, 
                               int cost, int damage, int areaRadius, int cd)
    : SpellCard(spellName, spellDesc, cost, damage, areaRadius, cd) 
{}

bool AreaDamageSpell::cast(SpellContext& context) {
    if (!checkBaseConditions(context)) return false;
    
    if (context.player.getEconomy().getMana() < manaCost) {
        std::cout << "Недостаточно маны!\n";
        return false;
    }
    
    std::cout << "Применяем " << name << "!\n";
    applyAreaDamage(context);
    
    context.player.getEconomy().spendMana(manaCost);
    currentCooldown = cooldown;
    return true;
}

void AreaDamageSpell::applyAreaDamage(SpellContext& context) {
    std::cout << "Взрыв магии в области " << context.targetX << "," << context.targetY << " (радиус: " << radius << ")!\n";
    
    bool hitSomething = false;
     
    int enemyX, enemyY;
    if (context.field.findEnemyPosition(enemyX, enemyY)) {
        int distX = abs(enemyX - context.targetX);
        int distY = abs(enemyY - context.targetY);
        if (distX <= radius && distY <= radius) {
            context.enemy.takeDamage(value);
            hitSomething = true;
            std::cout << "Враг получает " << value << " урона от взрыва!\n";
        }
    }
     
    for (auto& tower : context.field.getTowers()) {
        if (tower.getIsAlive()) {
            int towerX, towerY;
            tower.getPosition(towerX, towerY);
            int distX = abs(towerX - context.targetX);
            int distY = abs(towerY - context.targetY);
            if (distX <= radius && distY <= radius) {
                tower.takeDamage(value / 2);
                hitSomething = true;
                std::cout << "Башня получает " << (value / 2) << " урона от взрыва!\n";
            }
        }
    }
    
    if (!hitSomething) {
        std::cout << "В области не было целей.\n";
    }
}

// ==================== TrapSpell ====================
TrapSpell::TrapSpell(const std::string& spellName, const std::string& spellDesc, 
                   int cost, int trapDamage, int range, int cd)
    : SpellCard(spellName, spellDesc, cost, trapDamage, range, cd) 
{}

bool TrapSpell::cast(SpellContext& context) {
    if (!checkBaseConditions(context)) return false;
    
    if (context.player.getEconomy().getMana() < manaCost) {
        std::cout << "Недостаточно маны!\n";
        return false;
    }
    
    std::cout << "Применяем " << name << "!\n";
    applyTrap(context);
    
    context.player.getEconomy().spendMana(manaCost);
    currentCooldown = cooldown;
    return true;
}

void TrapSpell::applyTrap(SpellContext& context) {
    if (context.field.placeTrap(context.targetX, context.targetY, value)) {
        std::cout << "Ловушка установлена в (" << context.targetX << "," << context.targetY << ")!\n";
    } else {
        std::cout << "Невозможно установить ловушку здесь!\n";
    }
}

// ==================== Конкретные заклинания ====================
FireballSpell::FireballSpell() 
    : DirectDamageSpell("Огненный шар", "Базовое заклинание огня", 15, 25, 4, 2) 
{}

MagicArrowSpell::MagicArrowSpell()
    : DirectDamageSpell("Магическая стрела", "Наносит прямой урон", 20, 30, 5, 3) 
{}

IceStormSpell::IceStormSpell()
    : AreaDamageSpell("Ледяная буря", "Урон по площади", 25, 20, 3, 4) 
{}

MagicTrapSpell::MagicTrapSpell()
    : TrapSpell("Магическая ловушка", "Устанавливает ловушку", 15, 25, 3, 2) 
{}