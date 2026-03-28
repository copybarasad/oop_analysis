#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

#include "SaveManager.h"
#include "EntityData.h"
#include <vector>

class GameField;
class EntityManager;
class EntityFactory;
class RandomGenerator;
class ScoreSystem;
class LevelManager;

class GameStateManager {
private:
	SaveManager save_manager;
	
public:
	GameStateManager();
	
	void saveGame(int level, int turn, int score, int score_threshold,
	              int field_width, int field_height, int player_index,
	              int player_current_hp, int player_max_hp,
	              int player_intel, int player_dex, int player_str,
	              size_t player_hand_size, size_t player_spell_count,
	              bool player_is_melee,
	              int player_points, int player_kills,
	              const std::vector<int>& field_cells,
	              const std::vector<EntityData>& entities);
	
	struct LoadedGameState {
		int level;
		int turn;
		int score;
		int score_threshold;
		int field_width;
		int field_height;
		int player_index;
		int player_current_hp;
		int player_max_hp;
		int player_intel;
		int player_dex;
		int player_str;
		size_t player_hand_size;
		size_t player_spell_count;
		bool player_is_melee;
		int player_points;
		int player_kills;
		std::vector<int> field_cells;
		std::vector<EntityData> entities;
	};
	
	LoadedGameState loadGame();
	
	bool saveFileExists() const;
};

#endif
