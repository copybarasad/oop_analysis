#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "GameModel.h"

struct SpriteInfo {
    sf::Texture texture;
    unsigned int frame_width;
    unsigned int frame_height;
    unsigned int display_width;
    unsigned int display_height;
};

struct AnimationInfo {
    unsigned int start_x;
    unsigned int end_x;
    unsigned int y;
};

struct EntityAnimationState {
    std::string animation_name;
    int current_frame;
    float timer;
    float speed = 1.0f / 12.0f;
};

class GameView {
private:
    sf::RenderWindow& window_;
    GameModel& model_;
    std::unordered_map<std::string, SpriteInfo> sprite_repository_;
    std::unordered_map<std::string, AnimationInfo> animation_repository_;
    std::unordered_map<const Entity*, EntityAnimationState> animation_state_repository_;
    std::unordered_map<std::string, sf::Texture> textures_repository_;
    const float cell_size_ = 64.0f;
    const float spell_size_ = 32.0f;
    const float spell_draw_size_ = 96.0f;

    const float ui_max_timer_ = 1.0f;
    float ui_timer_ = 0.0f;

    sf::Texture tileset_texture_;
    sf::Texture ui_tileset_texture_;
    sf::View view_;
    sf::View ui_view_;

    sf::Font pressStartFont_;

    void loadSprite(const std::string& logical_name, const std::string& file_path, unsigned int fw, unsigned int fh, unsigned int dw, unsigned int dh);

    void loadAnimation(const std::string& logical_name, unsigned int start_x, unsigned int end_x, unsigned int y);

    void updateAnimationState(const Entity* entity, float dt);

    std::string getAnimationNameForEntity(const Entity* entity);

    sf::Vector2f calculateScale(int w1, int h1, int w2, int h2);

    void updateView();

    void drawField();

    void drawBuildings();

    void drawUIElement(float x, float y, int cx, int cy, int w, int h, float dw=0, float dh=0);

    void drawUI(const Player* player);

    void drawSelectedCell();

    void updateUITimer(float dt);

    void drawEndGame();

    void drawSpell(Spell* spell, float x, float y, float dw=0, float dh=0);

    void drawShopItem(const ShopItem* item, float x, float y);

    void drawShop();

    void drawDeck();

    void drawBtn(const ButtonModel& btn, int x, int y);

    void drawMenu();

    void drawNextLevel();
    
    void drawEntity(const Entity* entity);
public:
    GameView(sf::RenderWindow& window, GameModel& model);
    ~GameView() = default;

    void render(float dt);

};