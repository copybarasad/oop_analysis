//
// Created by Artem on 20.11.2025.
//

#include "ConsoleRenderer.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <iomanip>
#include <cstdlib>
#include "../Entities/Creatures/Player.h"
#include "../Logger/Logger.h"

void ConsoleRenderer::prepareConsole() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(1251);
}

void ConsoleRenderer::draw() {

    clearDisplay();
    std::vector<std::vector<std::string>> chars(this->field.getHeight(),
                                                std::vector<std::string>(this->field.getWidth(), " "));
    for (int x = 0; x < this->field.getHeight(); ++x) {
        for (int y = 0; y < this->field.getWidth(); ++y) {
            if (this->field.getFieldCells()[x][y].getCellType() == CellType::Impassable) {
                chars[x][y] = ConsoleRenderer::WALL;
            } else if (this->field.getFieldCells()[x][y].hasEntityInCell()) {
                chars[x][y] = this->entityTypeToCharS.at(this->field.getFieldCells()[x][y].getEntityInCellType());
            } else if (this->field.getFieldCells()[x][y].hasCellEvent()) {
                chars[x][y] = this->cellEventTypeToCharS.at(this->field.getFieldCells()[x][y].getCellEventType());
            }
        }
    }
    chars[this->field.getHeight() - 1][this->field.getWidth() - 1] = EXIT;



    std::string res = connectStrings(getStringWithGridRounded(chars), getStringWithEntitiesInfo(), getStringWithPlayerInfo(), getStringLegend(), getStringWithLastActions());
    std::cout << res;
}

std::string ConsoleRenderer::getStringWithGridRounded(const std::vector<std::vector<std::string>> &grid) {
    std::stringstream ss;
    if (grid.empty()) return "";

    int height = this->field.getHeight();
    int width = this->field.getWidth();

    ss << "╭";
    for (int x = 0; x < width; ++x) {
        ss << "───";
        if (x < width - 1) ss << "┬";
    }
    ss << "╮" << std::endl;

    for (int y = 0; y < height; ++y) {
        ss << "│";
        for (int x = 0; x < width; ++x) {
            ss << " " << grid[y][x] << " ";
            if (x < width - 1) ss << "│";
        }
        ss << "│" << std::endl;

        if (y < height - 1) {
            ss << "├";
            for (int x = 0; x < width; ++x) {
                ss << "───";
                if (x < width - 1) ss << "┼";
            }
            ss << "┤" << std::endl;
        }
    }

    ss << "╰";
    for (int x = 0; x < width; ++x) {
        ss << "───";
        if (x < width - 1) ss << "┴";
    }
    ss << "╯" << std::endl;
    return ss.str();
}

std::string ConsoleRenderer::getStringLegend() {
    std::stringstream ss;
    ss << "┌─────────────────────────────┐" << std::endl;
    ss << "│          Легенда            │" << std::endl;
    ss << "├─────────────┬───────────────┤" << std::endl;
    ss << "│ " << "◎" << " - Игрок   │ " << "●" << " - Враг      │" << std::endl;
    ss << "│ " << "■" << " - Стена   │ " << "▲" << " - Башня     │" << std::endl;
    ss << "│ " << "♦" << " - Спавнер │ " << "A" << " - Союзник   │" << std::endl;
    ss << "│ " << "→" << " - Выход   │ " << "×" << " - Ловушка   │" << std::endl;
    ss << "└─────────────┴───────────────┘" << std::endl;
    return ss.str();
}

std::string ConsoleRenderer::getStringWithEntitiesInfo() {
    std::stringstream ss;

    if (entities.empty()) {
        ss << "No entities." << std::endl;
        return ss.str();
    }

    ss << "┌────────Сущности─────────┐" << std::endl;

    ss << "│" << std::setw(3) << std::right << "№  " << "│"
       << std::setw(10) << std::left << "Type" << "│"
       << std::setw(5) << std::left << "x/y" << "│"
       << std::setw(4) << std::right << "HP" << "│" << std::endl;

    ss << "├───┼──────────┼─────┼────┤" << std::endl;

    for (size_t i = 0; i < entities.size(); ++i) {
        const auto &entity = entities[i];
        EntityType type = entity->getType();
        std::string typeStr = "Unknown";

        auto it = entityTypeToStr.find(type);
        if (it != entityTypeToStr.end()) {
            typeStr = it->second;
        }


        int x = entity->getXCoordinate();
        int y = entity->getYCoordinate();
        int hp = entity->getHealthPoints();

        std::string posStr = std::to_string(y + 1) + "/" + std::to_string(x + 1);
        if (posStr.length() > 5) {
            posStr = posStr.substr(0, 5);
        }

        ss << "│" << std::setw(3) << std::right << (i + 1) << "│"
           << std::setw(10) << std::left << typeStr << "│"
           << std::setw(5) << std::left << posStr << "│"
           << std::setw(4) << std::right << hp << "│" << std::endl;
    }
    ss << "└───┴──────────┴─────┴────┘" << std::endl;
    return ss.str();
}

void ConsoleRenderer::clearDisplay() {
    system("cls");
}

auto formatNumber = [](int num) -> std::string {
    std::ostringstream oss;
    if (num < 10) oss << "  " << num;
    else if (num < 100) oss << " " << num;
    else oss << num;
    return oss.str();
};



