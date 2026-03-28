#include "spell_context_factory.h"
#include "direct_damage_context.h"
#include "area_damage_context.h"
#include "enhancement_context.h"
#include "summon_context.h"
#include "trap_context.h"
#include "spell.h"
#include "player.h"
#include "game_field.h"

DirectDamageContext SpellContextFactory::createDirectDamageContext(
    Player& player, GameField& field,
    std::vector<Enemy>& enemies,
    std::vector<EnemyBuilding>& buildings,
    std::vector<EnemyTower>& towers,
    int targetX, int targetY) {
    return DirectDamageContext{player, field, enemies, buildings, towers, targetX, targetY};
}

AreaDamageContext SpellContextFactory::createAreaDamageContext(
    Player& player, GameField& field,
    std::vector<Enemy>& enemies,
    std::vector<EnemyBuilding>& buildings,
    std::vector<EnemyTower>& towers,
    int targetX, int targetY) {
    return AreaDamageContext{player, field, enemies, buildings, targetX, targetY};
}

EnhancementContext SpellContextFactory::createEnhancementContext() {
    return EnhancementContext{};
}

SummonContext SpellContextFactory::createSummonContext(
    Player& player, GameField& field,
    std::vector<Enemy>& enemies,
    std::vector<EnemyBuilding>& buildings,
    std::vector<EnemyTower>& towers,
    std::vector<Ally>& allies,
    int targetX, int targetY) {
    return SummonContext{player, field, enemies, buildings, towers, allies, targetX, targetY};
}

TrapContext SpellContextFactory::createTrapContext(
    Player& player, GameField& field,
    int targetX, int targetY) {
    return TrapContext{player, field, targetX, targetY};
}

bool SpellContextFactory::applySpellWithContext(
    Spell& spell, Player& player, GameField& field,
    std::vector<Enemy>& enemies,
    std::vector<EnemyBuilding>& buildings,
    std::vector<EnemyTower>& towers,
    std::vector<Ally>& allies,
    int targetX, int targetY) {

    auto directCtx = createDirectDamageContext(player, field, enemies, buildings, towers, targetX, targetY);
    if (spell.apply(directCtx)) return true;

    auto areaCtx = createAreaDamageContext(player, field, enemies, buildings, towers, targetX, targetY);
    if (spell.apply(areaCtx)) return true;

    auto enhanceCtx = createEnhancementContext();
    if (spell.apply(enhanceCtx)) return true;

    auto summonCtx = createSummonContext(player, field, enemies, buildings, towers, allies, targetX, targetY);
    if (spell.apply(summonCtx)) return true;

    auto trapCtx = createTrapContext(player, field, targetX, targetY);
    if (spell.apply(trapCtx)) return true;

    return false;
}

bool SpellContextFactory::canUseSpellWithContext(
    const Spell& spell, const Player& player, const GameField& field,
    const std::vector<Enemy>& enemies,
    const std::vector<EnemyBuilding>& buildings,
    const std::vector<EnemyTower>& towers,
    const std::vector<Ally>& allies,
    int targetX, int targetY) {

    Spell& mutableSpell = const_cast<Spell&>(spell);

    auto directCtx = DirectDamageContext{
        const_cast<Player&>(player),
        const_cast<GameField&>(field),
        const_cast<std::vector<Enemy>&>(enemies),
        const_cast<std::vector<EnemyBuilding>&>(buildings),
        const_cast<std::vector<EnemyTower>&>(towers),
        targetX, targetY};
    if (mutableSpell.canUse(directCtx)) return true;

    auto areaCtx = AreaDamageContext{
        const_cast<Player&>(player),
        const_cast<GameField&>(field),
        const_cast<std::vector<Enemy>&>(enemies),
        const_cast<std::vector<EnemyBuilding>&>(buildings),
        targetX, targetY};
    if (mutableSpell.canUse(areaCtx)) return true;

    auto enhanceCtx = createEnhancementContext();
    if (mutableSpell.canUse(enhanceCtx)) return true;

    auto summonCtx = SummonContext{
        const_cast<Player&>(player),
        const_cast<GameField&>(field),
        const_cast<std::vector<Enemy>&>(enemies),
        const_cast<std::vector<EnemyBuilding>&>(buildings),
        const_cast<std::vector<EnemyTower>&>(towers),
        const_cast<std::vector<Ally>&>(allies),
        targetX, targetY};
    if (mutableSpell.canUse(summonCtx)) return true;

    auto trapCtx = TrapContext{
        const_cast<Player&>(player),
        const_cast<GameField&>(field),
        targetX, targetY};
    if (mutableSpell.canUse(trapCtx)) return true;

    return false;
}

void SpellContextFactory::applyEnhancements(Spell& spell, int enhancementLevel) {
    for (int i = 0; i < enhancementLevel; ++i) {
        spell.applyEnhancement();
    }
}