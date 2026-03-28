#include "PlayerUpgradeRegistry.h"


void PlayerUpgradeRegistry::register_upgrade(PlayerUpgrade *upgrade) {
    if (upgrade == nullptr) return;
    if (!registry_.contains(upgrade->id())) {
        registry_[upgrade->id()] = upgrade;
    }
}

PlayerUpgrade * PlayerUpgradeRegistry::get(const std::string &id) const {
    const auto it = registry_.find(id);
    return (it != registry_.end()) ? it->second : nullptr;
}

std::vector<std::string> PlayerUpgradeRegistry::ids() const {
    std::vector<std::string> ids;
    ids.reserve(registry_.size());
    for (auto& [id, _] : registry_)
        ids.push_back(id);
    return ids;
}

void PlayerUpgradeRegistry::cleanup() {
    for (auto& [_, ptr] : registry_) {
        delete ptr;
    }
    registry_.clear();
}
