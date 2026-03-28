#ifndef LEVELSYSTEM_H
#define LEVELSYSTEM_H

class LevelSystem {
public:
    int calculateLevelSize(int level) const;
    int calculateTowerCount(int level) const;
    int calculateEnemyStrength(int level) const;
};

#endif