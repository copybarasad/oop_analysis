#include "GameSaver.h"
#include <filesystem>
#include <iostream>

GameSaver::GameSaver(const std::string& fileName) : fileName_(fileName) {}

void GameSaver::save(const TokenGameState& state) {

    std::ofstream file(fileName_);
    
    if (!file.is_open()) {
        throw FileOpenError(fileName_);
    }

    try {
        json j;


        j["player"]["hp"] = state.player.hp;
        j["player"]["score"] = state.player.score;
        j["player"]["x"] = state.player_x;
        j["player"]["y"] = state.player_y;

        j["player"]["weapon"]["damage"] = state.player.weapon.base_damage;
        j["player"]["weapon"]["radius"] = state.player.weapon.base_attack_radius;
        j["player"]["weapon"]["mode"] = state.player.weapon.attack_mode;

        j["player"]["hand"]["limit"] = state.player.hand.limit;
        j["player"]["hand"]["upgrade_coef"] = state.player.hand.upgrade_coef;
        j["player"]["hand"]["spells"] = json::array();

        for (const auto& spell : state.player.hand.spells) {
            json s;
            s["type"] = spell.type;
            if (!spell.damage.empty()) s["damage"] = spell.damage;
            if (!spell.range.empty())  s["range"]  = spell.range;
            if (!spell.area.empty())   s["area"]   = spell.area;
            if (!spell.enhancement.empty()) s["enhancement"] = spell.enhancement;
            if (!spell.count_allies.empty()) s["count_allies"] = spell.count_allies;
            j["player"]["hand"]["spells"].push_back(s);
        }

        // === FIELD ===
        j["field"]["width"] = state.field.width;
        j["field"]["height"] = state.field.height;
        j["field"]["cells"] = json::array();

        for (int i = 0; i < state.field.height; ++i) {
            json row = json::array();
            for (int x = 0; x < state.field.width; ++x) {
                const auto& c = state.field.cells[i][x];
                json cj;

                cj["type"] = c.type;
                cj["has_entity"] = c.has_entity;
                cj["has_event"] = c.has_event;

                if (c.has_event) {
                    cj["event"]["type"] = c.event.type;
                    cj["event"]["damage"] = c.event.damage;
                }

                if (c.has_entity) {
                    cj["entity"]["type"] = c.entity.type;
                    cj["entity"]["hp"] = c.entity.hp;
                    cj["entity"]["damage"] = c.entity.damage;
                    cj["entity"]["can_move"] = c.entity.can_move_on_cell;
                    cj["entity"]["can_hit"] = c.entity.can_hit_on_walking;
                    cj["entity"]["not_move_next"] = c.entity.not_move_next_turn;

                    if (c.entity.type == "EnemyBuilding") {
                        cj["entity"]["steps_to_spawn"] = c.entity.steps_to_spawn;
                    }

                    if (c.entity.type == "EnemyTower") {
                        cj["entity"]["current_tick"] = c.entity.current_tick;
                        cj["entity"]["base_tick"]   = c.entity.base_tick;
                        cj["entity"]["spell"]["type"]   = c.entity.spell.type;
                        cj["entity"]["spell"]["damage"] = c.entity.spell.damage;
                        cj["entity"]["spell"]["range"]  = c.entity.spell.range;
                    }
                }

                row.push_back(cj);
            }
            j["field"]["cells"].push_back(row);
        }

        j["level"] = state.level;

        file << j.dump(4);
        
        if (!file.good()) {
            throw FileWriteError(fileName_);
        }

    } catch (const json::exception& e) {
        throw SerializationError(std::string(e.what()));
    } catch (const FileException&) {
        throw; // Пробрасываем файловые исключения дальше
    } catch (const std::exception& e) {
        throw SaveException("Unexpected error during save: " + std::string(e.what()));
    }

}


