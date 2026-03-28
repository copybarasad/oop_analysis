#include "GameStateManager.h"
#include "GameException.h"
#include "EntityData.h"
#include <fstream>
#include <filesystem>

const std::string GAME_STATE_FILE = "game_save.dat";

GameStateManager::GameStateManager() : save_manager() {}

void GameStateManager::saveGame(int level, int turn, int score, int score_threshold,
                                  int field_width, int field_height, int player_index,
                                  int player_current_hp, int player_max_hp,
                                  int player_intel, int player_dex, int player_str,
                                  size_t player_hand_size, size_t player_spell_count,
                                  bool player_is_melee,
                                  int player_points, int player_kills,
                                  const std::vector<int>& field_cells,
                                  const std::vector<EntityData>& entities) {
	std::ofstream file(GAME_STATE_FILE);
	
	if (!file.is_open()) {
		throw FileWriteException(GAME_STATE_FILE, "GameStateManager::saveGame");
	}
	
	try {
		file << "GAME_SAVE_V1\n";
		file << "LEVEL=" << level << "\n";
		file << "TURN=" << turn << "\n";
		file << "SCORE=" << score << "\n";
		file << "SCORE_THRESHOLD=" << score_threshold << "\n";
		file << "FIELD_WIDTH=" << field_width << "\n";
		file << "FIELD_HEIGHT=" << field_height << "\n";
		file << "PLAYER_INDEX=" << player_index << "\n";
		file << "PLAYER_CURRENT_HP=" << player_current_hp << "\n";
		file << "PLAYER_MAX_HP=" << player_max_hp << "\n";
		file << "PLAYER_INT=" << player_intel << "\n";
		file << "PLAYER_DEX=" << player_dex << "\n";
		file << "PLAYER_STR=" << player_str << "\n";
		file << "PLAYER_HAND_SIZE=" << player_hand_size << "\n";
		file << "PLAYER_SPELL_COUNT=" << player_spell_count << "\n";
		file << "PLAYER_MELEE=" << (player_is_melee ? 1 : 0) << "\n";
		file << "PLAYER_POINTS=" << player_points << "\n";
		file << "PLAYER_KILLS=" << player_kills << "\n";
		file << "FIELD_CELL_COUNT=" << field_cells.size() << "\n";
		
		for (int cell_type : field_cells) {
			file << cell_type << " ";
		}
		file << "\n";
		
		file << "ENTITY_COUNT=" << entities.size() << "\n";
		
		for (const auto& entity_data : entities) {
			file << "ENTITY_TYPE=" << static_cast<int>(entity_data.type) << "\n";
			file << "ENTITY_INDEX=" << entity_data.index << "\n";
			file << "ENTITY_CURRENT_HP=" << entity_data.current_hp << "\n";
			file << "ENTITY_MAX_HP=" << entity_data.max_hp << "\n";
			file << "ENTITY_DAMAGE=" << entity_data.damage << "\n";
		}
		
		file.close();
		
		if (file.fail()) {
			throw FileWriteException(GAME_STATE_FILE, "GameStateManager::saveGame");
		}
	} catch (const std::exception&) {
		file.close();
		throw;
	}
}

GameStateManager::LoadedGameState GameStateManager::loadGame() {
	if (!saveFileExists()) {
		throw FileNotFoundException(GAME_STATE_FILE, "GameStateManager::loadGame");
	}
	
	std::ifstream file(GAME_STATE_FILE);
	
	if (!file.is_open()) {
		throw FileNotFoundException(GAME_STATE_FILE, "GameStateManager::loadGame");
	}
	
	LoadedGameState state;
	
	try {
		std::string line;
		std::getline(file, line);
		
		if (line != "GAME_SAVE_V1") {
			throw CorruptedDataException("Неверный формат файла сохранения", "GameStateManager::loadGame");
		}
		
		auto readValue = [&](const std::string& key) -> int {
			std::getline(file, line);
			size_t pos = line.find('=');
			if (pos == std::string::npos || line.substr(0, pos) != key) {
				throw CorruptedDataException("Ожидается ключ: " + key, "GameStateManager::loadGame");
			}
			return std::stoi(line.substr(pos + 1));
		};
		
		state.level = readValue("LEVEL");
		state.turn = readValue("TURN");
		state.score = readValue("SCORE");
		state.score_threshold = readValue("SCORE_THRESHOLD");
		state.field_width = readValue("FIELD_WIDTH");
		state.field_height = readValue("FIELD_HEIGHT");
		state.player_index = readValue("PLAYER_INDEX");
		state.player_current_hp = readValue("PLAYER_CURRENT_HP");
		state.player_max_hp = readValue("PLAYER_MAX_HP");
		state.player_intel = readValue("PLAYER_INT");
		state.player_dex = readValue("PLAYER_DEX");
		state.player_str = readValue("PLAYER_STR");
		state.player_hand_size = readValue("PLAYER_HAND_SIZE");
		state.player_spell_count = readValue("PLAYER_SPELL_COUNT");
		state.player_is_melee = readValue("PLAYER_MELEE") != 0;
		state.player_points = readValue("PLAYER_POINTS");
		state.player_kills = readValue("PLAYER_KILLS");
		
		int field_cell_count = readValue("FIELD_CELL_COUNT");
		std::getline(file, line);
		state.field_cells.clear();
		state.field_cells.reserve(field_cell_count);
		
		size_t pos = 0;
		for (int i = 0; i < field_cell_count; ++i) {
			size_t next_pos = line.find(' ', pos);
			if (next_pos == std::string::npos) {
				next_pos = line.length();
			}
			int cell_type = std::stoi(line.substr(pos, next_pos - pos));
			state.field_cells.push_back(cell_type);
			pos = next_pos + 1;
		}
		
		int entity_count = readValue("ENTITY_COUNT");
		
		for (int i = 0; i < entity_count; ++i) {
			EntityType type = static_cast<EntityType>(readValue("ENTITY_TYPE"));
			int index = readValue("ENTITY_INDEX");
			int current_hp = readValue("ENTITY_CURRENT_HP");
			int max_hp = readValue("ENTITY_MAX_HP");
			int damage = readValue("ENTITY_DAMAGE");
			
			state.entities.emplace_back(type, index, current_hp, max_hp, damage);
		}
		
		file.close();
	} catch (const std::exception&) {
		file.close();
		throw;
	}
	
	return state;
}

bool GameStateManager::saveFileExists() const {
	return std::filesystem::exists(GAME_STATE_FILE);
}
