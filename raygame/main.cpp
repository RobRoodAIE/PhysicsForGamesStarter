#define POINT_RENDER_SIZE 5
#include "demoGame.h"

int main()
{
	baseGame * app = new demoGame();
	app->init();
	app->targetFixedStep = 1.0f / 30.0f; // target physics tick rate
	app->gravityScale = 50.f;
	app->useGravity = true;
	app->useWrapping = true;

	while (!app->shouldClose())
	{
		app->tick();

		while (app->shouldTickFixed())
		{
			app->tickFixed();
		}

		app->draw();
	}

	app->exit();

	delete app;

	return 0;
}