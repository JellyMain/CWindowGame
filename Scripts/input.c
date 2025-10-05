#include <SDL_events.h>
#include "Headers/input.h"

#include "stateMachine.h"
#include "Headers/app.h"
#include "Headers/structs.h"

static bool isLeftMouseButtonClicked = false;

Vector2Int GetMoveDirection()
{
	Vector2Int input = {0, 0};

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


void ProcessInput()
{
	isLeftMouseButtonClicked = false;

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
			{
				exit(0);
				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					isLeftMouseButtonClicked = true;
				}
				break;
			}


			default:
				break;
		}
	}
}


bool IsLeftMouseButtonClicked()
{
	return isLeftMouseButtonClicked;
}


Vector2Int GetMousePosition()
{
	int xPosition;
	int yPosition;

	SDL_GetMouseState(&xPosition, &yPosition);

	return (Vector2Int){xPosition, yPosition};
}
