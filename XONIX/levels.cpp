#include "levels.h"

bool menu(SDL_Window* window, SDL_Renderer* renderer, Difficulty& level)
{
	bool quit = false; /*выход из меню в игру*/

	/*Переменные, необходимые для меню*/
	SDL_Texture* background = nullptr;
	TTF_Font* font = nullptr;
	MenuButton menuButton;
	initBackground(background, renderer); /*инициализация background*/
	initFont(font); /*инициализация шрифта*/
	initButtons(menuButton, renderer, font); /*инициализация кнопок*/

	SDL_Event event;

	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				destructorMenu(background, font, menuButton);
				return true;
			}

			switch (updateMenu(&event, menuButton)) /*обновление кнопок*/
			{
			case 4:
				break;
			case 1:
				level = Difficulty::Easy;
				quit = true;
				break;
			case 2:
				level = Difficulty::Medium;
				quit = true;
				break;
			case 3:
				level = Difficulty::Hard;
				quit = true;
				break;
			case 0: /*выход из приложения*/
				quit = true;
				destructorMenu(background, font, menuButton);
				return true;
			}
				
		}

		SDL_RenderClear(renderer);
		
		renderMenu(renderer, background, menuButton);

		SDL_RenderPresent(renderer);
	}

	destructorMenu(background, font, menuButton);
	return false;
}

void game(SDL_Window* window, SDL_Renderer* renderer, Difficulty& level)
{
	bool quit = false; /*выход из игры в меню*/

	SDL_Event event;

	/*переменные, необходимые для игры*/
	Map map;
	Player player;
	Enemies enemies;
	RecordsBox rBox;
	SDL_Texture* win;
	SDL_Texture* lose;

	/*инициализации*/
	initMap(map);
	initPlayer(player);
	initEnemies(enemies, level);
	initRecordsBox(rBox);
	initWinLose(win, lose, renderer);

	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				quit = true;
			updatePlayerInput(player, &event);
		}

		if (updatePlayer(player, enemies, map, rBox))
		{
			quit = true;
			renderWinLose(renderer, lose);
		}
		if (updateEnemies(enemies, map, player))
		{
			quit = true;
			renderWinLose(renderer, lose);
		}

		if (!quit)
		{
			SDL_RenderClear(renderer);

			renderGame(renderer, map, player, enemies, rBox);

			SDL_RenderPresent(renderer);
		}

		if (checkWin(rBox) && !quit) /*выигрыш*/
		{
			quit = true;
			renderWinLose(renderer, win);
		}

		SDL_Delay(100);
	}
	destructorGame(level, rBox, win, lose);
}
