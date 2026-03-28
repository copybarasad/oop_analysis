#include "command.h"
#include "game.h"
#include "game_saver.h"
#include "game_events.h"
#include <iostream>

CommandResult::CommandResult(bool s, bool t, const std::string& msg) 
    : success(s), turnConsumed(t), message(msg) {}

CommandResult MoveUpCommand::execute(Game& game, EventSystem* event_system) {
    bool moved = game.moveHunter(0, -1);
    
    if (event_system && moved) {
        GeneralEvent event("MOVE", "UP - Игрок переместился вверх");
        event_system->notify(event);
    }
    
    return CommandResult(moved, moved, moved ? "⬆️ Переместились на север" : "Не могу двигаться");
}

CommandResult MoveDownCommand::execute(Game& game, EventSystem* event_system) {
    bool moved = game.moveHunter(0, 1);
    
    if (event_system && moved) {
        GeneralEvent event("MOVE", "DOWN - Игрок переместился вниз");
        event_system->notify(event);
    }
    
    return CommandResult(moved, moved, moved ? "⬇️ Переместились на юг" : "Не могу двигаться");
}

CommandResult MoveLeftCommand::execute(Game& game, EventSystem* event_system) {
    bool moved = game.moveHunter(-1, 0);
    
    if (event_system && moved) {
        GeneralEvent event("MOVE", "LEFT - Игрок переместился влево");
        event_system->notify(event);
    }
    
    return CommandResult(moved, moved, moved ? "⬅️ Переместились на запад" : "Не могу двигаться");
}

CommandResult MoveRightCommand::execute(Game& game, EventSystem* event_system) {
    bool moved = game.moveHunter(1, 0);
    
    if (event_system && moved) {
        GeneralEvent event("MOVE", "RIGHT - Игрок переместился вправо");
        event_system->notify(event);
    }
    
    return CommandResult(moved, moved, moved ? "➡️ Переместились на восток" : "Не могу двигаться");
}

CommandResult AttackCommand::execute(Game& game, EventSystem* event_system) {
    int dracula_health_before = game.getDracula().getHealth();
    bool attacked = game.attackDracula();
    
    if (event_system && attacked) {
        int dracula_health_after = game.getDracula().getHealth();
        int damage = dracula_health_before - dracula_health_after;
        
        std::string message = "ATTACK - Нанесено " + std::to_string(damage) + " урона Дракуле";
        GeneralEvent event("ATTACK", message);
        event_system->notify(event);
    }
    
    return CommandResult(attacked, attacked, attacked ? "💥 Атака выполнена!" : "Не удалось атаковать");
}

CommandResult SwitchCombatCommand::execute(Game& game, EventSystem* event_system) {
    bool switched = game.switchCombatMode();
    
    if (event_system && switched) {
        std::string mode = game.getHunter().isRangedMode() ? "ДАЛЬНИЙ" : "БЛИЖНИЙ";
        GeneralEvent event("COMBAT_MODE", "SWITCH - Режим боя изменен на: " + mode);
        event_system->notify(event);
    }
    
    return CommandResult(switched, switched, switched ? "🔄 Режим боя изменен" : "Не удалось сменить режим");
}

CommandResult CastSpellCommand::execute(Game& game, EventSystem* event_system) {
    game.requestSpellSelection();
    
    if (event_system) {
        GeneralEvent event("UI", "SPELL_SELECTION - Запрошен выбор заклинания");
        event_system->notify(event);
    }
    
    return CommandResult(true, false, "✨ Выберите заклинание...");
}

CommandResult ShowStatusCommand::execute(Game& game, EventSystem* event_system) {
    game.requestStatus();
    
    if (event_system) {
        GeneralEvent event("UI", "SHOW_STATUS - Запрошен статус");
        event_system->notify(event);
    }
    
    return CommandResult(true, false, "📊 Статус запрошен");
}

CommandResult LookAroundCommand::execute(Game& game, EventSystem* event_system) {
    game.requestField();
    
    if (event_system) {
        GeneralEvent event("UI", "LOOK_AROUND - Запрошена карта");
        event_system->notify(event);
    }
    
    return CommandResult(true, false, "🗺️ Карта запрошена");
}

CommandResult ShowSpellsCommand::execute(Game& game, EventSystem* event_system) {
    game.requestSpells();
    
    if (event_system) {
        GeneralEvent event("UI", "SHOW_SPELLS - Запрошены заклинания");
        event_system->notify(event);
    }
    
    return CommandResult(true, false, "📜 Заклинания запрошены");
}

CommandResult HelpCommand::execute(Game& game, EventSystem* event_system) {
    game.requestHelp();
    
    if (event_system) {
        GeneralEvent event("UI", "HELP - Запрошена справка");
        event_system->notify(event);
    }
    
    return CommandResult(true, false, "Помощь запрошена");
}

CommandResult SaveGameCommand::execute(Game& game, EventSystem* event_system) {
    try {
        GameSaver saver("savegame.vhs");
        saver.saveGame(game);
        
        if (event_system) {
            GeneralEvent event("SYSTEM", "SAVE_GAME - Игра сохранена");
            event_system->notify(event);
        }
        
        return CommandResult(true, false, "Игра сохранена!");
    } catch (const std::exception& e) {
        if (event_system) {
            GeneralEvent event("ERROR", "SAVE_GAME - Ошибка: " + std::string(e.what()));
            event_system->notify(event);
        }
        return CommandResult(false, false, "Ошибка сохранения: " + std::string(e.what()));
    }
}

CommandResult LoadGameCommand::execute(Game& game, EventSystem* event_system) {
    try {
        GameSaver saver("savegame.vhs");
        if (saver.saveExists()) {
            saver.loadGame(game);
            
            if (event_system) {
                GeneralEvent event("SYSTEM", "LOAD_GAME - Игра загружена");
                event_system->notify(event);
            }
            
            return CommandResult(true, false, " Игра загружена!");
        } else {
            if (event_system) {
                GeneralEvent event("SYSTEM", "LOAD_GAME - Файл не найден");
                event_system->notify(event);
            }
            return CommandResult(false, false, "Файл сохранения не найден");
        }
    } catch (const std::exception& e) {
        if (event_system) {
            GeneralEvent event("ERROR", "LOAD_GAME - Ошибка: " + std::string(e.what()));
            event_system->notify(event);
        }
        return CommandResult(false, false, "Ошибка загрузки: " + std::string(e.what()));
    }
}

CommandResult QuitCommand::execute(Game& game, EventSystem* event_system) {
    game.setGameRunning(false);
    
    if (event_system) {
        GeneralEvent event("SYSTEM", "QUIT - Игра завершена");
        event_system->notify(event);
    }
    
    return CommandResult(true, false, "👋 Выход из игры");
}

CommandResult LibraryInteractCommand::execute(Game& game, EventSystem* event_system) {
    Position hunter_pos = game.getHunter().getPosition();
    
    game.requestLibraryInteraction(hunter_pos);
    
    if (event_system) {
        GeneralEvent event("UI", "LIBRARY_INTERACT - Запрошено взаимодействие");
        event_system->notify(event);
    }
    
    return CommandResult(true, false, "📚 Взаимодействие с библиотекой запрошено");
}