#include <random>
#include <set>
#include <stdexcept>

#include "combattable.hpp"
#include "creature.hpp"
#include "team.hpp"

Creature::Creature(int hp, int maxHP) : Combattable(hp, maxHP), Statusable() {
    m_Team = Team::Neutral;
}

Creature::Creature(const Creature &creature) : Combattable(creature) {
    m_Statuses = creature.m_Statuses;
}

void Creature::setMap(Map *map) { m_Map = map; }

void Creature::move(Direction direction) {
    if (!canMove()) {
        return;
    }

    Field *oldField = m_Map->at(m_Pos);
    Field *newField = m_Map->fieldInDirection(m_Pos, direction);

    if (oldField == nullptr || newField == nullptr) {
        return;
    }

    if (newField->canOccupy()) {
        oldField->deoccupy();
        newField->occupy(this);

        std::pair<int, int> moveVec = m_Map->directionToVec2D(direction);
        m_Pos.x += moveVec.first;
        m_Pos.y += moveVec.second;

        return;
    }

    if (newField->canAttack()) {
        newField->attack(this);
    }
}

bool Creature::canMove() {
    return !Statusable::hasStatus(Status::InSlime) &&
           !Statusable::hasStatus(Status::Staggered);
}

void Creature::makeRandomMove() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 7);

    Creature::move(static_cast<Direction>(dis(gen)));
}

void Creature::update() { Statusable::update(); }

void Creature::setTeam(Team t) { m_Team = t; }
Team Creature::team() { return m_Team; }
bool Creature::isEnemy() { return m_Team == Team::Enemy; }

std::string Creature::chr() { return "*"; }
Color Creature::color() {
    if (hasStatus(Status::InSlime)) {
        return Color::lime();
    }
    return baseColor();
}
Color Creature::baseColor() { return Color::white(); }

bytedump Creature::dump() {
    bytedump bytes;
    bytes.reserve(7);

    bytes.push_back(static_cast<std::byte>(m_HP));
    bytes.push_back(static_cast<std::byte>(m_MaxHP));
    bytes.push_back(static_cast<std::byte>(m_Team));
    bytes.push_back(static_cast<std::byte>(m_Weapon.type));
    bytes.push_back(static_cast<std::byte>(m_Weapon.damage));
    bytes.push_back(static_cast<std::byte>(m_Pos.x));
    bytes.push_back(static_cast<std::byte>(m_Pos.y));

    return bytes;
}

Creature::Creature(bytedump bytes) : Combattable(0, 0) {
    if (bytes.size() < 7) {
        throw std::length_error("creature dump must contain at least 1 byte");
    }

    m_HP = static_cast<int>(bytes[0]);
    m_MaxHP = static_cast<int>(bytes[1]);
    m_Team = static_cast<Team>(bytes[2]);
    m_Weapon = {
        static_cast<WeaponType>(bytes[3]),
        static_cast<int>(bytes[4]),
    };
    m_Pos = {
        static_cast<int>(bytes[5]),
        static_cast<int>(bytes[6]),
    };
}
