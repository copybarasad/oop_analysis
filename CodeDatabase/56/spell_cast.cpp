#include "spell_cast.h"
#include "combat_system.h"

bool SpellCast::cast(Spell& spell, Player& caster, Field& field, int target_x, int target_y) {
    if (!checkMana(caster, spell)) {
        return false;
    }
    
    switch(spell.getType()) {
        case 1:
            return castDirectSpell(spell, caster, field, target_x, target_y);
            
        case 2:
            return castAreaSpell(spell, caster, field, target_x, target_y);
            
        case 3:
            return castTrapSpell(spell, caster, field, target_x, target_y);
            
        default:
            return false;
    }
}

bool SpellCast::castDirectSpell(Spell& spell, Player& caster, Field& field, int target_x, int target_y) {
    DirectedSpell& direct_spell = static_cast<DirectedSpell&>(spell);
    
    if (!checkRange(caster, target_x, target_y, direct_spell.getRange())) {
        return false;
    }
    
    if (!isValidTarget(field, target_x, target_y)) {
        return false;
    }
    
    const auto& enemies = field.getEnemies();
    for (size_t i = 0; i < enemies.size(); i++) {
        if (enemies[i]->getX() == target_x && enemies[i]->getY() == target_y) {
            enemies[i]->takeDamage(direct_spell.getDamage());
            if (enemies[i]->getHP() <= 0) {
                field.setTypePlace(target_x, target_y, PLACE_EMPTY);
                field.removeEnemy(i);
            }
            caster.reduceMana(spell.getManaCost());
            return true;
        }
    }
    
    int building_count = field.getCntBuilding();
    for (int i = 0; i < building_count; i++) {
        Building* building = field.getBuild(i);
        if (building && building->getX() == target_x && building->getY() == target_y) {
            building->takeDamage(direct_spell.getDamage());
            if (building->getHP() <= 0) {
                field.setTypePlace(target_x, target_y, PLACE_EMPTY);
                field.removeBuilding(i);
            }
            caster.reduceMana(spell.getManaCost());
            return true;
        }
    }
    
    return false;
}
bool SpellCast::castAreaSpell(Spell& spell, Player& caster, Field& field, int target_x, int target_y) {
    AreaSpell& area_spell = static_cast<AreaSpell&>(spell);
    
    if (!checkRange(caster, target_x, target_y, area_spell.getRange())) {
        return false;
    }
    
    if (target_x < 0 || target_x >= field.getWidth() || 
        target_y < 0 || target_y >= field.getHeight()) {
        return false;
    }
    
    
    for (int dx = 0; dx < area_spell.getAreaX(); dx++) {
        for (int dy = 0; dy < area_spell.getAreaY(); dy++) {
            int current_x = target_x + dx;
            int current_y = target_y + dy;
            
            if (current_x < 0 || current_x >= field.getWidth() || 
                current_y < 0 || current_y >= field.getHeight()) {
                continue;
            }
            
            const auto& enemies = field.getEnemies();
            for (size_t i = 0; i < enemies.size(); i++) {
                if (enemies[i]->getX() == current_x && enemies[i]->getY() == current_y) {
                    enemies[i]->takeDamage(area_spell.getDamage());
                    if (enemies[i]->getHP() <= 0) {
                        field.setTypePlace(current_x, current_y, PLACE_EMPTY);
                        field.removeEnemy(i);
                    }
                }
            }
            
            int building_count = field.getCntBuilding();
            for (int i = 0; i < building_count; i++) {
                Building* building = field.getBuild(i);
                if (building && building->getX() == current_x && building->getY() == current_y) {
                    building->takeDamage(area_spell.getDamage());
                    if (building->getHP() <= 0) {
                        field.setTypePlace(current_x, current_y, PLACE_EMPTY);
                        field.removeBuilding(i);
                    }
                }
            }
            
            int tower_count = field.getCntSpellTower();
            for (int i = 0; i < tower_count; i++) {
                SpellTower* tower = field.getSpellTower(i);
                if (tower && tower->getX() == current_x && tower->getY() == current_y) {
                    tower->takeDamage(area_spell.getDamage());
                    if (tower->getHP() <= 0) {
                        field.setTypePlace(current_x, current_y, PLACE_EMPTY);
                        field.removeSpellTower(i);
                    }
                }
            }
        }
    }
    

    caster.reduceMana(spell.getManaCost());
    return true;
}

bool SpellCast::castTrapSpell(Spell& spell, Player& caster, Field& field, int target_x, int target_y) {
    TrapSpell& trap_spell = static_cast<TrapSpell&>(spell);
    
    if (!checkRange(caster, target_x, target_y, trap_spell.getRange())) {
        return false;
    }
    
    if (!field.Place_is_valid(target_x, target_y)) {
        return false;
    }
    
    field.setEffectPlace(target_x, target_y, "trap");  
    
    caster.reduceMana(spell.getManaCost());
    return true;
}

bool SpellCast::checkMana(Player& caster, Spell& spell) {
    return caster.getMana() >= spell.getManaCost();
}

bool SpellCast::checkRange(Player& caster, int target_x, int target_y, int range) {
    int distance = std::max(std::abs(caster.getX() - target_x), 
                           std::abs(caster.getY() - target_y));
    return distance <= range;
}

bool SpellCast::isValidTarget(Field& field, int x, int y) {
    if (x < 0 || x >= field.getWidth() || y < 0 || y >= field.getHeight()) {
        return false;
    }
    
    int cell_type = field.getTypePlace(x, y);
    if (cell_type == PLACE_PLAYER || cell_type >= PLACE_ENEMY) {
        return true;
    }
    
    return false;
}
bool SpellCast::castFromTower(Spell& spell, Field& field, int target_x, int target_y) {
    if (spell.getType() != 1) {
        return false;
    }
    return castDirectSpellFromTower(spell, field, target_x, target_y);
}

bool SpellCast::castDirectSpellFromTower(Spell& spell, Field& field, int target_x, int target_y) {
    DirectedSpell& direct_spell = static_cast<DirectedSpell&>(spell);
    
    if (!isValidTarget(field, target_x, target_y)) {
        return false;
    }
    
    Player* player = field.getPlayer();
    if (player && player->getX() == target_x && player->getY() == target_y) {
        player->takeDamage(direct_spell.getDamage());
        return true;
    }
    
    return false;
}