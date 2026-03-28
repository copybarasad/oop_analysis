#ifndef GAME_MARK_H
#define GAME_MARK_H

#include "../basics/Object.h"

class Mark final : public Object {
public:
    explicit Mark(const std::string &name = "Range") : Object(name + "Mark") {
    };

    void saveToJson(nlohmann::json *savefile) override {
    };
};
#endif
