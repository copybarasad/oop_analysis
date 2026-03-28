#pragma once
#ifndef GAMERENDERER_H
#define GAMERENDERER_H

#include "GameRenderData.h"

class GameRenderer {
public:
    virtual ~GameRenderer() = default;

    virtual bool initialize() = 0;
    virtual void clearScreen() = 0;
    virtual void render(const GameRenderData& data) = 0;

    static std::string centerText(const std::string& text, int width);
    static std::string repeatChar(char ch, int count);
    static void waitForAnyKey();
};

#endif 