#include "AppWindow.h"
#include <cstdio>

void process_input()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
		case SDL_EVENT_QUIT:
			APP_IS_RUNNING = 0;
			break;
		case SDL_EVENT_KEY_DOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				APP_IS_RUNNING = 0;
				break;
			}
		default:
			break;
	}
}

void update()
{

}

void render(SDL_Renderer* renderer) 
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_FRect ball_rect = {20, 20, 15, 15};
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &ball_rect);

	SDL_RenderPresent(renderer);
}

void 
destroy_window(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void
app()
{
	SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

	int WIN_WIDTH = 500, WIN_HEIGHT = 500;

    APP_IS_RUNNING = SDL_CreateWindowAndRenderer(WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_BORDERLESS, &window, &renderer) == 0;
    
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // SDL_RenderClear(renderer);

    // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // SDL_RenderPoint(renderer, WIN_WIDTH / 2, WIN_HEIGHT / 2);

    // SDL_RenderPresent(renderer);
    // SDL_Delay(10000);

	// setup();

	while (APP_IS_RUNNING)
	{
		process_input();
		update();
		render(renderer);
	}

	destroy_window(window, renderer);

}