#include "game_cycle.h"
#include "direct_spell.h"
#include "area_spell.h"
#include "trap_spell.h"
#include "create_spell.h"
#include "levelup_spell.h"
#include <iostream>

FileRAII::FileRAII(const std::string& filename, std::ios::openmode mode)
    : file(filename, mode) {
    if (!file.is_open()) {
        throw FileOpenException("Cannot open file: " + filename);
    }
}

FileRAII::~FileRAII() {
    if (file.is_open()) file.close();
}

void GameCycle::save(const Field& field, const string& filename) {
    try {
        FileRAII file(filename, std::ios::binary | std::ios::trunc | std::ios::out);
        auto& f = file.stream();

        // Поле
        f << field.get_width() << ' ' << field.get_height() << '\n';
        for (int y = 0; y < field.get_height(); ++y) {
            for (int x = 0; x < field.get_width(); ++x) {
                f << field.get_tile_symbol(x, y);
            }
            f << '\n';
        }

        // Игрок
        const Player* p = field.get_player();
        if (!p) throw SaveLoadException("No player in field");

        f << field.get_player_x() << ' ' << field.get_player_y() << ' '
          << p->get_health() << ' '
          << p->get_score() << ' '
          << p->get_level() << ' '
          << (p->get_mode() == Mode::CLOSE ? 0 : 1) << '\n';

        // Уровень поля 
        f << field.get_level() << '\n';

        // Рука заклинаний
        const Hand& hand = p->get_hand();
        f << hand.size() << '\n';
        for (const auto& spell : hand.get_spells()) {
            f << spell->get_name() << '\n';
        }

        // Враги
        const auto& enemies = field.get_enemies();
        f << enemies.size() << '\n';
        for (const auto& e : enemies) {
            f << e->get_x() << ' ' << e->get_y() << ' '
              << e->get_health() << ' ' << e->get_damage() << '\n';
        }

        // Здания
        const auto& buildings = field.get_buildings();
        std::vector<const Building*> active_buildings;
        for(const auto& b : buildings) {
            if (!b->is_destroyed()) {
                active_buildings.push_back(b.get());
            }
        }
        
        f << active_buildings.size() << '\n';
        for (const auto& b : active_buildings) {
            f << b->get_x() << ' ' << b->get_y() << ' '
              << b->get_health() << ' ' << b->get_interval() << '\n';
        }

        // Башни
        const auto& towers = field.get_towers();
        f << towers.size() << '\n';
        for (const auto& t : towers) {
            f << t->get_x() << ' ' << t->get_y() << ' '
              << t->get_damage() << ' ' << t->get_radius() << ' '
              << t->get_cooldown() << ' ' << t->get_current_cooldown() << '\n';
        }

        cout << "Game saved successfully to " << filename << "!\n";
    }
    catch (const std::exception& e) {
        throw SaveLoadException(std::string("Save failed: ") + e.what());
    }
}

std::unique_ptr<Field> GameCycle::load(const string& filename) {
    try {
        FileRAII file(filename, std::ios::binary | std::ios::in);
        auto& f = file.stream();

        // Поле
        int w, h;
        f >> w >> h;
        auto field = std::make_unique<Field>(w, h);

        for (int y = 0; y < h; y++) {
            string row;
            f >> row;
            for (int x = 0; x < w; x++) {
                field->set_tile_from_symbol(x, y, row[x]);
            }
        }

        // Игрок
        int px, py, health, score, level, mode;
        f >> px >> py >> health >> score >> level >> mode;

        field->create_player(px, py, health, 20, 10);
        Player* player = field->get_player();
        player->add_score(score);
        for (int i = 0; i < level; ++i) player->add_level_up();
        if (mode == 1) player->switch_mode();

        // Уровень поля
        int game_level;
        f >> game_level;
        field->set_level(game_level);

        // Рука заклинаний
        size_t hand_size;
        f >> hand_size;
        for (size_t i = 0; i < hand_size; ++i) {
            std::string spell_name;
            f >> spell_name;

            if (spell_name == "Direct") player->get_hand().add_spell(std::make_unique<DirectDamageSpell>());
            else if (spell_name == "Area") player->get_hand().add_spell(std::make_unique<AreaDamageSpell>());
            else if (spell_name == "Trap")  player->get_hand().add_spell(std::make_unique<TrapSpell>());
            else if (spell_name == "Create") player->get_hand().add_spell(std::make_unique<CreateSpell>());
            else if (spell_name == "Level") player->get_hand().add_spell(std::make_unique<LevelupSpell>());
            else { throw SaveLoadException("Unknown spell in save file: " + spell_name); }
        }

        // Враги
        size_t enemy_count;
        f >> enemy_count;
        for (size_t i = 0; i < enemy_count; ++i) {
            int x, y, hp, dmg;
            f >> x >> y >> hp >> dmg;
            field->create_enemy(x, y, hp, dmg);
        }

        // Здания
        size_t building_count;
        f >> building_count;
        for (size_t i = 0; i < building_count; ++i) {
            int x, y, hp, interval;
            f >> x >> y >> hp >> interval;
            /*std::cout << "DEBUG: Loading building at (" << x << ", " << y 
                    << "), hp=" << hp << ", interval=" << interval << "\n";
            std::cout << "Field size: " << field->get_width() << "x" << field->get_height() << "\n";
            std::cout << "Is valid pos: " << field->is_valid_position(x, y) << "\n";
            std::cout << "Is occupied: " << field->is_cell_occupied(x, y) << "\n";*/
            field->create_building(x, y, interval, hp);
        }

        // Башни
        size_t tower_count;
        f >> tower_count;
        for (size_t i = 0; i < tower_count; ++i) {
            int x, y, dmg, radius, cd, cur_cd;
            f >> x >> y >> dmg >> radius >> cd >> cur_cd;
            field->create_tower(x, y, dmg, radius, cd);

            if (!field->get_towers().empty()) {
                field->get_towers().back()->set_current_cooldown(cur_cd);
            }
        }

        cout << "Game loaded successfully!\n";
        return field;
    }
    catch (const std::ios_base::failure&) {
        throw CorruptedSaveException("Save file is corrupted or incomplete");
    }
    catch (const std::exception& e) {
        throw FileOpenException(std::string("Load failed: ") + e.what());
    }
}