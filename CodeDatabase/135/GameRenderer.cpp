#include "GameRenderer.h"
#include "Player.h"
#include "ICombatant.h"
#include "PlayerResources.h"
#include <iostream>
#include <string>
#include <cstdlib>

void GameRenderer::clearScreen() const {
	#ifdef _WIN32
		system("cls");
	#else
		system("clear");
	#endif
}

void GameRenderer::showPlayerInfo(const GameField& field, const EntityManager& entities, 
                                   int player_index) const {
	auto player_health = entities.getEntityHealth(player_index);
	if (!player_health.has_value()) return;
	
	auto [current_hp, max_hp] = player_health.value();
	
	auto* entity = entities.getEntity(player_index);
	if (!entity) return;
	auto* player = dynamic_cast<const Player*>(entity);
	if (!player) return;
	
	int player_damage = player->getDamage();
	bool is_melee = player->melee();
	int intel = player->getInt();
	int dex = player->getDex();
	int str = player->getStr();
	
	int player_x, player_y;
	field.indexToCoord(player_index, player_x, player_y);
	
	std::cout << "╔═══════════════════════════════════════╗\n";
	std::cout << "║           ИНФОРМАЦИЯ ОБ ИГРОКЕ        ║\n";
	std::cout << "╠═══════════════════════════════════════╣\n";
	std::cout << "║ ❤ HP:        " << current_hp << "/" << max_hp;
	
	int hp_spaces = 23 - (std::to_string(current_hp).length() + std::to_string(max_hp).length());
	for (int i = 0; i < hp_spaces; ++i) std::cout << " ";
	std::cout << "║\n";
	
	std::cout << "║ ⚔ Урон:      " << player_damage;
	int dmg_spaces = 26 - std::to_string(player_damage).length();
	for (int i = 0; i < dmg_spaces; ++i) std::cout << " ";
	std::cout << "║\n";
	
	std::cout << "║ 🗡 Оружие:    " << (is_melee ? "Ближний бой" : "Дальний бой");
	std::cout << (is_melee ? "         " : "         ");
	std::cout << "║\n";
	
	std::cout << "║ 📍 Позиция:  (" << player_x << ", " << player_y << ")";
	int pos_spaces = 25 - (std::to_string(player_x).length() + std::to_string(player_y).length());
	for (int i = 0; i < pos_spaces; ++i) std::cout << " ";
	std::cout << "║\n";
	
	std::cout << "║ 🧠 INT: " << intel 
	          << " | 👁 DEX: " << dex 
	          << " | 💪 STR: " << str;
	int attr_spaces = 11 - (std::to_string(intel).length() + 
	                         std::to_string(dex).length() + 
	                         std::to_string(str).length());
	for (int i = 0; i < attr_spaces; ++i) std::cout << " ";
	std::cout << "║\n";
	
	const PlayerResources& resources = player->getResources();
	std::cout << "║ 💰 Очки:     " << resources.getPoints();
	int points_spaces = 26 - std::to_string(resources.getPoints()).length();
	for (int i = 0; i < points_spaces; ++i) std::cout << " ";
	std::cout << "║\n";
	
	std::cout << "║ ⚔️ Убийств:   " << resources.getKills();
	int kills_spaces = 26 - std::to_string(resources.getKills()).length();
	for (int i = 0; i < kills_spaces; ++i) std::cout << " ";
	std::cout << "║\n";
	
	std::cout << "╚═══════════════════════════════════════╝\n\n";
}

void GameRenderer::showFieldGrid(const GameField& field, const EntityManager& entities) const {
	int width = field.getWidth();
	int height = field.getHeight();
	
	std::cout << "┌";
	for (int x = 0; x < width; ++x) {
		std::cout << "─";
	}
	std::cout << "┐\n";
	
	for (int y = 0; y < height; ++y) {
		std::cout << "│";
		for (int x = 0; x < width; ++x) {
			int index = field.coordToIndex(x, y);
			
			auto entity_type = entities.getEntityType(index);
			if (entity_type.has_value()) {
				switch (entity_type.value()) {
					case EntityType::PLAYER: std::cout << "P"; break;
					case EntityType::ENEMY: std::cout << "E"; break;
					case EntityType::BARRACKS: std::cout << "B"; break;
					case EntityType::TOWER: std::cout << "T"; break;
					case EntityType::ALLY: std::cout << "A"; break;
					default: std::cout << "?"; break;
				}
			} else if (!field.getCell(index).isPassable()) {
				std::cout << "#";
			} else if (field.getCell(index).isSlowing()) {
				std::cout << "~";
			} else if (field.getCell(index).hasTrap()) {
				std::cout << "X";
			} else {
				std::cout << ".";
			}
		}
		std::cout << "│\n";
	}
	
	std::cout << "└";
	for (int x = 0; x < width; ++x) {
		std::cout << "─";
	}
	std::cout << "┘\n";
}

