#ifndef LEVEL_PROGRESS_TRACKER_H
#define LEVEL_PROGRESS_TRACKER_H

class ScoreSystem;
class LevelTransitionHandler;

class LevelProgressTracker {
private:
	ScoreSystem& score_system;
	LevelTransitionHandler& transition_handler;

public:
	LevelProgressTracker(ScoreSystem& ss, LevelTransitionHandler& th);
	
	bool isLevelComplete() const;
	void handleLevelCompletion();
	
	int getCurrentScore() const;
	int getScoreThreshold() const;
};

#endif







