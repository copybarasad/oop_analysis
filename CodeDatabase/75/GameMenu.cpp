#include "GameMenu.h"
#include "ResourceManager.h"
#include "SaveManager.h"
#include <iostream>
#include <cmath>
#include <optional> 

GameMenu::GameMenu(sf::RenderWindow& window) 
    : m_window(window), m_selectedOption(0), m_time(0.0f), 
      m_selectingLevel(false), m_levelToLoad(1) 
{
    m_options = {"NEW STORY", "CONTINUE", "FREE PLAY", "EXIT"};
    m_font = ResourceManager::get().getFont();
}

GameMenu::Action GameMenu::run() {
    m_clock.restart();
    while (m_window.isOpen()) {
        float dt = m_clock.restart().asSeconds();
        update(dt);

        while (const std::optional event = m_window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                return Action::EXIT;
            } 
            else if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                handleInput(keyEvent->code);
                
                if (keyEvent->code == sf::Keyboard::Key::Enter) {
                    if (m_selectingLevel) {
                        return Action::FREE_PLAY; 
                    }
                    if (m_selectedOption == 0) return Action::NEW_GAME;
                    if (m_selectedOption == 1) {
                        if (SaveManager::saveFileExists()) return Action::LOAD_GAME;
                    }
                    if (m_selectedOption == 2) m_selectingLevel = true; 
                    if (m_selectedOption == 3) return Action::EXIT;
                }
                if (keyEvent->code == sf::Keyboard::Key::Escape) {
                    if (m_selectingLevel) m_selectingLevel = false;
                    else return Action::EXIT;
                }
            }
            else if (const auto* mouseMove = event->getIf<sf::Event::MouseMoved>()) {
                handleMouse(mouseMove->position.x, mouseMove->position.y, false);
            }
            else if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseBtn->button == sf::Mouse::Button::Left) {
                    handleMouse(mouseBtn->position.x, mouseBtn->position.y, true);
                    if (m_selectingLevel) return Action::FREE_PLAY;
                    if (m_selectedOption == 0) return Action::NEW_GAME;
                    if (m_selectedOption == 1 && SaveManager::saveFileExists()) return Action::LOAD_GAME;
                    if (m_selectedOption == 2) m_selectingLevel = true;
                    if (m_selectedOption == 3) return Action::EXIT;
                }
            }
        }
        render();
    }
    return Action::EXIT;
}

void GameMenu::update(float dt) {
    m_time += dt;
    if (m_particles.size() < 150) {
        MenuParticle p;
        p.pos = { (float)(rand() % m_window.getSize().x), (float)(rand() % m_window.getSize().y) };
        p.vel = { (rand()%50 - 25) / 10.0f, (rand()%50 - 25) / 10.0f };
        p.life = (rand() % 100) / 20.0f + 3.0f;
        p.size = (rand() % 4) + 1.0f;
        p.color = sf::Color(rand()%50, rand()%50 + 100, rand()%100 + 155, 150); 
        m_particles.push_back(p);
    }
    for (auto& p : m_particles) { p.pos += p.vel; p.life -= dt; }
    while (!m_particles.empty() && m_particles.front().life <= 0) m_particles.pop_front();
}

void GameMenu::handleInput(sf::Keyboard::Key key) {
    if (m_selectingLevel) {
        if (key == sf::Keyboard::Key::Left) {
            m_levelToLoad--; if (m_levelToLoad < 1) m_levelToLoad = 10;
        } else if (key == sf::Keyboard::Key::Right) {
            m_levelToLoad++; if (m_levelToLoad > 10) m_levelToLoad = 1;
        }
        return;
    }
    if (key == sf::Keyboard::Key::Up) {
        m_selectedOption--; if (m_selectedOption < 0) m_selectedOption = m_options.size() - 1;
    } else if (key == sf::Keyboard::Key::Down) {
        m_selectedOption++; if (m_selectedOption >= (int)m_options.size()) m_selectedOption = 0;
    }
}

void GameMenu::handleMouse(int x, int y, bool) {
    if (m_selectingLevel) return; 
    float startY = 300.0f; float boxH = 60.0f; float gap = 20.0f;
    float centerX = m_window.getSize().x / 2.0f; float boxW = 400.0f;
    for (size_t i = 0; i < m_options.size(); ++i) {
        float btnY = startY + i * (boxH + gap);
        sf::FloatRect bounds({centerX - boxW/2, btnY}, {boxW, boxH});
        if (bounds.contains({(float)x, (float)y})) { m_selectedOption = i; }
    }
}

