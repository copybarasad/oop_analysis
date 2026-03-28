#ifndef INCLUDE_CORE_GAME_CONTROLLER
#define INCLUDE_CORE_GAME_CONTROLLER

#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "game_state.h"
#include "save_manager.h"
#include "../io/command.h"
#include "../items/inventory.h"
#include "../items/spell.h"
#include "../ui/game_view.h"
#include "../ui/position.h"

template <class InputT>
class GameController {
private:
    GameState& game_;
    InputT input_;

    static bool IsLevelComplete(const GameBoard& board) {
        return board.enemies().empty() && board.enemy_towers().empty();
    }

    static bool IsGameOver(const GameBoard& board) {
        return !board.player().is_alive();
    }

    void SaveGame(IGameView& view, const std::string& name) {
        if (name.empty()) {
            std::cout << "Отмена.\n";
            return;
        }

        if (SaveManager::Save(game_.saves_dir, name, game_.board)) {
            std::cout << "Сохранено как '" << name << "'. Нажмите Enter.";
        } else {
            std::cout << "Ошибка сохранения. Нажмите Enter.";
        }
        std::cin.get();
        (void)view;
    }

    void LoadGamePrompt(IGameView& view) {
        auto saves = SaveManager::ListSaves(game_.saves_dir);
        if (saves.empty()) {
            std::cout << "Нет сохранений.\n";
            game_.NewGame();
            view.DisableTargetSelection();
            view.DisableAreaSelection();
            view.SetInventoryView(false);
            view.SetLevel(game_.board.current_level());
            return;
        }

        Command cmd = input_.ReadLoadCommand(saves);
        if (cmd.kind != Command::Kind::LoadGame || !cmd.text.has_value()) {
            return;
        }

        const std::string& saveName = *cmd.text;

        if (SaveManager::Load(game_.saves_dir, saveName, game_.board)) {
            std::cout << "Загружено '" << saveName << "'. Enter.";
            view.DisableTargetSelection();
            view.DisableAreaSelection();
            view.SetInventoryView(false);
            view.SetLevel(game_.board.current_level());
        } else {
            std::cout << "Ошибка загрузки. Enter.";
        }
        std::cin.get();
    }

    void OpenShop() {
        while (true) {
            std::cout << "\033[2J\033[H";
            game_.shop.DisplayShop();
            std::cout << "\nВаш счет: " << game_.board.player().score() << std::endl;

            Command cmd = input_.ReadShopCommand();
            if (cmd.kind == Command::Kind::ShopExit) {
                break;
            }
            if (cmd.kind == Command::Kind::Cancel) {
                input_.WaitForEnter();
                continue;
            }
            if (cmd.kind != Command::Kind::ShopBuy || !cmd.index.has_value()) {
                std::cout << "Неверный номер товара!" << std::endl;
                input_.WaitForEnter();
                continue;
            }

            int idx = *cmd.index;
            if (idx < 0 || idx >= game_.shop.item_count()) {
                std::cout << "Неверный номер товара!" << std::endl;
                input_.WaitForEnter();
                continue;
            }

            if (game_.shop.BuyItem(idx, game_.board.player())) {
                std::cout << "Покупка успешна!" << std::endl;
            }
            input_.WaitForEnter();
        }
    }

    void EquipSpellFromInventory(IGameView& view, int slot) {
        if (slot < 0 || slot >= game_.board.player().inventory().item_count()) {
            view.ShowError("Нет предмета в этом слоте!");
            return;
        }

        const InventoryItem* item = game_.board.player().inventory().get_item(slot);
        if (item->type() != ItemType::Spell) {
            view.ShowError("Это не заклинание!");
            return;
        }

        Spell* new_spell = game_.board.player().inventory().take_spell_from_slot(slot);
        if (new_spell == nullptr) {
            view.ShowError("Не удалось взять заклинание!");
            return;
        }

        Spell* current_spell = game_.board.player().hand().active_spell_copy();
        if (game_.board.player().hand().has_spell()) {
            game_.board.player().inventory().add_spell(current_spell);
        }

        game_.board.player().hand().equip_spell(new_spell);
        view.ShowWarning("Заклинание взято в руку!");
        view.SetInventoryView(false);
    }

