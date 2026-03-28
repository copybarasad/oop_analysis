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
