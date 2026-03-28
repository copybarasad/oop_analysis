#ifndef SPELL_CONTEXT_FACTORY_H
#define SPELL_CONTEXT_FACTORY_H

#include <vector>
#include <memory>

class Player;
class GameField;
class Enemy;
class EnemyBuilding;
class EnemyTower;
class Ally;
class Spell;

class DirectDamageContext;
class AreaDamageContext;
class EnhancementContext;
class SummonContext;
class TrapContext;

class SpellContextFactory {
public:
    static DirectDamageContext createDirectDamageContext(
        Player& player, GameField& field,
        std::vector<Enemy>& enemies,
        std::vector<EnemyBuilding>& buildings,
        std::vector<EnemyTower>& towers,
        int targetX, int targetY);

    static AreaDamageContext createAreaDamageContext(
        Player& player, GameField& field,
        std::vector<Enemy>& enemies,
        std::vector<EnemyBuilding>& buildings,
        std::vector<EnemyTower>& towers,
        int targetX, int targetY);

    static EnhancementContext createEnhancementContext();

    static SummonContext createSummonContext(
        Player& player, GameField& field,
        std::vector<Enemy>& enemies,
        std::vector<EnemyBuilding>& buildings,
        std::vector<EnemyTower>& towers,
        std::vector<Ally>& allies,
        int targetX, int targetY);

    static TrapContext createTrapContext(
        Player& player, GameField& field,
        int targetX, int targetY);

    static bool applySpellWithContext(
        Spell& spell, Player& player, GameField& field,
        std::vector<Enemy>& enemies,
        std::vector<EnemyBuilding>& buildings,
        std::vector<EnemyTower>& towers,
        std::vector<Ally>& allies,
        int targetX, int targetY);

    static bool canUseSpellWithContext(
        const Spell& spell, const Player& player, const GameField& field,
        const std::vector<Enemy>& enemies,
        const std::vector<EnemyBuilding>& buildings,
        const std::vector<EnemyTower>& towers,
        const std::vector<Ally>& allies,
        int targetX, int targetY);

    static void applyEnhancements(Spell& spell, int enhancementLevel);
};

#endif