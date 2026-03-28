#include "qtplatformmanager.h"

QTPlatformManager::QTPlatformManager(QWidget*)
{
    resize(600, 700);
    setWindowTitle("Warrior");
    show();

    auto gameModel = std::make_shared<GameModel>();
    m_controller = std::make_shared<Controller<QTInputHandler, QTRenderer, QTAudioManager>>(gameModel);
    m_timerID = startTimer(50);
}

void QTPlatformManager::timerEvent(QTimerEvent*)
{
    m_controller->update();
    update();
}
