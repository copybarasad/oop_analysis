#ifndef SETTINGSSERIALIZER_H
#define SETTINGSSERIALIZER_H

#include "ISerializer.h"
#include "../Settings.h"

class SettingsSerializer : public ISerializer<Settings> {
public:
    nlohmann::json serialize(const Settings& settings) const override;
    void deserialize(Settings& settings, const nlohmann::json& json) const override;
};

#endif

