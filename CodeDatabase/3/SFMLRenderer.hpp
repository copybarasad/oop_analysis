#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "LevelManager/LevelManager.hpp"
#include "ViewController/Managers/TextureManager.hpp"
#include "ViewController/Managers/render_types.hpp"
#include "types.hpp"

namespace CFG {
    inline const float WINDOW_WIDTH       = 2100;
    inline const float WINDOW_HEIGHT      = WINDOW_WIDTH / 16 * 9;

    inline const float GAMEBAR_WIDTH      = WINDOW_WIDTH - WINDOW_HEIGHT;
    inline const float GAMEBAR_START_X    = WINDOW_HEIGHT;

    inline const float GAMEBAR_PADDING    = WINDOW_HEIGHT / 30;
    

    inline const float GRID_STEP_X        = WINDOW_WIDTH / 10;
    inline const float GRID_STEP_Y        = WINDOW_HEIGHT / 10;

    inline const int LARGE_FONT_SIZE      = WINDOW_WIDTH / 20;
    inline const int BIG_FONT_SIZE        = WINDOW_WIDTH / 40;
    inline const int MID_FONT_SIZE        = WINDOW_WIDTH / 60;
    inline const int SMALL_FONT_SIZE      = WINDOW_WIDTH / 100;

    inline const std::string WINDOW_NAME  = "megaGame";
}

class SFMLRenderer {
public:
    SFMLRenderer(sf::RenderWindow& window);
    void draw(GameDTO& dto);
    bool isAlive();
    

private:
    sf::RenderWindow& window;
    TextureManager tm;

    void drawGame(GameDTO& dto);
    void drawUI(GameDTO& dto);
    void drawStartMenu(GameDTO& dto);
    void drawImproveMenu(GameDTO& dto);
    void drawWinMenu(GameDTO& dto);
    void drawLoseMenu(GameDTO& dto);

    void loadPlayerTexture(sf::Sprite& sprite, PlayerDTO player);
    void loadEnemyTexture(sf::Sprite& sprite, std::vector<EnemyDTO> enemies, Coordinates coord);
    void loadAttackTowerTexture(sf::Sprite& sprite, std::vector<TowerDTO> towers, Coordinates coord);

    void drawText(std::string str, int size, sf::Font& font, int x, int y);
};
