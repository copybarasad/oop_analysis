#ifndef GAMERENDERER_H
#define GAMERENDERER_H

class GameField;
class Player;

// Отрисовщик игрового состояния
class GameRenderer {
public:
    GameRenderer() = default;

    // Основные методы отрисовки
    void renderGameState(const GameField& field, const Player& player) const;
    void renderPlayerStats(const Player& player) const;
    void renderPlayerHand(const Player& player) const;
};

#endif