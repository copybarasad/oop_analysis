#include "JsonValidator.h"

void JsonValidator::throwValidationError(const std::string& msg) {
    throw std::runtime_error("[JSON validation error] " + msg);
}

void JsonValidator::checkBool(const json& j, const std::string& field) {
    if (!j.at(field).is_boolean())
        throwValidationError(field + " must be bool");
}

void JsonValidator::checkNumber(const json& j, const std::string& field) {
    if (!j.at(field).is_number())
        throwValidationError(field + " must be a number");
}

void JsonValidator::checkPositive(const json& j, const std::string& field) {
    checkNumber(j, field);
    if (j.at(field).get<int>() <= 0)
        throwValidationError(field + " must be > 0");
}

void JsonValidator::checkNonNegative(const json& j, const std::string& field) {
    checkNumber(j, field);
    if (j.at(field).get<int>() < 0)
        throwValidationError(field + " must be >= 0");
}

void JsonValidator::checkEnum(const json& j, const std::string& field, int min, int max) {
    checkNumber(j, field);
    int v = j.at(field).get<int>();
    if (v < min || v > max)
        throwValidationError(field + " is out of enum range");
}

void JsonValidator::checkCoords(const json& j, int w, int h) {
    int x = j.at("x").get<int>();
    int y = j.at("y").get<int>();
    if (x < 0 || x >= w || y < 0 || y >= h)
        throwValidationError("Invalid coords (" + std::to_string(x) + "," + std::to_string(y) + ")");
}


void JsonValidator::validateGame(const json& j) {
    // ������� �������� Game
    checkNonNegative(j, "levelIndex");
    checkNonNegative(j, "move");
    checkBool(j, "running");
    checkEnum(j, "turn", 0, 1);
    checkEnum(j, "actionMode", 0, 3);

    validateField(j.at("field"));

    int width = j.at("field").at("width").get<int>();
    int height = j.at("field").at("height").get<int>();

    // Game-level
    checkCoords(j.at("aimPos"), width, height);
    validateEnemies(j.at("enemies"), width, height);
    validatePlayer(j.at("player"), width, height);
}

void JsonValidator::validateField(const json& f) {
    checkPositive(f, "width");
    checkPositive(f, "height");

    int w = f.at("width").get<int>();
    int h = f.at("height").get<int>();

    validateCells(f.at("cells"), w, h);
    validateTowers(f.at("towers"), w, h);
    validateTraps(f.at("traps"), w, h);
}


void JsonValidator::validateCells(const json& cells, int w, int h) {
    if (!cells.is_array()) throwValidationError("cells must be array");

    for (auto& row : cells) {
        if (!row.is_array()) throwValidationError("cells row must be array");
        for (auto& cell : row) {
            checkBool(cell, "walkable");
            checkCoords(cell, w, h);
        }
    }
}


void JsonValidator::validateTowers(const json& arr, int w, int h) {
    for (auto& t : arr) {
        checkPositive(t, "damage");
        checkNonNegative(t, "range");
        checkNonNegative(t, "skip");
        checkCoords(t, w, h);
    }
}


void JsonValidator::validateTraps(const json& arr, int w, int h) {
    for (auto& tr : arr) {
        checkPositive(tr, "damage");
        checkCoords(tr, w, h);
    }
}


void JsonValidator::validateEnemies(const json& arr, int w, int h) {
    for (auto& e : arr) {
        checkPositive(e, "damage");
        checkPositive(e, "health");
        checkCoords(e, w, h);
    }
}


void JsonValidator::validatePlayer(const json& p, int w, int h) {
    checkPositive(p, "damage");
    checkPositive(p, "health");
    checkPositive(p, "maxHealth");

    if (p.at("health").get<int>() > p.at("maxHealth").get<int>())
        throwValidationError("player.health > player.maxHealth");

    checkEnum(p, "mode", 0, 1);
    checkCoords(p, w, h);

    validateHand(p.at("hand"));
}


void JsonValidator::validateHand(const json& hand) {
    checkNonNegative(hand, "size");

    if (!hand.at("spells").is_array())
        throwValidationError("hand.spells must be array");

    for (auto& s : hand.at("spells"))
        validateSpell(s);
}

void JsonValidator::validateSpell(const json& s) {
    checkPositive(s, "damage");

    std::string typeStr = s.at("type").get<std::string>();
    if (typeStr != "Damage" && typeStr != "Splash" && typeStr != "Trap") {
        throwValidationError("Invalid spell type: " + typeStr);
    }

    if (s.contains("distance")) checkPositive(s, "distance");
    if (s.contains("radius")) checkPositive(s, "radius");
}
