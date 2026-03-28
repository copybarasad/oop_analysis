#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QApplication>
#include <QDebug>
#include <fstream>
#include "gamemodel.h"
#include "visitor.h"
#include "inputhandler.h"
#include "visualization.h"
#include "handlerevent.h"
#include "constants.h"

template<typename TInputHandler, typename TRenderer>
class GameController : public QObject, public HandlerEvent {

private:
    int timerId;
    GameModel* gameModel;
    Visitor* visitor;
    TInputHandler inputHandler;
    Visualization<TRenderer>* visualizer;
    bool isRunning;

public:
    GameController(GameModel& model, Visualization<TRenderer>* visualizer = nullptr);
    ~GameController();


    void startNewGame();

    void timerEvent(QTimerEvent*) override;

    void processCommands();


    void keyboardHandler(QKeyEvent* e) override;

    void mouseMoveHandler(QMouseEvent* e) override;

    void mouseClickHandler(QMouseEvent* e) override;

    void mouseDragHandler(QDragEnterEvent* e) override;

    void mouseDropHandler(QDropEvent* e) override;



    void saveGame();
    void loadGame();

    HandlerEvent* getEventHandler();

    Visualization<TRenderer>* getVisualizer();

private:
    void executeCommand(const GameCommand& cmd);
};

#endif // GAMECONTROLLER_H
