#include "GameView.h"
#include <string>
#include <cmath>
#include <thread>
#include <chrono>
#include <iostream>

GameView::GameView(Game& game, float tile_size)
        : game_(game), tile_size_(tile_size) {}

bool GameView::loadFont(const std::string &path) {
    if (!font_.loadFromFile(path)) {
        return false;
    }
    status_text_.setFont(font_);
    status_text_.setCharacterSize(18);
    status_text_.setFillColor(sf::Color::White);

    message_text_.setFont(font_);
    message_text_.setCharacterSize(16);
    message_text_.setFillColor(sf::Color::Yellow);

    spells_text_.setFont(font_);
    spells_text_.setCharacterSize(16);
    spells_text_.setFillColor(sf::Color::Cyan);

    return true;
}


bool GameView::loadSounds(const std::string& shootPath, const std::string& hitPath) {
    if (!shoot_buffer_.loadFromFile(shootPath)) {
        std::cerr << "Failed to load sound: " << shootPath << std::endl;
        return false;
    }
    if (!hit_buffer_.loadFromFile(hitPath)) {
        std::cerr << "Failed to load sound: " << hitPath << std::endl;
        return false;
    }

    shoot_sound_.setBuffer(shoot_buffer_);
    hit_sound_.setBuffer(hit_buffer_);


    shoot_sound_.setVolume(50);
    hit_sound_.setVolume(60);

    return true;
}


void GameView::playShotAnimation(sf::RenderWindow& window) {
    const auto& path = game_.getLastShotPath();
    if (path.empty()) return;

    shoot_sound_.play();

    float startX = game_.getPlayer().getX() * tile_size_ + tile_size_ / 2;
    float startY = game_.getPlayer().getY() * tile_size_ + tile_size_ / 2;

    sf::RectangleShape arrow(sf::Vector2f(20.0f, 4.0f));
    arrow.setOrigin(10.0f, 2.0f);
    arrow.setFillColor(sf::Color::Yellow);
    arrow.setPosition(startX, startY);

    if (!path.empty()) {
        float dx = (path[0].first * tile_size_ + tile_size_ / 2) - startX;
        float dy = (path[0].second * tile_size_ + tile_size_ / 2) - startY;
        float angle = std::atan2(dy, dx) * 180 / 3.14159f;
        arrow.setRotation(angle);
    }

    float currentX = startX;
    float currentY = startY;
    const int steps_per_tile = 5;

    for (const auto& target : path) {
        float targetX = target.first * tile_size_ + tile_size_ / 2;
        float targetY = target.second * tile_size_ + tile_size_ / 2;

        float stepX = (targetX - currentX) / steps_per_tile;
        float stepY = (targetY - currentY) / steps_per_tile;

        for (int i = 0; i < steps_per_tile; ++i) {
            currentX += stepX;
            currentY += stepY;
            arrow.setPosition(currentX, currentY);

            window.clear(sf::Color::Black);
            draw(window);
            window.draw(arrow);
            window.display();

            sf::sleep(sf::milliseconds(10));
        }
        currentX = targetX;
        currentY = targetY;
    }

    hit_sound_.play();

    sf::CircleShape boom(5.0f);
    boom.setFillColor(sf::Color::Red);
    boom.setOrigin(5.0f, 5.0f);
    boom.setPosition(currentX, currentY);

    for (int i = 0; i < 10; ++i) {
        float radius = 5.0f + i * 2.5f;
        boom.setRadius(radius);
        boom.setOrigin(radius, radius);

        window.clear(sf::Color::Black);
        draw(window);
        window.draw(boom);
        window.display();
        sf::sleep(sf::milliseconds(20));
    }
}

