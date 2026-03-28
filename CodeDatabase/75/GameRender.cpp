#include "GameRender.h"
#include "Game.h" 
#include "ResourceManager.h"
#include "VisualEffects.h"
#include "WorldObject.h"
#include "Enemy.h"
#include "Player.h"
#include "GameField.h"
#include "Tower.h"
#include "Shield.h"
#include "Shop.h"
#include "Portal.h"
#include "ThornsArea.h"
#include "Trap.h"
#include "Ally.h"
#include "Bullet.h"
#include "Spell.h"
#include <cmath>
#include <iomanip>
#include <cstdint> 
#include <sstream>

float lerp(float a, float b, float t) { return a + (b - a) * t; }

GameRender::GameRender(sf::RenderWindow& window) : m_window(window), m_time(0.0f) {
    m_camera = window.getDefaultView();
}

void GameRender::changeZoom(float delta) { m_camera.zoom(delta); }

void GameRender::update(float dt) {
    m_time += dt;
    // Particles
    if (m_particles.size() < 80) {
        UIParticle p;
        p.pos = { (float)(rand() % m_window.getSize().x), (float)(rand() % m_window.getSize().y) };
        p.vel = { (rand()%20 - 10) / 20.0f, (rand()%20 - 10) / 20.0f };
        p.life = (rand() % 100) / 20.0f + 2.0f;
        p.size = (rand() % 3) + 1.0f;
        p.color = sf::Color(255, 255, 255, rand() % 50 + 20);
        m_particles.push_back(p);
    }
    for (auto& p : m_particles) { p.pos += p.vel; p.life -= dt; }
    while (!m_particles.empty() && m_particles.front().life <= 0) m_particles.pop_front();

    // Trails
    for (auto& t : m_trails) t.life -= dt * 2.0f;
    while (!m_trails.empty() && m_trails.front().life <= 0) m_trails.pop_front();
}

void GameRender::drawEntity(float x, float y, const std::string& tex, sf::Color col, float scale, bool withShadow) {
    sf::Sprite s(ResourceManager::get().getTexture(tex)); 
    if(s.getTexture().getSize().x == 0) return;
    
    float finalScale = (48.0f / s.getTexture().getSize().x) * scale; 
    float offset = (48.0f - s.getTexture().getSize().x * finalScale) / 2.0f; 
    
    if (withShadow) {
        sf::CircleShape shadow(16 * scale);
        shadow.setFillColor(sf::Color(0, 0, 0, 100));
        shadow.setScale({1.0f, 0.5f}); 
        shadow.setPosition({x + 24 - 16*scale, y + 36}); 
        m_window.draw(shadow);
    }

    s.setPosition({x + offset, y + offset}); 
    s.setScale({finalScale, finalScale}); 
    s.setColor(col); 
    
    if (withShadow) {
        float breathe = 1.0f + std::sin(m_time * 2.0f) * 0.02f;
        s.scale({1.0f, breathe}); 
    }
    
    m_window.draw(s);
}