TokenGameState GameSaver::load() {
    std::ifstream file(fileName_);
    
    if (!file.is_open()) {
        throw FileNotFoundError(fileName_);
    }

    try {
        json j = json::parse(file);

        checkCorrectFile(j);
        TokenGameState state;

        // === PLAYER ===
        state.player.hp      = j["player"]["hp"];
        state.player.score   = j["player"]["score"];
        state.player_x       = j["player"]["x"];
        state.player_y       = j["player"]["y"];

        state.player.weapon.base_damage        = j["player"]["weapon"]["damage"];
        state.player.weapon.base_attack_radius = j["player"]["weapon"]["radius"];
        state.player.weapon.attack_mode        = j["player"]["weapon"]["mode"];

        state.player.hand.limit = j["player"]["hand"]["limit"];
        state.player.hand.upgrade_coef = j["player"]["hand"]["upgrade_coef"];

        for (const auto& s : j["player"]["hand"]["spells"]) {
            TokenSpell sp;
            sp.type = s["type"];
            if (s.contains("damage"))      sp.damage = s["damage"];
            if (s.contains("range"))       sp.range  = s["range"];
            if (s.contains("area"))        sp.area   = s["area"];
            if (s.contains("enhancement")) sp.enhancement = s["enhancement"];
            if (s.contains("count_allies"))sp.count_allies = s["count_allies"];
            state.player.hand.spells.push_back(sp);
        }

        // === FIELD ===
        state.field.width  = j["field"]["width"];
        state.field.height = j["field"]["height"];
        state.field.cells.resize(state.field.height);

        for (int i = 0; i < state.field.height; ++i) {
            state.field.cells[i].resize(state.field.width);
            for (int x = 0; x < state.field.width; ++x) {
                const auto& cj = j["field"]["cells"][i][x];
                auto& c = state.field.cells[i][x];

                c.type       = cj["type"];
                c.has_entity = cj["has_entity"];
                c.has_event  = cj["has_event"];

                if (c.has_event) {
                    c.event.type   = cj["event"]["type"];
                    c.event.damage = cj["event"]["damage"];
                }

                if (c.has_entity) {
                    c.entity.type = cj["entity"]["type"];
                    c.entity.hp = cj["entity"]["hp"];
                    c.entity.damage = cj["entity"]["damage"];
                    c.entity.can_move_on_cell = cj["entity"]["can_move"];
                    c.entity.can_hit_on_walking = cj["entity"]["can_hit"];
                    c.entity.not_move_next_turn = cj["entity"]["not_move_next"];

                    if (c.entity.type == "EnemyBuilding") {
                        c.entity.steps_to_spawn = cj["entity"]["steps_to_spawn"];
                    }

                    if (c.entity.type == "EnemyTower") {
                        c.entity.current_tick = cj["entity"]["current_tick"];
                        c.entity.base_tick    = cj["entity"]["base_tick"];
                        c.entity.spell.type   = cj["entity"]["spell"]["type"];
                        c.entity.spell.damage = cj["entity"]["spell"]["damage"];
                        c.entity.spell.range  = cj["entity"]["spell"]["range"];
                    }
                }
            }
        }

        if (j.contains("level") && j["level"].is_number_integer()) {
            state.level = j["level"];
        } else {
            state.level = 1;
        }

        return state;

    } catch (const json::parse_error& e) {
        throw CorruptedSaveError(fileName_ + " (parse error: " + std::string(e.what()) + ")");
    } catch (const json::exception& e) {
        throw InvalidDataError(std::string(e.what()));
    } catch (const FileException&) {
        throw;
    } catch (const LoadException&) {
        throw;
    } catch (const std::exception& e) {
        throw LoadException("Unexpected error during load: " + std::string(e.what()));
    }

}


void GameSaver::checkCorrectFile(const json& j) {
    if (!j.contains("player") || !j.contains("field")) {
        throw InvalidDataError("Missing required fields: player or field");
    }

    if (!j["player"].contains("hp") || !j["player"].contains("score")) {
        throw InvalidDataError("Missing player data");
    }

    if (!j["field"].contains("width") || !j["field"].contains("height")) {
        throw InvalidDataError("Missing field dimensions");
    }

    int w = j["field"]["width"];
    int h = j["field"]["height"];

    if (w < 10 || w > 25 || h < 10 || h > 25) {
        throw InvalidDataError("Invalid field dimensions: " + std::to_string(w) + "x" + std::to_string(h));
    }
}


static const std::string kIndexFile = ".saves_index.json";

std::vector<std::string> GameSaver::listSaves() {
	std::vector<std::string> res;
	try {
		std::ifstream ifs(kIndexFile);
		if (!ifs) return res;

		json j;
		try {
			j = json::parse(ifs);
		} catch (...) {
			return res;
		}

		if (!j.is_array()) return res;

		for (const auto& it : j) {
			if (it.is_string()) res.push_back(it.get<std::string>());
		}
	} catch (...) {}
	return res;
}

void GameSaver::addSaveToIndex(const std::string& saveFile) {
	try {
		json j = json::array();
		{
			std::ifstream ifs(kIndexFile);
			if (ifs.is_open()) {
				try { j = json::parse(ifs); } catch (...) { j = json::array(); }
			}
		}

		std::vector<std::string> existing;
		for (const auto& v : j) if (v.is_string()) existing.push_back(v);

		if (std::find(existing.begin(), existing.end(), saveFile) == existing.end()) {
			existing.push_back(saveFile);
		}

		json out = json::array();
		for (const auto& s : existing) out.push_back(s);

		std::ofstream ofs(kIndexFile, std::ios::trunc);
		if (ofs.is_open()) ofs << out.dump(2);
	} catch (...) {
	}
}

void GameSaver::rebuildIndex(const std::vector<std::string>& saves) {
    try {
        json out = json::array();
        for (const auto& s : saves) {
            out.push_back(s);
        }

        std::ofstream ofs(kIndexFile, std::ios::trunc);
        if (!ofs.is_open()) {
            throw FileOpenError(kIndexFile);
        }
        
        ofs << out.dump(2);
        
        if (!ofs.good()) {
            throw FileWriteError(kIndexFile);
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Warning: Failed to rebuild saves index: " << e.what() << "\n";
    }
}