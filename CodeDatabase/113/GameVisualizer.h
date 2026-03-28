#ifndef GAMEVISUALIZER_H
#define GAMEVISUALIZER_H

#include "GameSession.h"
#include "IGameRenderer.h"

template<typename RendererType>
class GameVisualizer {
private:
    GameSession& m_session;
    RendererType m_renderer;

public:
    GameVisualizer(GameSession& session) : m_session(session) {}

    void renderGameState() {
        auto& gameState = m_session.getGameState();
        auto& player = m_session.getPlayer();

        // 1. Статистика
        m_renderer.renderStats(player, gameState.turnCount,
            gameState.enemies, gameState.towers,
            gameState.field);

        // 2. Заклинания
        m_renderer.renderSpellInfo(player);

        // 3. Поле
        m_renderer.renderField(gameState.field, player,
            gameState.enemies, gameState.towers);

        // 4. Подсказки по управлению
        m_renderer.renderControls();
    }

    void renderGameStart() {
        m_renderer.renderGameStart();
    }

    void renderVictory() {
        auto& gameState = m_session.getGameState();
        auto& player = m_session.getPlayer();
        m_renderer.renderVictory(gameState.turnCount, player.getScore());
    }

    void renderGameOver() {
        auto& gameState = m_session.getGameState();
        auto& player = m_session.getPlayer();
        m_renderer.renderGameOver(gameState.turnCount, player.getScore());
    }

    void renderInvalidCommand() {
        m_renderer.renderInvalidCommand();
    }

    RendererType& getRenderer() { return m_renderer; }
};

#endif // GAMEVISUALIZER_H