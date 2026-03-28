#include "Tcontrol.h"
#include "Tvizualizer.h"
#include "enemies.h"
#include "enemy.h"
#include "enemytower.h"
#include "field.h"
#include "hand.h"
#include "logger.h"
#include "player.h"
#include "raylib_render.h"
#include "trap.h"
#include "ui.h"

class Game {
private:
  Field *field;
  Player *player;
  Enemies enemies;

  UI ui;
  std::vector<EnemyTower> towers;
  Hand hand;
  DirectDamage directdamage;
  AreaDamage areadamage;
  GameControl<Converter> gc;
  Visualizer<RaylibRender> *drawer;
  Logger logger;
  bool area_dmg_active = false;
  std::vector<std::pair<int, int>> squareHighlight;
  std::vector<Trap> traps;
  // RaylibRender *raylibrenderer;
  unsigned int width;
  unsigned int height;
  unsigned int cellwidth;
  unsigned int cellheight;

public:
  Game();
  // void move_enemies();
  void setupController();
  void highlightArea(unsigned int centerX, unsigned int centerY,
                     unsigned int radius);
  bool isTowerAt(int x, int y);
  bool loop();

  // void mainDrawLoop();
  void removeHighlightArea();
  ~Game() {
    delete field;
    delete player;
    delete drawer;
    // delete raylibrenderer;
  }
};
