#pragma once
#include <vector>
#include "GameEvent.h"

class GameEventBus {
public:
    class Listener {
    public:
        virtual ~Listener() = default;
        virtual void OnEvent(const GameEvent& event) = 0;
    };

    void Subscribe(Listener* listener);
    void Unsubscribe(Listener* listener);
    void Publish(const GameEvent& event) const;

private:
    std::vector<Listener*> listeners_;
};
