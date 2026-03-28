#include "ActorStats.hpp"


Stats::Stats(long long prevLevelUp, long long experience, long long levelUp, int level) {
    this->prevLevelUpExperience = prevLevelUp;
    this->currentExperience = experience;
    this->levelUpExperience = levelUp;
    this->level = level;
    this->levelIncreased = false;
}


Stats::Stats() : Stats(0, 0, 100, 1) {}


Stats::~Stats() {}


void Stats::addExperience(int experience) {
    this->currentExperience += experience;
    if (this->currentExperience >= this->levelUpExperience) {
        this->level++;
        this->prevLevelUpExperience = this->levelUpExperience;
        this->levelUpExperience = this->levelUpExperience + 50 * this->level;
        levelIncreased = true;
    }
}


int Stats::getLevel() {
    return this->level;
}


std::vector<long long> Stats::getExperience() {
    return {this->prevLevelUpExperience, this->currentExperience, this->levelUpExperience};
}

bool Stats::checkAndSwitchLevelIncreased() {
    if (levelIncreased) {
        levelIncreased = false;
        return true;
    }
    return false;
}


StatsSaveData Stats::getStatsSavedata() {
    StatsSaveData data;
    data.currentExperience = this->currentExperience;
    data.level = this->level;
    data.levelIncreased = this->levelIncreased;
    data.levelUpExperience = this->levelUpExperience;
    data.prevLevelUpExperience = this->prevLevelUpExperience;
    return data;
}


void Stats::setStatsSavedata(StatsSaveData data) {
    this->currentExperience = data.currentExperience;
    this->level = data.level;
    this->levelIncreased = data.levelIncreased;
    this->levelUpExperience = data.levelUpExperience;
    this->prevLevelUpExperience = data.prevLevelUpExperience;
}