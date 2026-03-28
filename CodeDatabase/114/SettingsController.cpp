#include "SettingsController.h"
#include "../models/SaveLoadException.h"
#include "../models/serializers/SettingsSerializer.h"

#include "../models/events/EventNotifier.h"
#include "../models/events/PrintMessageEvent.h"

SettingsController::SettingsController(const std::string& settingsFilename)
    : settingsFilename(settingsFilename) {}

Settings SettingsController::fallbackToDefault() {
    return Settings::createDefault();
}

Settings SettingsController::loadFromFile() {
    try {
        auto json = fileController.loadJson(settingsFilename, false);
        SettingsSerializer serializer;
        Settings settings = Settings::createDefault();
        serializer.deserialize(settings, json);
        return settings;
    } catch (const LoadException& e) {
        // Файл отсутствует или невалиден — создаём дефолтные настройки и сохраняем
        PrintMessageEvent failEvent(std::string("Ошибка чтения настроек: ") + e.what());
        EventNotifier::getInstance().notify(failEvent);
        Settings settings = fallbackToDefault();
        try {
            SettingsSerializer serializer;
            fileController.saveJson(settingsFilename, serializer.serialize(settings));
        } catch (const SaveLoadException&) {
            // Если сохранить не удалось, продолжаем с дефолтными (но не маскируем исключение выше)
        }
        return settings;
    } catch (const std::exception& e) {
        PrintMessageEvent failEvent(std::string("Ошибка чтения настроек: ") + e.what());
        EventNotifier::getInstance().notify(failEvent);
        return fallbackToDefault();
    }
}

const Settings& SettingsController::getSettings() {
    if (!cachedSettings.has_value()) {
        cachedSettings = loadFromFile();
    }
    return *cachedSettings;
}

void SettingsController::saveSettings(const Settings& settings) {
    Settings toStore = settings.isValid() ? settings : fallbackToDefault();
    cachedSettings = toStore;
    SettingsSerializer serializer;
    fileController.saveJson(settingsFilename, serializer.serialize(toStore));
}

