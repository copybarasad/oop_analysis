#include "inputhandler.h"
#include <iostream>

InputHandler::InputHandler() : keyConfig(KeyConfig::getInstance()) {

    if (!keyConfig.loadConfig()) {
        std::cerr << "Используется конфигурация управления по умолчанию" << std::endl;
    }
}

void InputHandler::handleKeyPress(int keyCode) {

    CommandType cmdType = keyConfig.getCommand(keyCode);

    if (static_cast<int>(cmdType) != -1) {
        GameCommand cmd;
        cmd.x = 0;
        cmd.y = 0;


        switch (cmdType) {
        case CommandType::MOVE_LEFT:
            cmd.type = GameCommand::MOVE_LEFT;
            break;
        case CommandType::MOVE_RIGHT:
            cmd.type = GameCommand::MOVE_RIGHT;
            break;
        case CommandType::MOVE_UP:
            cmd.type = GameCommand::MOVE_UP;
            break;
        case CommandType::MOVE_DOWN:
            cmd.type = GameCommand::MOVE_DOWN;
            break;
        case CommandType::ATTACK_CLOSE:
            cmd.type = GameCommand::ATTACK_CLOSE;
            break;
        case CommandType::SWITCH_WEAPON:
            cmd.type = GameCommand::SWITCH_WEAPON;
            break;
        case CommandType::PAUSE_GAME:
            cmd.type = GameCommand::PAUSE_GAME;
            break;
        case CommandType::NEW_GAME:
            cmd.type = GameCommand::NEW_GAME;
            break;
        case CommandType::LOAD_GAME:
            cmd.type = GameCommand::LOAD_GAME;
            break;
        case CommandType::QUIT_GAME:
            cmd.type = GameCommand::QUIT_GAME;
            break;
        case CommandType::CONTINUE_LEVEL:
            cmd.type = GameCommand::CONTINUE_LEVEL;
            break;
        default:
            cmd.type = GameCommand::MOUSE_CLICK;
            break;
        }

        commands.push(cmd);
    }
}



void InputHandler::handleMouseClick(QMouseEvent* e) {
    if (!e) return;

    if (e->button() == Qt::LeftButton) {
        GameCommand cmd(GameCommand::MOUSE_CLICK);
        cmd.x = e->x();
        cmd.y = e->y();
        commands.push(cmd);
    }
}

void InputHandler::handleMouseMove(QMouseEvent* e, void* widgetPtr, std::vector<QPushButton*> buttons) {
    if (!e) return;

    QWidget* widget = static_cast<QWidget*>(widgetPtr);

    if (e->buttons() & Qt::LeftButton) {
        for (QPushButton* btn : buttons) {
            if (btn && btn->geometry().contains(e->pos())) {
                QString str = btn->property("spellName").toString();
                if (!str.isEmpty()) {

                    QDrag* drag = new QDrag(widget);
                    QMimeData* mime = new QMimeData();
                    mime->setText(str);
                    drag->setMimeData(mime);

                    QPixmap picture(40, 40);
                    picture.fill(Qt::red);
                    drag->setPixmap(picture);
                    drag->setHotSpot(QPoint(20, 20));

                    drag->exec(Qt::CopyAction | Qt::MoveAction);

                    GameCommand cmd(GameCommand::START_DRAG);
                    cmd.data = str.toStdString();
                    cmd.x = e->x();
                    cmd.y = e->y();
                    commands.push(cmd);

                    break;
                }
            }
        }
    }
}

void InputHandler::handleDragEnter(void* event) {
// Принимаем drag event
#ifdef USE_QT
    QDragEnterEvent* dragEvent = static_cast<QDragEnterEvent*>(event);
    dragEvent->acceptProposedAction();
#endif
}

void InputHandler::handleDrop(void* event) {
#ifdef USE_QT
    QDropEvent* dropEvent = static_cast<QDropEvent*>(event);
    const QMimeData* mimeData = dropEvent->mimeData();

    if (mimeData && mimeData->hasText()) {
        std::string spellName = mimeData->text().toStdString();
        QPoint pos = dropEvent->pos();

        GameCommand cmd(GameCommand::MOUSE_DROP, spellName, pos.x(), pos.y());
        commands.push(cmd);

        dropEvent->acceptProposedAction();
    }
#endif
}

GameCommand InputHandler::getNextCommand() {
    if (commands.empty()) {
        return GameCommand();
    }

    GameCommand cmd = commands.front();
    commands.pop();
    return cmd;
}

void InputHandler::handleKeyPressEvent(QKeyEvent* e) {
    if (!e) return;

    int keyCode = e->key();
    handleKeyPress(keyCode);
}

void InputHandler::clearCommands() { while (!commands.empty()) commands.pop(); }

size_t InputHandler::commandCount() const { return commands.size(); }

bool InputHandler::hasCommands() const { return !commands.empty(); }
