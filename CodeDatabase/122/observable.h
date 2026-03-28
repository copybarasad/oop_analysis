#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <memory>
#include "observer.h"
#include "constants.h"

class Observable
{
public:
    virtual void addObserver(std::unique_ptr<Observer> observer) = 0;
    virtual void removeDestroyedObservers() = 0;
    virtual void notifyObserver(int x, int y) = 0;

};

#endif // OBSERVABLE_H
