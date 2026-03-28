#pragma once

class Game;
class Field;
class Player;
class EnemyManager;
class Hand;

class IGameView
{
public:
    virtual void update(const Game &) = 0;
    virtual void drawField(const Field &field) = 0;
    virtual void drawPlayer(const Player &player) = 0;
    virtual void drawEnemies(const std::vector<Position> &enemies) = 0;
    virtual void drawLevel(int level) = 0;
    virtual void drawGameOver(const Player &player) = 0;
    virtual void drawHand(const Hand &hand) = 0;
    virtual void drawImproveOptions() = 0;
    virtual void showStartMenu() = 0;
    virtual void drawSaveDir(std::string folderPath) = 0;
    virtual void drawSave() = 0;
    virtual ~IGameView() = default;
};
