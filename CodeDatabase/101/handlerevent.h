#ifndef HANDLEREVENT_H
#define HANDLEREVENT_H
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QDropEvent>
#include <QDragEnterEvent>



class HandlerEvent {
public:
    virtual ~HandlerEvent() = default;

    virtual void keyboardHandler(QKeyEvent* e) = 0;
    virtual void mouseMoveHandler(QMouseEvent* e) = 0;
    virtual void mouseClickHandler(QMouseEvent* e) = 0;
    virtual void mouseDragHandler(QDragEnterEvent* e) = 0;
    virtual void mouseDropHandler(QDropEvent* e) = 0;
};

#endif // HANDLEREVENT_H
