#pragma once
#include "SaveData.hpp"
#include "FileHandler.hpp"
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <sstream>
#include <functional>

#define SAVEPATH "../saves/"
#define XORENCDECKEY "P4j<S.,Z;~LGU/S=kPJ9>(pH[51'LOcJ2.{yDCHbX'yd;P-<1TIihCD1z[qckms^N!`/p2?jssS9BLdqx~-B/sDeTs/HD2zVTM^E9(on_p2_H*YBXXHLwOqp*|5#QB.AZ.np!/HwKW`<$UU`PG?;jnil*b*n='uGYXn%+>xkcrZ*HA0n.o,mT,]u^unZpjWB=+-]Z!B=lch,FY^f}pg$>SuQGfjKHa+M*@-OAz02s3nxY]qc|<Y~O{]!Y4="

class SavesManager {
private:
    std::string xorEncryptDecrypt(const std::string& text, const std::string& key);
    bool isSaveCorrect(const std::string& text, std::string hash);
    std::size_t getHash(const std::string& text);
    std::string getCorrectSaveName(std::string saveName);
    std::string serializeData(SaveData& data);
    SaveData deserializeData(std::string& serializedData);
    std::string getStringFromEnemySaveData(std::vector<struct EnemySaveData>& enemySaveData);
    void parseEnemySaveData(std::stringstream& ss, std::vector<struct EnemySaveData>& enemySaveData);
    std::string getStringFromSpellSaveData(struct SpellSaveData& spellData);
    void parseSpellSaveData(std::stringstream& ss, SpellSaveData& spellData);
    std::string getStringFromCellsSaveData(std::vector<struct CellSaveData>& cellsData);
    void parseCellsSaveData(std::stringstream& ss, std::vector<struct CellSaveData>& cellsData);
public:
    SavesManager();
    ~SavesManager();
    void newSave(SaveData& data, std::string saveName);
    SaveData getLoadGameData(std::string saveName);
    std::vector<std::string> getSavesList(int start, int count);
};
