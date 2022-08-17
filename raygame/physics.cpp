#include "physics.h"

#include "raylib.h"

#include "glm/glm.hpp"

physObject::physObject()
{
	pos = { 0,0 };
	vel = { 0,0 };
	forces = { 0,0 };
	collider = shape{ shapeType::NONE };

	mass = 1.0f;
	drag = 1.0f;
	isStatic = false;
	useGravity = true;
}

void physObject::tickPhys(float delta)
{
	if (isStatic) { return; }

	// integrating forces into velocity
	vel += forces * delta;
	forces = { 0, 0 };

	// integrate linear drag into velocity
	vel *= 1.0f - delta * drag;

	// integrating velocity into position
	pos += vel * delta;
}

void physObject::draw() const
{
	switch (collider.type)
	{
	case shapeType::NONE:
		DrawPixel(pos.x, pos.y, RED);
		break;
	case shapeType::CIRCLE:
		DrawCircleLines(pos.x, pos.y, collider.circleData.radius, RED);
		break;
	case shapeType::AABB:
		DrawRectangleLines(pos.x - collider.aabbData.halfExtents.x, pos.y - collider.aabbData.halfExtents.y,
						   collider.aabbData.halfExtents.x * 2.0f, collider.aabbData.halfExtents.y * 2.0f, RED);
		break;
	default:
		break;
	}

	//printf_s("x%f,y:%f")
}

void physObject::addForce(glm::vec2 force)
{
	forces += force * (1.0f / mass);
}

void physObject::addAccel(glm::vec2 accel)
{
	forces += accel;
}

void physObject::addImpulse(glm::vec2 impulse)
{
	vel += impulse * (1.0f / mass);
}

void physObject::addVelocityChange(glm::vec2 delta)
{
	vel += delta;
}

float physObject::getMomentum() const
{
	return glm::length(vel * mass);
}
