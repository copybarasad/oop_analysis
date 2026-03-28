#include "save_and_load.hpp"
#include "game_controller.hpp"
#include "spell_direct_damage.hpp"
#include "spell_area.hpp"
#include "spell_trap.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <unordered_map>
#include <stdexcept>

class Save_load_error: public std::runtime_error{
    public:
        Save_load_error(const std::string& message): std::runtime_error("[Save/Load Error] " + message){}
};

class Save_write_error: public Save_load_error{
    public:
        Save_write_error(const std::string& path): Save_load_error("Failed to write to file: " + path){}
};

class Save_read_error: public Save_load_error{
    public:
        Save_read_error(const std::string& path): Save_load_error("Failed to read from file: " + path){}
};

class Save_file_not_found: public Save_load_error{
    public:
        Save_file_not_found(const std::string& path): Save_load_error("Save file not found: " + path){}
};

class Save_corrupted_data: public Save_load_error{
    public:
        Save_corrupted_data(const std::string& what): Save_load_error("Corrupted save data: " + what){}
};

std::string Save_and_load::escape_string(const std::string& s){
    std::string result;
    for(char c: s){
        result += c;
    }
    return result;
}

std::string Save_and_load::unescape_string(std::string& s){
    std::string result;
    for(int i = 0; i < s.length(); i++){
        result += s[i];
    }
    return result;
}

int Save_and_load::parse_int(const std::string& s, const std::string& field_name){
    std::istringstream iss(s);
    int value;
    if(!(iss >> value) || (iss >> std::ws).peek() != EOF) throw Save_corrupted_data("Invalid integer in field '" + field_name + "': " + s);
    return value;
}

void Save_and_load::validate_directory(std::string& dir_path){
    try{
        std::filesystem::create_directories(dir_path);
    } catch (...){
        throw Save_write_error("Cannot create directory: " + dir_path);
    }
}

void Save_and_load::save_game(Game_controller& game, const std::string& filename){
    std::string dir = "saves";
    std::string full_path = (std::filesystem::path(dir) / filename).string();
    validate_directory(dir);
    std::ofstream file(full_path);
    if(!file.is_open()) throw Save_write_error(full_path);
    auto& p = game.get_player();
    file << "GAME SAVE FILE\n";

    file << "field_width=" << game.get_game_field().get_width() << "\n";
    file << "field_height=" << game.get_game_field().get_height() << "\n\n";

    file << "[PLAYER]\n";
    file << "health=" << p.get_health() << "\n";
    file << "max_health=" << p.get_max_health() << "\n";
    file << "close_damage=" << p.get_close_damage() << "\n";
    file << "ranged_damage=" << p.get_ranged_damage() << "\n";
    file << "combat_style=" << static_cast<int>(p.get_combat_style()) << "\n";
    file << "score=" << p.get_score() << "\n";
    file << "position_x=" << p.get_position().get_x() << "\n";
    file << "position_y=" << p.get_position().get_y() << "\n\n";

    auto& field = game.get_game_field();
    file << "[FIELD]\n";
    file << "width=" << field.get_width() << "\n";
    file << "height=" << field.get_height() << "\n";
    for(int y = 0; y < field.get_height(); y++){
        for(int x = 0; x < field.get_width(); x++){
            Cell_type type = field.get_cell_type(Position(x, y));
            file << (type == Cell_type::BARRICADE ? '1' : '0');
        }
        file << "\n";
    }
    file << "\n";

    file << "current_level=" << game.get_current_level() << "\n";

    auto& enemies = game.get_enemies();
    file << "[ENEMIES]\n";
    file << "enemies_count=" << enemies.size() << "\n";
    for(int i = 0; i < enemies.size(); i++){
        auto& e = enemies[i];
        file << "enemy_" << i << "_health=" << e->get_health() << "\n";
        file << "enemy_" << i << "_damage=" << e->get_damage() << "\n";
        file << "enemy_" << i << "_x=" << e->get_position().get_x() << "\n";
        file << "enemy_" << i << "_y=" << e->get_position().get_y() << "\n";
        file << "enemy_" << i << "_alive=" << (e->is_alive() ? 1 : 0) << "\n";
    }
    file << "\n";

    auto& towers = game.get_towers();
    file << "[TOWERS]\n";
    file << "towers_count=" << towers.size() << "\n";
    for(int i = 0; i < towers.size(); i++){
        auto& t = towers[i];
        file << "tower_" << i << "_x=" << t.get_position().get_x() << "\n";
        file << "tower_" << i << "_y=" << t.get_position().get_y() << "\n";
        file << "tower_" << i << "_damage=" << t.get_damage() << "\n";
        file << "tower_" << i << "_radius=" << t.get_attack_radius() << "\n";
        file << "tower_" << i << "_cooldown=" << t.get_cooldown() << "\n";
        file << "tower_" << i << "_max_cooldown=" << t.get_max_cooldown() << "\n";
    }
    file << "\n";

    auto& traps = game.get_traps();
    file << "[TRAPS]\n";
    file << "traps_count=" << traps.size() << "\n";
    for(int i = 0; i < traps.size(); i++){
        auto& trap = traps[i];
        file << "trap_" << i << "_x=" << trap.pos.get_x() << "\n";
        file << "trap_" << i << "_y=" << trap.pos.get_y() << "\n";
        file << "trap_" << i << "_damage=" << trap.damage << "\n";
    }
    file << "\n";

    auto& spells = p.get_hand().get_spells();
    file << "[SPELLS]\n";
    file << "spells_count=" << spells.size() << "\n";
    for(int i = 0; i < spells.size(); i++){
        auto& spell = spells[i];
        file << "spell_" << i << "_name=" << escape_string(spell->get_name()) << "\n";
        file << "spell_" << i << "_damage=" << spell->get_damage() << "\n";
        file << "spell_" << i << "_radius=" << spell->get_radius() << "\n";
        if(dynamic_cast<const Direct_damage_spell*>(spell.get())) file << "spell_" << i << "_type=direct\n";
        else if(dynamic_cast<const Area_spell*>(spell.get())) file << "spell_" << i << "_type=area\n";
        else if(dynamic_cast<const Trap_spell*>(spell.get())) file << "spell_" << i << "_type=trap\n";
    }
}

