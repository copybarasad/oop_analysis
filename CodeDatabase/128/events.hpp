#pragma once
#include <string>
#include <utility>
#include "entity_snapshot.hpp"
#include "cell.hpp"
#include "spell.hpp"

struct Event {
    virtual ~Event() = default;
};

struct MoveEvent : public Event {
    EntitySnapshot entity;
    std::pair<int,int> from;
    std::pair<int,int> to;
    CellType fromCell;
    CellType toCell;
    MoveEvent(const EntitySnapshot& snap, std::pair<int,int> f, std::pair<int,int> t, CellType fc, CellType tc)
        : entity(snap),
          from(f), to(t),
          fromCell(fc), toCell(tc)
    {}
};

struct DamageEvent : public Event {
    EntitySnapshot attacker;
    EntitySnapshot target;
    int damage;
    int oldHp;
    int newHp;
    DamageEvent(const EntitySnapshot& atk, const EntitySnapshot& tgt, int dmg, int oldHp, int newHp)
        : attacker(atk), target(tgt),
          damage(dmg), oldHp(oldHp), newHp(newHp)
    {}
};

struct DeadEvent : public Event {
    EntitySnapshot entity;
    DeadEvent(const EntitySnapshot& e)
        : entity(e)
    {}
};

struct SpellUsedEvent : public Event {
    EntitySnapshot caster;
    int spellIndex;
    int x, y;
    SpellType type;
    SpellUsedEvent(const EntitySnapshot& c, int index, int tx, int ty, SpellType t)
        : caster(c), spellIndex(index), x(tx), y(ty), type(t)
    {}
};

struct WinEvent : public Event {
    int level;
    WinEvent(int lvl)
        : level(lvl)
    {}
};

struct UiMessageEvent : public Event {
    std::string text;
    UiMessageEvent(std::string t)
        : text(std::move(t)) {}
};

struct TurnCompletedEvent : public Event {
    int turnNumber;      
    bool combatHappened; 
    TurnCompletedEvent(int turn, bool combat)
        : turnNumber(turn), combatHappened(combat) {}
};