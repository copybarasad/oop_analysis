#pragma once
#include <memory>
#include <string>

class Field;
class Game;

class Renderer {
public:
    Renderer() = default;

    void render_field(const Field& field) const;
    void render_help() const;
    void render_game_over(const Field& field) const;

    void render_prompt(const std::string& text) const;
};
