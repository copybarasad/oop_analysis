#ifndef GAME_RESTART_HANDLER_H
#define GAME_RESTART_HANDLER_H

class GameField;
class EntityManager;
class LevelDirector;
class ScoreSystem;
class RandomGenerator;

class GameRestartHandler {
private:
	GameField& field;
	EntityManager& entity_manager;
	int& player_index;
	LevelDirector& level_director;
	ScoreSystem& score_system;
	RandomGenerator& rng;

public:
	GameRestartHandler(GameField& f, EntityManager& em, int& player_idx,
	                   LevelDirector& ld, ScoreSystem& ss, RandomGenerator& random_gen);
	
	void restartGame();
};

#endif
