#ifndef SFML_RENDERER_H
#define SFML_RENDERER_H

#include <SFML/Graphics.hpp>
#include <vector>

class Game;

class SfmlRenderer {
public:
    SfmlRenderer();

    void Draw(sf::RenderWindow& window, Game& game);

private:
    float cell_width_;
    float cell_height_;
    float panel_width_;

    sf::Font font_;
    sf::Text info_text_;
    sf::Text end_text_;

    sf::Texture hero_texture_;
    sf::Texture enemy_texture_;
    sf::Texture base_texture_;

    sf::Texture brus_texture_;
    sf::Texture wall_texture_;
    sf::Texture water_texture_;

    sf::Texture trap_texture_;
    sf::Texture tower_texture_;
    sf::Texture ally_texture_;

    sf::Sprite hero_sprite_;
    sf::Sprite enemy_sprite_;
    sf::Sprite base_sprite_;

    sf::Sprite brus_sprite_;
    sf::Sprite wall_sprite_;
    sf::Sprite water_sprite_;

    sf::Sprite trap_sprite_;
    sf::Sprite tower_sprite_;
    sf::Sprite ally_sprite_;

    sf::RectangleShape panel_rect_;
    std::vector<sf::Text> panel_lines_;
};

#endif