// Renderer.h
#ifndef RENDERER_H
#define RENDERER_H

class GameField;
class Player;

class ConsoleRenderer {
public:
    void render(const GameField& field, const Player& player) const;
};

#endif