void GameRender::render(const GameField* field, const Player* player, 
                       const std::vector<Enemy*>& enemies,
                       const std::vector<Ally*>& allies,
                       const std::vector<Trap*>& traps,
                       const std::vector<ThornsArea*>& thorns,
                       const std::vector<Shield*>& shields,
                       const std::vector<WorldObject*>& worldObjects,
                       const std::vector<VisualEvent>& effects,
                       bool isTargeting, int targetX, int targetY) {
    
    std::string fType = field->getFloorTexture();
    sf::Color bgCol = sf::Color(20, 20, 25);
    if(fType == "floor_grass") bgCol = sf::Color(10, 40, 10);
    m_window.clear(bgCol); 
    
    if (player) {
        float targetIdx = (float)player->getSelectedSpellIndex();
        m_currentSpellIndexSmooth = lerp(m_currentSpellIndexSmooth, targetIdx, 0.2f);
        if(std::abs(m_currentSpellIndexSmooth - targetIdx) < 0.01f) m_currentSpellIndexSmooth = targetIdx;

        // FIX: SNAP HP if difference is large (prevents "filling up" animation on level load)
        float realHP = (float)player->getHealth();
        float realMana = (float)player->getMana();
        
        if (std::abs(m_displayedHP - realHP) > 50.0f) m_displayedHP = realHP;
        else m_displayedHP = lerp(m_displayedHP, realHP, 0.1f);

        if (std::abs(m_displayedMana - realMana) > 50.0f) m_displayedMana = realMana;
        else m_displayedMana = lerp(m_displayedMana, realMana, 0.1f);

        sf::Vector2f targetPos(player->getVisualX() + 24.0f, player->getVisualY() + 24.0f);
        sf::Vector2f current = m_camera.getCenter();
        sf::Vector2f newCenter = current + (targetPos - current) * 0.1f;
        m_camera.setCenter(newCenter);
        m_window.setView(m_camera);
    }

    drawField(field);
    
    for (const auto* obj : worldObjects) {
        // NO SHADOW for world objects
        drawEntity((float)obj->getX() * 48.0f, (float)obj->getY() * 48.0f, obj->getTextureName(), obj->getColor(), obj->getScale(), false);
    }

    for (const auto& t : traps) { if (!t->isActive()) continue; drawEntity(t->getVisualX(), t->getVisualY(), "spell_trap", sf::Color(150, 0, 150), 1.0f, false); }
    for (const auto& a : thorns) { if (!a->isActive()) continue; for (const auto& p : a->getAffectedPositions()) drawEntity((float)p.first * 48.0f, (float)p.second * 48.0f, "thorns.png", sf::Color(100, 255, 100, 150), 1.0f, false); }
    
    for (const auto& s : shields) { 
        if (!s->isActive()) continue; 
        float r = s->getRadius() * 48.0f + 24.0f; 
        sf::CircleShape circle(r); circle.setOrigin({r, r}); circle.setPosition({s->getCenterX() * 48.0f + 24.0f, s->getCenterY() * 48.0f + 24.0f}); 
        circle.setFillColor(sf::Color(0, 255, 255, 40)); circle.setOutlineColor(sf::Color(0, 255, 255, 200)); 
        circle.setOutlineThickness(2.0f + std::sin(m_time * 5) * 1.0f); 
        m_window.draw(circle); 
    }
    
    for (const auto& a : allies) { 
        if (!a->isAlive()) continue; 
        drawEntity(a->getVisualX(), a->getVisualY(), "player", sf::Color(100, 255, 100), 1.0f, true); 
    }

    std::vector<const Enemy*> visibleEnemies;
    for (const auto& e : enemies) {
        if (!e->isAlive()) continue;
        visibleEnemies.push_back(e);
        std::string type = e->getTypeName();
        std::string texName = "orc";
        float scale = 1.0f;
        bool isBoss = false;
        
        if (type.find("ARCHER")!=std::string::npos) texName = "archer";
        else if (type.find("GOLEM")!=std::string::npos) { texName = "golem"; scale = 1.3f; }
        else if (type.find("ASSASSIN")!=std::string::npos) texName = "assassin";
        else if (type.find("NECROMANCER")!=std::string::npos) texName = "mage";
        else if (type.find("DEMON")!=std::string::npos || type.find("BOSS")!=std::string::npos) { texName = "boss"; isBoss = true; }
        
        sf::Color c = sf::Color::White;
        if (e->hasStatus(Character::Status::FROZEN)) c = sf::Color::Cyan;
        else if (e->hasStatus(Character::Status::BURNING)) c = sf::Color::Red;
        
        if (isBoss) {
            sf::Sprite bossSprite(ResourceManager::get().getTexture("boss"));
            bossSprite.setColor(c);
            bossSprite.setScale({4.5f, 4.5f});
            bossSprite.setPosition({e->getVisualX(), e->getVisualY()});
            m_window.draw(bossSprite);
        } else {
            drawEntity(e->getVisualX(), e->getVisualY(), texName, c, scale, true);
        }
        
        float hpW = 48.0f * (isBoss ? 3.0f : scale); 
        float barY = e->getVisualY() - 12.f;
        float offX = e->getVisualX() + (48.0f - hpW)/2.f;
        if(isBoss) offX = e->getVisualX(); 
        sf::RectangleShape bg({hpW, 4.f}); bg.setPosition({offX, barY}); bg.setFillColor(sf::Color(0,0,0,200)); m_window.draw(bg);
        sf::RectangleShape fill({hpW * ((float)e->getHealth()/e->getMaxHealth()), 4.f}); fill.setPosition({offX, barY}); fill.setFillColor(sf::Color::Red); m_window.draw(fill);

        for(auto* b : e->getBullets()) if(b->isActive()) {
            std::string bTex = "bullet_fireball";
            sf::Color bCol = sf::Color::White;
            if(b->getType() == Bullet::Type::ARROW) bTex = "bullet_arrow";
            else if(b->getType() == Bullet::Type::DARK_ORB) { bTex = "bullet_orb"; bCol = sf::Color::Magenta; }
            else if(b->getType() == Bullet::Type::BOSS_BLAST) { bTex = "bullet_orb"; bCol = sf::Color::Red; }
            
            drawEntity(b->getRenderX(), b->getRenderY(), bTex, bCol, 0.8f, false);
            m_trails.push_back({ {b->getRenderX()+24, b->getRenderY()+24}, 0.3f, bCol });
        }
    }

    if (player->isAlive()) {
        drawEntity(player->getVisualX(), player->getVisualY(), "player", sf::Color::White, 1.0f, true);
        for(auto* b : player->getBullets()) if(b->isActive()) {
             drawEntity(b->getRenderX(), b->getRenderY(), "bullet_fireball", sf::Color::Yellow, 0.7f, false);
             m_trails.push_back({ {b->getRenderX()+24, b->getRenderY()+24}, 0.3f, sf::Color::Yellow });
        }
    }

    // TRAILS
    for(const auto& t : m_trails) {
        sf::CircleShape s(4);
        s.setOrigin({2,2});
        s.setPosition(t.pos);
        sf::Color c = t.color;
        c.a = static_cast<uint8_t>(t.life / 0.3f * 255);
        s.setFillColor(c);
        m_window.draw(s);
    }

    // Effects
    for(const auto& ev : effects) {
        float ratio = ev.lifetime / ev.maxLifetime;
        sf::Color c = ev.color; c.a = static_cast<std::uint8_t>(255 * ratio);
        if(ev.type == VisualEvent::LIGHTNING || ev.type == VisualEvent::BEAM) {
            sf::Vertex line[] = { sf::Vertex{ {ev.startX * 48.0f + 24.0f, ev.startY * 48.0f + 24.0f}, c }, sf::Vertex{ {ev.endX * 48.0f + 24.0f, ev.endY * 48.0f + 24.0f}, c } };
            m_window.draw(line, 2, sf::PrimitiveType::Lines);
        } else if(ev.type == VisualEvent::NOVA || ev.type == VisualEvent::EXPLOSION) {
            float r = (1.0f - ratio) * 48.0f * 2.5f; sf::CircleShape circle(r); circle.setOrigin({r, r}); circle.setPosition({ev.startX * 48.0f + 24.0f, ev.startY * 48.0f + 24.0f}); circle.setFillColor(sf::Color::Transparent); circle.setOutlineColor(c); circle.setOutlineThickness(4.0f * ratio); m_window.draw(circle);
        } else if (ev.type == VisualEvent::FIRE_STORM || ev.type == VisualEvent::BLOOD) {
            for(int i=0; i<5; ++i) { float r = (rand()%8 + 2) * ratio; sf::CircleShape p(r); p.setFillColor(c); p.setPosition({ev.startX * 48.0f + (rand()%60 - 30) + 24.0f, ev.startY * 48.0f + (rand()%60 - 30) + 24.0f}); m_window.draw(p); }
        } else if (ev.type == VisualEvent::SLASH) {
            sf::Vertex line[] = { sf::Vertex{ {ev.startX * 48.0f + 24.0f, ev.startY * 48.0f + 24.0f}, c }, sf::Vertex{ {ev.endX * 48.0f + 24.0f, ev.endY * 48.0f + 24.0f}, c } };
            m_window.draw(line, 2, sf::PrimitiveType::Lines);
        } else if (ev.type == VisualEvent::ICE_BLAST) {
            sf::RectangleShape r1({48.0f * 1.5f * ratio, 2}); sf::RectangleShape r2({2, 48.0f * 1.5f * ratio}); r1.setOrigin({r1.getSize().x/2, 1}); r2.setOrigin({1, r2.getSize().y/2}); r1.setPosition({ev.startX * 48.0f + 24.0f, ev.startY * 48.0f + 24.0f}); r2.setPosition({ev.startX * 48.0f + 24.0f, ev.startY * 48.0f + 24.0f}); r1.setFillColor(sf::Color::Cyan); r2.setFillColor(sf::Color::Cyan); m_window.draw(r1); m_window.draw(r2);
        }
    }

    if (isTargeting) drawTargeting((float)targetX * 48.0f, (float)targetY * 48.0f, player);
    if (player->isAlive()) drawUI(player, visibleEnemies);
}