    void UseSpell(IGameView& view) {
        if (!game_.board.player().hand().has_spell()) {
            view.ShowError("В руке нет заклинания!");
            return;
        }

        const Spell* spell = game_.board.player().hand().active_spell();

        if (spell->type() == SpellType::Heal) {
            bool used = game_.board.player().hand().use_spell(game_.board);
            if (used) {
                view.ShowWarning("Заклинание использовано!");
                game_.board.player().hand().clear();
                game_.board.player().enhancement().reset();
            } else {
                view.ShowError("Не удалось использовать заклинание!");
            }
            return;
        }

        if (spell->type() == SpellType::Damage) {
            DamageSpell* damage_spell = dynamic_cast<DamageSpell*>(game_.board.player().hand().active_spell_copy());
            if (!damage_spell) {
                view.ShowError("Ошибка заклинания!");
                return;
            }

            std::vector<int> targets = damage_spell->GetTargetsInRange(game_.board);

            if (targets.empty()) {
                view.ShowError("Нет врагов в радиусе действия!");
                delete damage_spell;
                return;
            }

            view.EnableTargetSelection(targets);
            view.Render(game_.board);

            Command choice = input_.ReadTargetSelectionCommand();
            view.DisableTargetSelection();

            if (choice.kind != Command::Kind::SelectTarget || !choice.index.has_value()) {
                view.ShowError("Отмена использования заклинания.");
                delete damage_spell;
                return;
            }

            int target_number = *choice.index;
            if (target_number < 1 || target_number > static_cast<int>(targets.size())) {
                view.ShowError("Отмена использования заклинания.");
                delete damage_spell;
                return;
            }

            int target_enemy_index = targets[target_number - 1];
            damage_spell->UseWithTarget(game_.board, target_enemy_index);

            delete damage_spell;
            game_.board.player().hand().clear();
            game_.board.player().enhancement().reset();
            view.ShowWarning("Заклинание использовано!");
            game_.core.UpdateGame(game_.board);
            return;
        }

        if (spell->type() == SpellType::Splash) {
            SplashSpell* splash_spell = dynamic_cast<SplashSpell*>(game_.board.player().hand().active_spell_copy());
            if (!splash_spell) {
                view.ShowError("Ошибка заклинания!");
                return;
            }

            Command dir_cmd = input_.ReadDirectionSelectionCommand();
            if (dir_cmd.kind != Command::Kind::SelectDirection || !dir_cmd.direction.has_value()) {
                view.ShowError("Неверное направление! Отмена использования заклинания.");
                delete splash_spell;
                return;
            }

            Direction dir = *dir_cmd.direction;

            std::vector<Position> area = splash_spell->GetAreaPositions(game_.board, dir);
            view.EnableAreaSelection(area);
            view.Render(game_.board);

            Command confirm_cmd = input_.ReadConfirmAttackCommand();
            view.DisableAreaSelection();

            if (confirm_cmd.kind == Command::Kind::Confirm && confirm_cmd.flag.value_or(false)) {
                splash_spell->UseInDirection(game_.board, dir);
                delete splash_spell;
                game_.board.player().hand().clear();
                game_.board.player().enhancement().reset();
                view.ShowWarning("Заклинание использовано!");
                game_.core.UpdateGame(game_.board);
            } else {
                view.ShowError("Атака отменена.");
                delete splash_spell;
            }
            return;
        }

        if (spell->type() == SpellType::Trap) {
            game_.board.player().hand().use_spell(game_.board);
            game_.board.player().hand().clear();
            game_.board.player().enhancement().reset();
            view.ShowWarning("Ловушка будет установлена при следующем движении!");
            return;
        }

        if (spell->type() == SpellType::Summon) {
            game_.board.player().hand().use_spell(game_.board);
            game_.board.player().hand().clear();
            game_.board.player().enhancement().reset();
            view.ShowWarning("Союзник призван!");
            return;
        }

        if (spell->type() == SpellType::Enhance) {
            game_.board.player().hand().use_spell(game_.board);
            game_.board.player().hand().clear();
            view.ShowWarning("Усиление применено!");
            return;
        }
    }

