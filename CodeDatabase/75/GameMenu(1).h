#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <deque>

class GameMenu {
public:
    enum class Action {
        NEW_GAME,
        LOAD_GAME,
        FREE_PLAY,
        EXIT,
        NONE
    };

    struct MenuParticle {
        sf::Vector2f pos;
        sf::Vector2f vel;
        float life;
        sf::Color color;
        float size;
    };

    GameMenu(sf::RenderWindow& window);
    Action run();
    
    int getSelectedLevel() const { return m_levelToLoad; }

private:
    sf::RenderWindow& m_window;
    std::vector<std::string> m_options;
    int m_selectedOption;
    sf::Font m_font;
    
    // Visuals
    std::deque<MenuParticle> m_particles;
    sf::Clock m_clock;
    float m_time;
    
    // Submenu state
    bool m_selectingLevel;
    int m_levelToLoad;

    void update(float dt);
    void render();
    void handleInput(sf::Keyboard::Key key);
    void handleMouse(int x, int y, bool clicked);
    
    // Helper to draw rounded button
    void drawButton(float x, float y, float w, float h, const std::string& text, bool selected, bool disabled);
};