void GameRender::drawWorldObjects(const std::vector<WorldObject*>& objects) {
    for (const auto* obj : objects) {
        drawEntity((float)obj->getX() * 48.0f, (float)obj->getY() * 48.0f, obj->getTextureName(), obj->getColor(), obj->getScale(), false);
    }
}

void GameRender::drawTowers(const std::vector<Tower*>& towers) {
    for (const auto* t : towers) {
        if (!t) continue;
        drawEntity(t->getVisualX(), t->getVisualY(), "tower", sf::Color::White, 1.2f, false);
        sf::RectangleShape tur({20, 20}); tur.setOrigin({10, 10}); 
        tur.setPosition({t->getVisualX() + 24.0f, t->getVisualY() + 24.0f}); 
        tur.setFillColor(sf::Color(150, 30, 30)); tur.setRotation(sf::degrees(m_time * 90.0f)); m_window.draw(tur);
    }
}

void GameRender::drawHub(const Portal* portal, const Shop* shop) {
    if (portal) {
        sf::CircleShape p(20); p.setOrigin({20, 20}); 
        p.setPosition({(float)portal->getX() * 48.0f + 24.0f, (float)portal->getY() * 48.0f + 24.0f}); 
        p.setFillColor(sf::Color(0, 0, 0, 0)); p.setOutlineColor(sf::Color::Cyan); p.setOutlineThickness(3); m_window.draw(p);
        sf::VertexArray swirl(sf::PrimitiveType::LineStrip, 20);
        for(int i=0; i<20; ++i) { 
            float a = (i/19.0f) * 6.28f + portal->getRotation() * 0.05f; 
            float r = (i/19.0f) * 20.0f; 
            swirl[i].position = p.getPosition() + sf::Vector2f(cos(a)*r, sin(a)*r); 
            swirl[i].color = sf::Color::Cyan; 
        } 
        m_window.draw(swirl);
    }
    if (shop) {
        drawEntity((float)shop->getX() * 48.0f, (float)shop->getY() * 48.0f, "wall_brick", sf::Color(200, 150, 50), 2.0f, false);
        drawTextWithShadow("SHOP", (float)shop->getX()*48.0f, (float)shop->getY()*48.0f - 30, 14, sf::Color::Yellow);
    }
}

