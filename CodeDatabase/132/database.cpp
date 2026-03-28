#include "database.hpp"

DataBase::DataBase(std::string db_name): 
    db_name(db_name),
    c("dbname=" + db_name)
{
    if (!c.is_open()) {
        throw std::runtime_error("Failed to connect to database: " + db_name);
    }

    create_tables_if_not_exist();
}

void DataBase::create_tables_if_not_exist() {
    try {
        pqxx::work tx(c);
        
        tx.exec(
            "CREATE TABLE IF NOT EXISTS game_saves ("
            "id SERIAL PRIMARY KEY, "
            "save_name VARCHAR(255) UNIQUE NOT NULL, "
            "score INTEGER NOT NULL, "
            "coins INTEGER NOT NULL, "
            "time INTEGER NOT NULL, "
            "lvl INTEGER NOT NULL, "
            "cap_lvl INTEGER NOT NULL, "
            "hp_lvl INTEGER NOT NULL, "
            "width INTEGER NOT NULL, "
            "height INTEGER NOT NULL, "
            "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
            ")"
        );
        
        tx.exec(
            "CREATE TABLE IF NOT EXISTS players ("
            "id SERIAL PRIMARY KEY, "
            "game_id INTEGER NOT NULL REFERENCES game_saves(id) ON DELETE CASCADE, "
            "x INTEGER NOT NULL, "
            "y INTEGER NOT NULL, "
            "health_points INTEGER NOT NULL, "
            "can_move BOOLEAN NOT NULL, "
            "weapon_index INTEGER NOT NULL, "
            "capacity INTEGER NOT NULL, "
            "enhancement_level INTEGER NOT NULL, "
            "cards VARCHAR(255) NOT NULL"
            ")"
        );
        
        tx.exec(
            "CREATE TABLE IF NOT EXISTS mobs ("
            "id SERIAL PRIMARY KEY, "
            "game_id INTEGER NOT NULL REFERENCES game_saves(id) ON DELETE CASCADE, "
            "x INTEGER NOT NULL, "
            "y INTEGER NOT NULL, "
            "type VARCHAR(1) NOT NULL, "
            "health_points INTEGER NOT NULL, "
            "can_move BOOLEAN NOT NULL"
            ")"
        );
        
        tx.exec(
            "CREATE TABLE IF NOT EXISTS enemy_buildings ("
            "id SERIAL PRIMARY KEY, "
            "game_id INTEGER NOT NULL REFERENCES game_saves(id) ON DELETE CASCADE, "
            "x INTEGER NOT NULL, "
            "y INTEGER NOT NULL, "
            "type VARCHAR(1) NOT NULL, "
            "health_points INTEGER NOT NULL, "
            "cooldown INTEGER NOT NULL, "
            "timer INTEGER NOT NULL"
            ")"
        );
        
        tx.exec(
            "CREATE TABLE IF NOT EXISTS traps ("
            "id SERIAL PRIMARY KEY, "
            "game_id INTEGER NOT NULL REFERENCES game_saves(id) ON DELETE CASCADE, "
            "x INTEGER NOT NULL, "
            "y INTEGER NOT NULL, "
            "lvl INTEGER NOT NULL, "
            "solidity INTEGER NOT NULL"
            ")"
        );
        
        tx.exec(
            "CREATE TABLE IF NOT EXISTS landscape ("
            "id SERIAL PRIMARY KEY, "
            "game_id INTEGER NOT NULL REFERENCES game_saves(id) ON DELETE CASCADE, "
            "x INTEGER NOT NULL, "
            "y INTEGER NOT NULL, "
            "type VARCHAR(1) NOT NULL"
            ")"
        );
        
        tx.exec(
            "CREATE TABLE IF NOT EXISTS effects ("
            "id SERIAL PRIMARY KEY, "
            "game_id INTEGER NOT NULL REFERENCES game_saves(id) ON DELETE CASCADE, "
            "entity_type VARCHAR(20) NOT NULL, "
            "entity_id INTEGER NOT NULL, "
            "name VARCHAR(100) NOT NULL, "
            "initial_damage_done BOOLEAN NOT NULL, "
            "lifetime INTEGER NOT NULL, "
            "initial_damage INTEGER NOT NULL, "
            "tick_damage INTEGER NOT NULL, "
            "slowing_down BOOLEAN NOT NULL"
            ")"
        );
        
        tx.exec("CREATE INDEX IF NOT EXISTS idx_game_saves_name ON game_saves(save_name)");
        tx.exec("CREATE INDEX IF NOT EXISTS idx_players_game_id ON players(game_id)");
        tx.exec("CREATE INDEX IF NOT EXISTS idx_mobs_game_id ON mobs(game_id)");
        tx.exec("CREATE INDEX IF NOT EXISTS idx_buildings_game_id ON enemy_buildings(game_id)");
        tx.exec("CREATE INDEX IF NOT EXISTS idx_traps_game_id ON traps(game_id)");
        tx.exec("CREATE INDEX IF NOT EXISTS idx_landscape_game_id ON landscape(game_id)");
        tx.exec("CREATE INDEX IF NOT EXISTS idx_effects_game_id ON effects(game_id)");
        tx.exec("CREATE INDEX IF NOT EXISTS idx_effects_entity ON effects(entity_type, entity_id)");
        
        tx.commit();
        
    } catch (const std::exception& e) {
        std::cerr << "Error creating tables: " << e.what() << std::endl;
        throw;
    }
}

