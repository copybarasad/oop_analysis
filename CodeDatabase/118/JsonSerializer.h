#ifndef JSON_SERIALISER
#define JSON_SERIALISER

#include "nlohmann/json.hpp"
#include "PlayerConfig.h"
#include "GameLogic.h"
using json = nlohmann::json;

class JsonSerializer
{
public:
	static json Serialize(PlayerConfig&, GameLogic&);
	static void Deserialize(PlayerConfig&, GameLogic&, json);
};

#endif