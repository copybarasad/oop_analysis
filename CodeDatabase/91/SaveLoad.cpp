#include "SaveLoad.h"
#include <fstream>
#include <iostream>

// === Player ===
void to_json(nlohmann::json& j, const Player& p) {
    j = {
        {"name", p.getName()},
        {"health", p.getHealth()},
        {"maxHealth", p.getMaxHealth()},
        {"baseDamage", p.getBaseDamage()},
        {"position", p.getPosition()},
        {"combatType", p.getCombatType()},
        {"score", p.getScore()},
        {"spellHand", p.getSpellHand()},
        {"mana", p.getMana()},
        {"maxMana", p.getMaxMana()},
        {"enemiesDefeated", p.getEnemiesDefeated()},
        {"spellPurchaseCost", p.getSpellPurchaseCost()}
    };
}

void from_json(const nlohmann::json& j, Player& p) {
    std::string name = j.at("name").get<std::string>();
    Position pos = j.at("position").get<Position>();

    Player new_player(name, pos);

    new_player.setHealth(j.at("health").get<int>());
    new_player.setMaxHealth(j.at("maxHealth").get<int>());
    new_player.setBaseDamage(j.at("baseDamage").get<int>());
    new_player.setCombatType(j.at("combatType").get<CombatType>());
    new_player.setScore(j.at("score").get<int>());
    new_player.setMana(j.at("mana").get<int>());
    new_player.setMaxMana(j.at("maxMana").get<int>());
    new_player.setEnemiesDefeated(j.at("enemiesDefeated").get<int>());
    new_player.setSpellPurchaseCost(j.at("spellPurchaseCost").get<int>());

    SpellHand spell_hand;
    j.at("spellHand").get_to(spell_hand);
    new_player.getSpellHand() = std::move(spell_hand);

    p = std::move(new_player);
}

// === SpellHand ===
void to_json(nlohmann::json& j, const SpellHand& sh) {
    j = {
        {"maxSize", sh.getMaxSize()}
    };
    j["spells"] = nlohmann::json::array();
    for (const auto& spell_ptr : sh.getSpells()) {
        if (spell_ptr) {
            j["spells"].push_back(*spell_ptr);
        }
    }
}

void from_json(const nlohmann::json& j, SpellHand& sh) {
    size_t max_size = j.at("maxSize").get<size_t>();
    SpellHand temp_hand(max_size);

    const auto& spells_json = j.at("spells");
    for (const auto& spell_json : spells_json) {
        std::unique_ptr<ISpell> spell;
        spell_json.get_to(spell);
        if (spell) {
             temp_hand.addSpell(std::move(spell));
        }
    }
    sh = std::move(temp_hand);
}

// === ISpell ===
void to_json(nlohmann::json& j, const ISpell& s) {
    if (dynamic_cast<const DamageSpell*>(&s)) {
        j = {
            {"type", "DamageSpell"},
            {"data", *static_cast<const DamageSpell*>(&s)}
        };
    } else if (dynamic_cast<const AreaSpell*>(&s)) {
        j = {
            {"type", "AreaSpell"},
            {"data", *static_cast<const AreaSpell*>(&s)}
        };
    } else if (dynamic_cast<const TrapSpell*>(&s)) {
        j = {
            {"type", "TrapSpell"},
            {"data", *static_cast<const TrapSpell*>(&s)}
        };
    } else {
        throw std::runtime_error("Unknown ISpell type for serialization");
    }
}

void from_json(const nlohmann::json& j, std::unique_ptr<ISpell>& s) {
    std::string type = j.at("type").get<std::string>();
    const auto& data = j.at("data");

    if (type == "DamageSpell") {
        auto spell = std::make_unique<DamageSpell>();
        data.get_to(*spell);
        s = std::move(spell);
    } else if (type == "AreaSpell") {
        auto spell = std::make_unique<AreaSpell>();
        data.get_to(*spell);
        s = std::move(spell);
    } else if (type == "TrapSpell") {
        auto spell = std::make_unique<TrapSpell>();
        data.get_to(*spell);
        s = std::move(spell);
    } else {
        throw std::runtime_error("Unknown ISpell type for deserialization: " + type);
    }
}

// === DamageSpell ===
void to_json(nlohmann::json& j, const DamageSpell& s) {
    j = {
        {"damage", s.getDamage()},
        {"range", s.getRange()},
        {"name", s.getName()},
        {"description", s.getDescription()},
        {"manaCost", s.getManaCost()}
    };
}

void from_json(const nlohmann::json& j, DamageSpell& s) {
    int damage = j.at("damage").get<int>();
    int range = j.at("range").get<int>();
    std::string name = j.at("name").get<std::string>();
    int cost = j.at("manaCost").get<int>();

    DamageSpell temp_spell(damage, range, name, cost);
    s = std::move(temp_spell);
}

// === AreaSpell ===
void to_json(nlohmann::json& j, const AreaSpell& s) {
    j = {
        {"damage", s.getDamage()},
        {"range", s.getRange()},
        {"areaSize", s.getAreaSize()},
        {"name", s.getName()},
        {"description", s.getDescription()},
        {"manaCost", s.getManaCost()}
    };
}

void from_json(const nlohmann::json& j, AreaSpell& s) {
    int damage = j.at("damage").get<int>();
    int range = j.at("range").get<int>();
    int area_size = j.at("areaSize").get<int>();
    std::string name = j.at("name").get<std::string>();
    int cost = j.at("manaCost").get<int>();

    AreaSpell temp_spell(damage, range, area_size, name, cost);
    s = std::move(temp_spell);
}

