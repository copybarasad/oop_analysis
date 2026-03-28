#include "qtinputhandler.h"
#include <QKeyEvent>
#include <QWidget>

QTInputHandler::QTInputHandler()
{
    qApp->installEventFilter(this);
    setupBindings();
}

bool QTInputHandler::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        handleKeyPress(keyEvent->key());
        return true;
    }
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QWidget* widget = qobject_cast<QWidget*>(obj);
        if (!widget)
            return false;
        QPoint pos = mouseEvent->pos();
        handleMouseButton(pos.x(), pos.y(), widget->width());
        return true;
    }
    return QObject::eventFilter(obj, event);
}


void QTInputHandler::handleKeyPress(int key)
{
    auto& bindings = m_stateKeyBindings[currentState];
    auto it = bindings.find(key);
    if (it != bindings.end()) {
        m_commandQueue.push(it->second());
    }
}

void QTInputHandler::handleMouseButton(int x, int y, int widthWidget)
{
    m_commandQueue.push(std::make_shared<UseSpellCommand>(std::pair<int,int>{x, y}, widthWidget));
}

std::shared_ptr<GameCommand> QTInputHandler::getNextCommand()
{
    if (m_commandQueue.empty()) return nullptr;
    auto cmd = m_commandQueue.front();
    m_commandQueue.pop();
    return cmd;
}

void QTInputHandler::setState(GameState state)
{
    currentState = state;
    while (!m_commandQueue.empty()) m_commandQueue.pop();
}

void QTInputHandler::setupBindings()
{
    m_stateKeyBindings[GameState::INIT_STATE] = {
        {Qt::Key_N, [](){ return std::make_shared<NewGameCommand>(); }},
        {Qt::Key_L, [](){ return std::make_shared<LoadGameCommand>(); }}
    };

    m_stateKeyBindings[GameState::PLAYING_STATE] = {
        {Qt::Key_W, [](){ return std::make_shared<MoveCommand>(0, -1); }},
        {Qt::Key_A, [](){ return std::make_shared<MoveCommand>(-1, 0); }},
        {Qt::Key_S, [](){ return std::make_shared<MoveCommand>(0, 1); }},
        {Qt::Key_D, [](){ return std::make_shared<MoveCommand>(1, 0); }},
        {Qt::Key_Space, [](){ return std::make_shared<AttackCommand>(); }},
        {Qt::Key_Shift, [](){ return std::make_shared<ChangeTypeBattleCommand>(); }},
        {Qt::Key_1, [](){ return std::make_shared<BuyCardCommand>(CardType::FIREBALL); }},
        {Qt::Key_2, [](){ return std::make_shared<BuyCardCommand>(CardType::EARTHQUAKE); }},
        {Qt::Key_3, [](){ return std::make_shared<BuyCardCommand>(CardType::POISON_TRAP); }},
        {Qt::Key_4, [](){ return std::make_shared<BuyCardCommand>(CardType::SUMMON_ARCHER); }},
        {Qt::Key_5, [](){ return std::make_shared<BuyCardCommand>(CardType::UPGRADE_CARD); }},
        {Qt::Key_Z, [](){ return std::make_shared<UseDamageCardCommand>(CardType::FIREBALL); }},
        {Qt::Key_X, [](){ return std::make_shared<UseDamageCardCommand>(CardType::EARTHQUAKE); }},
        {Qt::Key_C, [](){ return std::make_shared<UseDamageCardCommand>(CardType::POISON_TRAP); }},
        {Qt::Key_V, [](){ return std::make_shared<UseDamageCardCommand>(CardType::SUMMON_ARCHER); }},
        {Qt::Key_B, [](){ return std::make_shared<UseUpgradeCardCommand>(); }},
        {Qt::Key_M, [](){ return std::make_shared<SaveGameCommand>(); }},
        {Qt::Key_L, [](){ return std::make_shared<LoadGameCommand>(); }}
    };

    m_stateKeyBindings[GameState::LEVEL_COMPLETE] = {
        {Qt::Key_H, [](){ return std::make_shared<UpgradeHealthCommand>(); }},
        {Qt::Key_U, [](){ return std::make_shared<UpgradeCardCommand>(); }}
    };

    m_stateKeyBindings[GameState::LEVEL_FAILED] = {
        {Qt::Key_N, [](){ return std::make_shared<NewGameCommand>(); }}
    };
}