    void ShowUpgradeMenu() {
        while (true) {
            std::cout << "\n========================================" << std::endl;
            std::cout << "         ВЫБОР УЛУЧШЕНИЯ" << std::endl;
            std::cout << "========================================" << std::endl;
            std::cout << "Выберите одно улучшение:" << std::endl;
            std::cout << "[1] +20 к максимальному здоровью (текущий бонус: +" 
                      << game_.board.player().max_health_bonus() << ")" << std::endl;
            std::cout << "[2] +5 к урону оружия (текущий бонус: +" 
                      << game_.board.player().weapon_damage_bonus() << ")" << std::endl;
            std::cout << "[3] +10 к силе заклинаний Удар | Удар по площади (текущий бонус: +" 
                      << game_.board.player().spell_power_bonus() << ")" << std::endl;
            std::cout << "[4] +2 слота в инвентаре (текущий бонус: +" 
                      << game_.board.player().max_inventory_bonus() << ")" << std::endl;
            std::cout << "========================================" << std::endl;

            Command choice_cmd = input_.ReadUpgradeChoiceCommand();
            if (choice_cmd.kind != Command::Kind::UpgradeSelect || !choice_cmd.index.has_value()) {
                std::cout << "\nНеверный выбор! Улучшение не применено." << std::endl;
                continue;
            }

            int choice = *choice_cmd.index;
            switch (choice) {
                case 1:
                    game_.board.player().upgrade_max_health(20);
                    game_.board.player().set_max_health(game_.board.player().total_max_health());
                    game_.board.player().set_health(game_.board.player().max_health());
                    std::cout << "\n✓ Максимальное здоровье увеличено! Новый максимум: " 
                              << game_.board.player().max_health() << std::endl;
                    break;

                case 2:
                    game_.board.player().upgrade_weapon_damage(5);
                    std::cout << "\n✓ Урон оружия увеличен! Бонус урона: +" 
                              << game_.board.player().weapon_damage_bonus() << std::endl;
                    break;

                case 3:
                    game_.board.player().upgrade_spell_power(10);
                    std::cout << "\n✓ Сила заклинаний увеличена! Бонус силы: +" 
                              << game_.board.player().spell_power_bonus() << std::endl;
                    break;

                case 4:
                    game_.board.player().upgrade_max_inventory(2);
                    game_.board.player().inventory().increase_max_slots(2);
                    std::cout << "\n✓ Размер инвентаря увеличен на 2 слота!" << std::endl;
                    break;

                default:
                    std::cout << "\nНеверный выбор! Улучшение не применено." << std::endl;
                    continue;
            }

            std::cout << "Нажмите Enter для продолжения...";
            std::cin.get();
            return;
        }
    }

