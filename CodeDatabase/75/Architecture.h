#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <optional>
#include <map>
#include "Game.h"
#include "GameRender.h"

// 1. Объект команды
enum class CommandType {
    None, 
    Move, // Комбинированная команда движения
    MoveUp, MoveDown, MoveLeft, MoveRight,
    Attack, SwitchMode, CastSpell, NextSpell, PrevSpell,
    Pause, Resume, Exit, Save, Load, // Exit = Escape
    
    // TargetMove - ОБЩАЯ команда (для контроллера)
    TargetMove, 
    
    // Специфичные команды из конфига (для биндинга клавиш)
    TargetMoveUp, TargetMoveDown, TargetMoveLeft, TargetMoveRight,
    TargetSelect, TargetCancel,
    
    OpenShop, Quit, Action
};

struct GameCommand {
    CommandType type;
    int dataX = 0; 
    int dataY = 0;
};

class SfmlInputHandler {
public:
    SfmlInputHandler(sf::RenderWindow& win, const std::map<sf::Keyboard::Key, CommandType>& bindings) 
        : m_window(win), m_bindings(bindings) {}

    GameCommand getCommand() {
        while (const std::optional event = m_window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) return {CommandType::Quit};
            
            if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                auto it = m_bindings.find(keyEvent->code);
                if (it != m_bindings.end()) {
                    CommandType cmd = it->second;
                    
                    // Преобразуем специфичные команды в общую TargetMove
                    if (cmd == CommandType::TargetMoveUp) return {CommandType::TargetMove, 0, -1};
                    if (cmd == CommandType::TargetMoveDown) return {CommandType::TargetMove, 0, 1};
                    if (cmd == CommandType::TargetMoveLeft) return {CommandType::TargetMove, -1, 0};
                    if (cmd == CommandType::TargetMoveRight) return {CommandType::TargetMove, 1, 0};
                    
                    if (cmd == CommandType::TargetSelect) return {CommandType::Action};
                    if (cmd == CommandType::Exit) return {CommandType::Pause};

                    return {cmd};
                }
            }
        }

        // Непрерывный ввод (Движение)
        int dx = 0;
        int dy = 0;
        
        for (const auto& [key, cmd] : m_bindings) {
            if (sf::Keyboard::isKeyPressed(key)) {
                if (cmd == CommandType::MoveUp) dy -= 1;
                if (cmd == CommandType::MoveDown) dy += 1;
                if (cmd == CommandType::MoveLeft) dx -= 1;
                if (cmd == CommandType::MoveRight) dx += 1;
            }
        }

        if (dx != 0 || dy != 0) {
            return {CommandType::Move, dx, dy};
        }

        return {CommandType::None};
    }

private:
    sf::RenderWindow& m_window;
    std::map<sf::Keyboard::Key, CommandType> m_bindings;
};

// 3. Визуализатор
template <typename RenderT>
class GameVisualizer {
public:
    GameVisualizer(sf::RenderWindow& window) : m_renderer(window) {}

    void update(float dt) { m_renderer.update(dt); }
    void changeZoom(float delta) { m_renderer.changeZoom(delta); }

    void render(Game& game) {
        m_renderer.render(
            game.getField(), game.getPlayer(), game.getEnemies(), game.getAllies(),
            game.getTraps(), game.getThorns(), game.getShields(), game.getWorldObjects(),
            game.getVisualEvents(), game.isTargeting(), game.getTargetX(), game.getTargetY()
        );
        m_renderer.drawTowers(game.getTowers());
        if (game.getState() == Game::State::HUB) m_renderer.drawHub(game.getPortal(), game.getShop());
        if (game.isPaused()) m_renderer.drawPaused();
        if (game.getState() == Game::State::GAME_OVER) m_renderer.drawGameOver();
        if (game.getState() == Game::State::VICTORY) m_renderer.drawVictory(game.getLevelIndex());
        if (game.getState() == Game::State::GAME_COMPLETED) m_renderer.drawGameCompleted();
        if (game.isShopOpen()) m_renderer.drawShopUI(game.getShop(), game.getPlayer());
    }

private:
    RenderT m_renderer;
};

// 4. Контроллер
template <typename InputT, typename VisualizerT>
class GameController {
public:
    GameController(Game& game, InputT& input, VisualizerT& viz, sf::RenderWindow& win) 
        : m_game(game), m_input(input), m_viz(viz), m_window(win) {}

    void run() {
        sf::Clock clock;
        while (m_window.isOpen() && m_game.isRunning()) {
            float dt = clock.restart().asSeconds();
            GameCommand cmd = m_input.getCommand();
            
            // ЛОГИКА ЗУМА: Если нажаты стрелки (TargetMove), но мы НЕ целимся -> меняем зум
            if (cmd.type == CommandType::TargetMove && !m_game.isTargeting()) {
                if (cmd.dataY < 0) m_viz.changeZoom(0.9f);
                if (cmd.dataY > 0) m_viz.changeZoom(1.1f);
            } else {
                m_game.executeCommand(cmd);
            }
            
            if (!m_game.isPaused()) m_game.update(dt);
            m_viz.update(dt);
            m_viz.render(m_game);
            m_window.display();
        }
    }

private:
    Game& m_game;
    InputT& m_input;
    VisualizerT& m_viz;
    sf::RenderWindow& m_window;
};