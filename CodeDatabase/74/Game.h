#ifndef GAME_H
#define GAME_H

#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "Spell.h"
#include "InputHandler.h"
#include "Types.h"
#include <vector>
#include <memory>

class Game {
private:
    bool running;
    Turn turn;
    PlayerActionMode actionMode;
    Field field;
    Player player;
    std::vector<Enemy> enemies;
    int currentSpellIndex = -1;
    Coords aimPos{ -1, -1 };
    int move;
    std::unique_ptr<InputHandler> inputHandler;
    int currentLevelIndex = 0;

    // Новые поля для рендеринга
    std::string currentMessage;
    bool aimActive = false;
    bool splashAim = false;
    int radius = 0;
    bool trapAim = false;
    bool showHand = false;

public:
    Game(int width, int height, int numOfEnemies, int numOfTowers, int numOfUnwalkableCells);

    void start();
    void stop();


    // --- �������� ������ ---
    void handleInput(Command cmd);
    void enemiesTurn();
    void switchTurn();
    bool allEnemiesDead() const;

    // --- ������ ---
    void castSpell(int index);
    int selectSpellFromHand();
    void applySplashSpell(std::shared_ptr<Spell> spell, const Coords& center);
    void placeTrap(std::shared_ptr<Spell> spell, const Coords& pos, int currentSpellIndex);

    // --- ��������������� ---
    bool isCellOccupied(int x, int y);
    bool isCellOccupiedByEnemy(int x, int y) const;

    void updateInputHandler();

    Player& getPlayer() { return player; }
    Field& getField() { return field; }
    std::vector<Enemy>& getEnemies() { return enemies; }
    Coords& getAimPos() { return aimPos; }
    PlayerActionMode& getActionMode() { return actionMode; }
    int& getCurrentSpellIndex() { return currentSpellIndex; }
    bool is_running() { return running; }

    // Новые const геттеры для рендеринга
    const Field& getField() const { return field; }
    const Player& getPlayer() const { return player; }
    const std::vector<Enemy>& getEnemies() const { return enemies; }
    std::string getCurrentMessage() const { return currentMessage; }
    bool isAimActive() const { return aimActive; }
    Coords getAimPos() const { return aimPos; }
    bool isSplashAim() const { return splashAim; }
    int getRadius() const { return radius; }
    bool isTrapAim() const { return trapAim; }
    const Hand& getHand() const { return player.getHand(); }
    bool getShowHand() const { return showHand; }

    // Сеттеры для рендеринга
    void setCurrentMessage(const std::string& msg) { currentMessage = msg; }
    void setAim(bool active, const Coords& pos, bool splash = false, int r = 0, bool trap = false) {
        aimActive = active;
        aimPos = pos;
        splashAim = splash;
        radius = r;
        trapAim = trap;
    }
    void setShowHand(bool show) { showHand = show; }

    json serialize() const;
    void deserialize(const json& j);
    void save() const;

    void setCurrentLevelIndex(int index) { currentLevelIndex = index; }
    int getCurrentLevelIndex() const { return currentLevelIndex; }

};

#endif // GAME_H
