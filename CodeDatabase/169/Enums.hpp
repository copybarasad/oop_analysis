#pragma once

enum class AttackMode {
	Melee,
	Ranged
};

enum class MoveResult {
	Moved,
	Blocked,
	OccupiedEnemy,
	OutOfBounds,
	Slowed
};

enum class OccupantType {
	None,
	Player,
	Enemy,
	Building,
	Tower
};

enum class CellType {
	Normal,
	Impassable,
	Slowing
};

