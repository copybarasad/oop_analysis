#include "Spell.h"
#include "Character.h"
#include "Game.h"
#include "VisualEffects.h"
#include "Enemy.h" 
#include "Player.h" 

void castFireball(Character* caster, Character* target, Game* game) {
    if (!game) return;
    game->addVisualEvent(VisualEvent::FIRE_STORM, caster->getX(), caster->getY(), target ? target->getX() : caster->getX(), target ? target->getY() : caster->getY(), sf::Color::Red);
    if (target) target->getDamaged(25);
}

void castIceShard(Character* caster, Character* target, Game* game) {
    if (!game || !target) return;
    game->addVisualEvent(VisualEvent::ICE_BLAST, caster->getX(), caster->getY(), target->getX(), target->getY(), sf::Color::Cyan);
    target->getDamaged(15);
    target->applyStatus(Character::Status::FROZEN, 3000);
}

void castHeal(Character* caster, Character*, Game* game) {
    if (!caster) return;
    caster->heal(30);
    if(game) game->addVisualEvent(VisualEvent::NOVA, caster->getX(), caster->getY(), 0, 0, sf::Color::Green);
}

void castLightning(Character* caster, Character* target, Game* game) {
    if (!game || !target) return;
    game->addVisualEvent(VisualEvent::LIGHTNING, caster->getX(), caster->getY(), target->getX(), target->getY(), sf::Color::Yellow);
    target->getDamaged(40);
}

void castShield(Character* caster, Character*, Game* game) {
    if (!game || !caster) return;
    game->createShield(caster->getX(), caster->getY(), SpellConstants::SHIELD_BASE_DURATION_MS, 3);
}

void castTrap(Character* caster, Character*, Game* game) {
    if (!game || !caster) return;
    game->createTrap(caster->getX(), caster->getY(), 50, 30000);
}

void castThorns(Character* caster, Character*, Game* game) {
    if (!game || !caster) return;
    game->createThornsArea(caster->getX(), caster->getY(), 4, 15000, 10);
}

void castTeleport(Character*, Character*, Game* game) {
    if (!game) return;
    game->teleportPlayer(5);
}

void castNova(Character*, Character*, Game* game) {
    if (!game) return;
    game->castNova(60, 5); 
}

void castLifeDrain(Character*, Character* target, Game* game) {
    if (!game || !target) return;
    game->castLifeDrain(target, 20);
}

void castSummonAlly(Character* caster, Character*, Game* game) {
    if (!game || !caster) return;
    game->summonAlly(caster->getX(), caster->getY());
}

void castPoisonCloud(Character* caster, Character*, Game* game) {
    if (!game || !caster) return;
    game->createThornsArea(caster->getX(), caster->getY(), 5, 10000, 5);
    game->addVisualEvent(VisualEvent::NOVA, caster->getX(), caster->getY(), 0, 0, sf::Color::Green);
}

void castTimeStop(Character* caster, Character*, Game* game) {
    if (!game) return;
    game->addVisualEvent(VisualEvent::NOVA, caster->getX(), caster->getY(), 0, 0, sf::Color::White);
    for(auto* e : game->getEnemies()) {
        if(e->isAlive()) e->applyStatus(Character::Status::FROZEN, 3000);
    }
}

void castMeteor(Character* caster, Character* target, Game* game) {
    if (!game) return;
    int tx = target ? target->getX() : caster->getX() + 5;
    int ty = target ? target->getY() : caster->getY();
    game->addVisualEvent(VisualEvent::EXPLOSION, tx, ty, 0, 0, sf::Color(255, 100, 0));
    for(auto* e : game->getEnemies()) {
        if(e->isAlive()) {
            int dx = e->getX() - tx; int dy = e->getY() - ty;
            if (dx*dx + dy*dy <= 9) e->getDamaged(100);
        }
    }
}

void castRage(Character* caster, Character*, Game* game) {
    if (!caster) return;
    if (Player* p = dynamic_cast<Player*>(caster)) {
        p->upgradeMeleeDamage(); 
        if(game) game->addVisualEvent(VisualEvent::NOVA, caster->getX(), caster->getY(), 0, 0, sf::Color::Red);
    }
}

