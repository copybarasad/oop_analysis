#ifndef SCORE_SYSTEM_H
#define SCORE_SYSTEM_H

#include "IEntity.h"

class ScoreSystem {
private:
	int current_score;
	int score_threshold;
	
	int getScoreForEntityType(EntityType type) const;

public:
	ScoreSystem();
	explicit ScoreSystem(int threshold);
	
	void addScoreForKill(EntityType entity_type);
	void addScore(int points);
	
	int getScore() const;
	int getThreshold() const;
	void setThreshold(int threshold);
	
	bool isLevelComplete() const;
	void reset();
	void resetScore();
};

#endif
