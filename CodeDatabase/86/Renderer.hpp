#pragma once

class GameWorld;

class Renderer {
public:
    virtual ~Renderer() = default;
    virtual void render(const GameWorld& world) const = 0;
};

class ConsoleRenderer : public Renderer {
public:
    void render(const GameWorld& world) const override;
};
