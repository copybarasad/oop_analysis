#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>


struct StandardKeyboardLayout {
    static const char KEYUP =     'w';
    static const char KEYLEFT =   'a';
    static const char KEYRIGHT =  'd';
    static const char KEYDOWN =   's';
    static const char KEYE =      'e';
    static const char KEYQ =      'q';
    static const char KEYESC =     27;
    static const char KEYENTER = '\n';
    static const char KEY1 =      '1';
    static const char KEY2 =      '2';
    static const char KEY3 =      '3';
    static const char KEY4 =      '4';
};

class KeyboardLayout {
private:
    std::unordered_map<std::string, std::vector<char>> commandToKeys;
    std::unordered_map<char, std::string> keyToCommand;
    bool valid = false;

    void setDefaults();
    bool validateLayout();
    char parseKey(const std::string& keyStr);
public:

    char KEYUP;
    char KEYLEFT;
    char KEYRIGHT;
    char KEYDOWN;
    char KEYE;
    char KEYQ;
    char KEYESC;
    char KEYENTER;
    char KEY1;
    char KEY2;
    char KEY3;
    char KEY4;

    KeyboardLayout();
    explicit KeyboardLayout(const std::string& configFile);
    ~KeyboardLayout() = default;

    bool loadFromFile(const std::string& configFile);
    bool isValid() const { return valid; }
    void printLayout() const;
    std::string getCommandForKey(char key) const;
};