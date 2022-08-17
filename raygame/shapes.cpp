#include "shapes.h"

#include "glm/glm.hpp"

#include "physics.h"

bool checkCircleCircle(glm::vec2 posA, circle circleA, glm::vec2 posB, circle circleB)
{
	// get the distance
	glm::vec2 offset = (posA - posB);
	float distSq = glm::dot(offset, offset);
	// get the sum of the radii
	float sum = circleA.radius + circleB.radius;

	return distSq < (sum * sum);
}

bool checkAABBAABB(glm::vec2 posA, aabb aabbA, glm::vec2 posB, aabb aabbB)
{
	return posA.x - aabbA.halfExtents.x < posB.x + aabbB.halfExtents.x &&  // l within r
		   posA.x + aabbA.halfExtents.x > posB.x - aabbB.halfExtents.x &&  // r within l
		   posA.y - aabbA.halfExtents.y < posB.y + aabbB.halfExtents.y &&  // t within b
		   posA.y + aabbA.halfExtents.y > posB.y - aabbB.halfExtents.y;    // b within t
}

bool checkCircleAABB(glm::vec2 posA, circle circ, glm::vec2 posB, aabb ab)
{
	// find the nearest point in the AABB in the direction of the circle
	//
	// to do this, we can clamp the center of the circle to the bounds of the AABB
	float distX = posA.x - glm::clamp(posA.x, posB.x - ab.halfExtents.x, posB.x + ab.halfExtents.x);
	float distY = posA.y - glm::clamp(posA.y, posB.y - ab.halfExtents.y, posB.y + ab.halfExtents.y);

	return (distX * distX + distY * distY) < (circ.radius * circ.radius);
}

void resolvePhysBodies(physObject& lhs, physObject& rhs, float elasticity, glm::vec2 normal, float pen)
{
	// depenetrate (aka separate) the two objects
	if (!lhs.isStatic)
	{
		lhs.pos += normal * pen;
	}
	if (!rhs.isStatic)
	{
		rhs.pos -= normal * pen;
	}

#if _DEBUG
	float initialMomentum = lhs.getMomentum() + rhs.getMomentum();
#endif

	// calculate resolution impulse
	//   normal and pen are passed by reference and will be updated
	float impulseMag = resolveCollision(lhs.pos, lhs.vel, lhs.mass, rhs.pos, rhs.vel, rhs.mass, elasticity, normal);
	if(lhs.isStatic || rhs.isStatic) { impulseMag *= 2.0f; }
	glm::vec2 impulse = impulseMag * normal;

	if (!(lhs.isStatic || rhs.isStatic))
	{
		pen *= .51f;
	}

	// depenetrate (aka separate) the two objects
	if (!lhs.isStatic)
	{
		lhs.addImpulse(impulse);
	}
	if (!rhs.isStatic)
	{
		rhs.addImpulse(-impulse); // remember: this gets an equal but opposite force
	}

#if _DEBUG
	float finalMomentum = lhs.getMomentum() + rhs.getMomentum();
	//assert(abs(initialMomentum - finalMomentum) < FLT_EPSILON * 100 && "Momentum changed as a result of the collision");
#endif
}

float resolveCollision(glm::vec2 posA, glm::vec2 velA, float massA,
	                  glm::vec2 posB, glm::vec2 velB, float massB,
	                  float elasticity, glm::vec2 normal)
{
	// calculate the relative velocity
	glm::vec2 relVel = velA - velB;

	// calculate the impulse magnitude
	float impulseMag = glm::dot(-(1.0f + elasticity) * relVel, normal) /
		glm::dot(normal, normal * (1 / massA + 1 / massB));

	// return impulse to apply to both objects
	return impulseMag;
}
