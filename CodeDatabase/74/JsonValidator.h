#pragma once
#include <stdexcept>
#include <string>
#include "Serialization.h"

class JsonValidator {
public:
    static void validateGame(const json& j);

private:
    // ����� ��������
    static void checkBool(const json& j, const std::string& field);
    static void checkNumber(const json& j, const std::string& field);
    static void checkPositive(const json& j, const std::string& field);
    static void checkNonNegative(const json& j, const std::string& field);

    static void checkEnum(const json& j, const std::string& field, int min, int max);
    static void checkCoords(const json& j, int width, int height);

    // ����������
    static void validateField(const json& j);
    static void validateCells(const json& j, int width, int height);
    static void validateTowers(const json& j, int width, int height);
    static void validateTraps(const json& j, int width, int height);

    static void validateEnemies(const json& j, int width, int height);
    static void validatePlayer(const json& j, int width, int height);
    static void validateHand(const json& hand);
    static void validateSpell(const json& spell);

    static void throwValidationError(const std::string& msg);
};