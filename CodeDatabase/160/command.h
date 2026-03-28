#ifndef INCLUDE_IO_COMMAND
#define INCLUDE_IO_COMMAND

#include <optional>
#include <string>
#include <utility>

#include "../ui/direction.h"

struct Command {
    enum class Kind {
        None,
        Quit,

        StartNewGame,
        StartLoadGame,

        Move,
        ChangeWeapon,
        ToggleInventory,
        OpenShop,
        CastSpell,

        SaveGame,
        LoadPrompt,
        LoadGame,

        EquipSpellSlot,

        ShopBuy,
        ShopExit,

        SelectTarget,
        SelectDirection,
        Confirm,

        UpgradeSelect,

        Cancel
    };

    Kind kind{Kind::None};
    std::optional<Direction> direction{};
    std::optional<int> index{};
    std::optional<std::string> text{};
    std::optional<bool> flag{};

    static Command None();
    static Command Cancel();

    static Command Quit();
    static Command StartNewGame();
    static Command StartLoadGame();

    static Command Move(Direction dir);

    static Command ChangeWeapon();
    static Command ToggleInventory();
    static Command OpenShop();
    static Command CastSpell();

    static Command SaveGame(std::string name);
    static Command LoadPrompt();
    static Command LoadGame(std::string name);

    static Command EquipSpellSlot(int slotIndex);
    static Command ShopBuy(int itemIndex);
    static Command ShopExit();

    static Command SelectTarget(int targetNumberOneBased);
    static Command SelectDirection(Direction dir);
    static Command Confirm(bool yes);

    static Command UpgradeSelect(int optionOneBased);

private:
    explicit Command(Kind k) : kind(k) {}
};

#endif
