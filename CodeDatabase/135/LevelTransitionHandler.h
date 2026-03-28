#ifndef LEVEL_TRANSITION_HANDLER_H
#define LEVEL_TRANSITION_HANDLER_H

#include <cstddef>

class GameField;
class EntityManager;
class LevelDirector;
class ScoreSystem;
class GameRenderer;
class UpgradeSystem;
class UpgradeMenu;
class SpellFactory;
class RandomGenerator;

class LevelTransitionHandler {
private:
	GameField& field;
	EntityManager& entity_manager;
	int& player_index;
	LevelDirector& level_director;
	ScoreSystem& score_system;
	GameRenderer& renderer;
	UpgradeSystem& upgrade_system;
	UpgradeMenu& upgrade_menu;
	SpellFactory& spell_factory;
	RandomGenerator& rng;
	
	struct PlayerState {
		int intelligence;
		int dexterity;
		int strength;
		int max_hp;
		bool is_melee;
		size_t hand_size;
	};
	
	PlayerState savePlayerState();
	void restorePlayerState(const PlayerState& state);

public:
	LevelTransitionHandler(GameField& f, EntityManager& em, int& player_idx,
	                       LevelDirector& ld, ScoreSystem& ss, GameRenderer& rend,
	                       UpgradeSystem& us, UpgradeMenu& um, SpellFactory& sf,
	                       RandomGenerator& random_gen);
	
	void transitionToNextLevel();
};

#endif
