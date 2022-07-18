#pragma once
static const enum CollisionShapeType
{
	SHAPE_UNKNOWN = -1,
	COLLISION_SHAPE_SPHERE,
	COLLISION_SHAPE_MESH,
};

static const enum CollisionAttribute
{
	COLLISION_ATTR_NONE = 0,
	COLLISION_ATTR_PLAYER = 0b1 << 0,
	COLLISION_ATTR_ENEMY = 0b1 << 1,
	COLLISION_ATTR_ENEMY_BULLET = 0b1 << 2,
	COLLISION_ATTR_BOSS = 0b1 << 3,
};