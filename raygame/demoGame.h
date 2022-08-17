#pragma once

#include "baseGame.h"	// inheriting from baseGame

class demoGame : public baseGame
{
public:
	void onTick() override;
	void onDraw() const override;
};