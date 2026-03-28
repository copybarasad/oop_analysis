#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "PlayerUpgrade.h"
#include "types/PlayerUpgradeGold.h"
#include "upgrade/types/PlayerUpgradeDamage.h"
#include "upgrade/types/PlayerUpgradeHealth.h"
#include "upgrade/types/PlayerUpgradeSpell.h"

class PlayerUpgradeRegistry {
public:
    static PlayerUpgradeRegistry& instance() {
        static PlayerUpgradeRegistry inst;

        return inst;
    }

    void register_upgrade(PlayerUpgrade* upgrade);

    PlayerUpgrade* get(const std::string& id) const;

    std::vector<std::string> ids() const;

    void cleanup();

private:
    PlayerUpgradeRegistry() {
        register_upgrade(new PlayerUpgradeDamage());
        register_upgrade(new PlayerUpgradeHealth());
        register_upgrade(new PlayerUpgradeSpell());
        register_upgrade(new PlayerUpgradeGold());
    }
    ~PlayerUpgradeRegistry() = default;

    PlayerUpgradeRegistry(const PlayerUpgradeRegistry&) = delete;
    PlayerUpgradeRegistry& operator=(const PlayerUpgradeRegistry&) = delete;

    std::unordered_map<std::string, PlayerUpgrade*> registry_;
};
