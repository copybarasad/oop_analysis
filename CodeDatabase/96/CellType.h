#pragma once
#include <string>

enum class CellType { Empty, Wall, Stun };
std::string typeCelltoString(CellType type);
CellType stringToTypeCell(const std::string& s);
