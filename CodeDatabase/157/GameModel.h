#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <vector>
#include <string>
#include <cstddef>
#include "GameField.h"
#include "Player.h"
#include "Hand.h"

enum class UnitType { Enemy, Tower, Trap };

struct UnitState {
    int x, y, hp;
    UnitType type;
    int cooldown;
};

class GameModel {
public:
    GameModel(int w = 10, int h = 10, std::size_t hand_limit = 5);
    bool movePlayerTo(int x, int y);
    bool attackAt(int x, int y);
    bool useAt(std::size_t idx, int x, int y);
    void stepTurn();
    void nextLevel();
    void saveToFile(const std::string &fn) const;
    void loadFromFile(const std::string &fn);
    int width() const noexcept { return field_->width(); }
    int height() const noexcept { return field_->height(); }
    int playerX() const noexcept { return player_->x(); }
    int playerY() const noexcept { return player_->y(); }
    int playerHp() const noexcept { return player_->hp(); }
    const std::vector<UnitState> units() const;
    int level() const noexcept { return level_; }
    const std::vector<std::string> hand() const;
    bool addCard(const std::string &name);
    std::size_t handLimit() const noexcept { return player_->hand().capacity(); }
private:
    std::unique_ptr<GameField> field_;
    std::unique_ptr<Player> player_;
    int level_ = 1;
    int baseWidth_ = 10;
    int baseHeight_ = 10;
    void startLevel();
};

#endif // GAMEMODEL_H
