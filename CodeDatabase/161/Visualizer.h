#ifndef VISUALIZER_H
#define VISUALIZER_H



#include <memory>
#include <vector>
#include <string>
#include "../map/Field.h"
#include "../logic/World.h"
#include "../entites/Player.h"

class Visualizer {
public:
	static void Draw(Field& field, World& world, const std::shared_ptr<Player>& player);

private:
	static void PrintFrame(const std::vector<std::vector<std::string>>& frame,
						   int width, int height, int player_score, int);
};


#endif //VISUALIZER_H
