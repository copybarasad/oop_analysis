#ifndef LEVEL_MANAGER
#define LEVEL_MANAGER

#include "GameLogic.h"
#include "PlayerConfig.h"
#include "LevelTemplate.h"

class LevelManager
{
	std::vector<LevelTemplate> templates;
	void RemoveRandomHalfSpells(std::vector<std::shared_ptr<ISpell>>&, std::vector<int>&);
public:
	LevelManager(const std::vector<LevelTemplate>&);
	void LoadNextLevel(const PlayerConfig&, GameLogic&);
	void PrepareStateForNextLevel(PlayerConfig&, GameLogic&);
	int GetCountLevels() { return (int)templates.size(); };
};

#endif