std::string ConsoleRenderer::getStringWithPlayerInfo() {
    std::stringstream ss;

    int x = this->player.getXCoordinate();
    int y = player.getYCoordinate();
    int health = player.getHealthPoints();
    bool isCloseRange = player.isCloseRangeAttackSelected();
    long score = player.getScore();

    const auto &closeAttack = player.getCloseRangeAttack();
    const auto &longAttack = player.getLongRangeAttack();
    const auto &spellHand = player.getSpellHand();


    ss << "┌────────── PLAYER ──────────┐" << std::endl;
    ss << "│ Param       Value          │" << std::endl;
    ss << "├────────────────────────────┤" << std::endl;
    ss << "│ x/y         " << std::right << std::setw(2) << y+1 << "/" << x+1 << "           │" << std::endl;
    ss << "│ Health      " << std::setw(10) << formatNumber(health) << "     │" << std::endl;
    ss << "│ Score       " << std::setw(10) << formatNumber(score) << "     │" << std::endl;
    ss << "├─────── ATTACKS ────────────┤" << std::endl;
    ss << "│ Close      " << std::setw(10) << formatNumber(closeAttack.getDamage()) << "      │" << std::endl;
    ss << "│ Long       " << std::setw(10) << formatNumber(longAttack.getDamage()) << "      │" << std::endl;
    ss << "│ Range      " << std::setw(10) << formatNumber(longAttack.getRange()) << "      │" << std::endl;
    ss << "│ Active     " << std::left << std::setw(10) << (isCloseRange ? "CLOSE" : "LONG") << std::right << "      │" << std::endl;
    ss << "├─────── SPELLS ─────────────┤" << std::endl;
    ss << "│ Spells.C   " << std::setw(10) << formatNumber(spellHand.getSpells().size()) << "      │" << std::endl;
    ss << "└────────────────────────────┘" << std::endl;

    return ss.str();
}

std::vector<std::string> getStringLinesVector(const std::string& s){
    std::vector<std::string> res;
    std::string temp;
    for(char i : s){
        if(i == '\n'){
            res.push_back(temp);
            temp = "";
        }else{
            temp += i;
        }
    }
    return res;
}


std::string createEmptyStringSameLength(const std::string& input) {
    std::string result;

    for (size_t i = 0; i < input.length(); ) {
        result += ' ';
        unsigned char c = static_cast<unsigned char>(input[i]);

        if (c < 128) {
            i += 1;
        } else if (c < 194) {
            i += 1;
        } else if (c < 224) {
            i += 2;
        } else if (c < 240) {
            i += 3;
        } else if (c < 248) {
            i += 4;
        } else {
            i += 1;
        }
    }

    return result;
}

std::string
ConsoleRenderer::connectStrings(const std::string &grid, const std::string &entitiesInfo, const std::string &playerInfo,
                                const std::string &legend, const std::string lastActions) {
    std::vector<std::string> gridLines = getStringLinesVector(grid);
    std::vector<std::string> entitiesInfoLines = getStringLinesVector(entitiesInfo);
    std::vector<std::string> playerInfoLines = getStringLinesVector(playerInfo);
    std::vector<std::string> legendLines = getStringLinesVector(legend);
    std::vector<std::string> actionLines = getStringLinesVector(lastActions);

    std::string res;

    std::string gridSpace = createEmptyStringSameLength(gridLines[0]);
    std::string playerInfoSpace = createEmptyStringSameLength(playerInfoLines[0]);
    

    for(int i = 0; i < std::max(std::max(gridLines.size(), entitiesInfoLines.size()), playerInfoLines.size()); i++){
        if(i < gridLines.size()){
            res += gridLines[i];
            res += "   ";
        }

        if(i < playerInfoLines.size()){
            if(i >= gridLines.size()){
                res += gridSpace;
                res += "   ";
            }
            res += playerInfoLines[i];
            res += "   ";
        }

        if(i < entitiesInfoLines.size()){
            if(i >= gridLines.size()){
                res += gridSpace;
                res += "   ";
            }
            if(i >= playerInfoLines.size()){
                res += playerInfoSpace;
                res += "   ";
            }
            res += entitiesInfoLines[i];
        }

        res += '\n';

    }
    res += '\n';

    std::string legendSize = createEmptyStringSameLength(legendLines[0]);
    for(int i = 0; i < std::max(legendLines.size(), actionLines.size()); i++){
        if(i < legendLines.size()){
            res += legendLines.at(i) + "   ";
        }else{
            res += legendSize + "   ";
        }

        if(i < actionLines.size()){
            res += actionLines.at(i);
        }
        res += "\n";
    }



    return res;
}

std::string ConsoleRenderer::getStringWithLastActions() {
    std::stringstream ss;

    std::vector<std::string> sssss = Logger::getLastMessages();
    ss << "┌─────────────────────────────────────── LAST ACTIONS ───────────────────────────────────────┐" << std::endl;
    for(std::string s : Logger::getLastMessages()){
        ss << s << std::endl;
    }
    ss << "└────────────────────────────────────────────────────────────────────────────────────────────┘" << std::endl;

    return ss.str();
}
