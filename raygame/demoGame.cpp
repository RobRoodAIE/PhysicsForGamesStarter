#include "demoGame.h"

#include "raylib.h"

#include <iostream>
#include <limits>

void demoGame::onTick()
{
	bool mb0 = IsMouseButtonPressed(0);
	bool mb1 = IsMouseButtonPressed(1);
	bool mb2 = IsMouseButtonPressed(2);

	if (mb0 || mb1 || mb2)
	{
		physObjects.emplace_back();
		std::cout << "Added a physics object!" << std::endl;

		auto& babyPhys = physObjects[physObjects.size() - 1];
		auto mousePos = GetMousePosition();
		babyPhys.pos = { mousePos.x, mousePos.y };

		if (mb0)
		{
			shape circS;
			circS.type = shapeType::CIRCLE;
			circS.circleData = circle{ 25.0f };
			babyPhys.collider = circS;
		}
		else if (mb1)
		{
			shape aabbS;
			aabbS.type = shapeType::AABB;
			aabbS.aabbData = aabb{ glm::vec2(25.0f, 25.0f) / 2.0f } ;
			babyPhys.collider = aabbS;
			babyPhys.isStatic = false;
		}
		else if (mb2)
		{
			shape aabbS;
			aabbS.type = shapeType::AABB;
			aabbS.aabbData = aabb{ glm::vec2(30.0f, 30.0f)};
			babyPhys.collider = aabbS;
			babyPhys.isStatic = true;
		}

		//babyPhys.addImpulse({ 0, 50 });
	}
}

void demoGame::onDraw() const
{
	DrawText("Left-click to add a dynamic ball to the scene.", 5, 5, 20, BLACK);
	DrawText("Right-click to add a static box to the scene.", 5, 25, 20, BLACK);
}
