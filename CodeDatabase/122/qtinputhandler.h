#ifndef QTINPUTHANDLER_H
#define QTINPUTHANDLER_H

#include "constants.h"
#include "gamecommands.h"
#include <map>
#include <queue>
#include <QObject>

class QTInputHandler : QObject
{
public:
    QTInputHandler();

    bool eventFilter(QObject* obj, QEvent* event) override;
    void handleKeyPress(int key);
    void handleMouseButton(int x, int y, int widthWidget);
    std::shared_ptr<GameCommand> getNextCommand();
    void setState(GameState state);

private:
    void setupBindings();

    std::unordered_map<GameState, std::unordered_map<int, std::function<std::shared_ptr<GameCommand>()>>> m_stateKeyBindings;
    std::queue<std::shared_ptr<GameCommand>> m_commandQueue;
    GameState currentState{GameState::INIT_STATE};
};

#endif // QTINPUTHANDLER_H
