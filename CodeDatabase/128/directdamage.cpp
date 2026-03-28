#include "directdamage.hpp"
#include "gamemodel.hpp"
#include "constants.hpp"
#include "event_dispatcher.hpp"
#include "events.hpp"
#include "entity_snapshot_factory.hpp"

#include <iostream>
#include <cmath>

bool DirectDamageSpell::UseSpell(Game& game, const SpellParams& params)
{
    auto& field     = game.getField();
    auto& player    = game.getPlayer();
    auto& enemies   = game.getEnemies();
    auto& buildings = game.getBuildings();
    auto& towers    = game.getTowers();
    EntitySnapshotFactory factory;
    int radius = params.getDirectRadius();
    EventDispatcher::instance().notify(
        UiMessageEvent("⚡ CHOOSE TARGET ⚡\n"
                       "1. Alien\n"
                       "2. Enemy building or tower\n"
                       "0. Cancel\n> ")
    );
    int targetType;
    std::cin >> targetType;
    if (targetType == 0)
        return false;
    EventDispatcher::instance().notify(
        UiMessageEvent("🎯 Enter target coordinates (x y):")
    );
    int tx, ty;
    std::cin >> tx >> ty;
    if (!game.InBounds(tx, ty)) {
        EventDispatcher::instance().notify(
            UiMessageEvent("❌ Invalid coordinates: target outside the field.")
        );
        return false;
    }
    double dist = std::hypot(tx - player.getX(), ty - player.getY());
    if (dist > radius) {
        EventDispatcher::instance().notify(
            UiMessageEvent("❌ Target out of range.")
        );
        return false;
    }
    auto casterSnap = factory.MakePlayerSnapshot(game);
    EventDispatcher::instance().notify(
        SpellUsedEvent(casterSnap, -1, tx, ty, SpellType::kDirectDamage)
    );
    if (targetType == 1)
    {
        for (auto& e : enemies)
        {
            if (e.x == tx && e.y == ty && e.enemy.isAlive())
            {
                int oldHp = e.enemy.getHealth();
                e.enemy.TakeDamage(configs::kDirectDamage);
                int newHp = e.enemy.getHealth();
                auto targetSnap = factory.MakeEnemySnapshot(e);
                EventDispatcher::instance().notify(
                    DamageEvent(casterSnap, targetSnap,
                                configs::kDirectDamage,
                                oldHp, newHp)
                );
                if (!e.enemy.isAlive()) {
                    field.At(tx, ty).clear();
                    EventDispatcher::instance().notify(
                        DeadEvent(targetSnap)
                    );
                    game.RewardPlayer(RewardType::KillEnemy);
                }
                return true;
            }
        }
        EventDispatcher::instance().notify(
            UiMessageEvent("⚠️ No alien found at (" +
                           std::to_string(tx) + "," + std::to_string(ty) + ").")
        );
        return false;
    }
    if (targetType == 2)
    {
        for (auto& b : buildings)
        {
            if (b.getX() == tx && b.getY() == ty && b.isAlive())
            {
                int oldHp = b.getHealth();
                b.TakeDamage(configs::kDirectDamage);
                int newHp = b.getHealth();
                auto targetSnap = factory.MakeBuildingSnapshot(b);
                EventDispatcher::instance().notify(
                    DamageEvent(casterSnap, targetSnap,
                                configs::kDirectDamage,
                                oldHp, newHp)
                );
                if (!b.isAlive()) {
                    field.At(tx, ty).clear();
                    EventDispatcher::instance().notify(
                        DeadEvent(targetSnap)
                    );
                    game.RewardPlayer(RewardType::DestroyStructure);
                }
                return true;
            }
        }
        for (auto& t : towers)
        {
            if (t.getX() == tx && t.getY() == ty && t.isAlive())
            {
                int oldHp = t.getHealth();
                t.TakeDamage(configs::kDirectDamage);
                int newHp = t.getHealth();
                auto targetSnap = factory.MakeTowerSnapshot(t);
                EventDispatcher::instance().notify(
                    DamageEvent(casterSnap, targetSnap,
                                configs::kDirectDamage,
                                oldHp, newHp)
                );
                if (!t.isAlive()) {
                    field.At(tx, ty).clear();
                    EventDispatcher::instance().notify(
                        DeadEvent(targetSnap)
                    );
                    game.RewardPlayer(RewardType::DestroyStructure);
                }
                return true;
            }
        }
        EventDispatcher::instance().notify(
            UiMessageEvent("⚠️ No building or tower found at (" +
                           std::to_string(tx) + "," + std::to_string(ty) + ").")
        );
        return false;
    }
    return false;
}

std::string DirectDamageSpell::getName() const noexcept {
    return "kDirectDamage";
}
