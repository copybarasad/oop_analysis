#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <deque>
#include "VisualEffects.h"
#include "LevelConfig.h" 

class GameField;
class Player;
class Enemy;
class Ally;
class Trap;
class ThornsArea;
class Shield;
class WorldObject;
class Tower;
class Portal;
class Shop;
class Bullet;

struct UIParticle {
    sf::Vector2f pos;
    sf::Vector2f vel;
    float life;
    float size;
    sf::Color color;
};

struct TrailParticle {
    sf::Vector2f pos;
    float life;
    sf::Color color;
};

class GameRender {
public:
    GameRender(sf::RenderWindow& window);
    void update(float dt);
    void changeZoom(float delta);
    void render(const GameField* field, const Player* player, 
                const std::vector<Enemy*>& enemies,
                const std::vector<Ally*>& allies,
                const std::vector<Trap*>& traps,
                const std::vector<ThornsArea*>& thorns,
                const std::vector<Shield*>& shields,
                const std::vector<WorldObject*>& worldObjects,
                const std::vector<VisualEvent>& effects,
                bool isTargeting, int targetX, int targetY);

    void drawTowers(const std::vector<Tower*>& towers);
    void drawHub(const Portal* portal, const Shop* shop);
    void drawShopUI(const Shop* shop, const Player* player);
    void drawPaused();
    void drawGameOver();
    void drawVictory(int level);
    void drawGameCompleted();
    
private:
    sf::RenderWindow& m_window;
    sf::View m_camera;
    float m_time;
    std::deque<UIParticle> m_particles;
    std::deque<TrailParticle> m_trails;
    
    float m_currentSpellIndexSmooth = 0.0f;
    float m_displayedHP = 100.0f;
    float m_displayedMana = 100.0f;

    void drawField(const GameField* field);
    void drawEntity(float x, float y, const std::string& tex, sf::Color col = sf::Color::White, float scale = 1.0f, bool withShadow = false);
    void drawWorldObjects(const std::vector<WorldObject*>& objects);
    void drawTargeting(float x, float y, const Player* player);
    void drawUI(const Player* player, const std::vector<const Enemy*>& visibleEnemies);
    void drawCapsuleBar(float x, float y, float w, float h, float cur, float max, sf::Color fillCol, std::string label);
    void drawSpellWheel(const Player* player);
    void drawRadar(float x, float y, float radius, const Player* player, const std::vector<const Enemy*>& enemies);
    void drawLogs();
    void drawTextWithShadow(const std::string& textStr, float x, float y, int size, sf::Color color);
    void drawDecorations(const std::vector<Decoration>& decorations);
};