#include <SDL_events.h>
#include "Headers/input.h"

#include "Headers/app.h"
#include "Headers/structs.h"

Vector2 GetMoveDirection()
{
	Vector2 input = {0, 0};

	const Uint8 *keyState = SDL_GetKeyboardState(NULL);

	if (keyState[SDL_SCANCODE_W])
	{
		input.y = -1;
	}
	if (keyState[SDL_SCANCODE_S])
	{
		input.y = 1;
	}
	if (keyState[SDL_SCANCODE_A])
	{
		input.x = -1;
	}
	if (keyState[SDL_SCANCODE_D])
	{
		input.x = 1;
	}

	return input;
}


void ProcessInput(App *app)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				exit(0);
				break;

			default:
				break;
		}
	}
}
