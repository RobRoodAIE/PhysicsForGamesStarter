#include "baseGame.h"

#include "raylib.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"

#include <iostream>
#include <limits>
#include <cassert>

#include "enumUtils.h"
#include "mathUtils.h"

bool checkCircleCircle(const glm::vec2& posA, const shape& shapeA, const glm::vec2& posB, const shape& shapeB);
bool checkAABBAABB(const glm::vec2& posA, const shape& shapeA, const glm::vec2& posB, const shape& shapeB);
bool checkCircleAABB(const glm::vec2& posA, const shape& shapeA, const glm::vec2& posB, const shape& shapeB);

glm::vec2 depenetrateCircleCircle(const glm::vec2& posA, const shape& shapeA, const glm::vec2& posB, const shape& shapeB, float& pen);
glm::vec2 depenetrateAABBAABB(const glm::vec2& posA, const shape& shapeA, const glm::vec2& posB, const shape& shapeB, float& pen);
glm::vec2 depenetrateCircleAABB(const glm::vec2& posA, const shape& shapeA, const glm::vec2& posB, const shape& shapeB, float& pen);

collisionMap setupCollisionChecks()
{
	collisionMap map;
	
	map[static_cast<uint8_t>(shapeType::CIRCLE | shapeType::CIRCLE)] = checkCircleCircle;
	map[static_cast<uint8_t>(shapeType::AABB | shapeType::AABB)] = checkAABBAABB;
	map[static_cast<uint8_t>(shapeType::CIRCLE | shapeType::AABB)] = checkCircleAABB;

	return map;
}

depenetrationMap setupDepenetrator()
{
	depenetrationMap map;

	map[static_cast<uint8_t>(shapeType::CIRCLE | shapeType::CIRCLE)] = depenetrateCircleCircle;
	map[static_cast<uint8_t>(shapeType::AABB | shapeType::AABB)] = depenetrateAABBAABB;
	map[static_cast<uint8_t>(shapeType::CIRCLE | shapeType::AABB)] = depenetrateCircleAABB;

	return map;
}

collisionMap baseGame::collisionCheckers = setupCollisionChecks();
depenetrationMap baseGame::collisionDepenetrators = setupDepenetrator();

bool checkCircleCircle(const glm::vec2& posA, const shape& shapeA, const glm::vec2& posB, const shape& shapeB)
{
	return checkCircleCircle(posA, shapeA.circleData, posB, shapeB.circleData);
}

bool checkAABBAABB(const glm::vec2& posA, const shape& shapeA, const glm::vec2& posB, const shape& shapeB)
{
	return checkAABBAABB(posA, shapeA.aabbData, posB, shapeB.aabbData);
}

bool checkCircleAABB(const glm::vec2& posA, const shape& shapeA, const glm::vec2& posB, const shape& shapeB)
{
	return checkCircleAABB(posA, shapeA.circleData, posB, shapeB.aabbData);
}

// Returns mtv direction
// Out penetration depth stored in 'pen'
glm::vec2 depenetrateCircleCircle(const glm::vec2& posA, const shape& shapeA, const glm::vec2& posB, const shape& shapeB, float &pen)
{
	float dist = glm::length(posA - posB);
	float sum = shapeA.circleData.radius + shapeB.circleData.radius;

	pen = sum - dist;

	return glm::normalize(posA - posB);
}

glm::vec2 depenetrateAABBAABB(const glm::vec2& posA, const shape& shapeA, const glm::vec2& posB, const shape& shapeB, float& pen)
{
	float mtvDist = 0.0f;
	glm::vec2 mtvAxis = {};

	glm::vec2 rawOffset = posB - posA;
	glm::vec2 offset = glm::abs(rawOffset);

	float xPen = shapeA.aabbData.halfExtents.x + shapeB.aabbData.halfExtents.x - offset.x;
	if (xPen < 0.0f) { pen = 0.0f; return {}; }
	float yPen = shapeA.aabbData.halfExtents.y + shapeB.aabbData.halfExtents.y - offset.y;
	if (yPen < 0.0f) { pen = 0.0f; return {}; }

	if (xPen < yPen)
	{
		pen = xPen;
		mtvAxis.x = copysign(1.0f, -rawOffset.x);
	}
	else
	{
		pen = yPen;
		mtvAxis.y = copysign(1.0f, -rawOffset.y);
	}

	return mtvAxis;
}

