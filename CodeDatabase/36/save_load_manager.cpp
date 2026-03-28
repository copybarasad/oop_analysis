#include "save_load_manager.h"
#include "enemy.h"
#include "enemy_base.h"
#include "enemy_tower.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h"
#include "summon_spell.h"
#include "enhancement_spell.h"
// УДАЛЕНО: #include "console_ui.h" 
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>

void SaveLoadManager::SaveGame(const World& world, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw SaveLoadException("Failed to open file for saving: " + filename);
    }

    try {
        SerializeWorld(world, file);
    } catch (const std::exception& e) {
        file.close();
        throw SaveLoadException("Error during saving: " + std::string(e.what()));
    }

    file.close();
}

World SaveLoadManager::LoadGame(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw SaveLoadException("Failed to open file for loading: " + filename);
    }

    try {
        World world = DeserializeWorld(file);
        file.close();
        
        // УДАЛЕНО: ConsoleUI::PrintWorld(world); 
        // Причина: Теперь за отрисовку отвечает GameDriver::Run -> Visualizer::Update
        
        return world;
    } catch (const std::exception& e) {
        file.close();
        throw SaveLoadException("Error during loading: " + std::string(e.what()));
    }
}

void SaveLoadManager::SerializeWorld(const World& world, std::ofstream& file) {
    int level = world.GetLevel();
    int turn = world.GetTurn();
    file.write(reinterpret_cast<const char*>(&level), sizeof(level));
    file.write(reinterpret_cast<const char*>(&turn), sizeof(turn));

    int width = world.GetField().GetWidth();
    int height = world.GetField().GetHeight();
    file.write(reinterpret_cast<const char*>(&width), sizeof(width));
    file.write(reinterpret_cast<const char*>(&height), sizeof(height));

    const Player& player = world.GetPlayer();
    int health = player.Health();
    int max_health = player.MaxHealth();
    int damage = player.Damage();
    bool is_ranged = player.IsRanged();
    bool skip_move = player.ShouldSkipMove();
    auto pos = player.GetPosition();
    int max_spells = player.GetMaxSpells();
    int enhancement_stacks = player.GetEnhancementStacks();
    int max_enhancement_stacks = player.GetMaxEnhancementStacks();

    file.write(reinterpret_cast<const char*>(&health), sizeof(health));
    file.write(reinterpret_cast<const char*>(&max_health), sizeof(max_health));
    file.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
    file.write(reinterpret_cast<const char*>(&is_ranged), sizeof(is_ranged));
    file.write(reinterpret_cast<const char*>(&skip_move), sizeof(skip_move));
    file.write(reinterpret_cast<const char*>(&pos.first), sizeof(pos.first));
    file.write(reinterpret_cast<const char*>(&pos.second), sizeof(pos.second));
    file.write(reinterpret_cast<const char*>(&max_spells), sizeof(max_spells));
    file.write(reinterpret_cast<const char*>(&enhancement_stacks), sizeof(enhancement_stacks));
    file.write(reinterpret_cast<const char*>(&max_enhancement_stacks), sizeof(max_enhancement_stacks));

    const auto& spells = player.GetSpells();
    int spell_count = spells.size();
    file.write(reinterpret_cast<const char*>(&spell_count), sizeof(spell_count));
    for (const auto& spell : spells) {
        std::string name = spell->Name();
        int name_len = name.size();
        file.write(reinterpret_cast<const char*>(&name_len), sizeof(name_len));
        file.write(name.c_str(), name_len);
        if (name == "Direct Damage") {
            DirectDamageSpell* dd = dynamic_cast<DirectDamageSpell*>(spell.get());
            if (dd) {
                int dmg = dd->GetDamage();
                int rad = dd->GetRadius();
                file.write(reinterpret_cast<const char*>(&dmg), sizeof(dmg));
                file.write(reinterpret_cast<const char*>(&rad), sizeof(rad));
            }
        } else if (name == "Area Damage") {
            AreaDamageSpell* ad = dynamic_cast<AreaDamageSpell*>(spell.get());
            if (ad) {
                int dmg = ad->GetDamage();
                int area = ad->GetArea();
                file.write(reinterpret_cast<const char*>(&dmg), sizeof(dmg));
                file.write(reinterpret_cast<const char*>(&area), sizeof(area));
            }
        } else if (name == "Trap") {
            TrapSpell* ts = dynamic_cast<TrapSpell*>(spell.get());
            if (ts) {
            }
        } else if (name == "Summon") {
            SummonSpell* ss = dynamic_cast<SummonSpell*>(spell.get());
            if (ss) {
                int count = ss->GetCount();
                file.write(reinterpret_cast<const char*>(&count), sizeof(count));
            }
        } else if (name == "Enhancement") {
            EnhancementSpell* es = dynamic_cast<EnhancementSpell*>(spell.get());
            if (es) {
            }
        }
    }

    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            const Cell& cell = world.GetField().CellAt(r, c);
            CellType type = cell.GetType();
            int enemy_idx = cell.GetEnemyIndex();
            int trap_dmg = cell.GetTrapDamage();
            file.write(reinterpret_cast<const char*>(&type), sizeof(type));
            file.write(reinterpret_cast<const char*>(&enemy_idx), sizeof(enemy_idx));
            file.write(reinterpret_cast<const char*>(&trap_dmg), sizeof(trap_dmg));
        }
    }

    const auto& enemies = world.GetEnemies();
    int enemy_count = enemies.size();
    file.write(reinterpret_cast<const char*>(&enemy_count), sizeof(enemy_count));
    for (const auto& enemy : enemies) {
        int health = enemy.Health();
        int damage = enemy.Damage();
        file.write(reinterpret_cast<const char*>(&health), sizeof(health));
        file.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
    }

    const auto& bases = world.GetBases();
    int base_count = bases.size();
    file.write(reinterpret_cast<const char*>(&base_count), sizeof(base_count));
    for (const auto& base : bases) {
        auto pos = base.Position();
        int health = base.Health();
        int damage = base.Damage();
        int spawn_interval = base.GetSpawnInterval();
        int counter = base.GetCounter();
        file.write(reinterpret_cast<const char*>(&pos.first), sizeof(pos.first));
        file.write(reinterpret_cast<const char*>(&pos.second), sizeof(pos.second));
        file.write(reinterpret_cast<const char*>(&health), sizeof(health));
        file.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
        file.write(reinterpret_cast<const char*>(&spawn_interval), sizeof(spawn_interval));
        file.write(reinterpret_cast<const char*>(&counter), sizeof(counter));
    }

    const auto& towers = world.GetTowers();
    int tower_count = towers.size();
    file.write(reinterpret_cast<const char*>(&tower_count), sizeof(tower_count));
    for (const auto& tower : towers) {
        auto pos = tower.Position();
        int health = tower.Health();
        int damage = tower.DirectDamageSpell();
        int attack_radius = tower.GetAttackRadius();
        int cast_interval = tower.GetCastInterval();
        int counter = tower.GetCounter();
        file.write(reinterpret_cast<const char*>(&pos.first), sizeof(pos.first));
        file.write(reinterpret_cast<const char*>(&pos.second), sizeof(pos.second));
        file.write(reinterpret_cast<const char*>(&health), sizeof(health));
        file.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
        file.write(reinterpret_cast<const char*>(&attack_radius), sizeof(attack_radius));
        file.write(reinterpret_cast<const char*>(&cast_interval), sizeof(cast_interval));
        file.write(reinterpret_cast<const char*>(&counter), sizeof(counter));
    }
}

