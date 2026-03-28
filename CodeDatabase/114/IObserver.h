#ifndef IOBSERVER_H
#define IOBSERVER_H

class GameEvent;

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void onEvent(const GameEvent& event) = 0;
};

#endif

