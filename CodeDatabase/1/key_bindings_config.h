#pragma once
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <set>
#include <cctype>

class EventSystem;

class KeyBindingsConfig {
private:
    std::map<std::string, std::string> key_bindings_;
    std::map<std::string, std::string> default_bindings_;
    
    void initializeDefaultBindings();
    
    std::string normalizeKey(const std::string& key) const;
    
    bool hasDuplicateKeys() const;
    bool hasDuplicateCommands() const;
    bool hasMissingCommands() const;
    bool hasUnknownCommands() const;
    
    bool createDefaultConfigFile(const std::string& filename) const;

public:
    KeyBindingsConfig();
    
    void loadFromFile(const std::string& filename, EventSystem* event_system = nullptr);
    void saveToFile(const std::string& filename, EventSystem* event_system = nullptr) const;
    
    std::string getCommandForKey(const std::string& key) const;
    bool validateConfig() const;
    std::vector<std::string> getValidationErrors() const;
    void setBinding(const std::string& key, const std::string& command);
    void setDefaultBindings();
    void clearBindings();
    const std::map<std::string, std::string>& getKeyBindings() const { return key_bindings_; }
    const std::map<std::string, std::string>& getDefaultBindings() const { return default_bindings_; }
    
    std::vector<std::pair<std::string, std::string>> getBindingsForDisplay() const;
    std::map<std::string, std::vector<std::pair<std::string, std::string>>> getBindingsByCategory() const;
};