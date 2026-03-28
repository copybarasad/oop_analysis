#include "Utils.h"

std::string Utils::generateRandomSpellName() {
    static std::mt19937 rng(std::time(nullptr));
    std::uniform_int_distribution<int> lengthDist(4, 8);
    std::uniform_int_distribution<int> vowelDist(0, 4);
    std::uniform_int_distribution<int> consonantDist(0, 19);

    const std::string vowels = "qeyuioa";
    const std::string consonants = "wsdrftghjklpmnbvcxz";

    int totalWords = rng() % 2 + 1;
    std::string result;

    for (int word = 0; word < totalWords; ++word)
    {
        if (word > 0) result += " ";

        int len = lengthDist(rng);
        bool isVowel = (rng() % 2) == 0;

        for (int i = 0; i < len; ++i)
        {
            if (isVowel)
                result += vowels[vowelDist(rng)];
            else
                result += consonants[consonantDist(rng)];

            isVowel = !isVowel;
        }
    }

    if (!result.empty()) {
        result[0] = std::toupper(result[0]);
    }

    return result;
}
