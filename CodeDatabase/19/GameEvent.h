#pragma once
#include <string>

class GameEvent {
public:
    GameEvent(const std::string &type, const std::string &details)
        : type_(type),
          details_(details) {
    }

    [[nodiscard]] std::string type() const {
        return type_;
    }

    [[nodiscard]] std::string details() const {
        return details_;
    }

private:
    std::string type_;
    std::string details_;
};