World SaveLoadManager::DeserializeWorld(std::ifstream& file) {
    int level, turn;
    file.read(reinterpret_cast<char*>(&level), sizeof(level));
    file.read(reinterpret_cast<char*>(&turn), sizeof(turn));

    int width, height;
    file.read(reinterpret_cast<char*>(&width), sizeof(width));
    file.read(reinterpret_cast<char*>(&height), sizeof(height));

    int health, max_health, damage, max_spells, enhancement_stacks, max_enhancement_stacks;
    bool is_ranged, skip_move;
    std::pair<int,int> pos;
    file.read(reinterpret_cast<char*>(&health), sizeof(health));
    file.read(reinterpret_cast<char*>(&max_health), sizeof(max_health));
    file.read(reinterpret_cast<char*>(&damage), sizeof(damage));
    file.read(reinterpret_cast<char*>(&is_ranged), sizeof(is_ranged));
    file.read(reinterpret_cast<char*>(&skip_move), sizeof(skip_move));
    file.read(reinterpret_cast<char*>(&pos.first), sizeof(pos.first));
    file.read(reinterpret_cast<char*>(&pos.second), sizeof(pos.second));
    file.read(reinterpret_cast<char*>(&max_spells), sizeof(max_spells));
    file.read(reinterpret_cast<char*>(&enhancement_stacks), sizeof(enhancement_stacks));
    file.read(reinterpret_cast<char*>(&max_enhancement_stacks), sizeof(max_enhancement_stacks));

    int spell_count;
    file.read(reinterpret_cast<char*>(&spell_count), sizeof(spell_count));
    std::vector<std::unique_ptr<Spell>> spells;
    for (int i = 0; i < spell_count; ++i) {
        int name_len;
        file.read(reinterpret_cast<char*>(&name_len), sizeof(name_len));
        std::string name(name_len, '\0');
        file.read(&name[0], name_len);
        if (name == "Direct Damage") {
            int dmg, rad;
            file.read(reinterpret_cast<char*>(&dmg), sizeof(dmg));
            file.read(reinterpret_cast<char*>(&rad), sizeof(rad));
            auto spell = std::make_unique<DirectDamageSpell>(dmg, rad);
            spells.push_back(std::move(spell));
        } else if (name == "Area Damage") {
            int dmg, area;
            file.read(reinterpret_cast<char*>(&dmg), sizeof(dmg));
            file.read(reinterpret_cast<char*>(&area), sizeof(area));
            auto spell = std::make_unique<AreaDamageSpell>(dmg, area);
            spells.push_back(std::move(spell));
        } else if (name == "Trap") {
            auto spell = std::make_unique<TrapSpell>();
            spells.push_back(std::move(spell));
        } else if (name == "Summon") {
            int count;
            file.read(reinterpret_cast<char*>(&count), sizeof(count));
            auto spell = std::make_unique<SummonSpell>(count);
            spells.push_back(std::move(spell));
        } else if (name == "Enhancement") {
            auto spell = std::make_unique<EnhancementSpell>();
            spells.push_back(std::move(spell));
        }
    }

    Player player(max_health, damage, max_spells + 1);
    player.SetHealth(health);
    player.SetMaxHealth(max_health);
    player.SetDamage(damage);
    if (is_ranged) player.SwitchCombatMode();
    if (skip_move) player.SetSkipNextMove(true);
    player.SetPosition(pos.first, pos.second);
    player.SetEnhancementStacks(enhancement_stacks);
    player.SetMaxEnhancementStacks(max_enhancement_stacks);
    for (auto& spell : spells) {
        player.AddSpell(std::move(spell));
    }

    World world(width, height, level, std::move(player));
    world.SetTurn(turn);

    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            CellType type;
            int enemy_idx, trap_dmg;
            file.read(reinterpret_cast<char*>(&type), sizeof(type));
            file.read(reinterpret_cast<char*>(&enemy_idx), sizeof(enemy_idx));
            file.read(reinterpret_cast<char*>(&trap_dmg), sizeof(trap_dmg));
            Cell& cell = const_cast<Field&>(world.GetField()).CellAt(r, c);
            cell.SetType(type);
            cell.SetEnemyIndex(enemy_idx);
            cell.SetTrapDamage(trap_dmg);
        }
    }

    world.MutablePlayer().SetPosition(pos.first, pos.second);
    const_cast<Field&>(world.GetField()).CellAt(pos.first, pos.second).SetType(CellType::Player);

    int enemy_count;
    file.read(reinterpret_cast<char*>(&enemy_count), sizeof(enemy_count));
    std::vector<Enemy> enemies;
    for (int i = 0; i < enemy_count; ++i) {
        int health, damage;
        file.read(reinterpret_cast<char*>(&health), sizeof(health));
        file.read(reinterpret_cast<char*>(&damage), sizeof(damage));
        enemies.emplace_back(health, damage);
    }
    world.ClearEnemies();
    world.SetEnemies(std::move(enemies));

    int base_count;
    file.read(reinterpret_cast<char*>(&base_count), sizeof(base_count));
    std::vector<EnemyBase> bases;
    for (int i = 0; i < base_count; ++i) {
        int pos_r, pos_c, health, damage, spawn_interval, counter;
        file.read(reinterpret_cast<char*>(&pos_r), sizeof(pos_r));
        file.read(reinterpret_cast<char*>(&pos_c), sizeof(pos_c));
        file.read(reinterpret_cast<char*>(&health), sizeof(health));
        file.read(reinterpret_cast<char*>(&damage), sizeof(damage));
        file.read(reinterpret_cast<char*>(&spawn_interval), sizeof(spawn_interval));
        file.read(reinterpret_cast<char*>(&counter), sizeof(counter));
        bases.emplace_back(pos_r, pos_c, spawn_interval, damage);
        bases.back().SetHealth(health);
        bases.back().SetCounter(counter);
    }
    world.ClearBases();
    world.SetBases(std::move(bases));

    int tower_count;
    file.read(reinterpret_cast<char*>(&tower_count), sizeof(tower_count));
    std::vector<EnemyTower> towers;
    for (int i = 0; i < tower_count; ++i) {
        int pos_r, pos_c, health, damage, attack_radius, cast_interval, counter;
        file.read(reinterpret_cast<char*>(&pos_r), sizeof(pos_r));
        file.read(reinterpret_cast<char*>(&pos_c), sizeof(pos_c));
        file.read(reinterpret_cast<char*>(&health), sizeof(health));
        file.read(reinterpret_cast<char*>(&damage), sizeof(damage));
        file.read(reinterpret_cast<char*>(&attack_radius), sizeof(attack_radius));
        file.read(reinterpret_cast<char*>(&cast_interval), sizeof(cast_interval));
        file.read(reinterpret_cast<char*>(&counter), sizeof(counter));
        towers.emplace_back(pos_r, pos_c, health, attack_radius, damage, cast_interval);
        towers.back().SetCounter(counter);
    }
    world.ClearTowers();
    world.SetTowers(std::move(towers));

    world.MutablePlayer().RemoveSpellAt(world.MutablePlayer().GetSpellCount() - 1);
    player.DecreaseMaxSpells();

    return world;
}
