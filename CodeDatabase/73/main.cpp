#include <SFML/Graphics.hpp>
#include "Game.h"
#include "GameView.h"
#include "GameExceptions.h"
#include "InputHandler.h"
#include "GameLauncher.h"
#include "Logger.h"
#include <iostream>
#include <cstring>

const float TILE_SIZE = 32.0f;

void showEndScreen(sf::RenderWindow& window, const std::string& message, sf::Font& font) {
    sf::Text endText(message, font, 40);
    endText.setFillColor(sf::Color::White);
    sf::FloatRect textRect = endText.getLocalBounds();
    endText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    endText.setPosition(window.getSize().x/2.0f, window.getSize().y/2.0f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed)) {
                window.close();
                return;
            }
        }
        window.clear(sf::Color::Black);
        window.draw(endText);
        window.display();
    }
}

int main(int argc, char* argv[]) {
    try {
        int field_width = 15;
        int field_height = 15;
        Game game(field_width, field_height);

        ILogger* logger = nullptr;
        bool useFileLog = false;

        if (argc >= 3 && std::strcmp(argv[1], "-log") == 0) {
            if (std::strcmp(argv[2], "file") == 0) {
                logger = new FileLogger("game_log.txt");
                useFileLog = true;
                std::cout << "Logging to game_log.txt" << std::endl;
            } else {
                logger = new ConsoleLogger();
                std::cout << "Logging to console" << std::endl;
            }
        }

        if (logger) {
            game.addObserver(logger);
        }

        InputHandler inputHandler;
        inputHandler.loadConfig("controls.conf");

        sf::RenderWindow window(sf::VideoMode(900, 950), "RPG Game Lab 4", sf::Style::Titlebar | sf::Style::Close);
        window.setFramerateLimit(60);

        GameView gameView(game, TILE_SIZE);
        
        if (!gameView.loadFont("arial.ttf")) {
            std::cerr << "Error loading font!" << std::endl;
        }

        if (!gameView.loadSounds("shoot.wav", "hit.wav")) {
            std::cerr << "Warning: Could not load sound files (shoot.wav, hit.wav)" << std::endl;
        }


        GameController<InputHandler> controller(game, inputHandler);
        GameVisualizer<GameView> visualizer(game, gameView);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                if (event.type == sf::Event::KeyPressed) {
                    try {
                        if (event.key.code == sf::Keyboard::Q) window.close();

                        if (game.getGameState() == GameState::UpgradeMenu) {
                            if (event.key.code == sf::Keyboard::Num1) game.upgradePlayer(1);
                            if (event.key.code == sf::Keyboard::Num2) game.upgradePlayer(2);
                        } else {
                            controller.handleInput(event);
                        }

                        if (game.hasNewShotAnimation()) {
                            gameView.playShotAnimation(window);
                            game.clearShotAnimation();
                            game.endPlayerTurn();
                        }

                    } catch (const GameException& e) {
                        game.setLastMessage(std::string("Error: ") + e.what());
                    }
                }

                if (game.getGameState() == GameState::SelectingSpellTarget) {
                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                        int tx = event.mouseButton.x / TILE_SIZE;
                        int ty = event.mouseButton.y / TILE_SIZE;
                        game.applySpell(tx, ty);
                    }
                }
            }

            window.clear(sf::Color::Black);

            if (game.getGameState() == GameState::GameOver) {
                sf::Font f; f.loadFromFile("arial.ttf");
                showEndScreen(window, "GAME OVER (Press Key)", f);
                window.close();
            } else if (game.getGameState() == GameState::Victory) {
                sf::Font f; f.loadFromFile("arial.ttf");
                showEndScreen(window, "VICTORY!", f);
                window.close();
            } else {
                visualizer.render(window);
            }

            window.display();
        }

        if (logger && useFileLog) delete logger;
        else if (logger) delete logger;

    } catch (const std::exception& e) {
        std::cerr << "Critical: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}