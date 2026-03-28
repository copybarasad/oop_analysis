#pragma once
#include "renderer.hpp"

class ConsoleRenderer : public Renderer {
public:
    void render(const Field& field, const Hero& hero) const override;
};