#pragma once

#include "glm/vec2.hpp"

struct circle
{
	float radius;
};

struct aabb
{
	glm::vec2 halfExtents;
};

enum class shapeType : uint8_t
{
	NONE	= 0,
	CIRCLE	= 1 << 0,
	AABB	= 1 << 1
};

struct shape
{
	shapeType type;
	union
	{
		circle circleData;
		aabb aabbData;
	};
};

bool checkCircleCircle(glm::vec2 posA, circle circleA, glm::vec2 posB, circle circleB);
bool checkAABBAABB(glm::vec2 posA, aabb aabbA, glm::vec2 posB, aabb aabbB);
bool checkCircleAABB(glm::vec2 posA, circle circ, glm::vec2 posB, aabb ab);

void resolvePhysBodies(class physObject& lhs, class physObject& rhs, float elasticity, glm::vec2 normal, float pen);

float resolveCollision(glm::vec2 posA, glm::vec2 velA, float massA,
					  glm::vec2 posB, glm::vec2 velB, float massB,
				      float elasticity, glm::vec2 normal);
