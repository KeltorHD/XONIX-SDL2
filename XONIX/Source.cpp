#include "levels.h"

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("Xonix", 100, 100, GAME_WIDTH, GAME_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	bool quit = false;
	Difficulty level;

	while (!quit)
	{
		quit = menu(window, renderer, level);
		if (!quit)
			game(window, renderer, level);
	}

	/*очищение памяти, выход из SDL*/
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit(); /* выход из SDL */

	return 1;
}