// === TrapSpell ===
void to_json(nlohmann::json& j, const TrapSpell& s) {
    j = {
        {"damage", s.getDamage()},
        {"range", s.getRange()},
        {"name", s.getName()},
        {"description", s.getDescription()},
        {"manaCost", s.getManaCost()}
    };
}

void from_json(const nlohmann::json& j, TrapSpell& s) {
    int damage = j.at("damage").get<int>();
    int range = j.at("range").get<int>();
    std::string name = j.at("name").get<std::string>();
    int cost = j.at("manaCost").get<int>();

    TrapSpell temp_spell(damage, range, name, cost);
    s = std::move(temp_spell);
}

// === Enemy ===
void to_json(nlohmann::json& j, const Enemy& e) {
    j = {
        {"health", e.getHealth()},
        {"maxHealth", e.getMaxHealth()},
        {"damage", e.getDamage()},
        {"position", e.getPosition()}
    };
}

void from_json(const nlohmann::json& j, Enemy& e) {
    e.loadState(j);
}

// === GameField ===
void to_json(nlohmann::json& j, const GameField& gf) {
    j = {
        {"width", gf.getWidth()},
        {"height", gf.getHeight()}
    };
    j["cells"] = nlohmann::json::array();
    for (const auto& row : gf.getCells()) {
        nlohmann::json row_json = nlohmann::json::array();
        for (const auto& cell : row) {
            row_json.push_back(cell);
        }
        j["cells"].push_back(row_json);
    }
}

void from_json(const nlohmann::json& j, GameField& gf) {
    gf.loadState(j);
}

// === ITower ===
void to_json(nlohmann::json& j, const ITower& t) {
    if (const Tower* tower_ptr = dynamic_cast<const Tower*>(&t)) {
        j = {
            {"type", "Tower"},
            {"data", *tower_ptr}
        };
    } else {
        throw std::runtime_error("Unknown ITower type for serialization");
    }
}

void from_json(const nlohmann::json& j, std::unique_ptr<ITower>& t) {
    std::string type = j.at("type").get<std::string>();
    const auto& data = j.at("data");

    if (type == "Tower") {
        auto tower = std::make_unique<Tower>(Position(0,0));
        data.get_to(*tower);
        t = std::move(tower);
    } else {
        throw std::runtime_error("Unknown ITower type for deserialization: " + type);
    }
}

void to_json(nlohmann::json& j, const Tower& t) {
    j = {
        {"position", t.getPosition()},
        {"range", t.getRange()},
        {"cooldown", t.getCooldown()},
        {"active", t.isActive()},
        {"attack_spell", t.getAttackSpell()},
        {"health", t.getHealth()}
    };
}

void from_json(const nlohmann::json& j, Tower& t) {
    t.loadState(j);
}

// === Game ===
void to_json(nlohmann::json& j, const Game& g) {
    nlohmann::json towers_json = nlohmann::json::array();
    for (const auto& tower_ptr : g.getTowers()) {
        if (tower_ptr) {
            towers_json.push_back(*tower_ptr);
        }
    }

    j = {
        {"player", g.getPlayer()},
        {"enemies", g.getEnemies()},
        {"field", g.getField()},
        {"turnCount", g.getTurnCount()},
        {"activeTraps", g.getTraps()},
        {"towers", towers_json}
    };
}

void from_json(const nlohmann::json& j, Game& g) {
    g.loadState(j);
}

// === Save/Load ===
void saveGame(const Game& game, const std::string& filepath) {
    try {
        nlohmann::json j;
        try {
            to_json(j, game);
        } catch (const std::exception& e) {
            std::cerr << "Error during serialization: " << e.what() << std::endl;
            throw;
        }

        std::ofstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file for writing: " + filepath);
        }

        file << j.dump(4);
        file.close();
        std::cout << "Game saved successfully to " << filepath << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error saving game: " << e.what() << std::endl;
    }
}

std::unique_ptr<Game> loadGame(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for reading: " + filepath);
    }

    try {
        nlohmann::json j;
        file >> j;
        file.close();

        if (j.find("field") == j.end() || j.find("player") == j.end()) {
             throw std::runtime_error("Invalid save file format: missing 'field' or 'player' keys.");
        }

        int field_width = j.at("field").at("width").get<int>();
        int field_height = j.at("field").at("height").get<int>();
        auto loaded_game = std::make_unique<Game>(field_width, field_height);

        loaded_game->loadState(j);
        return loaded_game;

    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON Parse error loading game from " << filepath << ": " << e.what() << std::endl;
        file.close();
        return nullptr; // Ошибка парсинга
    } catch (const nlohmann::json::type_error& e) {
        std::cerr << "JSON Type error loading game from " << filepath << ": " << e.what() << std::endl;
        file.close();
        return nullptr; // Ошибка типа
    } catch (const nlohmann::json::out_of_range& e) {
        std::cerr << "JSON Out of range error loading game from " << filepath << ": " << e.what() << std::endl;
        file.close();
        return nullptr; // Ошибка доступа к ключу
    } catch (const std::exception& e) {
        std::cerr << "Error loading game from " << filepath << ": " << e.what() << std::endl;
        file.close();
        return nullptr; // Любая другая ошибка
    }
}