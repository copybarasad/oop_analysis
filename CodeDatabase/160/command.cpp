#include "../include/io/command.h"

Command Command::None() { return Command{Kind::None}; }
Command Command::Cancel() { return Command{Kind::Cancel}; }

Command Command::Quit() { return Command{Kind::Quit}; }
Command Command::StartNewGame() { return Command{Kind::StartNewGame}; }
Command Command::StartLoadGame() { return Command{Kind::StartLoadGame}; }

Command Command::Move(Direction dir) {
    Command c{Kind::Move};
    c.direction = dir;
    return c;
}

Command Command::ChangeWeapon() { return Command{Kind::ChangeWeapon}; }
Command Command::ToggleInventory() { return Command{Kind::ToggleInventory}; }
Command Command::OpenShop() { return Command{Kind::OpenShop}; }
Command Command::CastSpell() { return Command{Kind::CastSpell}; }

Command Command::SaveGame(std::string name) {
    Command c{Kind::SaveGame};
    c.text = std::move(name);
    return c;
}

Command Command::LoadPrompt() { return Command{Kind::LoadPrompt}; }

Command Command::LoadGame(std::string name) {
    Command c{Kind::LoadGame};
    c.text = std::move(name);
    return c;
}

Command Command::EquipSpellSlot(int slotIndex) {
    Command c{Kind::EquipSpellSlot};
    c.index = slotIndex;
    return c;
}

Command Command::ShopBuy(int itemIndex) {
    Command c{Kind::ShopBuy};
    c.index = itemIndex;
    return c;
}

Command Command::ShopExit() { return Command{Kind::ShopExit}; }

Command Command::SelectTarget(int targetNumberOneBased) {
    Command c{Kind::SelectTarget};
    c.index = targetNumberOneBased;
    return c;
}

Command Command::SelectDirection(Direction dir) {
    Command c{Kind::SelectDirection};
    c.direction = dir;
    return c;
}

Command Command::Confirm(bool yes) {
    Command c{Kind::Confirm};
    c.flag = yes;
    return c;
}

Command Command::UpgradeSelect(int optionOneBased) {
    Command c{Kind::UpgradeSelect};
    c.index = optionOneBased;
    return c;
}