void GameRender::drawShopUI(const Shop*, const Player* player) {
    m_window.setView(m_window.getDefaultView());
    sf::RectangleShape overlay(sf::Vector2f(m_window.getSize().x, m_window.getSize().y)); overlay.setFillColor(sf::Color(0, 0, 0, 230)); m_window.draw(overlay);
    drawTextWithShadow("MYSTIC ARMORY", 100, 50, 50, sf::Color(255, 215, 0));
    drawTextWithShadow("Score: " + std::to_string(player->getScore()), 100, 110, 30, sf::Color::White);
    drawTextWithShadow("[CLICK ITEMS TO BUY]    [ESC] TO EXIT", 100, 650, 20, sf::Color(150, 150, 150));
    float startX = 100.0f; float startY = 150.0f; float boxW = 350.0f; float boxH = 50.0f; float gap = 20.0f;
    const auto& font = ResourceManager::get().getFont();
    auto drawBtn = [&](int idx, std::string txt, int cost, sf::Color col) {
        float y = startY + idx * (boxH + gap); sf::RectangleShape btn({boxW, boxH}); btn.setPosition({startX, y});
        sf::Vector2i mouse = sf::Mouse::getPosition(m_window); bool hover = btn.getGlobalBounds().contains({(float)mouse.x, (float)mouse.y});
        if (hover) { btn.setFillColor(sf::Color(col.r, col.g, col.b, 100)); btn.setOutlineColor(col); btn.setOutlineThickness(2); } else { btn.setFillColor(sf::Color(30, 30, 30, 200)); btn.setOutlineColor(sf::Color(100, 100, 100)); btn.setOutlineThickness(1); } m_window.draw(btn);
        sf::Text t(font); t.setString(txt + " (" + std::to_string(cost) + ")"); t.setCharacterSize(20); t.setFillColor(player->getScore() >= cost ? sf::Color::White : sf::Color(150, 50, 50)); t.setPosition({startX + 10, y + 12}); m_window.draw(t);
    };
    drawBtn(0, "Buy Random Spell", 300, sf::Color::Cyan); drawBtn(1, "Upgrade Melee (+10)", 400, sf::Color::Red); drawBtn(2, "Upgrade Ranged (+15)", 400, sf::Color::Yellow); drawBtn(3, "Upgrade Max HP (+100)", 500, sf::Color::Green);
    m_window.setView(m_camera);
}

