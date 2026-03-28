#include "EventSystem.h"

void EventSystem::Push(const std::string& text) {

    recent.push_back(text);
    if (recent.size() > maxRecent)
        recent.erase(recent.begin());


    if (target == LogTarget::File && file.is_open()) {
        file << text << "\n";  
        file.flush();
    } else if (target == LogTarget::Screen) {
        std::cout << text << "\n"; 
    }
}

void EventSystem::SetLogTarget(LogTarget t, const std::string& filename) {
    target = t;

    if (file.is_open())
        file.close();

    if(target == LogTarget::File) {
        if(filename.empty()) {
            std::cerr << "Warning: no filename provided for file logging, falling back to screen.\n";
            target = LogTarget::Screen;
            return;
        }

        std::filesystem::path filePath(filename);
        if (filePath.has_parent_path()) {
            std::error_code ec;
            std::filesystem::create_directories(filePath.parent_path(), ec);
            if(ec) {
                std::cerr << "Warning: failed to create directories for log file: " 
                          << ec.message() << "\n";
                target = LogTarget::Screen;
                return;
            }
        }

   
        file.open(filename, std::ios::out);
        if (!file.is_open()) {
            std::cerr << "Warning: failed to open log file: " << filename << "\n";
            target = LogTarget::Screen;
        }
    }


    recent.clear();
}


void EventSystem::Notify(const DamageEvent& e) {
    Push(e.target + " took " + std::to_string(e.damage) +
         " dmg at (" + std::to_string(e.x) + "," +
         std::to_string(e.y) + ")");
}

void EventSystem::Notify(const SpellEvent& e) {
    std::string text = "Spell cast: " + e.spellName;
    if (e.x && e.y)
        text += " at (" + std::to_string(*e.x) + "," + std::to_string(*e.y) + ")";
    Push(text);
}

void EventSystem::Notify(const MoveEvent& e) {
    Push(e.entityName + " moved to (" +
         std::to_string(e.x) + "," + std::to_string(e.y) + ")");
}

void EventSystem::Notify(const GameMessageEvent& e) {
    Push(e.message);
}
