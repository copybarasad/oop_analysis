#pragma once

class Game;
class GameField;

class IRender {
public:
    virtual ~IRender() = default;
    virtual void RenderField(const GameField& field) = 0;
    virtual void RenderHUD(const Game& game) = 0;
    virtual void RenderMessage(const char* msg) = 0;
};