bool DataBase::save_game(const std::string& save_name, const GameImage& img) {
    try {
        pqxx::work tx(c);
        
        tx.exec(
            "INSERT INTO game_saves (save_name, score, coins, time, lvl, cap_lvl, hp_lvl, width, height) "
            "VALUES (" + tx.quote(save_name) + ", " + 
            tx.quote(img.score) + ", " + 
            tx.quote(img.coins) + ", " + 
            tx.quote(img.time) + ", " + 
            tx.quote(img.lvl) + ", " + 
            tx.quote(img.cap_lvl) + ", " +
            tx.quote(img.hp_lvl) + ", " +
            tx.quote(img.field_img.width) + ", " + 
            tx.quote(img.field_img.height) + ") " +
            "ON CONFLICT (save_name) DO UPDATE SET "
            "score = EXCLUDED.score, coins = EXCLUDED.coins, time = EXCLUDED.time, "
            "lvl = EXCLUDED.lvl, cap_lvl = EXCLUDED.cap_lvl, hp_lvl = EXCLUDED.hp_lvl, width = EXCLUDED.width, height = EXCLUDED.height"
        );
        
        auto result = tx.exec(
            "SELECT id FROM game_saves WHERE save_name = " + tx.quote(save_name)
        );
        int game_id = result[0][0].as<int>();
        
        tx.exec("DELETE FROM effects WHERE game_id = " + tx.quote(game_id));
        tx.exec("DELETE FROM players WHERE game_id = " + tx.quote(game_id));
        tx.exec("DELETE FROM mobs WHERE game_id = " + tx.quote(game_id));
        tx.exec("DELETE FROM enemy_buildings WHERE game_id = " + tx.quote(game_id));
        tx.exec("DELETE FROM traps WHERE game_id = " + tx.quote(game_id));
        tx.exec("DELETE FROM landscape WHERE game_id = " + tx.quote(game_id));
        
        save_players(tx, img.field_img.players, game_id);
        save_mobs(tx, img.field_img.enemies, "E", game_id);
        save_mobs(tx, img.field_img.allies, "A", game_id);
        save_buildings(tx, img.field_img.enemy_towers, "T", game_id);
        save_buildings(tx, img.field_img.enemy_spawners, "S", game_id);
        save_traps(tx, img.field_img.traps, game_id);
        save_landscape(tx, img.field_img.stones, "#", game_id);
        save_landscape(tx, img.field_img.swamps, "~", game_id);
        
        tx.commit();
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error saving game: " << e.what() << std::endl;
        return false;
    }
}

void DataBase::save_players(
    pqxx::work& tx, 
    const std::map<Position, PlayerImage>& players, 
    int game_id
)
{
    for (const auto& [pos, player] : players) {
        auto result = tx.exec(
            "INSERT INTO players (game_id, x, y, health_points, can_move, weapon_index, "
            "capacity, enhancement_level, cards) "
            "VALUES (" + 
            tx.quote(game_id) + ", " + 
            tx.quote(pos.x) + ", " + 
            tx.quote(pos.y) + ", " + 
            tx.quote(player.health_points) + ", " + 
            tx.quote(player.can_move) + ", " + 
            tx.quote(player.weapon_index) + ", " + 
            tx.quote(player.hand_image.capacity) + ", " + 
            tx.quote(player.hand_image.enhancement_level) + ", " + 
            tx.quote(player.hand_image.cards) + 
            ") RETURNING id"
        );
        
        int player_id = result[0][0].as<int>();
        save_effects(tx, player.effect_img, "player", player_id, game_id);
    }
}

