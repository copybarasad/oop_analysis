#include "../include/ui/renderer.h"
#include <iostream>

Renderer::Renderer() : current_message_(""), show_inventory_(false), show_target_selection_(false), show_area_selection_(false), current_level_(1) {}

void Renderer::RenderBoard(const GameBoard& board) {
    std::cout << "\033[2J\033[H";

    int hp_percentage = (board.player().health() * 100) / board.player().max_health();
    int hp_cubes = (hp_percentage + 9) / 10;
    if (hp_cubes > 10) { hp_cubes = 10; }
    if (hp_cubes < 0) { hp_cubes = 0; }

    std::cout << "▣";
    for (int c = 0; c < board.cols(); ++c) {
        std::cout << "╍";
    }
    std::cout << "▣" << std::endl;

    for (int r = 0; r < board.rows(); ++r) {
        std::cout << "╏";
        for (int c = 0; c < board.cols(); ++c) {
            CellType ct = board.cell_type(r, c);
            
            bool in_area = false;
            if (show_area_selection_) {
                for (const Position& pos : area_positions_) {
                    if (pos.row() == r && pos.col() == c) {
                        in_area = true;
                        break;
                    }
                }
            }
            
            if (in_area) {
                std::cout << "x";
            } else if (show_target_selection_ && ct == CellType::Enemy) {
                Position current_pos(r, c);
                int enemy_index = -1;
                
                for (size_t i = 0; i < board.enemies().size(); ++i) {
                    if (board.EnemyPosition(i).row() == r && 
                        board.EnemyPosition(i).col() == c) {
                        enemy_index = i;
                        break;
                    }
                }
                
                int target_number = -1;
                for (size_t i = 0; i < target_indices_.size(); ++i) {
                    if (target_indices_[i] == enemy_index) {
                        target_number = i + 1;
                        break;
                    }
                }
                
                if (target_number != -1) {
                    std::cout << target_number;
                } else {
                    std::cout << GetCellSymbol(ct);
                }
            } else {
                std::cout << GetCellSymbol(ct);
            }
        }
        std::cout << "╏";
        
        switch (r) {
            case 0:
                std::cout << "    ┏      ♂ DUNGEON ♂ [УРОВЕНЬ " << this->GetLevel() << "]     ┓";
                break;
            case 1:
                std::cout << "    HP: ";
                for (int cube = 0; cube < 10; ++cube) {
                    if (cube < hp_cubes) {
                        std::cout << "█";
                    } else {
                        std::cout << "░";
                    }
                }
                std::cout << " [" << board.player().health() << "/" << board.player().max_health() << "]";
                break;
            case 2:
                std::cout << "    Оружие: " << board.player().weapon().name() 
                          << " (Урон:" << board.player().weapon().damage() 
                          << " Радиус:" << board.player().weapon().range() << ")";
                break;
            case 3:
                std::cout << "    В свободной руке: ";
                if (board.player().hand().has_spell()) {
                    std::cout << board.player().hand().active_spell()->name();
                    if (board.player().has_trap()) {
                        std::cout << " [ЛОВУШКА ГОТОВА]";
                    }
                } else {
                    std::cout << "Пусто";
                }
                if (board.player().enhancement().has_boost()) {
                    std::cout << " [УСИЛЕНО x" << (board.player().enhancement().damage_boost() / 10) << "]";
                }
                break;
            case 4:
                std::cout << "    Счет: " << board.player().score();
                break;
            case 5:
                if (show_inventory_) {
                    std::cout << "    === ИНВЕНТАРЬ ===";
                } else {
                    std::cout << "    === ЛЕГЕНДА ===";
                }
                break;
            default:
                if (this->show_inventory_) {
                    this->RenderInventory(board, r);
                } else {
                    this->RenderLegend(r);
                }
                break;
        }

        std::cout << std::endl;
    }
    
    std::cout << "▣";
    for (int c = 0; c < board.cols(); ++c) {
        std::cout << "╍";
    }
    std::cout << "▣" << std::endl;

    if (!current_message_.empty()) {
        std::cout << current_message_ << std::endl;
    } else if (board.player().is_frozen()) {
        std::cout << "ЗАМОРОЗКА: ты попался в ледяную ловушку!" << std::endl;
    } else {
        std::cout << std::endl;
    }
    
    ClearMessage();
    if (show_area_selection_) {
        std::cout << "ОБЛАСТЬ АТАКИ: Нажмите E для подтверждения атаки или 0 для отмены." << std::endl;
    } else if (show_target_selection_) {
        std::cout << "ВЫБОР ЦЕЛИ: Введите номер врага (1-" << target_indices_.size() << ") или 0 для отмены." << std::endl;
    } else if (this->show_inventory_) {
        std::cout << "Инвентарь: Введите номер предмета (1-9 в зависимости от величины) для выбора или I для выхода." << std::endl;
    } else {
        std::cout << "Управление: W|A|S|D - Движение, C - Смена оружия, F - Заклинание, I - Инвентарь, M - Магазин, K - сохранить, L - загрузить, Q - Выход" << std::endl;
    }
}

std::string Renderer::GetCellSymbol(CellType cell_type) const {
    switch (cell_type) {
        case CellType::Empty:      return "∙";
        case CellType::Wall:       return "□";
        case CellType::Player:     return "⊻";
        case CellType::Enemy:      return "✠";
        case CellType::EnemyTower: return "☗";
        case CellType::Freeze:     return "❄";
        case CellType::Trap:       return "⚠";
        case CellType::FriendEnemy: return "☺"; 
        default:                   return "?";
    }
}

void Renderer::ShowError(const std::string& message) {
    current_message_ = "ОШИБКА: " + message;
}

void Renderer::ShowWarning(const std::string& message) {
    current_message_ = "ПРЕДУПРЕЖДЕНИЕ: " + message;
}

void Renderer::ClearMessage() {
    current_message_.clear();
}

void Renderer::RenderLegend(int row) const {
    switch (row) {
        case 6:
            std::cout << "    ⊻ = Игрок    ✠ = Враг    ☺ = Союзник";
            break;
        case 7:
            std::cout << "    □ = Стена   ❄ = Мерзлота    ⚠ = Ловушка";
            break;
        case 8:
            std::cout << "    ☗ = Вражеская башня";
            break;
        case 9:
            std::cout << "    ∙ = Пустая клетка";
            break;
    }
}

void Renderer::RenderInventory(const GameBoard& board, int row) const {
    int slot = row - 6;
    if (slot < 0 || slot >= board.player().inventory().max_slots()) {
        return;
    }

    std::cout << "    [" << slot + 1 << "] ";
    
    if (slot < board.player().inventory().item_count()) {
        const InventoryItem *item = board.player().inventory().get_item(slot);
        std::cout << item->name();
        if (item->quantity() > 1) {
            std::cout << " x" << item->quantity();
        }
    } else {
        std::cout << "Пусто";
    }
}

void Renderer::ToggleInventory() {
    show_inventory_ = !show_inventory_;
}

void Renderer::SetInventoryView(bool show) {
    show_inventory_ = show;
}

void Renderer::EnableTargetSelection(const std::vector<int>& targets) {
    show_target_selection_ = true;
    target_indices_ = targets;
}

void Renderer::DisableTargetSelection() {
    show_target_selection_ = false;
    target_indices_.clear();
}

void Renderer::EnableAreaSelection(const std::vector<Position>& positions) {
    show_area_selection_ = true;
    area_positions_ = positions;
}

void Renderer::DisableAreaSelection() {
    show_area_selection_ = false;
    area_positions_.clear();
}