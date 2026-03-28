#ifndef GAME_ALLYSPAWNER_H
#define GAME_ALLYSPAWNER_H

#include  "../basics/Object.h"


class AllyBase : public Object {
private:
    int amount_;

public:
    explicit AllyBase(const int amount) : Object("AllyBase", ObjTypes::Neither),
                                                   amount_(amount) {
    };

    void spawnAlly();

    void update() override;

    void saveToJson(nlohmann::json *savefile) override;

    void loadFromJson(nlohmann::json *savefile) override;
};


#endif
