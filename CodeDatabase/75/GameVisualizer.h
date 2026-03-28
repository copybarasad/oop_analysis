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

