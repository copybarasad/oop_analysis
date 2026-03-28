#include "RenderController.h"
#include "../views/GeneralView.h"
#include "../models/GameField.h"
#include "../models/events/EventNotifier.h"
#include "../models/events/RenderFieldEvent.h"
#include "../models/events/PrintMessageEvent.h"

template <class Renderer>
RenderController<Renderer>::RenderController()
    : renderer(std::make_unique<Renderer>()) {
    EventNotifier::getInstance().subscribe(this);
}

template <class Renderer>
RenderController<Renderer>::~RenderController() {
    EventNotifier::getInstance().unsubscribe(this);
}

template <class Renderer>
void RenderController<Renderer>::onEvent(const GameEvent& event) {
    if (!renderer) {
        return;
    }
    
    EventType type = event.getType();
    
    if (type == EventType::FIELD_RENDER_REQUEST) {
        // Отрисовка поля
        const RenderFieldEvent* renderEvent = static_cast<const RenderFieldEvent*>(&event);
        if (renderEvent && renderEvent->getField()) {
            GameField* field = renderEvent->getField();
            renderer->printField(*field);
            renderer->printPlayer(field->getPlayer());
            renderer->printEnemies(field->getEnemies());
            renderer->printBuildings(field->getBuilding());
        }
    } else if (type == EventType::MESSAGE_PRINT) {
        // Вывод сообщения
        const PrintMessageEvent* printEvent = static_cast<const PrintMessageEvent*>(&event);
        if (printEvent) {
            renderer->print(printEvent->getMessage());
        }
    }
}

template RenderController<GeneralView>::RenderController();
template RenderController<GeneralView>::~RenderController();