std::unique_ptr<Game_controller> Save_and_load::load_game(const std::string& filename){
    std::string full_path = (std::filesystem::path("saves") / filename).string();
    if(!std::filesystem::exists(full_path) || !std::filesystem::is_regular_file(full_path)) throw Save_file_not_found(full_path);
    std::ifstream file(full_path);
    if(!file.is_open()) throw Save_read_error(full_path);
    std::unordered_map<std::string, std::string> data;
    std::string line;
    while(std::getline(file, line)){
        if(line.empty() || line[0] == '#' || line.find('[') == 0) continue;
        int eq = line.find('=');
        if(eq == std::string::npos) continue;
        std::string key = line.substr(0, eq);
        std::string value = line.substr(eq + 1);
        data[key] = value;
    }

    if(data.find("field_width") == data.end() || data.find("field_height") == data.end()) throw Save_corrupted_data("Missing field dimensions");
    int loaded_width = parse_int(data.at("field_width"), "field_width");
    int loaded_height = parse_int(data.at("field_height"), "field_height");

    std::vector<std::vector<Cell_type>> field_grid(loaded_height, std::vector<Cell_type>(loaded_width));
    std::vector<std::string> field_rows;
    bool in_field = false;
    file.clear();
    file.seekg(0);
    while(std::getline(file, line)){
        if(line == "[FIELD]"){
            in_field = true;
            continue;
        }
        if(in_field){
            if(line.empty() || line.find('[') == 0) break;
            if(line.find('=') != std::string::npos) continue;
            field_rows.push_back(line);
        }
    }
    for(int y = 0; y < loaded_height && y < field_rows.size(); y++){
        const std::string& row = field_rows[y];
        for(int x = 0; x < loaded_width && x < row.size(); x++){
            field_grid[y][x] = (row[x] == '1') ? Cell_type::BARRICADE : Cell_type::EMPTY;
        }
    }

    auto controller = std::make_unique<Game_controller>(loaded_height, loaded_width, std::move(field_grid));
    controller->clear_all_entities();

    int level = data.count("current_level") ? parse_int(data.at("current_level"), "current_level") : 1;
    controller->set_current_level(level);

    try{
        int health = parse_int(data.at("health"), "health");
        int close_damage = parse_int(data.at("close_damage"), "close_damage");
        int ranged_damage = parse_int(data.at("ranged_damage"), "ranged_damage");
        int score = parse_int(data.at("score"), "score");
        int x = parse_int(data.at("position_x"), "position_x");
        int y = parse_int(data.at("position_y"), "position_y");
        Position pos(x, y);
        controller->reset_player(health, close_damage, ranged_damage, pos);
        controller->get_player().add_score(score);
        if(parse_int(data.at("combat_style"), "combat_style") == 1){
            controller->get_player().switch_combat_style(Combat_style::RANGED_COMBAT);
        }
        controller->get_player().update_alive_state();
    } catch(...){
        throw Save_corrupted_data("Player data");
    }

    int enemy_count = data.count("enemies_count") ? parse_int(data.at("enemies_count"), "enemies_count") : 3;
    controller->set_initial_enemy_count(enemy_count);
    for(int i = 0; i < enemy_count; i++){
        try{
            std::string prefix = "enemy_" + std::to_string(i) + "_";
            int health = parse_int(data.at(prefix + "health"), prefix + "health");
            int damage = parse_int(data.at(prefix + "damage"), prefix + "damage");
            int x = parse_int(data.at(prefix + "x"), prefix + "x");
            int y = parse_int(data.at(prefix + "y"), prefix + "y");
            bool alive = parse_int(data.at(prefix + "alive"), prefix + "alive") != 0;
            Position pos(x, y);
            auto enemy = std::make_unique<Enemy>(health, damage, pos);
            if(!alive) enemy->take_damage(1000);
            controller->add_enemy(std::move(enemy));
        } catch(...){
            throw Save_corrupted_data("Enemy " + std::to_string(i));
        }
    }

    int tower_count = data.count("towers_count") ? parse_int(data.at("towers_count"), "towers_count") : 0;
    for(int i = 0; i < tower_count; i++){
        try{
            std::string prefix = "tower_" + std::to_string(i) + "_";
            int x = parse_int(data.at(prefix + "x"), prefix + "x");
            int y = parse_int(data.at(prefix + "y"), prefix + "y");
            int damage = parse_int(data.at(prefix + "damage"), prefix + "damage");
            int radius = parse_int(data.at(prefix + "radius"), prefix + "radius");
            int cooldown = parse_int(data.at(prefix + "cooldown"), prefix + "cooldown");
            int max_cd = parse_int(data.at(prefix + "max_cooldown"), prefix + "max_cooldown");
            Enemy_tower tower(Position(x, y), damage, radius);
            tower.set_cooldown(cooldown);
            tower.set_max_cooldown(max_cd);
            controller->add_tower(tower);
        } catch(...){
            throw Save_corrupted_data("Tower " + std::to_string(i));
        }
    }

    int trap_count = data.count("traps_count") ? parse_int(data.at("traps_count"), "traps_count") : 0;
    for(int i = 0; i < trap_count; i++){
        try{
            std::string prefix = "trap_" + std::to_string(i) + "_";
            int x = parse_int(data.at(prefix + "x"), prefix + "x");
            int y = parse_int(data.at(prefix + "y"), prefix + "y");
            int damage = parse_int(data.at(prefix + "damage"), prefix + "damage");
            controller->add_trap({Position(x, y), damage});
        } catch(...){
            throw Save_corrupted_data("Trap " + std::to_string(i));
        }
    }

    Player_hand new_hand(controller->get_player().get_hand().get_max_size());
    new_hand.clear();
    int spell_count = data.count("spells_count") ? parse_int(data.at("spells_count"), "spells_count") : 0;
    for(int i = 0; i < spell_count; i++){
        try{
            std::string prefix = "spell_" + std::to_string(i) + "_";
            std::string name = unescape_string(data.at(prefix + "name"));
            int damage = parse_int(data.at(prefix + "damage"), prefix + "damage");
            int radius = parse_int(data.at(prefix + "radius"), prefix + "radius");
            std::string type = data.at(prefix + "type");

            std::unique_ptr<Spell> spell;
            if(type == "direct") spell = std::make_unique<Direct_damage_spell>(name, damage, radius);
            else if(type == "area") spell = std::make_unique<Area_spell>(name, damage, radius);
            else if(type == "trap") spell = std::make_unique<Trap_spell>(name, damage, radius);
            if(spell && new_hand.can_add_spell()) new_hand.add_spell(std::move(spell));
        } catch(...){
            throw Save_corrupted_data("Spell " + std::to_string(i));
        }
    }

    controller->get_player().set_hand(std::move(new_hand));
    return controller;
}