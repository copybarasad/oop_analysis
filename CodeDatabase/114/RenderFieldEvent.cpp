#include "RenderFieldEvent.h"
#include <sstream>
#include <iomanip>
#include <ctime>

RenderFieldEvent::RenderFieldEvent(GameField* gameField)
    : GameEvent(EventType::FIELD_RENDER_REQUEST, ""),
      field(gameField) {
}

std::string RenderFieldEvent::toString() const {
    std::stringstream ss;
    time_t timestamp = getTimestamp();
    std::tm* timeInfo = std::localtime(&timestamp);
    ss << std::put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
    ss << " [" << static_cast<int>(getType()) << "] Запрос на отрисовку поля";
    return ss.str();
}

