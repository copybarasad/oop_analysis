#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include "FileController.h"
#include "../models/Settings.h"
#include <optional>
#include <string>

class SettingsController {
private:
    std::string settingsFilename;
    FileController fileController;
    std::optional<Settings> cachedSettings;

    Settings loadFromFile();
    static Settings fallbackToDefault();

public:
    explicit SettingsController(const std::string& settingsFilename = "settings.json");

    const Settings& getSettings();
    void saveSettings(const Settings& settings);
    void resetCache() { cachedSettings.reset(); }
};

#endif

