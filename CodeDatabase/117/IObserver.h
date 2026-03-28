#ifndef IOBSERVER_H
#define IOBSERVER_H

#include "Event.h"

class IObserver {
public:
    virtual void seeEvent(const Event& event) = 0;
    virtual ~IObserver() = default;
};

#endif