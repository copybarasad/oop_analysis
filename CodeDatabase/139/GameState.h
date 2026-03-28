#pragma once


class IRenderer;
class IConverter;


class GameState {
public:
    virtual void update() = 0;
    virtual void render(IRenderer& renderer) = 0;
    virtual IConverter* getConverter() = 0;
};