void GameRender::drawField(const GameField* field) {
    const auto& wall = ResourceManager::get().getTexture("wall_brick"); 
    std::string floorTexName = field->getFloorTexture();
    const auto& specificFloor = ResourceManager::get().getTexture(floorTexName);
    sf::Sprite s(specificFloor); float scaleX = (48.0f/s.getTexture().getSize().x) * 1.05f; float scaleY = (48.0f/s.getTexture().getSize().y) * 1.05f; s.setScale({scaleX, scaleY});
    sf::Vector2f center = m_camera.getCenter();
    int x1 = std::max(0, (int)((center.x - m_camera.getSize().x/2)/48.0f)-1); int y1 = std::max(0, (int)((center.y - m_camera.getSize().y/2)/48.0f)-1);
    int x2 = std::min(field->getWidth(), (int)((center.x + m_camera.getSize().x/2)/48.0f)+2); int y2 = std::min(field->getHeight(), (int)((center.y + m_camera.getSize().y/2)/48.0f)+2);
    for(int y=y1; y<y2; ++y) for(int x=x1; x<x2; ++x) {
        s.setPosition({(float)x*48.0f, (float)y*48.0f}); Cell::Type t = field->getCell(x,y).getType();
        s.setTexture(specificFloor); s.setColor(sf::Color(255, 255, 255)); m_window.draw(s);
        if (t == Cell::WALL) { s.setTexture(wall); s.setColor(sf::Color(255, 255, 255)); m_window.draw(s); } 
    }
}
void GameRender::drawTargeting(float x, float y, const Player*) {
    sf::RectangleShape r({48.0f, 48.0f}); r.setPosition({x, y}); r.setFillColor(sf::Color::Transparent); r.setOutlineColor(sf::Color::Green); r.setOutlineThickness(2.0f + std::sin(m_time * 10) * 1.0f); m_window.draw(r);
}
void GameRender::drawUI(const Player* player, const std::vector<const Enemy*>& visibleEnemies) {
    m_window.setView(m_window.getDefaultView()); float pX = 20, pY = 20;
    drawCapsuleBar(pX + 10, pY, 300, 20, m_displayedHP, (float)player->getMaxHealth(), sf::Color(220, 20, 20), "HP");
    drawCapsuleBar(pX + 10, pY + 35, 300, 20, m_displayedMana, (float)player->getMaxMana(), sf::Color(20, 100, 255), "MP");
    drawTextWithShadow("KILLS: " + std::to_string(player->getKills()), pX, pY + 80, 22, sf::Color::Red);
    drawSpellWheel(player); drawRadar(m_window.getSize().x - 90, 90, 70, player, visibleEnemies); drawLogs(); m_window.setView(m_camera);
}
void GameRender::drawCapsuleBar(float x, float y, float w, float h, float cur, float max, sf::Color fillCol, std::string label) {
    sf::CircleShape leftCap(h/2); leftCap.setPosition({x, y}); leftCap.setFillColor(sf::Color(30,30,30,200));
    sf::RectangleShape body({w, h}); body.setPosition({x+h/2, y}); body.setFillColor(sf::Color(30,30,30,200));
    sf::CircleShape rightCap(h/2); rightCap.setPosition({x+w, y}); rightCap.setFillColor(sf::Color(30,30,30,200));
    m_window.draw(leftCap); m_window.draw(body); m_window.draw(rightCap);
    float pct = cur / max;
    if(pct > 1.0f) pct = 1.0f;
    if(pct < 0.0f) pct = 0.0f;
    float fillW = w * pct;
    if (fillW > 0) {
        sf::CircleShape lFill(h/2); lFill.setPosition({x, y}); lFill.setFillColor(fillCol);
        sf::RectangleShape bFill({fillW, h}); bFill.setPosition({x+h/2, y}); bFill.setFillColor(fillCol);
        m_window.draw(lFill); m_window.draw(bFill);
        if (pct > 0.98f) { sf::CircleShape rFill(h/2); rFill.setPosition({x+fillW, y}); rFill.setFillColor(fillCol); m_window.draw(rFill); }
    }
    sf::RectangleShape shine({w, h/2}); shine.setPosition({x+h/2, y}); shine.setFillColor(sf::Color(255,255,255,30)); m_window.draw(shine);
    drawTextWithShadow(label, x - 35, y - 2, 16, sf::Color::White);
}
void GameRender::drawSpellWheel(const Player* player) {
    auto& hand = player->getHand(); int count = hand.getSize(); if (count == 0) return;
    float startX = m_window.getSize().x / 2.0f; float startY = m_window.getSize().y - 80.0f; int selected = hand.getSelectedIndex();
    float radius = 120.0f; float arcSpacing = 0.4f; 
    for (int i = 0; i < count; ++i) {
        float offset = (float)i - m_currentSpellIndexSmooth;
        float x = startX + std::sin(offset * arcSpacing) * radius * 1.5f;
        float y = startY - std::cos(offset * arcSpacing) * 40.0f + std::abs(offset)*10.0f;
        float dist = std::abs(offset);
        float scale = 1.0f - std::min(dist * 0.2f, 0.5f);
        if (i == selected) scale = 1.3f;
        float alpha = 1.0f - std::min(dist * 0.3f, 0.8f);
        sf::Color baseCol = sf::Color(255, 255, 255, static_cast<std::uint8_t>(255 * alpha));
        const Spell* s = hand.getSpell(i);
        sf::Color glowCol = sf::Color::White;
        if(s) {
            std::string n = s->getName();
            if(n.find("Fireball")!=std::string::npos) glowCol = sf::Color::Red;
            else if(n.find("Heal")!=std::string::npos) glowCol = sf::Color::Green;
            else glowCol = sf::Color::Cyan; 
        }
        if (dist < 0.5f) {
            sf::Sprite glow(ResourceManager::get().getTexture("ui_glow"));
            glow.setOrigin({32,32}); glow.setPosition({x, y}); glow.setScale({scale*2.0f, scale*2.0f}); glow.setColor(sf::Color(glowCol.r, glowCol.g, glowCol.b, 150));
            m_window.draw(glow);
        }
        sf::RectangleShape slot({50.0f * scale, 50.0f * scale});
        slot.setOrigin({25.0f*scale, 25.0f*scale}); slot.setPosition({x, y});
        slot.setFillColor(sf::Color(20, 20, 30, 200)); slot.setOutlineColor(dist < 0.1f ? glowCol : sf::Color::White); slot.setOutlineThickness(dist < 0.1f ? 3.0f : 1.0f);
        m_window.draw(slot);
        if (s) {
            std::string icon = "bullet_orb";
            std::string n = s->getName();
            if (n.find("Fireball") != std::string::npos) icon = "bullet_fireball";
            else if (n.find("Ice") != std::string::npos) icon = "spell_ice";
            else if (n.find("Lightning") != std::string::npos) icon = "spell_lightning";
            else if (n.find("Heal") != std::string::npos) icon = "spell_heal";
            else if (n.find("Shield") != std::string::npos) icon = "spell_shield";
            else if (n.find("Trap") != std::string::npos) icon = "spell_trap";
            else if (n.find("Thorns") != std::string::npos) icon = "spell_thorns";
            else if (n.find("Teleport") != std::string::npos) icon = "spell_teleport";
            else if (n.find("Nova") != std::string::npos) icon = "spell_nova";
            else if (n.find("Drain") != std::string::npos) icon = "spell_drain";
            else if (n.find("Ally") != std::string::npos) icon = "spell_ally";
            else if (n.find("Poison") != std::string::npos) icon = "spell_poison";
            else if (n.find("Time") != std::string::npos) icon = "spell_time";
            else if (n.find("Meteor") != std::string::npos) icon = "spell_meteor";
            else if (n.find("Rage") != std::string::npos) icon = "spell_rage";
            if (ResourceManager::get().getTexture(icon).getSize().x > 0) {
                sf::Sprite ic(ResourceManager::get().getTexture(icon));
                float sScale = (32.0f * scale) / ic.getTextureRect().size.x;
                ic.setScale({sScale, sScale}); ic.setOrigin({(float)ic.getTextureRect().size.x/2, (float)ic.getTextureRect().size.y/2});
                ic.setPosition({x, y}); m_window.draw(ic);
            }
            drawTextWithShadow(s->getName().substr(0,4), x, y-10*scale, (int)(14*scale), baseCol);
        }
    }
}
void GameRender::drawRadar(float x, float y, float radius, const Player* player, const std::vector<const Enemy*>& enemies) {
    sf::CircleShape bg(radius); bg.setOrigin({radius, radius}); bg.setPosition({x, y}); bg.setFillColor(sf::Color(0, 20, 40, 200)); bg.setOutlineColor(sf::Color::Cyan); bg.setOutlineThickness(2); m_window.draw(bg);
    sf::Vertex line[] = { sf::Vertex{ {x, y}, sf::Color::Cyan }, sf::Vertex{ {x + std::cos(m_time * 3) * radius, y + std::sin(m_time * 3) * radius}, sf::Color(0, 255, 255, 50) } }; m_window.draw(line, 2, sf::PrimitiveType::Lines); 
    sf::CircleShape p(3); p.setOrigin({3,3}); p.setPosition({x, y}); p.setFillColor(sf::Color::Green); m_window.draw(p);
    float range = 40.0f;
    for(const auto* e : enemies) { float dx = e->getX() - player->getX(); float dy = e->getY() - player->getY(); if (sqrt(dx*dx + dy*dy) < range) { sf::Vector2f pos(x + (dx/range)*radius, y + (dy/range)*radius); sf::CircleShape ed(3); ed.setOrigin({3,3}); ed.setPosition(pos); ed.setFillColor(sf::Color::Red); m_window.draw(ed); } }
}
void GameRender::drawLogs() {
    const auto& logs = VisualEffects::getLogs(); float y = m_window.getSize().y - 30;
    for(auto it = logs.rbegin(); it != logs.rend(); ++it) { drawTextWithShadow(it->text, 20, y, 16, it->color); y -= 22; }
}
void GameRender::drawTextWithShadow(const std::string& textStr, float x, float y, int size, sf::Color color) {
    const auto& font = ResourceManager::get().getFont(); sf::Text txt(font); txt.setString(textStr); txt.setCharacterSize(size);
    txt.setPosition({x + 2, y + 2}); txt.setFillColor(sf::Color::Black); m_window.draw(txt);
    txt.setPosition({x, y}); txt.setFillColor(color); m_window.draw(txt);
}
void GameRender::drawDecorations(const std::vector<Decoration>& decorations) {
    for(const auto& d : decorations) { sf::RectangleShape r({48.0f, 48.0f}); r.setPosition({(float)d.x * 48.0f + 24.0f, (float)d.y * 48.0f + 24.0f}); r.setOrigin({24.0f, 24.0f}); r.setFillColor(d.color); m_window.draw(r); }
}
void GameRender::drawPaused() { 
    m_window.setView(m_window.getDefaultView()); sf::RectangleShape bg({(float)m_window.getSize().x, (float)m_window.getSize().y}); bg.setFillColor(sf::Color(0,0,0,150)); m_window.draw(bg);
    drawTextWithShadow("PAUSED", m_window.getSize().x/2 - 100, m_window.getSize().y/2 - 50, 60, sf::Color::White);
}
void GameRender::drawGameOver() { 
    m_window.setView(m_window.getDefaultView()); sf::RectangleShape bg({(float)m_window.getSize().x, (float)m_window.getSize().y}); bg.setFillColor(sf::Color(50,0,0,150)); m_window.draw(bg);
    drawTextWithShadow("YOU DIED", m_window.getSize().x/2 - 150, m_window.getSize().y/2 - 50, 80, sf::Color::Red);
}
void GameRender::drawVictory(int) { 
    m_window.setView(m_window.getDefaultView()); sf::RectangleShape bg({(float)m_window.getSize().x, (float)m_window.getSize().y}); bg.setFillColor(sf::Color(50,50,0,150)); m_window.draw(bg);
    drawTextWithShadow("VICTORY", m_window.getSize().x/2 - 140, m_window.getSize().y/2 - 50, 80, sf::Color::Yellow);
}
void GameRender::drawGameCompleted() { 
    m_window.setView(m_window.getDefaultView()); drawTextWithShadow("GAME COMPLETE", m_window.getSize().x/2 - 200, m_window.getSize().y/2 - 50, 60, sf::Color::Cyan);
}