void GameRenderer::render(const GameField& field, const EntityManager& entities, 
                          int player_index, int game_level, int game_turn) const {
	clearScreen();
	
	int enemy_count = entities.findEntitiesByType(EntityType::ENEMY).size();
	int barracks_count = entities.findEntitiesByType(EntityType::BARRACKS).size();
	
	if (player_index >= 0 && entities.hasEntity(player_index)) {
		showPlayerInfo(field, entities, player_index);
	}
	
	std::cout << "📊 Уровень: " << game_level << " | 🕐 Ход: " << game_turn 
	          << " | 👹 Врагов: " << enemy_count << " | 🏰 Зданий: " << barracks_count << "\n\n";
	
	showFieldGrid(field, entities);
	showLegend();
}

void GameRenderer::showLegend() const {
	std::cout << "\n📖 Легенда: [P] Игрок | [E] Враг | [B] Казарма | [T] Башня | [A] Союзник | [X] Ловушка\n";
	std::cout << "           [#] Стена | [~] Замедление | [.] Пусто\n";
}

void GameRenderer::showPlayerTurnStart(int x, int y, bool is_slowed) const {
	std::cout << "\n╔════════════════════════════════════════╗\n";
	if (is_slowed) {
		std::cout << "║   ЭФФЕКТ ЗАМЕДЛЕНИЯ - ПРОПУСК ХОДА!   ║\n";
		std::cout << "╚════════════════════════════════════════╝\n";
		std::cout << "→ Текущая позиция: (" << x << ", " << y << ")\n";
		std::cout << "→ Вы замедлены и не можете двигаться!\n";
		std::cout << "→ Эффект замедления закончится на следующий ход.\n";
	} else {
		std::cout << "║            ВАШ ХОД                    ║\n";
		std::cout << "╚════════════════════════════════════════╝\n";
		std::cout << "→ Текущая позиция: (" << x << ", " << y << ")\n";
		std::cout << "\nВведите команду:\n";
		std::cout << "  w/a/s/d - движение (вверх/влево/вниз/вправо)\n";
		std::cout << "  e       - сменить оружие\n";
		std::cout << "  c       - применить заклинание\n";
		std::cout << "  b       - магазин заклинаний\n";
	}
}

void GameRenderer::showMoveResult(const MoveResult& result) const {
	if (!result.success) {
		std::cout << "✗ Ошибка: " << result.error_message << "\n";
		return;
	}
	
	std::cout << "\n--- ДЕЙСТВИЯ ИГРОКА ---\n";
	std::cout << "→ ПЕРЕМЕЩЕНИЕ " << result.direction << "\n";
	std::cout << "  Старая позиция: (" << result.old_x << ", " << result.old_y << ")\n";
	std::cout << "  Новая позиция:  (" << result.new_x << ", " << result.new_y << ")\n";
	
	if (result.stepped_on_slow_cell) {
		std::cout << "  ⚠ ВНИМАНИЕ: Вы попали на замедляющую клетку (~)!\n";
		std::cout << "  ⚠ На следующий ход вы не сможете двигаться!\n";
	}
	
	if (result.trap_triggered) {
		std::cout << "  💥 ВНИМАНИЕ: Вы активировали ловушку!\n";
	}
}

void GameRenderer::showAttackResult(const AttackResult& result) const {
	if (!result.success) {
		return;
	}
	
	std::cout << "\n--- ДЕЙСТВИЯ ИГРОКА ---\n";
	std::cout << "⚔ АТАКА " << result.target_type_name << " на позиции (" 
	          << result.target_x << ", " << result.target_y << ")!\n";
	std::cout << "  Использовано оружие: " 
	          << (result.is_melee ? "Ближний бой" : "Дальний бой") << "\n";
	std::cout << "  Нанесено урона: " << result.damage << "\n";
	std::cout << "  HP цели: " << result.target_hp_before << " → " << result.target_hp_after 
	          << " (из " << result.target_max_hp << ")\n";
	
	if (result.target_killed) {
		std::cout << "  ★ ЦЕЛЬ УНИЧТОЖЕНА! ★\n";
		std::cout << "  → Получено очков за убийство: +" << result.score_gained << "\n";
		if (result.resource_points_gained > 0) {
			std::cout << "  → Получено очков ресурсов: +" << result.resource_points_gained << "\n";
			std::cout << "  → Убито врагов: " << result.total_kills << "\n";
		}
	}
}

void GameRenderer::showWeaponChange(const WeaponChangeResult& result) const {
	std::cout << "\n--- ДЕЙСТВИЯ ИГРОКА ---\n";
	std::cout << "✓ Оружие сменено!\n";
	std::cout << "  Текущее оружие: " 
	          << (result.is_melee ? "⚔ Ближний бой" : "🏹 Дальний бой") << "\n";
	std::cout << "  Текущий урон: " << result.damage << "\n";
}

