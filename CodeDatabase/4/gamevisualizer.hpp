#pragma once
#include "renderer.hpp"
#include "field.hpp"
#include "hero.hpp"

template<typename RendererType>
class GameVisualizer {
    RendererType renderer;
public:
    void update(const Field& field, const Hero& hero);
};

template<typename RendererType>
void GameVisualizer<RendererType>::update(const Field& field, const Hero& hero) {
    renderer.render(field, hero);
}