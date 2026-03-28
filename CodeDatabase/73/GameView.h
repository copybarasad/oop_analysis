#ifndef GAMELB1_GAMEVIEW_H
#define GAMELB1_GAMEVIEW_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Game.h"

class GameView {
public:
    GameView(Game& game, float tile_size);
    bool loadFont(const std::string& path);
    bool loadSounds(const std::string& shootPath, const std::string& hitPath);

    void draw(sf::RenderWindow& window);

    void playShotAnimation(sf::RenderWindow& window);

private:
    Game& game_;
    float tile_size_;
    sf::Font font_;
    sf::Text status_text_;
    sf::Text message_text_;
    sf::Text spells_text_;

    sf::SoundBuffer shoot_buffer_;
    sf::SoundBuffer hit_buffer_;
    sf::Sound shoot_sound_;
    sf::Sound hit_sound_;
};

#endif //GAMELB1_GAMEVIEW_H