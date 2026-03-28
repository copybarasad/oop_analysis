#include "areadamage.hpp"
#include "gamemodel.hpp"
#include "constants.hpp"
#include "event_dispatcher.hpp"
#include "events.hpp"
#include "entity_snapshot_factory.hpp"

#include <iostream>
#include <cmath>

std::string AreaDamageSpell::getName() const noexcept {
    return "kAreaDamage";
}

bool AreaDamageSpell::UseSpell(Game& game, const SpellParams& params) 
{
    auto& field     = game.getField();
    auto& enemies   = game.getEnemies();
    auto& buildings = game.getBuildings();
    auto& towers    = game.getTowers();
    auto& player    = game.getPlayer();
    EntitySnapshotFactory factory;
    EventDispatcher::instance().notify(
        UiMessageEvent("🔥 AREA DAMAGE SPELL 🔥")
    );
    EventDispatcher::instance().notify(
        UiMessageEvent("🎯 Enter target coordinates (x y):")
    );
    int tx, ty;
    std::cin >> tx >> ty;
    if (!game.InBounds(tx, ty)) {
        EventDispatcher::instance().notify(
            UiMessageEvent("❌ Invalid coordinates!")
        );
        return false;
    }
    int px = player.getX();
    int py = player.getY();
    int distance = std::max(std::abs(tx - px), std::abs(ty - py));
    if (distance > configs::kAreaRadius) {
        EventDispatcher::instance().notify(
            UiMessageEvent("🚫 Target too far! Out of spell range.")
        );
        return false;
    }
    auto casterSnap = factory.MakePlayerSnapshot(game);
    EventDispatcher::instance().notify(
        SpellUsedEvent(casterSnap, -1, tx, ty, SpellType::kAreaDamage)
    );
    int side = params.getAreaSize();
    EventDispatcher::instance().notify(
        UiMessageEvent("💥 Area damage applied!")
    );
    for (int dy = 0; dy < side; ++dy) {
        for (int dx = 0; dx < side; ++dx) {

            int cx = tx + dx;
            int cy = ty + dy;
            if (!game.InBounds(cx, cy)) continue;
            for (auto& e : enemies) {
                if (e.x == cx && e.y == cy && e.enemy.isAlive()) {
                    int oldHp = e.enemy.getHealth();
                    e.enemy.TakeDamage(configs::kAreaDamage);
                    int newHp = e.enemy.getHealth();
                    auto targetSnap = factory.MakeEnemySnapshot(e);
                    EventDispatcher::instance().notify(
                        DamageEvent(casterSnap, targetSnap,
                                    configs::kAreaDamage,
                                    oldHp, newHp)
                    );
                    if (!e.enemy.isAlive()) {
                        field.At(cx, cy).clear();
                        EventDispatcher::instance().notify(DeadEvent(targetSnap));
                        game.RewardPlayer(RewardType::KillEnemy);
                    }
                }
            }
            for (auto& b : buildings) {
                if (b.getX() == cx && b.getY() == cy && b.isAlive()) {
                    int oldHp = b.getHealth();
                    b.TakeDamage(configs::kAreaDamage);
                    int newHp = b.getHealth();
                    auto targetSnap = factory.MakeBuildingSnapshot(b);
                    EventDispatcher::instance().notify(
                        DamageEvent(casterSnap, targetSnap,
                                    configs::kAreaDamage,
                                    oldHp, newHp)
                    );
                    if (!b.isAlive()) {
                        field.At(cx, cy).clear();
                        EventDispatcher::instance().notify(DeadEvent(targetSnap));
                        game.RewardPlayer(RewardType::DestroyStructure);
                    }
                }
            }
            for (auto& t : towers) {
                if (t.getX() == cx && t.getY() == cy && t.isAlive()) {
                    int oldHp = t.getHealth();
                    t.TakeDamage(configs::kAreaDamage);
                    int newHp = t.getHealth();
                    auto targetSnap = factory.MakeTowerSnapshot(t);
                    EventDispatcher::instance().notify(
                        DamageEvent(casterSnap, targetSnap,
                                    configs::kAreaDamage,
                                    oldHp, newHp)
                    );
                    if (!t.isAlive()) {
                        field.At(cx, cy).clear();
                        EventDispatcher::instance().notify(DeadEvent(targetSnap));
                        game.RewardPlayer(RewardType::DestroyStructure);
                    }
                }
            }
        }
    }
    EventDispatcher::instance().notify(
        UiMessageEvent("💨 Area damage spell used!")
    );

    return true;
}