glm::vec2 depenetrateCircleAABB(const glm::vec2& posA, const shape& shapeA, const glm::vec2& posB, const shape& shapeB, float& pen)
{
	glm::vec2 circConstrained = glm::clamp(posA, posB - shapeB.aabbData.halfExtents, posB + shapeB.aabbData.halfExtents);
	glm::vec2 circOffset = circConstrained - posA;
	float distSq = glm::dot(circOffset, circOffset);
	float radiusSq = shapeA.circleData.radius * shapeA.circleData.radius;

	if (distSq < radiusSq)
	{
		// center of circle is not inside of aabb
		if (distSq != 0.0f)
		{
			float dist = sqrt(distSq);
			pen = shapeA.circleData.radius - dist;
			return glm::normalize(-circOffset);
		}
		// center of circle is inside of aabb
		else
		{
			glm::vec2 offset = posA - posB;
			glm::vec2 offsetAbs = glm::abs(offset);

			glm::vec2 overlap = shapeB.aabbData.halfExtents - offsetAbs;

			glm::vec2 mtv = {};

			if (overlap.x < overlap.y)
			{
				pen = overlap.x;
				mtv.x = copysign(1.0f, offset.x);
			}
			else
			{
				pen = overlap.y;
				mtv.y = copysign(1.0f, offset.y);
			}

			pen += shapeA.circleData.radius;
			return glm::normalize(mtv);	
		}
	}
	else
	{
		// not penetrating
		pen = 0.0f;
		return {};
	}
}

baseGame::baseGame()
{
	targetFixedStep = 1.0f / 30.0f;
	accumulatedFixedTime = 0.0f;
	gravityScale = 1.0f;
	useGravity = true;
	useWrapping = false;
}

void baseGame::init()
{
	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 800;
	int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	SetTargetFPS(60);
	//--------------------------------------------------------------------------------------

	onInit();
}

void baseGame::tick()
{
	// do update
	accumulatedFixedTime += GetFrameTime();

	onTick();
}

void baseGame::tickFixed()
{
	// do physics
	accumulatedFixedTime -= targetFixedStep;

	float fWidth = (float)GetScreenWidth();
	float fHeight = (float)GetScreenHeight();

	//std::cout << "PHYS TICK" << std::endl;
	for (auto& i : physObjects)
	{
		if (useGravity && i.useGravity)
		{
			i.addAccel(glm::vec2{ 0, 9.81f } * gravityScale);
		}
		i.tickPhys(targetFixedStep);

		// position wrap
		if (useWrapping)
		{
			i.pos.x = wrap(i.pos.x, fWidth);
			i.pos.y = wrap(i.pos.y, fHeight);
		}
	}
	
	for (size_t i = 0; i < 5; ++i)
	{
		bool isClean = true;
		for (auto& i : physObjects)
		{
			for (auto& j : physObjects)
			{
				// skip self collision
				if (&i == &j) { continue; }
				if (i.collider.type == shapeType::NONE || j.collider.type == shapeType::NONE) { continue; }
				if (i.isStatic) { continue; }

				// collider type w/ lower number should always be left
				auto* lhs = &i;
				auto* rhs = &j;

				// swap'em around if necessary
				if (static_cast<uint8_t>(i.collider.type) > static_cast<uint8_t>(j.collider.type))
				{
					lhs = &j;
					rhs = &i;
				}

				bool collision = false;
				auto pairType = static_cast<collisionPair>(lhs->collider.type | rhs->collider.type);
				collision = collisionCheckers[pairType](lhs->pos, lhs->collider, rhs->pos, rhs->collider);

				if (collision)
				{
					isClean = false;

					float pen = 0.0f;
					glm::vec2 normal = collisionDepenetrators[pairType](lhs->pos, lhs->collider, rhs->pos, rhs->collider, pen);
					// edge-case: what if two objects are on top of each other?
					//			  we'll get a zero vector which will result in
					//			  degenerate floating point values
					if (lhs->pos == rhs->pos) { normal = { 0,1 }; }
#if _DEBUG
					float debugLength = glm::length(normal);
					assert((float)abs(debugLength - 1.0f) < FLT_EPSILON * 100);
#endif
					resolvePhysBodies(*lhs, *rhs, 1.0f, normal, pen);
				}
			}
		}

		if (isClean) { break; }
	}

	onTickPhys();
}

void baseGame::draw() const
{
	// Draw
	//----------------------------------------------------------------------------------
	BeginDrawing();

	ClearBackground(RAYWHITE);

	for (const auto& i : physObjects)
	{
		i.draw();
	}

	onDraw();

	EndDrawing();
	//----------------------------------------------------------------------------------
}

void baseGame::exit()
{
	onExit();

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

}

bool baseGame::shouldClose() const
{
	return WindowShouldClose();
}

bool baseGame::shouldTickFixed() const
{
	return accumulatedFixedTime >= targetFixedStep;
}
