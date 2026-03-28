#include "game_serializer.h"
#include "save_exceptions.h"

json GameSerializer::PositionToJson(const Position& pos) {
    return json{{"x", pos.x}, {"y", pos.y}};
}

Position GameSerializer::JsonToPosition(const json& j) {
    return {j["x"].get<int>(), j["y"].get<int>()};
}

json GameSerializer::GameFieldToJson(const GameField& field) {
    json field_data;
    field_data["width"] = field.GetWidth();
    field_data["height"] = field.GetHeight();
    
    json cells = json::array();
    for (int y = 0; y < field.GetHeight(); ++y) {
        for (int x = 0; x < field.GetWidth(); ++x) {
            json cell;
            cell["x"] = x;
            cell["y"] = y;
            cell["type"] = static_cast<int>(field.GetCellType({x, y}));
            
            if (field.HasTrap({x, y})) {
                cell["trap_damage"] = field.GetTrapDamage({x, y});
            }
            
            cells.push_back(cell);
        }
    }
    field_data["cells"] = cells;
    
    return field_data;
}

void GameSerializer::JsonToGameField(const json& j, GameField& field) {
    int width = j["width"].get<int>();
    int height = j["height"].get<int>();
    field = GameField(width, height);
    
    for (const auto& cell_data : j["cells"]) {
        Position pos = {cell_data["x"].get<int>(), cell_data["y"].get<int>()};
        CellType type = static_cast<CellType>(cell_data["type"].get<int>());
        field.SetCell(pos, type);
        
        if (cell_data.contains("trap_damage")) {
            field.PlaceTrap(pos, cell_data["trap_damage"].get<int>());
        }
    }
}

json GameSerializer::PlayerToJson(const Player& player) {
    json player_data;
    player_data["position"] = PositionToJson(player.GetPosition());
    player_data["health"] = player.GetHealth();
    player_data["max_health"] = player.GetMaxHealth();
    player_data["score"] = player.GetScore();
    player_data["combat_mode"] = static_cast<int>(player.GetCombatMode());
    
    json spells = json::array();
    size_t hand_size = player.GetHand().GetSize();
    for (size_t i = 0; i < hand_size; ++i) {
        const Spell* spell = player.GetHand().GetSpell(i);
        if (spell) {
            spells.push_back(static_cast<int>(spell->GetType()));
        }
    }
    player_data["spells"] = spells;
    
    return player_data;
}

void GameSerializer::JsonToPlayer(const json& j, Player& player) {
    player.SetPosition(JsonToPosition(j["position"]));
    player.SetHealth(j["health"].get<int>());
    player.SetMaxHealth(j["max_health"].get<int>());
    player.SetScore(j["score"].get<int>());
    player.SetCombatMode(static_cast<CombatMode>(j["combat_mode"].get<int>()));
    
    player.ClearHand();
    for (const auto& spell_type : j["spells"]) {
        player.GetHand().AddSpecificSpell(static_cast<SpellType>(spell_type.get<int>()));
    }
}

json GameSerializer::EnemyManagerToJson(const EnemyManager& em) {
    json manager_data;
    
    json enemies = json::array();
    for (const auto& enemy : em.GetEnemies()) {
        json enemy_data;
        enemy_data["position"] = PositionToJson(enemy.GetPosition());
        enemy_data["health"] = enemy.GetHealth();
        enemy_data["damage"] = enemy.GetDamage();
        enemy_data["alive"] = enemy.IsAlive();
        enemies.push_back(enemy_data);
    }
    manager_data["enemies"] = enemies;
    
    json towers = json::array();
    for (const auto& tower : em.GetTowers()) {
        json tower_data;
        tower_data["position"] = PositionToJson(tower.GetPosition());
        tower_data["health"] = tower.GetHealth();
        tower_data["alive"] = tower.IsAlive();
        towers.push_back(tower_data);
    }
    manager_data["towers"] = towers;
    
    return manager_data;
}

void GameSerializer::JsonToEnemyManager(const json& j, EnemyManager& em) {
    em.Clear();
    
    for (const auto& enemy_data : j["enemies"]) {
        Enemy enemy(JsonToPosition(enemy_data["position"]), 
                   enemy_data["health"].get<int>(), 
                   enemy_data["damage"].get<int>());
        if (!enemy_data["alive"].get<bool>()) {
            enemy.SetHealth(0);
        }
        em.AddEnemy(enemy);
    }
    
    for (const auto& tower_data : j["towers"]) {
        Position pos = JsonToPosition(tower_data["position"]);
        EnemyTower tower(pos);
        
        bool alive = tower_data["alive"].get<bool>();
        int health = alive ? tower_data["health"].get<int>() : 0;
        tower.SetHealth(health);
        
        em.GetTowers().push_back(tower);
    }
}

void GameSerializer::ValidateGameState(const json& j) {
    if (!j.contains("version") || !j.contains("level")) {
        throw SaveFileCorrupted("Некорректный формат файла сохранения");
    }

    int level = j["level"].get<int>();
    if (level < 1 || level > 1000) {
        throw SaveFileCorrupted("Некорректный уровень: " + std::to_string(level));
    }
}
