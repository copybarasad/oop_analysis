#ifndef GAME_VISUALIZER_HPP
#define GAME_VISUALIZER_HPP

#include <vector>

class GameField;
class Player;
class Enemy;

template <typename TRenderer> 
class GameVisualizer {
public:
  GameVisualizer() = default;

  explicit GameVisualizer(const TRenderer &renderer) : renderer_(renderer) {}

  void renderMainMenu(bool hasSave) { renderer_.renderMainMenu(hasSave); }
  void renderInvalidMenuChoice() { renderer_.renderInvalidMenuChoice(); }
  void renderGameOver(bool won) { renderer_.renderGameOver(won); }
  void renderLevelHeader() { renderer_.renderLevelHeader(); }
  void renderStatus(const Player &player) { renderer_.renderStatus(player); }
  void renderEnemyInfo(const std::vector<Enemy> &enemies) {
    renderer_.renderEnemyInfo(enemies);
  }
  void renderField(const GameField &field, const Player &player,
                   const std::vector<Enemy> &enemies) {
    renderer_.renderField(field, player, enemies);
  }

private:
  TRenderer renderer_{};
};

#endif