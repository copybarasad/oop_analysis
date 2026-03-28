#include "input.h"
#include <algorithm>
#include <vector>
/*COMMAND Converter::ProcessMove(const char keypress) {
  switch (keypress) {}
  return NOTHING;
}*/
KeyboardKey readKey() { return static_cast<KeyboardKey>(GetKeyPressed()); }
/*COMMAND Converter::ProcessMove() const {
  switch (GetKeyPressed()) {
  case KEY_W:
    return COMMAND::MOVED_UP;

  case KEY_S:
    return COMMAND::MOVED_DOWN;
  case KEY_D:
    return COMMAND::MOVED_RIGHT;
  case KEY_A:
    return COMMAND::MOVED_LEFT;
  case KEY_E:
    return COMMAND::DIRECTSPELL;
  case KEY_R:
    return COMMAND::AREASPELL;
  case KEY_T:
    return COMMAND::TRAPSPELL;
  default:
    return COMMAND::NOTHING;
  }
}
*/
Converter::Converter(const std::string &config_file) {
  if (!loadConfig(config_file)) {
    std::cerr << "Failed to load controls from " << config_file
              << ". Using default controls." << std::endl;
    setDefaultMaps();
  }
}

bool Converter::loadConfig(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    fprintf(stderr, "\033[31mError: \033[0m Couldnt open file\n");
    return false;
  }

  // Clear existing mappings
  keytoCommand.clear();
  commandtoKey.clear();

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') {
      continue;
    }

    size_t commentPos = line.find('#');
    if (commentPos != std::string::npos) {
      line = line.substr(0, commentPos);
    }

    // Trim whitespace
    line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

    // Parse key=value pair
    size_t equalsPos = line.find('=');
    if (equalsPos == std::string::npos) {
      continue; // Invalid line format
    }

    std::string commandStr = line.substr(0, equalsPos);
    std::string keyStr = line.substr(equalsPos + 1);

    // Convert command string to COMMAND enum
    COMMAND cmd = COMMAND::UNSET;
    if (commandStr == "MOVED_UP")
      cmd = COMMAND::MOVED_UP;
    else if (commandStr == "MOVED_DOWN")
      cmd = COMMAND::MOVED_DOWN;
    else if (commandStr == "MOVED_LEFT")
      cmd = COMMAND::MOVED_LEFT;
    else if (commandStr == "MOVED_RIGHT")
      cmd = COMMAND::MOVED_RIGHT;
    else if (commandStr == "DIRECTSPELL")
      cmd = COMMAND::DIRECTSPELL;
    else if (commandStr == "AREASPELL")
      cmd = COMMAND::AREASPELL;
    else if (commandStr == "TRAPSPELL")
      cmd = COMMAND::TRAPSPELL;
    else {
      std::cerr << "Unknown command: " << commandStr << std::endl;
      continue;
    }

    // Convert key string to KeyboardKey
    KeyboardKey key = stringToKey(keyStr);
    if (key == KEY_NULL) {
      std::cerr << "Unknown key: " << keyStr << std::endl;
      continue;
    } else {
      std::cout << "\033[36m[MAP]:\033[0m Mapping key '" << key
                << "' to command '" << (int)cmd << "'" << std::endl;
    }

    // Store mappings
    keytoCommand[key] = cmd;
    commandtoKey[cmd] = key;
  }

  file.close();

  // Validate the loaded configuration
  if (!checkConfig()) {
    std::cerr << "Invalid configuration detected. Using defaults." << std::endl;
    keytoCommand.clear();
    commandtoKey.clear();
    setDefaultMaps();
    return false;
  }

  return true;
}
void Converter::setDefaultMaps() {
  keytoCommand.clear();
  commandtoKey.clear();

  for (const auto &[cmd, key] : defaults) {
    keytoCommand[key] = cmd;
    commandtoKey[cmd] = key;
  }
}
COMMAND Converter::ProcessMove() const {
  // Check all configured keys
  for (const auto &[key, cmd] : keytoCommand) {
    if (IsKeyPressed(key)) {
      return cmd;
    }
  }
  return COMMAND::NOTHING;
}
void Converter::reloadConfig(const std::string &configFile) {
  loadConfig(configFile);
}
std::unordered_map<COMMAND, KeyboardKey> Converter::getCurrentMappings() const {
  return commandtoKey;
}