// === FACTORY ===
Spell::Spell(SpellType type, int level) : m_type(type), m_level(level) {
    switch (type) {
        case SpellType::FIREBALL:
            m_name = "Fireball"; m_description = "Deals 25 damage";
            m_manaCost = 2; m_targetType = SpellTarget::ENEMY; m_effect = castFireball; break;
        case SpellType::ICE_SHARD:
            m_name = "Ice Shard"; m_description = "Deals 15 damage and freezes";
            m_manaCost = 2; m_targetType = SpellTarget::ENEMY; m_effect = castIceShard; break;
        case SpellType::HEAL:
            m_name = "Heal"; m_description = "Restores 30 HP";
            m_manaCost = 5; m_targetType = SpellTarget::SELF; m_effect = castHeal; break;
        case SpellType::LIGHTNING:
            m_name = "Lightning"; m_description = "Deals 40 damage";
            m_manaCost = 4; m_targetType = SpellTarget::ENEMY; m_effect = castLightning; break;
        case SpellType::SHIELD:
            m_name = "Shield"; m_description = "Creates a protective barrier";
            m_manaCost = 5; m_targetType = SpellTarget::SELF; m_effect = castShield; break;
        case SpellType::TRAP:
            m_name = "Trap"; m_description = "Explosive trap";
            m_manaCost = 3; m_targetType = SpellTarget::NO_TARGET; m_effect = castTrap; break;
        case SpellType::THORNS:
            m_name = "Thorns"; m_description = "Area damage over time";
            m_manaCost = 3; m_targetType = SpellTarget::NO_TARGET; m_effect = castThorns; break;
        case SpellType::TELEPORT:
            m_name = "Teleport"; m_description = "Blink forward";
            m_manaCost = 10; m_targetType = SpellTarget::SELF; m_effect = castTeleport; break;
        case SpellType::NOVA:
            m_name = "Nova"; m_description = "Explosion around self";
            m_manaCost = 20; m_targetType = SpellTarget::SELF; m_effect = castNova; break;
        case SpellType::LIFEDRAIN:
            m_name = "Drain"; m_description = "Steal life from enemy";
            m_manaCost = 8; m_targetType = SpellTarget::ENEMY; m_effect = castLifeDrain; break;
        case SpellType::SUMMON_ALLY:
            m_name = "Ally"; m_description = "Summons a helper";
            m_manaCost = 20; m_targetType = SpellTarget::NO_TARGET; m_effect = castSummonAlly; break;
        case SpellType::POISON_CLOUD:
            m_name = "Poison"; m_description = "Toxic cloud";
            m_manaCost = 15; m_targetType = SpellTarget::NO_TARGET; m_effect = castPoisonCloud; break;
        case SpellType::TIME_STOP:
            m_name = "TimeStop"; m_description = "Freezes enemies";
            m_manaCost = 30; m_targetType = SpellTarget::NO_TARGET; m_effect = castTimeStop; break;
        case SpellType::METEOR:
            m_name = "Meteor"; m_description = "Massive area damage";
            m_manaCost = 40; m_targetType = SpellTarget::ENEMY; m_effect = castMeteor; break;
        case SpellType::RAGE:
            m_name = "Rage"; m_description = "Increase Melee Dmg";
            m_manaCost = 25; m_targetType = SpellTarget::SELF; m_effect = castRage; break;
    }

    if (m_manaCost < 1) m_manaCost = 1;
}

Spell::Spell(const std::string& name, const std::string& description, int manaCost,
             SpellTarget targetType, std::function<void(Character* caster, Character* target, Game* game)> effect,
             int level)
    : m_type(SpellType::FIREBALL), m_name(name), m_description(description), m_manaCost(manaCost), 
      m_targetType(targetType), m_effect(effect), m_level(level) {}

const std::string& Spell::getName() const { return m_name; }
const std::string& Spell::getDescription() const { return m_description; }
int Spell::getManaCost() const { return m_manaCost; }
SpellTarget Spell::getTargetType() const { return m_targetType; }
int Spell::getLevel() const { return m_level; }

bool Spell::cast(Character* caster, Character* target, Game* game) const {
    Player* p = dynamic_cast<Player*>(caster);
    if (p) {
        if (p->getMana() >= m_manaCost) {
            p->useMana(m_manaCost);
            if (m_effect) m_effect(caster, target, game);
            return true;
        }
    } else {
        if (caster->getMana() >= m_manaCost) {
            caster->useMana(m_manaCost);
            if (m_effect) m_effect(caster, target, game);
            return true;
        }
    }
    VisualEffects::printColored("Not enough Mana!", sf::Color::Blue);
    return false;
}

void Spell::upgrade() {
    if (canUpgrade()) m_level++;
}

bool Spell::canUpgrade() const { return m_level < MAX_LEVEL; }
int Spell::getUpgradeCost() const { return BASE_UPGRADE_COST * m_level; }