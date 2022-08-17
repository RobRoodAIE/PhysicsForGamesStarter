#pragma once

#include <glm/vec2.hpp>

inline float wrap(float value, float limit)
{
	return fmodf(fmodf(value, limit) + limit, limit);
}

inline int wrap(int value, int limit)
{
	return (value % limit) + limit % limit;
}

inline bool checkSAT(glm::vec2 axis, float minA, float maxA, float minB, float maxB, glm::vec2& mtvAxis, float mtvDist)
{
	float axisLen = glm::length(axis);
	float axisLenSq = axisLen * axisLen;

	float dist1 = maxB - minA;
	float dist2 = maxA - minB;

	if (dist1 <= 0.0f || dist2 <= 0.0f) { return false; }

	float overlap = dist1 < dist2 ? dist1 : -dist2;
	glm::vec2 sep = axis * (overlap / axisLenSq);

	float sepLen = glm::length(sep);
	float sepLenSq = sepLen * sepLen;

	if (sepLenSq < mtvDist)
	{
		mtvDist = sepLenSq;
		mtvAxis = sep;
	}

	return true;
}