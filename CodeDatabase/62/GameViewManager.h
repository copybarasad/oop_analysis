#pragma once
#include "GameModel.h"
#include "Renderer.h"

// Шаблонный класс визуализации игры
// Параметр шаблона: класс, отвечающий за способ отрисовки игры
template<typename RendererType>
class GameViewManager {
private:
    GameModel& model_;
    RendererType renderer_;

public:
    template<typename... Args>
    GameViewManager(GameModel& model, Args&&... args)
        : model_(model)
        , renderer_(std::forward<Args>(args)...) {}
    
    // Реагирует на изменения в игре и вызывает отрисовку
    void render(float dt) {
        renderer_.render(dt);
    }
};

