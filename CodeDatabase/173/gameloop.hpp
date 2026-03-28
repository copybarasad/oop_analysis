#ifndef GAMELOOP_HPP
#define GAMELOOP_HPP

#include <chrono>
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <random>
#include <stdexcept>
#include <thread>
#include <vector>

#include "../ally.hpp"
#include "../enemy.hpp"
#include "../map/map.hpp"
#include "../player.hpp"
#include "../save/save.hpp"
#include "../spell/factory.hpp"
#include "../utils/input.hpp"
#include "../utils/xdg.hpp"
#include "../weapon/factory.hpp"
#include "message/message.hpp"

class GameLoop : public Dumpable {
  public:
    GameLoop();

    void run();
    void runOnce();
    void runSession();
    bool runLevel();

    void onLose();
    void onWin();

    bool hasNextLevel();
    void loadNextLevel();
    Field *selectField();

    void playerMove();
    bool hasAliveEnemies();
    void allyMove();
    void enemyMove();
    void buildingMove();
    void finalizeMove();
    void mapUpdate();

    void levelTransition();
    void suggestUpgrades();

    void save();
    bytedump dump() override;
    bytedump dumpAllies();
    bytedump dumpEnemies();
    bytedump dumpBuildings();

    void loadSelf(bytedump bytes);
    void loadMap(bytedump bytes);
    void loadPlayer(bytedump bytes);
    void loadAllies(bytedump bytes);
    void loadEnemies(bytedump bytes);
    void loadBuildings(bytedump bytes);
    void load();

    bool trySave();
    bool tryLoad();

    // TODO: make a separate class for rendering.
    void render();

    void help();
    void kyo();

    // TODO: make a separate class for handling user input.
    Message *receiveInput();

  private:
    int m_Level;
    Map *m_Map;
    Player *m_Player;

    SpellFactory *m_SpellFactory;
    WeaponFactory *m_WeaponFactory;

    std::vector<Ally *> m_Allies;
    std::vector<Enemy *> m_Enemies;
    std::vector<Building *> m_Buildings;

    void loadLevel1();
    void loadLevel2();
    void loadLevel3();

    void collectSpawnedCreatures();
};

#endif /* GAMELOOP_HPP */
