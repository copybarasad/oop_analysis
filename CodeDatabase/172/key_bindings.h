#pragma once

#include <cctype>
#include <fstream>
#include <map>
#include <set>
#include <string>

#include "command.h"
#include "game.h"
#include "pos.h"


namespace game {
    class KeyBindings {
        public:
            // Key -> Command (with all parameters inside)
            std::map<char, Command> key2command;

            // Default bindings
            static KeyBindings defaults() {
                KeyBindings kb;

                kb.key2command['q'] = Command{CommandType::Quit};

                kb.key2command['w'] = Command{CommandType::Move, Direction::Up   };
                kb.key2command['a'] = Command{CommandType::Move, Direction::Left };
                kb.key2command['s'] = Command{CommandType::Move, Direction::Down };
                kb.key2command['d'] = Command{CommandType::Move, Direction::Right};

                kb.key2command['f'] = Command{CommandType::Attack};
                kb.key2command['m'] = Command{CommandType::SwitchMode};

                kb.key2command['k'] = Command{CommandType::Save};
                kb.key2command['l'] = Command{CommandType::Load};

                return kb;
            }

            static KeyBindings load(const std::string& filename) {
                KeyBindings Defaults = defaults();
                KeyBindings Parsed;

                bool status = Parsed.parseFile(filename);
                if (!status) return Defaults;

                if (!Parsed.validate(Defaults)) return Defaults;

                return Parsed;
            }
        
        private:
            using ActionSignature = std::string;

            static std::string trim(const std::string& s)
            {
                size_t l = 0, r = s.size();

                while (l < r && std::isspace( (unsigned char)s[l] ))     ++l;
                while (r > l && std::isspace( (unsigned char)s[r - 1] )) --r;

                return s.substr(l, r - l);
            }

            static std::string toUpper(std::string s)
            {
                for (char& c: s) c = (char)std::toupper( (unsigned char)c );
                return s;
            }

            static ActionSignature signature(const Command& cmd)
            {
                std::string signature = std::to_string( (int)cmd.type  );
                signature += ",dir="  + std::to_string( (int)cmd.dir   );
                signature += ",ind="  + std::to_string( (int)cmd.index );
                signature += ",w="    + std::to_string( (int)cmd.w    );
                signature += ",h="    + std::to_string( (int)cmd.h    );

                return signature;
            }

            bool parseFile(const std::string& filename)
            {
                std::ifstream in(filename);
                if (!in.is_open()) return false;

                std::set<char> usedKeys;
                std::set<ActionSignature> usedActions;

                std::string line;
                int lineNum = 0;

                while (std::getline(in, line))
                {
                    ++lineNum;
                    std::string s = trim(line);

                    if (s.empty()) continue;

                    size_t eq = s.find('=');
                    if (eq == std::string::npos) return false;  // '=' not in s

                    std::string action = toUpper(trim(s.substr(0, eq)));
                    std::string keyString = trim(s.substr(eq + 1));
                    if (keyString.size() != 1) return false;

                    char key = keyString[0];

                    //! 1 - 9 are reserved for spells
                    if (key >= '1' && key <= '9') return false;

                    Command cmd;
                    if (!actionToCommand(action, cmd)) return false;

                    // key already used
                    if (usedKeys.count(key)) return false;

                    ActionSignature sig = signature(cmd);
                    
                    // action already used
                    if (usedActions.count(sig)) return false;

                    usedKeys.insert(key);
                    usedActions.insert(sig);

                    key2command[key] = cmd;
                }

                return true;
            }

            bool validate(const KeyBindings& defaults) const
            {
                std::set<ActionSignature> have;
                for (const auto& kv: key2command) {
                    have.insert(signature(kv.second));
                }

                std::set<ActionSignature> need;
                for (const auto& kv: defaults.key2command) {
                    need.insert(signature(kv.second));
                }

                // any command not in key bindings -> error
                for (const auto& req: need) {
                    if (!have.count(req)) return false;
                }

                return true;
            }

            static bool actionToCommand(const std::string& action, Command& out)
            {
                if (action == "QUIT")        { out = Command{CommandType::Quit}; return true; }

                if (action == "MOVE_UP")     { out = Command{CommandType::Move, Direction::Up};    return true; }
                if (action == "MOVE_LEFT")   { out = Command{CommandType::Move, Direction::Left};  return true; }
                if (action == "MOVE_DOWN")   { out = Command{CommandType::Move, Direction::Down};  return true; }
                if (action == "MOVE_RIGHT")  { out = Command{CommandType::Move, Direction::Right}; return true; }

                if (action == "ATTACK")      { out = Command{CommandType::Attack};     return true; }
                if (action == "SWITCH_MODE") { out = Command{CommandType::SwitchMode}; return true; }

                if (action == "SAVE")        { out = Command{CommandType::Save}; return true; }
                if (action == "LOAD")        { out = Command{CommandType::Load}; return true; }

                return false;
            }
    };
}