void GameMenu::render() {
    m_window.clear(sf::Color(5, 5, 15)); 
    
    // Draw Art Background
    const sf::Texture& artTex = ResourceManager::get().getTexture("menu_art");
    sf::Sprite art(artTex);
    float scale = m_window.getSize().x / (float)artTex.getSize().x;
    art.setScale({scale, scale});
    art.setColor(sf::Color(100, 100, 150, 100)); // Dimmed
    m_window.draw(art);

    for (const auto& p : m_particles) {
        sf::CircleShape s(p.size); s.setPosition(p.pos);
        sf::Color c = p.color; c.a = static_cast<std::uint8_t>(std::min(255.0f, p.life * 50.0f));
        s.setFillColor(c); m_window.draw(s);
    }
    
    sf::Text title(m_font); title.setString("MAGICAL BATTLE"); title.setCharacterSize(70);
    title.setFillColor(sf::Color::Cyan); title.setStyle(sf::Text::Bold);
    sf::Text glow = title; glow.setFillColor(sf::Color(0, 255, 255, 50));
    glow.setPosition({title.getPosition().x + 2, title.getPosition().y + 2});
    sf::FloatRect tr = title.getLocalBounds();
    title.setPosition({(m_window.getSize().x - tr.size.x)/2.0f, 80.0f + sin(m_time)*5.0f});
    glow.setPosition({(m_window.getSize().x - tr.size.x)/2.0f + 4, 80.0f + sin(m_time)*5.0f + 4});
    m_window.draw(glow); m_window.draw(title);

    if (m_selectingLevel) {
        sf::Text txt(m_font); txt.setString("SELECT LEVEL"); txt.setCharacterSize(40); txt.setFillColor(sf::Color::White);
        sf::FloatRect b = txt.getLocalBounds(); txt.setPosition({(m_window.getSize().x - b.size.x)/2.0f, 300.0f}); m_window.draw(txt);
        txt.setString("< Level " + std::to_string(m_levelToLoad) + " >"); txt.setCharacterSize(60); txt.setFillColor(sf::Color::Yellow);
        b = txt.getLocalBounds(); txt.setPosition({(m_window.getSize().x - b.size.x)/2.0f, 400.0f}); m_window.draw(txt);
        txt.setString("Press ENTER to Start"); txt.setCharacterSize(20); txt.setFillColor(sf::Color(150, 150, 150));
        b = txt.getLocalBounds(); txt.setPosition({(m_window.getSize().x - b.size.x)/2.0f, 500.0f}); m_window.draw(txt);
    } else {
        float startY = 300.0f; float boxW = 400.0f; float boxH = 60.0f; float gap = 20.0f;
        for (size_t i = 0; i < m_options.size(); ++i) {
            bool selected = (i == (size_t)m_selectedOption);
            bool disabled = (i == 1 && !SaveManager::saveFileExists());
            drawButton((m_window.getSize().x - boxW)/2.0f, startY + i * (boxH + gap), boxW, boxH, m_options[i], selected, disabled);
        }
    }
    m_window.display();
}

void GameMenu::drawButton(float x, float y, float w, float h, const std::string& text, bool selected, bool disabled) {
    float r = h / 2.0f;
    sf::CircleShape left(r); left.setPosition({x, y}); 
    sf::RectangleShape body({w - h, h}); body.setPosition({x + r, y});
    sf::CircleShape right(r); right.setPosition({x + w - h, y});
    
    sf::Color baseColor = disabled ? sf::Color(30, 30, 30, 150) : (selected ? sf::Color(50, 50, 80, 200) : sf::Color(20, 20, 30, 150));
    sf::Color outlineColor = disabled ? sf::Color(50, 50, 50) : (selected ? sf::Color::Cyan : sf::Color(100, 100, 100));
    
    if (selected && !disabled) {
        sf::CircleShape gLeft = left; gLeft.setRadius(r+2); gLeft.setPosition({x-2, y-2}); gLeft.setFillColor(sf::Color(0, 255, 255, 100));
        sf::RectangleShape gBody = body; gBody.setSize({w-h, h+4}); gBody.setPosition({x+r, y-2}); gBody.setFillColor(sf::Color(0, 255, 255, 100));
        sf::CircleShape gRight = right; gRight.setRadius(r+2); gRight.setPosition({x+w-h-2, y-2}); gRight.setFillColor(sf::Color(0, 255, 255, 100));
        m_window.draw(gLeft); m_window.draw(gBody); m_window.draw(gRight);
    }
    
    left.setFillColor(baseColor); body.setFillColor(baseColor); right.setFillColor(baseColor);
    left.setOutlineThickness(2); left.setOutlineColor(outlineColor);
    right.setOutlineThickness(2); right.setOutlineColor(outlineColor);

    m_window.draw(left); m_window.draw(body); m_window.draw(right);
    
    sf::RectangleShape outline({w-h, 2}); outline.setFillColor(outlineColor);
    outline.setPosition({x+r, y}); m_window.draw(outline); 
    outline.setPosition({x+r, y+h-2}); m_window.draw(outline); 

    sf::Text txt(m_font); txt.setString(text); txt.setCharacterSize(30);
    txt.setFillColor(disabled ? sf::Color(100, 100, 100) : (selected ? sf::Color::White : sf::Color(200, 200, 200)));
    sf::FloatRect b = txt.getLocalBounds();
    txt.setPosition({x + w/2.0f - b.size.x/2.0f, y + h/2.0f - b.size.y/2.0f - 5});
    m_window.draw(txt);
}