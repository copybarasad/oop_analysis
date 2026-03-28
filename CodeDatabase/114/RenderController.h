#ifndef RENDERCONTROLLER_H
#define RENDERCONTROLLER_H

#include "../models/GameField.h"
#include "../models/events/IObserver.h"
#include "../models/events/GameEvent.h"
#include <memory>
#include <utility>

class GeneralView;

template <class Renderer>
class RenderController : public IObserver {
private:
    std::unique_ptr<Renderer> renderer;

public:
    explicit RenderController();
    
    ~RenderController();

    void onEvent(const GameEvent& event);

};

#endif

