#include "ScoreSystem.h"

ScoreSystem::ScoreSystem() : current_score(0), score_threshold(100) {}

ScoreSystem::ScoreSystem(int threshold) : current_score(0), score_threshold(threshold) {}

int ScoreSystem::getScoreForEntityType(EntityType type) const {
	switch (type) {
		case EntityType::ENEMY:
			return 10;
		case EntityType::BARRACKS:
			return 50;
		case EntityType::TOWER:
			return 30;
		case EntityType::ALLY:
			return 0;
		case EntityType::PLAYER:
			return 0;
		case EntityType::UNDEFINED:
			return 0;
	}
	return 0;
}

void ScoreSystem::addScoreForKill(EntityType entity_type) {
	current_score += getScoreForEntityType(entity_type);
}

void ScoreSystem::addScore(int points) {
	current_score += points;
}

int ScoreSystem::getScore() const {
	return current_score;
}

int ScoreSystem::getThreshold() const {
	return score_threshold;
}

void ScoreSystem::setThreshold(int threshold) {
	score_threshold = threshold;
}

bool ScoreSystem::isLevelComplete() const {
	return current_score >= score_threshold;
}

void ScoreSystem::reset() {
	current_score = 0;
}

void ScoreSystem::resetScore() {
	current_score = 0;
}