void DataBase::save_mobs(
    pqxx::work& tx, 
    const std::map<Position, MobImage>& mobs, 
    const std::string& type, int game_id
)
{
    for (const auto& [pos, mob] : mobs) {
        auto result = tx.exec(
            "INSERT INTO mobs (game_id, x, y, type, health_points, can_move) "
            "VALUES (" + 
            tx.quote(game_id) + ", " + 
            tx.quote(pos.x) + ", " + 
            tx.quote(pos.y) + ", " + 
            tx.quote(type) + ", " + 
            tx.quote(mob.health_points) + ", " + 
            tx.quote(mob.can_move) + 
            ") RETURNING id"
        );
        
        int mob_id = result[0][0].as<int>();
        save_effects(tx, mob.effect_img, "mob", mob_id, game_id);
    }
}

void DataBase::save_buildings(
    pqxx::work& tx, 
    const std::map<Position, BuildingImage>& buildings, 
    const std::string& type, int game_id
)
{
    for (const auto& [pos, building] : buildings) {
        auto result = tx.exec(
            "INSERT INTO enemy_buildings (game_id, x, y, type, health_points, cooldown, timer) "
            "VALUES (" + 
            tx.quote(game_id) + ", " + 
            tx.quote(pos.x) + ", " + 
            tx.quote(pos.y) + ", " + 
            tx.quote(type) + ", " + 
            tx.quote(building.health_points) + ", " + 
            tx.quote(building.cooldown) + ", " + 
            tx.quote(building.timer) + 
            ") RETURNING id"
        );
        
        int building_id = result[0][0].as<int>();
        save_effects(tx, building.effect_img, "building", building_id, game_id);
    }
}

void DataBase::save_traps(pqxx::work& tx, const std::map<Position, TrapImage>& traps, int game_id) {
    for (const auto& [pos, trap] : traps) {
        tx.exec(
            "INSERT INTO traps (game_id, x, y, lvl, solidity) "
            "VALUES (" + 
            tx.quote(game_id) + ", " + 
            tx.quote(pos.x) + ", " + 
            tx.quote(pos.y) + ", " + 
            tx.quote(trap.lvl) + ", " + 
            tx.quote(trap.solidity) + ")"
        );
    }
}

void DataBase::save_landscape(
    pqxx::work& tx, 
    const std::vector<Position>& positions, 
    const std::string& type, int game_id
) 
{
    for (const auto& pos : positions) {
        tx.exec(
            "INSERT INTO landscape (game_id, x, y, type) VALUES (" + 
            tx.quote(game_id) + ", " + 
            tx.quote(pos.x) + ", " + 
            tx.quote(pos.y) + ", " + 
            tx.quote(type) + ")"
        );
    }
}

void DataBase::save_effects(
    pqxx::work& tx, const std::vector<EffectImage>& effects, 
    const std::string& entity_type, 
    int entity_id, 
    int game_id
)
{
    for (const auto& effect : effects) {
        tx.exec(
            "INSERT INTO effects (game_id, entity_type, entity_id, name, "
            "initial_damage_done, lifetime, initial_damage, tick_damage, slowing_down) "
            "VALUES (" + 
            tx.quote(game_id) + ", " + 
            tx.quote(entity_type) + ", " + 
            tx.quote(entity_id) + ", " + 
            tx.quote(effect.name) + ", " + 
            tx.quote(effect.initial_damage_done) + ", " + 
            tx.quote(effect.lifetime) + ", " + 
            tx.quote(effect.initial_damage) + ", " + 
            tx.quote(effect.tick_damage) + ", " + 
            tx.quote(effect.slowing_down) + ")"
        );
    }
}

bool DataBase::save_exists(const std::string& save_name) {
    try {
        pqxx::work tx(c);
        auto result = tx.exec(
            "SELECT COUNT(*) FROM game_saves WHERE save_name = " + tx.quote(save_name)
        );
        int count = result[0][0].as<int>();
        return count > 0;
    } catch (const std::exception& e) {
        std::cerr << "Error checking if save exists: " << e.what() << std::endl;
        return false;
    }
}

