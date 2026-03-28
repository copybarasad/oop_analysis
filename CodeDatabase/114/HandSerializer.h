#ifndef HANDSERIALIZER_H
#define HANDSERIALIZER_H

#include "ISerializer.h"
#include "../Hand.h"

class HandSerializer : public ISerializer<Hand> {
public:
    nlohmann::json serialize(const Hand& hand) const override;
    void deserialize(Hand& hand, const nlohmann::json& json) const override;
};

#endif

