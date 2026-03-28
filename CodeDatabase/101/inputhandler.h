#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "keyconfig.h"
#include <queue>
#include <vector>
#include <QKeyEvent>
#include <QPushButton>
#include <QDrag>
#include <QMimeData>

struct GameCommand {
    enum Type {
        MOVE_LEFT,
        MOVE_RIGHT,
        MOVE_UP,
        MOVE_DOWN,
        ATTACK_CLOSE,
        SWITCH_WEAPON,
        PAUSE_GAME,
        NEW_GAME,
        LOAD_GAME,
        QUIT_GAME,
        CONTINUE_LEVEL,
        MOUSE_CLICK,
        MOUSE_DROP,
        START_DRAG
    };

    Type type;
    std::string data;
    int x, y;

    GameCommand(Type t = MOUSE_CLICK, const std::string& d = "", int px = 0, int py = 0)
        : type(t), data(d), x(px), y(py) {}
};

class InputHandler {
private:
    std::queue<GameCommand> commands;
    KeyConfig& keyConfig;

public:
    InputHandler();

    void handleKeyPress(int keyCode);

    void handleMouseClick(QMouseEvent* e);
    void handleMouseMove(QMouseEvent* e, void* widget, std::vector<QPushButton*> buttons);
    void handleDragEnter(void* event);
    void handleDrop(void* event);
    void handleKeyPressEvent(QKeyEvent* e);
    bool hasCommands() const;
    GameCommand getNextCommand();

    void clearCommands();
    size_t commandCount() const;
};

#endif // INPUTHANDLER_H
