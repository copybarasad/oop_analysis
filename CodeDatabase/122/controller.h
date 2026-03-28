#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "gamecommands.h"
#include "qtinputhandler.h"
#include "view.h"
#include "qtaudiomanager.h"
#include "logger.h"

template<typename TInputHandler, typename TRenderer, typename TAudioManager>
class Controller
{
public:
    Controller(std::shared_ptr<GameModel> gameModel)
        :
        m_inputHandler(std::make_shared<TInputHandler>()),
        m_gameModel(gameModel),
        m_audioManager(std::make_shared<TAudioManager>()),
        m_renderer(std::make_shared<View<QTRenderer>>()),
        m_logger(std::make_shared<Logger>())
    {
        m_renderer->renderInitState();
        m_inputHandler->setState(GameState::INIT_STATE);
        m_audioManager->soundInitState();
    }

    void update()
    {
        auto command = m_inputHandler->getNextCommand();
        if (command)
        {
            m_audioManager->stopAllSounds();
            command->execute(*m_gameModel);
            if (m_gameModel->inLevel())
            {
                m_inputHandler->setState(GameState::PLAYING_STATE);
                m_gameModel->updateGameState();

                if (m_gameModel->isLevelComplete())
                {
                    m_inputHandler->setState(GameState::LEVEL_COMPLETE);
                    m_renderer->renderLevelComplete();
                    m_audioManager->soundLevelComplete();
                    return;
                }
                if (m_gameModel->isLevelFailed())
                {
                    m_inputHandler->setState(GameState::LEVEL_FAILED);
                    m_renderer->renderLevelFailed();
                    m_audioManager->soundLevelFailed();
                    return;
                }
            }
        }
    }

private:
    std::shared_ptr<TInputHandler> m_inputHandler;
    std::shared_ptr<GameModel> m_gameModel;
    std::shared_ptr<TAudioManager> m_audioManager;
    std::shared_ptr<View<TRenderer>> m_renderer;
    std::shared_ptr<Logger> m_logger;
};

#endif // CONTROLLER_H
