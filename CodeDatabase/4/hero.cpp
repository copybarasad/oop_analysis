#include "hero.hpp"
#include "field.hpp"
#include "entitymanager.hpp"
#include "fireball.hpp"
#include "sunstrike.hpp"
#include "trap.hpp"
#include "allyspell.hpp"
#include "upgradespell.hpp"
#include <random>

Hero::Hero()
    : Entity(0, 150, 100), max_health(150), handle(6) { 
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 4);
    std::unique_ptr<Spell> initial_spell = nullptr;
    switch (dis(gen)) {
        case 0: initial_spell = std::make_unique<Fireball>(); break;
        case 1: initial_spell = std::make_unique<Sunstrike>(); break;
        case 2: initial_spell = std::make_unique<TrapSpell>(); break;
        case 3: initial_spell = std::make_unique<AllySpell>(); break;
        case 4: initial_spell = std::make_unique<UpgradeSpell>(); break;
    }
    if (initial_spell) {
        handle.add_spell(std::move(initial_spell));
    }
}

void Hero::swap_weapon() {
    if (weapon == SWORD) {
        weapon = BOW;
        damage = bow_damage;
        atack_distance = high;
    } else {
        weapon = SWORD;
        damage = sword_damage;
        atack_distance = low;
    }
}

int Hero::get_hp() const {
    return health;
}

bool Hero::canMoveTo(const std::vector<std::vector<FieldCell>>& box, int x, int y) const {
    if (x < 0 || x >= static_cast<int>(box[0].size()) ||
        y < 0 || y >= static_cast<int>(box.size())) {
        return false;
    }
    return box[y][x].get_entity() == ZERO && box[y][x].get_type() != HARD;
}

std::string Hero::get_weapon_name() const {
    switch (weapon) {
        case SWORD: return "Sword";
        case BOW:   return "Bow";
        default:    return "Unknown";
    }
}

int Hero::move(std::vector<std::vector<FieldCell>>& box, int& x, int& y, char action) {
    if (skip_turn) {
        skip_turn = false;
        return 0;
    }
    int dx = 0, dy = 0;
    bool is_direction = false;
    switch (action) {
        case 'w': dy = 1; is_direction = true; break;
        case 'a': dx = -1; is_direction = true; break;
        case 's': dy = -1; is_direction = true; break;
        case 'd': dx = 1; is_direction = true; break;
        case 'q': return -1;
        default: 
            std::cout << "Invalid move!\n";
            return 0;
    }
    if (!is_direction) return 0;
    int width = box[0].size();
    int height = box.size();
    for (int step = 1; step <= atack_distance; ++step) {
        int tx = x + dx * step;
        int ty = y + dy * step;
        if (tx < 0 || tx >= width || ty < 0 || ty >= height) break;
        EntityType ent = box[ty][tx].get_entity();
        int id = box[ty][tx].id;
        if (ent == MOB || ent == TOWER || ent == ATTACK_TOWER) {
            return id;
        }
        if (box[ty][tx].get_type() == HARD) {
            break;
        }
        if (ent != ZERO && ent != HERO) {
            break;
        }
    }
    int new_x = x + dx;
    int new_y = y + dy;
    if (!canMoveTo(box, new_x, new_y)) {
        return 0;
    }
    box[y][x] = FieldCell(box[y][x].get_type());
    x = new_x;
    y = new_y;
    box[y][x] = FieldCell(box[y][x].get_type(), HERO, 0);
    if (box[y][x].get_type() == SLOW) {
        skip_turn = true;
    }
    return 0;
}

int Hero::get_mana() const {
    return mana;
}

void Hero::waste_mana(int cost) {
    mana -= cost;
}

void Hero::take_mana(int cost) {
    mana += cost;
}

void Hero::show_spell_shop() {
    std::cout << "Choose spell:" << std::endl;
    std::cout << "===============" << std::endl;
    std::cout << 0 << " Fireball" << std::endl;
    std::cout << "Damage: " << 100 << std::endl;
    std::cout << "Distance: " << 9 << std::endl;
    std::cout << "Cost: " << 10 << std::endl;
    std::cout << "===============" << std::endl;
    std::cout << 1 << " Sunstrike" << std::endl;
    std::cout << "Damage: " << 100 << std::endl;
    std::cout << "Square: " << 2 << std::endl;
    std::cout << "Cost: " << 30 << std::endl;
    std::cout << "===============" << std::endl;
    std::cout << 2 << " Trap" << std::endl;
    std::cout << "Damage: " << 100 << std::endl;
    std::cout << "Distance: " << 3 << std::endl;
    std::cout << "Cost: " << 15 << std::endl;
    std::cout << "=============" << std::endl;
    std::cout << 3 << " Ally" << std::endl;
    std::cout << "Cost: " << 20 << std::endl;
    std::cout << "=============" << std::endl;
    std::cout << 4 << " Upgrade" << std::endl;
    std::cout << "Cost: " << 30 << std::endl;
}

void Hero::get_spell() {
    int temp_id;
    int temp_mana;
    show_spell_shop();
    std::cin >> temp_id;
    std::unique_ptr<Spell> new_spell = nullptr;
    switch(temp_id) {
        case 0: new_spell = std::make_unique<Fireball>(); break;
        case 1: new_spell = std::make_unique<Sunstrike>(); break;
        case 2: new_spell = std::make_unique<TrapSpell>(); break;
        case 3: new_spell = std::make_unique<AllySpell>(); break;
        case 4: new_spell = std::make_unique<UpgradeSpell>(); break;
    }
    if (!new_spell) return;
    temp_mana = get_mana();
    if (temp_mana >= new_spell->cost()) {
        waste_mana(new_spell->cost());
        if (!handle.add_spell(std::move(new_spell))) {
            std::cout << "Hand is full!\n";
        }
    } else {
        std::cout << "Not enough mana!\n";
    }
}

bool Hero::use_spell(int spell_index, Field& field, EntityManager& roster, int tx, int ty) {
    if (spell_index < 0 || static_cast<size_t>(spell_index) >= handle.size()) {
        return false;
    }
    auto spell = handle.use_spell(spell_index);
    if (!spell) return false;
    if (spell->cost() > mana) {
        handle.add_spell(std::move(spell));
        std::cout << "Not enough mana!\n";
        return false;
    }
    if (dynamic_cast<UpgradeSpell*>(spell.get())) {
        mana -= spell->cost();
        spell->cast(field, roster, *this, tx, ty); 
        return true;
    }
    for (int i = 0; i < upgrade_level; ++i) {
        spell->upgrade();
    }
    bool success = spell->cast(field, roster, *this, tx, ty);
    if (success) {
        mana -= spell->cost();
        upgrade_level = 0;
        return true;
    } else {
        handle.add_spell(std::move(spell));
        return false;
    }
}

std::unique_ptr<Spell> Hero::create_spell_by_id(int id) {
    switch (id) {
        case 0: return std::make_unique<Fireball>();
        case 1: return std::make_unique<Sunstrike>();
        case 2: return std::make_unique<TrapSpell>();
        case 3: return std::make_unique<AllySpell>();
        case 4: return std::make_unique<UpgradeSpell>();
        default: return nullptr;
    }
}