void GameRenderer::showSpellCastResult(const SpellCastResult& result) const {
	if (result.cancelled) {
		std::cout << "Применение заклинания отменено.\n";
		return;
	}
	
	if (result.success) {
		std::cout << "✓ Заклинание успешно применено!\n";
	} else {
		if (!result.error_message.empty()) {
			std::cout << "✗ " << result.error_message << "\n";
		} else {
			std::cout << "⚠ Заклинание не было применено: неподходящая цель или условия!\n";
			std::cout << "  Ход потрачен, но заклинание удалено из руки.\n";
		}
	}
}

void GameRenderer::showEnemyTurnHeader() const {
	std::cout << "\n--- ДЕЙСТВИЯ ВРАГОВ ---\n";
}

void GameRenderer::showEnemyAction(const EnemyAction& action) const {
	if (action.moved) {
		std::cout << "  → Враг переместился: (" << action.enemy_x << ", " << action.enemy_y 
		          << ") → (" << action.new_x << ", " << action.new_y << ")\n";
	}
	
	if (action.attacked) {
		std::cout << "  ⚔ Враг на позиции (" << action.enemy_x << ", " << action.enemy_y << ") атакует вас!\n";
		std::cout << "    Получено урона: " << action.damage_dealt << "\n";
		std::cout << "    Ваше HP: " << action.player_hp_before << " → " << action.player_hp_after 
		          << " (из " << action.player_max_hp << ")\n";
	}
}

void GameRenderer::showAllyTurnHeader() const {
	std::cout << "\n--- ДЕЙСТВИЯ СОЮЗНИКОВ ---\n";
}

void GameRenderer::showAllyAction(const AllyAction& action) const {
	if (action.moved) {
		std::cout << "  → Союзник переместился: (" << action.ally_x << ", " << action.ally_y 
		          << ") → (" << action.new_x << ", " << action.new_y << ")\n";
	}
	
	if (action.attacked) {
		std::cout << "  ⚔ Союзник атакует врага на позиции (" << action.target_x << ", " << action.target_y << ")!\n";
		std::cout << "    Нанесено урона: " << action.damage_dealt << "\n";
		if (action.enemy_killed) {
			std::cout << "    ★ Враг уничтожен!\n";
		}
	}
}

void GameRenderer::showTowerAction(const TowerAction& action) const {
	std::cout << "  🗼 Башня на позиции (" << action.tower_x << ", " << action.tower_y << ") атакует вас!\n";
	std::cout << "    Получено урона: " << action.damage_dealt << "\n";
	std::cout << "    Ваше HP: " << action.player_hp_before << " → " << action.player_hp_after 
	          << " (из " << action.player_max_hp << ")\n";
}

void GameRenderer::showUpdateEvents(const UpdateResult& result) const {
	if (result.spawns.empty()) {
		return;
	}
	
	std::cout << "\n--- ИГРОВЫЕ СОБЫТИЯ ---\n";
	
	for (const auto& spawn : result.spawns) {
		if (spawn.success) {
			std::cout << "  ★ СПАУН ВРАГА!\n";
			std::cout << "    Вражеское здание на позиции (" << spawn.barracks_x << ", " << spawn.barracks_y 
			          << ") создало нового врага\n";
			std::cout << "    Враг появился на позиции: (" << spawn.spawned_x << ", " << spawn.spawned_y << ")\n";
		} else {
			std::cout << "  ⚠ Вражеское здание на (" << spawn.barracks_x << ", " << spawn.barracks_y 
			          << ") не смогло создать врага (нет свободного места)\n";
		}
	}
}

void GameRenderer::showGameOverScreen() const {
	std::cout << "\n╔═════════════════════════════════════════╗\n";
	std::cout << "║          ИГРА ОКОНЧЕНА!                ║\n";
	std::cout << "║           Вы проиграли...              ║\n";
	std::cout << "╚═════════════════════════════════════════╝\n";
}

void GameRenderer::showLevelCompleteScreen(int score, int threshold) const {
	std::cout << "\n╔═════════════════════════════════════════╗\n";
	std::cout << "║        УРОВЕНЬ ЗАВЕРШЕН!               ║\n";
	std::cout << "╚═════════════════════════════════════════╝\n";
	std::cout << "\nОчки: " << score << "/" << threshold << "\n";
}

void GameRenderer::showWelcomeScreen() const {
	std::cout << "╔═════════════════════════════════════════╗\n";
	std::cout << "║       Добро пожаловать в игру!         ║\n";
	std::cout << "╚═════════════════════════════════════════╝\n";
	std::cout << "\n📋 Цель: Наберите достаточно очков для завершения уровня!\n";
}