void GameView::draw(sf::RenderWindow &window) {
    const auto& field = game_.getField();

    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            sf::RectangleShape tile(sf::Vector2f(tile_size_, tile_size_));
            tile.setPosition(x * tile_size_, y * tile_size_);
            switch (field.getCellType(x, y)) {
                case CellType::kEmpty:       tile.setFillColor(sf::Color(50, 50, 50)); break;
                case CellType::kObstacle:    tile.setFillColor(sf::Color(128, 128, 128)); break;
                case CellType::kSlowingTrap: tile.setFillColor(sf::Color(0, 100, 0)); break;
                default:                     tile.setFillColor(sf::Color(50, 50, 50)); break;
            }
            window.draw(tile);
        }
    }

    for(const auto& trap : game_.getTraps()) {
        sf::CircleShape trap_shape(tile_size_ / 3);
        trap_shape.setPosition(trap.getX() * tile_size_ + tile_size_/6, trap.getY() * tile_size_ + tile_size_/6);
        trap_shape.setFillColor(sf::Color(255, 100, 0, 150));
        window.draw(trap_shape);
    }
    for(const auto& building : game_.getBuildings()) {
        sf::RectangleShape tile(sf::Vector2f(tile_size_, tile_size_));
        tile.setPosition(building.getX() * tile_size_, building.getY() * tile_size_);
        tile.setFillColor(sf::Color(139, 69, 19));
        window.draw(tile);
    }
    for(const auto& tower : game_.getTowers()) {
        sf::RectangleShape tile(sf::Vector2f(tile_size_, tile_size_));
        tile.setPosition(tower.getX() * tile_size_, tower.getY() * tile_size_);
        tile.setFillColor(sf::Color(100, 100, 100));
        tile.setOutlineColor(sf::Color::Red);
        tile.setOutlineThickness(-2);
        window.draw(tile);
    }
    for(const auto& enemy : game_.getEnemies()) {
        sf::RectangleShape tile(sf::Vector2f(tile_size_, tile_size_));
        tile.setPosition(enemy.getX() * tile_size_, enemy.getY() * tile_size_);
        tile.setFillColor(sf::Color::Red);
        window.draw(tile);
    }
    for(const auto& ally : game_.getAllies()) {
        sf::RectangleShape tile(sf::Vector2f(tile_size_, tile_size_));
        tile.setPosition(ally.getX() * tile_size_, ally.getY() * tile_size_);
        tile.setFillColor(sf::Color::Blue);
        window.draw(tile);
    }
    const auto& player = game_.getPlayer();
    sf::RectangleShape player_tile(sf::Vector2f(tile_size_, tile_size_));
    player_tile.setPosition(player.getX() * tile_size_, player.getY() * tile_size_);
    player_tile.setFillColor(sf::Color::Green);
    window.draw(player_tile);

    float window_height = static_cast<float>(window.getSize().y);
    float ui_base_y = window_height - 120;

    sf::RectangleShape ui_bg(sf::Vector2f(static_cast<float>(window.getSize().x), 120));
    ui_bg.setPosition(0, ui_base_y);
    ui_bg.setFillColor(sf::Color(20, 20, 20));
    window.draw(ui_bg);

    std::string status = "Level: " + std::to_string(game_.getCurrentLevel()) +
                         " | HP: " + std::to_string(player.getHealth()) +
                         " | Score: " + std::to_string(player.getScore()) +
                         " | Mode: " + player.getModeName();
    status_text_.setString(status);
    status_text_.setPosition(10, ui_base_y + 5);
    window.draw(status_text_);

    sf::Text controls_text = status_text_;
    controls_text.setString("[F5] Save  [F6] Load  [N] Next Level (when clear)  [Q] Quit");
    controls_text.setCharacterSize(14);
    controls_text.setFillColor(sf::Color(180, 180, 180));
    controls_text.setPosition(10, ui_base_y + 30);
    window.draw(controls_text);

    std::string message = "Log: " + game_.getLastMessage();
    message_text_.setString(message);
    message_text_.setPosition(10, ui_base_y + 55);
    window.draw(message_text_);

    const auto& spells = game_.getPlayer().getHand().getSpells();
    std::string spells_display_text = "Spells: ";
    if (spells.empty()) {
        spells_display_text += "(Empty)";
    } else {
        for(size_t i = 0; i < spells.size(); ++i) {
            spells_display_text += "[" + std::to_string(i+1) + "] " + spells[i]->getName() + "  ";
        }
    }
    spells_text_.setString(spells_display_text);
    spells_text_.setPosition(10, ui_base_y + 80);
    window.draw(spells_text_);
}