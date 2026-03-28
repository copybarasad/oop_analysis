#ifndef GAME_IOBSERVER_H
#define GAME_IOBSERVER_H

#include "Event.h"

namespace Game {

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void onNotify(const Event& event) = 0;
};

}

#endif
