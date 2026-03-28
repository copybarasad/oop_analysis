#include "LevelProgressTracker.h"
#include "ScoreSystem.h"
#include "LevelTransitionHandler.h"

LevelProgressTracker::LevelProgressTracker(ScoreSystem& ss, LevelTransitionHandler& th)
	: score_system(ss), transition_handler(th) {}

bool LevelProgressTracker::isLevelComplete() const {
	return score_system.isLevelComplete();
}

void LevelProgressTracker::handleLevelCompletion() {
	transition_handler.transitionToNextLevel();
}

int LevelProgressTracker::getCurrentScore() const {
	return score_system.getScore();
}

int LevelProgressTracker::getScoreThreshold() const {
	return score_system.getThreshold();
}