bool Converter::checkConfig() const {
  std::vector<COMMAND> requiredCommands = {
      COMMAND::MOVED_UP,    COMMAND::MOVED_DOWN,  COMMAND::MOVED_LEFT,
      COMMAND::MOVED_RIGHT, COMMAND::DIRECTSPELL, COMMAND::AREASPELL,
      COMMAND::TRAPSPELL};

  for (const auto &cmd : requiredCommands) {
    if (commandtoKey.find(cmd) == commandtoKey.end()) {
      std::cerr << "Missing command: " << static_cast<int>(cmd) << std::endl;
      return false;
    }
  }

  std::unordered_map<KeyboardKey, int> keyUsage;
  for (const auto &[key, cmd] : keytoCommand) {
    keyUsage[key]++;
    if (keyUsage[key] > 1) {
      std::cerr << "Key " << keyToStr(key)
                << " is assigned to multiple commands!" << std::endl;
      return false;
    }
  }

  if (commandtoKey.size() != keytoCommand.size()) {
    std::cerr << "How did we get here" << std::endl;
    return false;
  }

  return true;
}
KeyboardKey Converter::stringToKey(const std::string &keyStr) const {
  std::string upperStr = keyStr;
  std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);

  // Static map for better performance
  static const std::unordered_map<std::string, KeyboardKey> keyMap = {
      // Alphabet keys
      {"A", KEY_A},
      {"B", KEY_B},
      {"C", KEY_C},
      {"D", KEY_D},
      {"E", KEY_E},
      {"F", KEY_F},
      {"G", KEY_G},
      {"H", KEY_H},
      {"I", KEY_I},
      {"J", KEY_J},
      {"K", KEY_K},
      {"L", KEY_L},
      {"M", KEY_M},
      {"N", KEY_N},
      {"O", KEY_O},
      {"P", KEY_P},
      {"Q", KEY_Q},
      {"R", KEY_R},
      {"S", KEY_S},
      {"T", KEY_T},
      {"U", KEY_U},
      {"V", KEY_V},
      {"W", KEY_W},
      {"X", KEY_X},
      {"Y", KEY_Y},
      {"Z", KEY_Z},

      // Number keys (top row)
      {"ZERO", KEY_ZERO},
      {"ONE", KEY_ONE},
      {"TWO", KEY_TWO},
      {"THREE", KEY_THREE},
      {"FOUR", KEY_FOUR},
      {"FIVE", KEY_FIVE},
      {"SIX", KEY_SIX},
      {"SEVEN", KEY_SEVEN},
      {"EIGHT", KEY_EIGHT},
      {"NINE", KEY_NINE},

      // Number keys (numpad)
      {"KP_0", KEY_KP_0},
      {"KP_1", KEY_KP_1},
      {"KP_2", KEY_KP_2},
      {"KP_3", KEY_KP_3},
      {"KP_4", KEY_KP_4},
      {"KP_5", KEY_KP_5},
      {"KP_6", KEY_KP_6},
      {"KP_7", KEY_KP_7},
      {"KP_8", KEY_KP_8},
      {"KP_9", KEY_KP_9},

      // Function keys
      {"F1", KEY_F1},
      {"F2", KEY_F2},
      {"F3", KEY_F3},
      {"F4", KEY_F4},
      {"F5", KEY_F5},
      {"F6", KEY_F6},
      {"F7", KEY_F7},
      {"F8", KEY_F8},
      {"F9", KEY_F9},
      {"F10", KEY_F10},
      {"F11", KEY_F11},
      {"F12", KEY_F12},

      // Arrow keys
      {"UP", KEY_UP},
      {"DOWN", KEY_DOWN},
      {"LEFT", KEY_LEFT},
      {"RIGHT", KEY_RIGHT},

      // Special keys
      {"SPACE", KEY_SPACE},
      {"ENTER", KEY_ENTER},
      {"ESCAPE", KEY_ESCAPE},
      {"BACKSPACE", KEY_BACKSPACE},
      {"TAB", KEY_TAB},
      {"CAPS_LOCK", KEY_CAPS_LOCK},

      // Modifier keys
      {"LSHIFT", KEY_LEFT_SHIFT},
      {"RSHIFT", KEY_RIGHT_SHIFT},
      {"LCTRL", KEY_LEFT_CONTROL},
      {"RCTRL", KEY_RIGHT_CONTROL},
      {"LALT", KEY_LEFT_ALT},
      {"RALT", KEY_RIGHT_ALT},
      {"LSUPER", KEY_LEFT_SUPER},
      {"RSUPER", KEY_RIGHT_SUPER},

      // Punctuation and symbols
      {"GRAVE", KEY_GRAVE},
      {"MINUS", KEY_MINUS},
      {"EQUAL", KEY_EQUAL},
      {"LEFT_BRACKET", KEY_LEFT_BRACKET},
      {"RIGHT_BRACKET", KEY_RIGHT_BRACKET},
      {"BACKSLASH", KEY_BACKSLASH},
      {"SEMICOLON", KEY_SEMICOLON},
      {"APOSTROPHE", KEY_APOSTROPHE},
      {"COMMA", KEY_COMMA},
      {"PERIOD", KEY_PERIOD},
      {"SLASH", KEY_SLASH},

      // Numpad symbols
      {"KP_DECIMAL", KEY_KP_DECIMAL},
      {"KP_DIVIDE", KEY_KP_DIVIDE},
      {"KP_MULTIPLY", KEY_KP_MULTIPLY},
      {"KP_SUBTRACT", KEY_KP_SUBTRACT},
      {"KP_ADD", KEY_KP_ADD},
      {"KP_ENTER", KEY_KP_ENTER},
      {"KP_EQUAL", KEY_KP_EQUAL},

      // Media keys
      {"MENU", KEY_MENU},
      {"VOLUME_UP", KEY_VOLUME_UP},
      {"VOLUME_DOWN", KEY_VOLUME_DOWN},

      // Navigation keys
      {"INSERT", KEY_INSERT},
      {"HOME", KEY_HOME},
      {"PAGE_UP", KEY_PAGE_UP},
      {"DELETE", KEY_DELETE},
      {"END", KEY_END},
      {"PAGE_DOWN", KEY_PAGE_DOWN},
      {"PRINT_SCREEN", KEY_PRINT_SCREEN},
      {"PAUSE", KEY_PAUSE},
      {"SCROLL_LOCK", KEY_SCROLL_LOCK},

      // Additional keys
      {"LEFT", KEY_LEFT},
      {"RIGHT", KEY_RIGHT},
      {"UP", KEY_UP},
      {"DOWN", KEY_DOWN},
      {"NUM_LOCK", KEY_NUM_LOCK}};

  // Check if it's a direct match
  auto it = keyMap.find(upperStr);
  if (it != keyMap.end()) {
    return it->second;
  }

  // Check if it's a single character (for letters that weren't caught above)
  if (upperStr.length() == 1) {
    char c = upperStr[0];
    if (c >= 'A' && c <= 'Z') {
      return static_cast<KeyboardKey>(KEY_A + (c - 'A'));
    }
    if (c >= '0' && c <= '9') {
      return static_cast<KeyboardKey>(KEY_ZERO + (c - '0'));
    }

    // Check special single characters
    static const std::unordered_map<char, KeyboardKey> charMap = {
        {'`', KEY_GRAVE},         {'-', KEY_MINUS},
        {'=', KEY_EQUAL},         {'[', KEY_LEFT_BRACKET},
        {']', KEY_RIGHT_BRACKET}, {'\\', KEY_BACKSLASH},
        {';', KEY_SEMICOLON},     {'\'', KEY_APOSTROPHE},
        {',', KEY_COMMA},         {'.', KEY_PERIOD},
        {'/', KEY_SLASH}};

    auto charIt = charMap.find(c);
    if (charIt != charMap.end()) {
      return charIt->second;
    }
  }

  // Check for KP_ prefix for numpad (e.g., "KP_0" to "KP_9")
  if (upperStr.length() == 4 && upperStr.substr(0, 3) == "KP_" &&
      upperStr[3] >= '0' && upperStr[3] <= '9') {
    return static_cast<KeyboardKey>(KEY_KP_0 + (upperStr[3] - '0'));
  }

  return KEY_NULL;
}
std::string Converter::keyToStr(KeyboardKey key) const {
  // Comprehensive switch statement for all Raylib keys
  switch (key) {
  // Alphabet keys
  case KEY_A:
    return "A";
  case KEY_B:
    return "B";
  case KEY_C:
    return "C";
  case KEY_D:
    return "D";
  case KEY_E:
    return "E";
  case KEY_F:
    return "F";
  case KEY_G:
    return "G";
  case KEY_H:
    return "H";
  case KEY_I:
    return "I";
  case KEY_J:
    return "J";
  case KEY_K:
    return "K";
  case KEY_L:
    return "L";
  case KEY_M:
    return "M";
  case KEY_N:
    return "N";
  case KEY_O:
    return "O";
  case KEY_P:
    return "P";
  case KEY_Q:
    return "Q";
  case KEY_R:
    return "R";
  case KEY_S:
    return "S";
  case KEY_T:
    return "T";
  case KEY_U:
    return "U";
  case KEY_V:
    return "V";
  case KEY_W:
    return "W";
  case KEY_X:
    return "X";
  case KEY_Y:
    return "Y";
  case KEY_Z:
    return "Z";

  // Number keys (top row)
  case KEY_ZERO:
    return "0";
  case KEY_ONE:
    return "1";
  case KEY_TWO:
    return "2";
  case KEY_THREE:
    return "3";
  case KEY_FOUR:
    return "4";
  case KEY_FIVE:
    return "5";
  case KEY_SIX:
    return "6";
  case KEY_SEVEN:
    return "7";
  case KEY_EIGHT:
    return "8";
  case KEY_NINE:
    return "9";

  // Function keys
  case KEY_F1:
    return "F1";
  case KEY_F2:
    return "F2";
  case KEY_F3:
    return "F3";
  case KEY_F4:
    return "F4";
  case KEY_F5:
    return "F5";
  case KEY_F6:
    return "F6";
  case KEY_F7:
    return "F7";
  case KEY_F8:
    return "F8";
  case KEY_F9:
    return "F9";
  case KEY_F10:
    return "F10";
  case KEY_F11:
    return "F11";
  case KEY_F12:
    return "F12";

  // Arrow keys
  case KEY_UP:
    return "UP";
  case KEY_DOWN:
    return "DOWN";
  case KEY_LEFT:
    return "LEFT";
  case KEY_RIGHT:
    return "RIGHT";

  // Special keys
  case KEY_SPACE:
    return "SPACE";
  case KEY_ENTER:
    return "ENTER";
  case KEY_ESCAPE:
    return "ESCAPE";
  case KEY_BACKSPACE:
    return "BACKSPACE";
  case KEY_TAB:
    return "TAB";
  case KEY_CAPS_LOCK:
    return "CAPS_LOCK";

  // Modifier keys
  case KEY_LEFT_SHIFT:
    return "LSHIFT";
  case KEY_RIGHT_SHIFT:
    return "RSHIFT";
  case KEY_LEFT_CONTROL:
    return "LCTRL";
  case KEY_RIGHT_CONTROL:
    return "RCTRL";
  case KEY_LEFT_ALT:
    return "LALT";
  case KEY_RIGHT_ALT:
    return "RALT";
  case KEY_LEFT_SUPER:
    return "LSUPER";
  case KEY_RIGHT_SUPER:
    return "RSUPER";

  // Punctuation and symbols
  case KEY_GRAVE:
    return "`";
  case KEY_MINUS:
    return "-";
  case KEY_EQUAL:
    return "=";
  case KEY_LEFT_BRACKET:
    return "[";
  case KEY_RIGHT_BRACKET:
    return "]";
  case KEY_BACKSLASH:
    return "\\";
  case KEY_SEMICOLON:
    return ";";
  case KEY_APOSTROPHE:
    return "'";
  case KEY_COMMA:
    return ",";
  case KEY_PERIOD:
    return ".";
  case KEY_SLASH:
    return "/";

  // Numpad keys
  case KEY_KP_0:
    return "KP_0";
  case KEY_KP_1:
    return "KP_1";
  case KEY_KP_2:
    return "KP_2";
  case KEY_KP_3:
    return "KP_3";
  case KEY_KP_4:
    return "KP_4";
  case KEY_KP_5:
    return "KP_5";
  case KEY_KP_6:
    return "KP_6";
  case KEY_KP_7:
    return "KP_7";
  case KEY_KP_8:
    return "KP_8";
  case KEY_KP_9:
    return "KP_9";
  case KEY_KP_DECIMAL:
    return "KP_DECIMAL";
  case KEY_KP_DIVIDE:
    return "KP_DIVIDE";
  case KEY_KP_MULTIPLY:
    return "KP_MULTIPLY";
  case KEY_KP_SUBTRACT:
    return "KP_SUBTRACT";
  case KEY_KP_ADD:
    return "KP_ADD";
  case KEY_KP_ENTER:
    return "KP_ENTER";
  case KEY_KP_EQUAL:
    return "KP_EQUAL";

  // Navigation keys
  case KEY_INSERT:
    return "INSERT";
  case KEY_HOME:
    return "HOME";
  case KEY_PAGE_UP:
    return "PAGE_UP";
  case KEY_DELETE:
    return "DELETE";
  case KEY_END:
    return "END";
  case KEY_PAGE_DOWN:
    return "PAGE_DOWN";
  case KEY_PRINT_SCREEN:
    return "PRINT_SCREEN";
  case KEY_PAUSE:
    return "PAUSE";
  case KEY_SCROLL_LOCK:
    return "SCROLL_LOCK";
  case KEY_NUM_LOCK:
    return "NUM_LOCK";

  // Media keys
  case KEY_MENU:
    return "MENU";
  case KEY_VOLUME_UP:
    return "VOLUME_UP";
  case KEY_VOLUME_DOWN:
    return "VOLUME_DOWN";

  default:
    // Check if it's a numpad key that wasn't caught
    if (key >= KEY_KP_0 && key <= KEY_KP_9) {
      return "KP_" + std::to_string(key - KEY_KP_0);
    }

    // Generic fallback
    return "KEY_" + std::to_string(static_cast<int>(key));
  }
}