GameImage* DataBase::load_game(const std::string& save_name) {
    try {
        if (!save_exists(save_name)) {
            throw std::runtime_error("Save with name '" + save_name + "' does not exist");
        }
        
        pqxx::work tx(c);
        
        auto [score, coins, time, lvl, cap_lvl, hp_lvl, width, height] = tx.query1<int, int, int, int, int, int, int, int>(
            "SELECT score, coins, time, lvl, cap_lvl, hp_lvl, width, height FROM game_saves WHERE save_name = " + tx.quote(save_name)
        );
        
        auto [game_id] = tx.query1<int>(
            "SELECT id FROM game_saves WHERE save_name = " + tx.quote(save_name)
        );
        
        FieldImage field;
        field.width = width;
        field.height = height;
        
        load_players(tx, field, game_id);
        load_mobs(tx, field, "E", game_id);
        load_mobs(tx, field, "A", game_id);
        load_buildings(tx, field, "T", game_id);
        load_buildings(tx, field, "S", game_id);
        load_traps(tx, field, game_id);
        load_landscape(tx, field, "#", game_id);
        load_landscape(tx, field, "~", game_id);
        
        tx.commit();
        
        GameImage* img = new GameImage(score, coins, time, lvl, cap_lvl, hp_lvl, field);
        return img;
    } catch (const std::exception& e) {
        std::cerr << "Error loading game: " << e.what() << std::endl;
        return nullptr;
    }
}

void DataBase::load_players(pqxx::work& tx, FieldImage& field, int game_id) {
    auto result = tx.query<int, int, int, bool, int, int, int, std::string, int>(
        "SELECT x, y, health_points, can_move, weapon_index, "
        "capacity, enhancement_level, cards, id FROM players WHERE game_id = " + tx.quote(game_id)
    );
    
    for (const auto& [x, y, hp, can_move, weapon_idx, capacity, enh_level, cards, player_id] : result) {
        HandImage hand(capacity, enh_level, cards);
        auto effects = load_effects(tx, "player", player_id, game_id);
        
        field.players.insert({ {x, y}, PlayerImage(hp, can_move, weapon_idx, hand, effects) });
    }
}

void DataBase::load_mobs(pqxx::work& tx, FieldImage& field, const std::string& type, int game_id) {
    auto result = tx.query<int, int, int, bool, int>(
        "SELECT x, y, health_points, can_move, id FROM mobs WHERE game_id = " + 
        tx.quote(game_id) + " AND type = " + tx.quote(type)
    );
    
    auto& mob_map = (type == "E") ? field.enemies : field.allies;
    
    for (const auto& [x, y, hp, can_move, mob_id] : result) {
        auto effects = load_effects(tx, "mob", mob_id, game_id);
        mob_map.insert({{ x, y }, MobImage(hp, can_move, effects)});
    }
}

void DataBase::load_buildings(pqxx::work& tx, FieldImage& field, const std::string& type, int game_id) {
    auto result = tx.query<int, int, int, int, int, int>(
        "SELECT x, y, health_points, cooldown, timer, id FROM enemy_buildings "
        "WHERE game_id = " + tx.quote(game_id) + " AND type = " + tx.quote(type)
    );
    
    auto& building_map = (type == "T") ? field.enemy_towers : field.enemy_spawners;
    
    for (const auto& [x, y, hp, cd, timer, building_id] : result) {
        auto effects = load_effects(tx, "building", building_id, game_id);
        building_map.insert({{ x, y }, BuildingImage(hp, cd, timer, effects)});
    }
}

void DataBase::load_traps(pqxx::work& tx, FieldImage& field, int game_id) {
    auto result = tx.query<int, int, int, int>(
        "SELECT x, y, lvl, solidity FROM traps WHERE game_id = " + tx.quote(game_id)
    );
    
    for (const auto& [x, y, lvl, solidity] : result) {
        field.traps.insert({{ x, y }, TrapImage(lvl, solidity)});
    }
}

void DataBase::load_landscape(pqxx::work& tx, FieldImage& field, const std::string& type, int game_id) {
    auto result = tx.query<int, int>(
        "SELECT x, y FROM landscape WHERE game_id = " + 
        tx.quote(game_id) + " AND type = " + tx.quote(type)
    );
    
    auto& landscape_vec = (type == "#") ? field.stones : field.swamps;
    
    for (const auto& [x, y] : result) {
        Position pos(x, y);
        landscape_vec.push_back(pos);
    }
}

std::vector<EffectImage> DataBase::load_effects(
    pqxx::work& tx, 
    const std::string& entity_type, 
    int entity_id, int game_id
) 
{
    std::vector<EffectImage> effects;
    
    auto result = tx.query<std::string, bool, int, int, int, bool>(
        "SELECT name, initial_damage_done, lifetime, initial_damage, tick_damage, slowing_down "
        "FROM effects WHERE game_id = " + tx.quote(game_id) + 
        " AND entity_type = " + tx.quote(entity_type) + 
        " AND entity_id = " + tx.quote(entity_id)
    );
    
    for (const auto& [name, initial_damage_done, lifetime, initial_damage, tick_damage, slowing_down] : result) {
        effects.emplace_back(name, initial_damage_done, lifetime, initial_damage, tick_damage, slowing_down);
    }
    
    return effects;
}