#ifndef QTPLATFORMMANAGER_H
#define QTPLATFORMMANAGER_H

#include <QMainWindow>
#include <QTimerEvent>
#include <memory>
#include "controller.h"

class QTPlatformManager : public QMainWindow
{
    Q_OBJECT
public:
    QTPlatformManager(QWidget* parent = nullptr);
    virtual ~QTPlatformManager() = default;
    void timerEvent(QTimerEvent *event) override;

private:
    std::shared_ptr<Controller<QTInputHandler, QTRenderer, QTAudioManager>> m_controller;
    int m_timerID;
};

#endif // QTPLATFORMMANAGER_H
