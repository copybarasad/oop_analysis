#include "pathfinder.h"
#include "enemy.h"        
#include <queue>
#include <vector>
#include <cmath>
#include <stdexcept>

bool Pathfinder::FindStep(const GameField& field, const Position& start, const Position& goal,
						  Position& step, const std::vector<Enemy>& others) {
	std::queue<Position> q;
	std::vector<std::vector<bool>> visited(field.GetHeight(), std::vector<bool>(field.GetWidth(), false));
	std::vector<std::vector<Position>> parent(field.GetHeight(), std::vector<Position>(field.GetWidth(), {-1, -1}));

	q.push(start);
	visited[start.y][start.x] = true;

	int dx[4] = {1, -1, 0, 0};
	int dy[4] = {0, 0, 1, -1};
	bool pathFound = false;

	while (!q.empty()) {
		Position cur = q.front(); q.pop();
		if (cur == goal) { pathFound = true; break; }

		for (int i = 0; i < 4; ++i) {
			Position next = {cur.x + dx[i], cur.y + dy[i]};
			if (next.x < 0 || next.y < 0 || next.x >= field.GetWidth() || next.y >= field.GetHeight())
				continue;
			if (visited[next.y][next.x]) continue;
			if (!field.IsWalkable(next) && next != goal) continue;

			visited[next.y][next.x] = true;
			parent[next.y][next.x] = cur;
			q.push(next);
		}
	}

	if (!pathFound) return false;

	Position temp = goal;
	while (!(parent[temp.y][temp.x] == start)) {
		if (parent[temp.y][temp.x].x == -1) return false;
		temp = parent[temp.y][temp.x];
	}

	for (const auto& e : others) {
		if (e.IsAlive() && e.GetPosition() == temp) return false;
	}

	step = temp;
	return true;
}
