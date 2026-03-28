#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

class Game;

class LevelManager {
public:
    static int calculateEnemyCount(int level);

    static int calculateBuildingCount(int level);

    static int calculateTowerCount(int level);

    static int calculateFieldWidth(int level);

    static int calculateFieldHeight(int level);

    static int calculateEnemyHealth(int level);

    static int calculateEnemyDamage(int level);

    static int calculateEnemyMinDamage(int level);

    static int calculateEnemyMaxDamage(int level);

    static int getScoreTarget(int level);

    static bool isLevelComplete(const Game &game);

    static void initializeLevel(Game &game, int level);
};

#endif
