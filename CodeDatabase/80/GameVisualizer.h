#pragma once
#ifndef GAMEVISUALIZER_H
#define GAMEVISUALIZER_H

#include "GameRenderer.h"
#include "GameRenderData.h"

template<typename RendererType>
class GameVisualizer {
private:
    RendererType renderer;

public:
    explicit GameVisualizer(int screenWidth = 80)
        : renderer(screenWidth) {
        renderer.initialize();
    }

    ~GameVisualizer() = default;

    void render(const GameRenderData& data) {
        renderer.render(data);
    }

    void renderWithMessage(const GameRenderData& baseData, const std::string& message) {
 
        GameRenderData modifiedData = baseData;
        modifiedData.statusMessage = message;
        renderer.render(modifiedData);
    }

    void showPrompt(const std::string& prompt) {
        GameRenderData data;
        data.inputPrompt = prompt;
        renderer.render(data);
    }

    void clear() {
        renderer.clearScreen();
    }

    RendererType& getRenderer() { return renderer; }
    const RendererType& getRenderer() const { return renderer; }
};

#endif 