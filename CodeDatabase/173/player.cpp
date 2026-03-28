#include <algorithm>
#include <memory>
#include <stdexcept>

#include "creature.hpp"
#include "player.hpp"
#include "weapon/weapon.hpp"

Player::Player(int hp, int maxHP, int spellHandCap) : Creature(hp, maxHP) {
    m_Score = 0;
    m_Hand = std::make_unique<Hand>(spellHandCap);
    Creature::setTeam(Team::Player);
}

Player::Player(const Player &player)
    : Creature(player), m_Score(player.m_Score),
      m_Hand(std::make_unique<Hand>(*player.m_Hand)) {}

void Player::setSecondaryWeapon(Weapon weapon) { m_SecondaryWeapon = weapon; }

void Player::rangedAttack(Position pos) {
    if (m_Weapon.type != WeaponType::Ranged) {
        return;
    }

    Field *field = m_Map->at(pos);
    if (field == nullptr) {
        return;
    }

    if (field->canAttack()) {
        field->attack(this);
    }
}

void Player::swapWeapon() { std::swap(m_Weapon, m_SecondaryWeapon); };

Hand *Player::spellHand() { return m_Hand.get(); }

bool Player::canCastSpell(int index, Field *target) {
    return m_Hand->canCast(index, this, m_Map, target);
}

void Player::castSpell(int index, Field *target) {
    m_Hand->cast(index, this, m_Map, target);
}

std::string Player::chr() { return "@"; }
Color Player::baseColor() { return Color::magenta(); }

bytedump Player::dump() {
    bytedump bytes;

    bytes.push_back(static_cast<std::byte>(m_HP));
    bytes.push_back(static_cast<std::byte>(m_MaxHP));
    bytes.push_back(static_cast<std::byte>(m_Weapon.type));
    bytes.push_back(static_cast<std::byte>(m_Weapon.damage));
    bytes.push_back(static_cast<std::byte>(m_SecondaryWeapon.type));
    bytes.push_back(static_cast<std::byte>(m_SecondaryWeapon.damage));

    bytedump handBytes = m_Hand->dump();
    bytes.insert(bytes.end(), handBytes.begin(), handBytes.end());

    bytes.push_back(static_cast<std::byte>(m_Pos.x));
    bytes.push_back(static_cast<std::byte>(m_Pos.y));

    return bytes;
}

Player::Player(bytedump bytes) : Creature(0, 0) {
    // params (6) + hand (at least 3) + position (2)
    if (bytes.size() < 11) {
        throw std::length_error("player dump must contain at least 11 bytes");
    }

    Creature::m_HP = static_cast<int>(bytes[0]);
    Creature::m_MaxHP = static_cast<int>(bytes[1]);

    m_Weapon = {
        static_cast<WeaponType>(bytes[2]),
        static_cast<int>(bytes[3]),
    };
    m_SecondaryWeapon = {
        static_cast<WeaponType>(bytes[4]),
        static_cast<int>(bytes[5]),
    };

    Positionable::m_Pos = {
        static_cast<int>(bytes[bytes.size() - 2]),
        static_cast<int>(bytes[bytes.size() - 1]),
    };

    bytedump handBytes(bytes.begin() + 6, bytes.end() - 2);
    m_Hand = std::make_unique<Hand>(handBytes);
}

std::string Player::toString() {
    std::string res;

    res += Template("HP  : %d / %d\n").populate(m_HP, m_MaxHP);
    res += Template("DMG : %d\n").populate(m_Weapon.damage);
    res += Template("Pos : x=%d, y=%d\n").populate(m_Pos.x + 1, m_Pos.y + 1);
    res += Template("SPL : %d / %d\n").populate(m_Hand->len(), m_Hand->cap());
    res += m_Hand->toString();

    return res;
}