    void NextLevel(IGameView& view) {
        int next_level = game_.board.current_level() + 1;

        std::cout << "\n========================================" << std::endl;
        std::cout << "    УРОВЕНЬ " << game_.board.current_level() << " ПРОЙДЕН!" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Очки: " << game_.board.player().score() << std::endl;

        ShowUpgradeMenu();

        std::cout << "\nПереход на уровень " << next_level << "..." << std::endl;
        std::cout << "- Здоровье восстановлено" << std::endl;
        std::cout << "- Половина заклинаний утеряна" << std::endl;
        std::cout << "- Враги стали сильнее" << std::endl;
        std::cout << "\nНажмите Enter для продолжения...";
        std::cin.get();

        game_.board.player().set_health(game_.board.player().max_health());

        Inventory& inv = game_.board.player().inventory();
        std::vector<int> spell_slots;

        for (int i = 0; i < inv.item_count(); ++i) {
            if (inv.get_item(i)->type() == ItemType::Spell) {
                spell_slots.push_back(i);
            }
        }

        int to_remove = static_cast<int>(spell_slots.size()) / 2;
        for (int i = 0; i < to_remove && !spell_slots.empty(); ++i) {
            int rand_idx = rand() % static_cast<int>(spell_slots.size());
            int slot = spell_slots[rand_idx];

            delete inv.get_item(slot)->take_spell();

            spell_slots.erase(spell_slots.begin() + rand_idx);
        }

        game_.board.set_level(next_level);
        game_.RebuildBoardForLevel(next_level);
        game_.board.player().set_health(game_.board.player().max_health());

        view.SetLevel(game_.board.current_level());
    }

public:
    explicit GameController(GameState& game) : game_(game), input_() {}

    void Run(IGameView& view) {
        Command start = input_.ReadStartCommand();
        if (start.kind == Command::Kind::StartLoadGame) {
            LoadGamePrompt(view);
        } else {
            game_.NewGame();
        }

        view.DisableTargetSelection();
        view.DisableAreaSelection();
        view.SetInventoryView(false);
        view.SetLevel(game_.board.current_level());

        while (!IsGameOver(game_.board)) {
            view.Render(game_.board);

            if (IsLevelComplete(game_.board)) {
                NextLevel(view);
                continue;
            }

            Command cmd = input_.ReadMainCommand(view.IsInventoryShown());

            if (cmd.kind == Command::Kind::EquipSpellSlot && cmd.index.has_value()) {
                EquipSpellFromInventory(view, *cmd.index);
                continue;
            }

            if (cmd.kind == Command::Kind::SaveGame && cmd.text.has_value()) {
                SaveGame(view, *cmd.text);
                continue;
            }

            if (cmd.kind == Command::Kind::LoadGame) {
                LoadGamePrompt(view);
                continue;
            }

            if (cmd.kind == Command::Kind::LoadPrompt) {
                LoadGamePrompt(view);
                continue;
            }

            if (cmd.kind == Command::Kind::Quit) {
                std::cout << "Игра окончена по требованию игрока." << std::endl;
                break;
            }

            if (cmd.kind == Command::Kind::ChangeWeapon) {
                game_.board.ChangePlayerWeapon();
                game_.core.UpdateGame(game_.board);
                continue;
            }

            if (cmd.kind == Command::Kind::ToggleInventory) {
                view.ToggleInventory();
                continue;
            }

            if (cmd.kind == Command::Kind::OpenShop) {
                OpenShop();
                continue;
            }

            if (cmd.kind == Command::Kind::CastSpell) {
                UseSpell(view);
                game_.core.UpdateGame(game_.board);
                continue;
            }

            if (cmd.kind == Command::Kind::Move && cmd.direction.has_value()) {
                bool moved = game_.board.MovePlayer(*cmd.direction);
                if (!moved) {
                    if (game_.board.player().on_freeze_cell()) {
                        view.ShowWarning("Вы заморожены! К сожалению, пропуск хода!");
                    } else {
                        view.ShowWarning("Сделать шаг в эту сторону невозможно.");
                    }
                }

                game_.core.UpdateGame(game_.board);

                if (IsLevelComplete(game_.board)) {
                    NextLevel(view);
                    continue;
                }

                continue;
            }

            view.ShowError("Неизвестная команда! Попробуй W|A|S|D - шаг, C - смена оружия или Q для выхода.");
        }

        std::cout << "\n========================================" << std::endl;
        std::cout << "         ИГРА ОКОНЧЕНА!" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Достигнутый уровень: " << game_.board.current_level() << std::endl;
        std::cout << "Итоговый счет: " << game_.board.player().score() << std::endl;
        std::cout << "========================================" << std::endl;
    }
};

#endif
