#pragma once

#include <vector>
#include <unordered_map>

#include "physics.h"

using collisionPair = uint8_t;
using collisionFunc = bool(*)(const glm::vec2&, const shape&, const glm::vec2&, const shape&);
using collisionMap = std::unordered_map<collisionPair, collisionFunc>;

using depenetrationFunc = glm::vec2(*)(const glm::vec2&, const shape&, const glm::vec2&, const shape&, float& pen);
using depenetrationMap = std::unordered_map < collisionPair, depenetrationFunc> ;

class baseGame
{
protected:
	float accumulatedFixedTime;

	std::vector<physObject> physObjects;
	static collisionMap collisionCheckers;
	static depenetrationMap collisionDepenetrators;

	void virtual onInit() { }
	void virtual onTick() { }
	void virtual onTickPhys() { }
	void virtual onDraw() const { }
	void virtual onExit() { }
public:
	baseGame();

	void init();
	void tick();
	void tickFixed();
	void draw() const;
	void exit();

	bool shouldClose() const;
	bool shouldTickFixed() const;

	float targetFixedStep;
	float gravityScale;

	bool useGravity;
	bool useWrapping;
};