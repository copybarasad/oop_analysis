#ifndef EVENTS_H
#define EVENTS_H

#include "constants.h"

struct Event
{
public:
    virtual ~Event() = default;
};

struct MoveEvent : public Event
{
    Actor m_actor;
    CellType m_oldCell;
    std::pair<int, int> m_oldPos;
    std::pair<int, int> m_newPos;

    MoveEvent(Actor actor, CellType type, std::pair<int, int> oldPos, std::pair<int, int> newPos)
    :
    m_actor(actor), m_oldCell(type), m_oldPos(oldPos), m_newPos(newPos) {}
};

struct AttackEvent : public Event
{
    Actor m_attacker;
    Actor m_target;
    int m_oldHealth; //
    int m_damage;

    AttackEvent(Actor attacker, Actor target, int oldHealth, int damage)
    :
        m_attacker(attacker), m_target(target), m_oldHealth(oldHealth), m_damage(damage) {}
};

struct DeadEvent : public Event
{
    Actor m_actor;
    std::pair<int, int> m_targetPos;
    DeadEvent(Actor actor, std::pair<int, int> targetPos) : m_actor(actor), m_targetPos(targetPos) {}
};

struct BuyCardEvent : public Event
{
    CardType m_card;
    int m_newCount;

    BuyCardEvent(CardType card, int newCount)
    :
    m_card(card), m_newCount(newCount) {}
};

struct UseSpellEvent : public Event
{
    CardType m_card;
    int m_newCount;

    UseSpellEvent(CardType card, int newCount)
    :
    m_card(card), m_newCount(newCount) {}
};


struct LevelLoadedEvent : public Event
{
    int m_newBoardSize;

    LevelLoadedEvent(int newBoardSize)
    :
    m_newBoardSize(newBoardSize) {}
};

struct NewCellEvent : public Event
{
    CellType m_type;
    std::pair<int, int> m_coord;

    NewCellEvent(CellType type, std::pair<int, int> coord)
    :
    m_type(type), m_coord(coord) {}
};

struct WinEvent : public Event
{
    int m_points;
    WinEvent(int points) : m_points(points) {}
};

#endif // EVENTS_H
