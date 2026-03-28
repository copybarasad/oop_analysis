#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "GameSession.h"
#include "IInputHandler.h"
#include <memory>

template<typename InputHandlerType>
class GameManager {
private:
    GameSession& m_session;
    InputHandlerType m_inputHandler;

public:
    GameManager(GameSession& session) : m_session(session) {}

    void processNextCommand() {
        auto command = m_inputHandler.getCommand();
        if (command) {
            command->execute(m_session);
        }
    }
};

#endif // GAMEMANAGER_H