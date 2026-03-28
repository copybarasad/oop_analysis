#include "Renderer.h"
#include <iostream>

SfmlRenderer::SfmlRenderer()
    : window(sf::VideoMode({800, 800}), "OOP Game Project")
{
    window.setFramerateLimit(30);

    if (!blockedTex.loadFromFile("textures/block.png")) std::cerr << "Err: block.png\n";
    if (!emptyTex.loadFromFile("textures/plain.png")) std::cerr << "Err: plain.png\n";
    if (!buildingTex.loadFromFile("textures/building.png")) std::cerr << "Err: building.png\n";
    if (!enemyTex.loadFromFile("textures/enemy.png")) std::cerr << "Err: enemy.png\n";
    if (!font.openFromFile("textures/arial.ttf")) std::cerr << "Warning: arial.ttf not found\n";
}

void SfmlRenderer::render(const GameLogic& game, int selectedSpellIndex) {
    window.clear(sf::Color::Black);

    GameState state = game.getState();

    if (state == GameState::Menu) {
        drawMenu();
    } else if (state == GameState::LevelUpMenu) {
        drawLevelUp();
    } else if (state == GameState::GameOver) {
        drawGameOver();
    } else if (state == GameState::Victory) {
        drawVictory();
    } else if (state == GameState::Playing && game.getField()) {
        drawField(*game.getField(), selectedSpellIndex);
        if (game.getPlayer()) {
            drawUI(*game.getPlayer(), selectedSpellIndex);
        }
    }

    window.display();
}

void SfmlRenderer::drawMenu() {
    sf::Text text(font, "MAIN MENU\n[N] New Game\n[L] Load Game\n[ESC] Exit\n[C] Continue", 30);
    text.setPosition({200, 200});
    window.draw(text);
}

void SfmlRenderer::drawLevelUp() {
    sf::Text title(font, "LEVEL COMPLETE!", 40);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition({200, 100});

    sf::Text subtitle(font, "Choose upgrade:\n[1] Health (+20)\n[2] Damage (+5)\n[3] Spell Upgrade", 25);
    subtitle.setPosition({150, 200});
    window.draw(title);
    window.draw(subtitle);
}

void SfmlRenderer::drawGameOver() {
    sf::Text text(font, "GAME OVER\n[N] New Game\n[L] Load Game", 30);
    text.setFillColor(sf::Color::Red);
    text.setPosition({200, 300});
    window.draw(text);
}

void SfmlRenderer::drawVictory() {
    sf::Text text(font, "VICTORY!\n[N] New Game", 30);
    text.setFillColor(sf::Color::Yellow);
    text.setPosition({250, 300});
    window.draw(text);
}

void SfmlRenderer::drawField(const Field& field, int selectedSpellIndex) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    int hoverX = mousePos.x / static_cast<int>(tileSize + spacing);
    int hoverY = mousePos.y / static_cast<int>(tileSize + spacing);

    for(int y=0; y<field.get_rows(); ++y) {
        for(int x=0; x<field.get_cols(); ++x) {
            float px = x * (tileSize + spacing);
            float py = y * (tileSize + spacing);
            sf::RectangleShape shape({tileSize, tileSize});
            shape.setPosition({px, py});

            const Cell& cell = field.get_cell(x, y);

            switch(cell.getType()) {
                case CellType::Empty:
                    if(cell.getTrap()) shape.setFillColor(sf::Color(0, 150, 150));
                    else shape.setTexture(&emptyTex);
                    break;
                case CellType::Blocked: shape.setTexture(&blockedTex); break;
                case CellType::Player: shape.setFillColor(sf::Color::Blue); break;
                case CellType::Enemy: shape.setTexture(&enemyTex); break;
                case CellType::Building: shape.setTexture(&buildingTex); break;
                case CellType::Tower: shape.setFillColor(sf::Color::Red); break;
                case CellType::Ally: shape.setFillColor(sf::Color::Green); break;
            }

            if(cell.getProperty() == CellProperty::Slowing) {
                shape.setOutlineColor(sf::Color::Yellow);
                shape.setOutlineThickness(2.f);
            }


            if (selectedSpellIndex != -1 && x == hoverX && y == hoverY) {
                shape.setFillColor(sf::Color(255, 100, 100, 150)); // Полупрозрачный красный
            }

            window.draw(shape);
        }
    }
}

void SfmlRenderer::drawUI(const Player& player, int selectedSpellIndex) {
    sf::Text stats(font, "", 20);
    stats.setPosition({10, 10});
    stats.setString("HP: " + std::to_string(player.get_health()) +
                    " | Score: " + std::to_string(player.get_score()) +
                    " | Buffs: " + std::to_string(player.getBuffCharges()));
    stats.setFillColor(sf::Color::White);
    stats.setOutlineColor(sf::Color::Black);
    stats.setOutlineThickness(1);
    window.draw(stats);

    if (selectedSpellIndex != -1) {
        sf::Text spellText(font, "Spell Selected: " + std::to_string(selectedSpellIndex + 1), 20);
        spellText.setPosition({10, 40});
        spellText.setFillColor(sf::Color::Green);
        window.draw(spellText);
    }
}