#include "PrintMessageEvent.h"
#include <sstream>
#include <iomanip>
#include <ctime>

PrintMessageEvent::PrintMessageEvent(const std::string& msg)
    : GameEvent(EventType::MESSAGE_PRINT, ""),
      message(msg) {
}

std::string PrintMessageEvent::toString() const {
    std::stringstream ss;
    time_t timestamp = getTimestamp();
    std::tm* timeInfo = std::localtime(&timestamp);
    ss << std::put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
    ss << " [" << static_cast<int>(getType()) << "] Вывод сообщения: " << message;
    return ss.str();
}

