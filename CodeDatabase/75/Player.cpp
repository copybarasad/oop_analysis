#include "Player.h"
#include "Game.h"
#include "VisualEffects.h"
#include <set>

Player::Player(int startX, int startY) 
    : Character(startX, startY, 'P', "Hero", 100, 100), 
      m_score(0), m_kills(0), m_isRangedMode(false), 
      m_meleeDamage(20), m_rangedDamage(30),
      m_isTargeting(false), m_manaRegenTimer(0.0f), m_hpRegenTimer(0.0f)
{
    addRandomSpell();
}

Player::~Player() {
    for(auto* b : m_bullets) delete b;
}

void Player::update(float dt) {
    Character::update(dt);
    updateStatusEffects();
    
    m_manaRegenTimer += dt; 
    if (m_manaRegenTimer > 0.1f) { 
        if (m_mana < m_maxMana) m_mana += 1; 
        m_manaRegenTimer = 0;
    }

    m_hpRegenTimer += dt;
    if (m_hpRegenTimer > 3.0f) { 
        if (m_health < m_maxHealth) m_health += 1;
        m_hpRegenTimer = 0;
    }

    for (auto* b : m_bullets) { if (b->isActive()) b->update(dt); }
    auto it = std::remove_if(m_bullets.begin(), m_bullets.end(), [](Bullet* b){
        if (!b->isActive()) { delete b; return true; }
        return false;
    });
    m_bullets.erase(it, m_bullets.end());
}

void Player::addToHand(Spell* spell) { 
    Spell* existing = m_hand.findSpell(spell->getType());
    if (existing) { existing->upgrade(); VisualEffects::printColored(existing->getName() + " Upgraded!", sf::Color::Yellow); delete spell; } 
    else { m_hand.addSpell(spell); }
}

Hand& Player::getHand() { return m_hand; }
const Hand& Player::getHand() const { return m_hand; }
void Player::nextSpell() { m_hand.nextSpell(); }
void Player::prevSpell() { m_hand.prevSpell(); }
int Player::getSelectedSpellIndex() const { return m_hand.getSelectedIndex(); }
void Player::addRandomSpell() { int r = rand() % 14; addToHand(new Spell((SpellType)r)); }

void Player::attack(Bullet::Direction dir) {
    if (m_mana >= 1) { 
        m_bullets.push_back(new Bullet(m_x, m_y, dir, m_rangedDamage, Bullet::Type::MAGIC_MISSILE)); 
        useMana(1); 
    } else { 
        VisualEffects::printColored("Not enough mana!", sf::Color::Blue); 
    }
}

void Player::switchAttackMode() { 
    m_isRangedMode = !m_isRangedMode; 
    std::string mode = m_isRangedMode ? "RANGED" : "MELEE"; 
    VisualEffects::printColored("Switched to " + mode, VisualEffects::Color::YELLOW); 
}

void Player::handleInput(sf::Keyboard::Key, Game*) {}