#pragma once
#include <string>

class GameObserver {
public:
    virtual ~GameObserver() = default;
    virtual void on_log_message(const std::string& msg) = 0;
    virtual void on_field_change() = 0;
    virtual void on_spells_display(const std::vector<std::string>& spells) = 0;
    virtual void on_shop_display(const std::vector<std::string>& items) = 0;
};
