#ifndef GAMELB1_GAME_H
#define GAMELB1_GAME_H

#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "Ally.h"
#include "EnemyTower.h"
#include "Trap.h"
#include <vector>
#include <string>
#include <memory>
#include "Logger.h"

enum class GameState {
    PlayerTurn,
    Aiming,
    SelectingSpellTarget,
    UpgradeMenu,
    GameOver,
    Victory
};

class Game : public Observable {
public:
    Game(int field_width, int field_height);

    void processPlayerMove(char direction);
    void processPlayerAction(char action);
    void processPlayerAim(char direction);
    void endPlayerTurn();
    void selectSpell(int index);
    void applySpell(int target_x, int target_y);
    void cancelSpellSelection();

    void tryNextLevel();
    void upgradePlayer(int choice);
    void restartGame();
    void saveGame(const std::string& filename);
    void loadGame(const std::string& filename);
    int getCurrentLevel() const;

    const Field& getField() const;
    Player& getPlayer();
    const std::vector<Enemy>& getEnemies() const;
    const std::vector<EnemyBuilding>& getBuildings() const;
    const std::vector<Ally>& getAllies() const;
    const std::vector<EnemyTower>& getTowers() const;
    const std::vector<Trap>& getTraps() const;

    GameState getGameState() const;
    const std::string& getLastMessage() const;
    void setLastMessage(const std::string& message);

    bool damageEntityAt(int x, int y, int damage);
    void spawnAlly(int p_x, int p_y, int count, int health, int damage);
    void placeTrap(int x, int y, int damage);
    void damagePlayer(int damage);
    bool canPlaceEntityAt(int x, int y, bool ignore_player = false) const;

    const std::vector<std::pair<int, int>>& getLastShotPath() const;
    bool hasNewShotAnimation() const;
    void clearShotAnimation();

private:
    void initializeLevel();
    void updateAfterPlayerTurn();

    void updateEnemies();
    void updateBuildings();
    void updateTowers();
    void updateAllies();
    void checkTraps();

    void spawnEnemyNear(int x, int y);
    void cleanupDestroyedEntities();
    void giveSpellOnKill();
    void addRandomSpell();

    void clearEntities();

    Field field_;
    Player player_;
    std::vector<Enemy> enemies_;
    std::vector<EnemyBuilding> buildings_;
    std::vector<Ally> allies_;
    std::vector<EnemyTower> towers_;
    std::vector<Trap> traps_;

    GameState game_state_;
    std::string last_turn_message_;

    int selected_spell_index_ = -1;
    int enemies_killed_since_last_spell_ = 0;

    int current_level_ = 1;

    std::vector<std::pair<int, int>> last_shot_path_;
    bool has_new_shot_ = false;
};

#endif //GAMELB1_GAME_H