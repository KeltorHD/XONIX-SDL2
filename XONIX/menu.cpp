#include "menu.h"

void initBackground(SDL_Texture*& background, SDL_Renderer* renderer)
{
	SDL_Surface* bmp = SDL_LoadBMP("resources/background.bmp");

	if (bmp == nullptr)
	{
		throw "Нет картинки background.bmp";
	}

	background = SDL_CreateTextureFromSurface(renderer, bmp);
	SDL_FreeSurface(bmp);
}

void initButtons(MenuButton& menuButton, SDL_Renderer* renderer, TTF_Font* font)
{
	char name[4][20] = {
		"resources/btn1.bmp",
		"resources/btn2.bmp",
		"resources/btn3.bmp",
		"resources/btn4.bmp",
	};

	SDL_Color color = { 0, 0, 0 };

	FILE* file;
	file = fopen("resources/records.txt", "r");
	if (file == nullptr)
	{
		throw "Не удается открыть файл с рекордами!";
	}

	for (size_t i = 0; i < BUTTON_COUNT; i++)
	{
		menuButton.btns[i].btnBorder.w = BUTTON_WIDTH;
		menuButton.btns[i].btnBorder.h = BUTTON_HEIGTH;
		menuButton.btns[i].btnBorder.x = GAME_WIDTH / 2 - BUTTON_WIDTH / 4 * 3;
		menuButton.btns[i].btnBorder.y = GAME_HEIGHT / 4 * 1 + int(i) * 100;

		SDL_Surface* myImage = SDL_LoadBMP(name[i]);
		if (myImage == nullptr)
		{
			throw name[i];
		}

		menuButton.btns[i].btnBack = SDL_CreateTextureFromSurface(renderer, myImage);
		SDL_FreeSurface(myImage);

		if (i != BUTTON_COUNT - 1)
		{
			char* text = (char*)calloc(60, sizeof(char));
			strcat(text, "Рекорд: ");
			char* tmp = (char*)calloc(5, sizeof(char));
			int input;
			fscanf(file, "%d", &input);
			_itoa(input, tmp, 10);
			strcat(text, tmp);

			menuButton.btns[i].text = TTF_RenderUTF8_Blended(font, text, color);

			free(text);
			free(tmp);
		}
	}
	fclose(file);
}

void renderBackground(SDL_Renderer* renderer, SDL_Texture* background)
{
	SDL_RenderCopy(renderer, background, NULL, NULL); /*отрисовка background*/
}

void renderButtons(SDL_Renderer* renderer, MenuButton& menuButton)
{
	for (size_t i = 0; i < BUTTON_COUNT; i++)
	{
		SDL_RenderCopy(renderer, menuButton.btns[i].btnBack, NULL, &menuButton.btns[i].btnBorder);

		if (i != BUTTON_COUNT - 1)
		{
			SDL_Texture* tPerc = SDL_CreateTextureFromSurface(renderer, menuButton.btns[i].text);
			SDL_Rect pRect = { 
				menuButton.btns[i].btnBorder.x + menuButton.btns[i].btnBorder.w + 25,
				menuButton.btns[i].btnBorder.y + 10,
				200, 50 };
			SDL_RenderCopy(renderer, tPerc, nullptr, &pRect);
			SDL_DestroyTexture(tPerc);
		}
	}
}

void renderMenu(SDL_Renderer* renderer, SDL_Texture* background, MenuButton& menuButton)
{
	renderBackground(renderer, background);
	renderButtons(renderer, menuButton);
}

int updateMenu(SDL_Event* event, MenuButton& menuButton)
{
	if ((event->type == SDL_MOUSEBUTTONDOWN) && (event->button.button == SDL_BUTTON_LEFT))
	{
		for (size_t i = 0; i < BUTTON_COUNT; i++)
		{
			if (((menuButton.btns[i].btnBorder.x <= event->button.x) && (event->button.x <= menuButton.btns[i].btnBorder.x + menuButton.btns[i].btnBorder.w))
				&& ((menuButton.btns[i].btnBorder.y <= event->button.y) && (event->button.y <= menuButton.btns[i].btnBorder.y + menuButton.btns[i].btnBorder.h)))
			{
				if (i + 1 == 4)
				{
					return 0;
				}
				else
				{
					return int(i) + 1;
				}
					
			}
		}
	}
	return 4;
}

void destructorMenu(SDL_Texture* background, TTF_Font* font, MenuButton& menuButton)
{
	SDL_DestroyTexture(background);
	TTF_CloseFont(font);
	TTF_Quit();
	for (size_t i = 0; i < BUTTON_COUNT; i++)
	{
		SDL_DestroyTexture(menuButton.btns[i].btnBack);

		if (i != BUTTON_COUNT - 1)
		{
			SDL_FreeSurface(menuButton.btns[i].text);
		}
	}
}
