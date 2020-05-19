#include "game.h"

#include <random>
#include <ctime>
#include <malloc.h>

void initMap(Map& map)
{
	map.col = (GAME_HEIGHT - RECORDS_OFFSET) / BLOCK_WIDTH;
	map.row = GAME_WIDTH / BLOCK_WIDTH;

	for (int i = 0; i < map.row; i++)
	{
		for (int j = 0; j < map.col; j++)
		{
			map.blocks[i][j].x = i;
			map.blocks[i][j].y = j;
			if (i == 0 || j == 0 || i == map.row - 1 || j == map.col - 1)
			{
				map.blocks[i][j].status = typeBlock::CAPTURED;
			}
			else
			{
				map.blocks[i][j].status = typeBlock::NONCAPTURED;
			}
		}
	}
}

void initPlayer(Player& player)
{
	player.x = 0;
	player.y = 1;
	player.dir = dirPlayerType::none;
}

void initEnemies(Enemies& enemies, Difficulty diff)
{
	srand(unsigned(time(NULL)));
	enemies.counter = diff;
	for (size_t i = 0; i < size_t(diff); i++)
	{
		enemies.mas[i].x = 4 * int(i) + 2;
		enemies.mas[i].y = 4 * int(i) + 2;

		enemies.mas[i].dir_hor = dirEnemyType(rand() % 2);
		enemies.mas[i].dir_ver = dirEnemyType(rand() % 2 + 2);
	}
}

void initRecordsBox(RecordsBox& rBox)
{
	rBox.border.y = GAME_HEIGHT - RECORDS_OFFSET;
	rBox.border.x = 0;
	rBox.border.w = GAME_WIDTH;
	rBox.border.h = RECORDS_OFFSET;

	rBox.box.x = rBox.border.x + 2;
	rBox.box.y = rBox.border.y + 2;
	rBox.box.w = rBox.border.w - 4;
	rBox.box.h = rBox.border.h - 4;

	rBox.score = 0;
	rBox.percent = 0;

	initFont(rBox.font);
	SDL_Color color = { 0, 0, 0 };
	rBox.textScore = TTF_RenderUTF8_Blended(rBox.font, "Очки: 0", color);
	rBox.textPerc = TTF_RenderUTF8_Blended(rBox.font, "Захват: 0/100", color);
}

void initWinLose(SDL_Texture*& win, SDL_Texture*& lose, SDL_Renderer* renderer)
{
	SDL_Surface* tmp = SDL_LoadBMP("resources/win.bmp");
	SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 255, 255, 255));

	win = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);

	tmp = SDL_LoadBMP("resources/lose.bmp");
	SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 255, 255, 255));

	lose = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
}

void renderMap(SDL_Renderer* renderer, Map& map)
{
	SDL_Rect rect;
	rect.w = rect.h = BLOCK_WIDTH;
	for (int i = 0; i < map.row; i++)
	{
		for (int j = 0; j < map.col; j++)
		{
			rect.x = map.blocks[i][j].x * BLOCK_WIDTH;
			rect.y = map.blocks[i][j].y * BLOCK_WIDTH;

			switch (map.blocks[i][j].status)
			{
			case typeBlock::CAPTURED:
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
				break;
			case typeBlock::NONCAPTURED:
				SDL_SetRenderDrawColor(renderer, 0, 162, 232, 0);
				break;
			case typeBlock::PROCESSED:
				SDL_SetRenderDrawColor(renderer, 4, 210, 227, 0);
				break;
			}
			SDL_RenderFillRect(renderer, &rect);
		}
	}
}

void renderPlayer(SDL_Renderer* renderer, Player& player)
{
	SDL_Rect border, p;
	border.x = player.x * BLOCK_WIDTH;
	border.y = player.y * BLOCK_WIDTH;
	border.w = border.h = BLOCK_WIDTH;
	p.x = border.x + 1;
	p.y = border.y + 1;
	p.w = p.h = BLOCK_WIDTH - 2;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderFillRect(renderer, &border);
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 0);
	SDL_RenderFillRect(renderer, &p);
}

void renderEnemies(SDL_Renderer* renderer, Enemies& enemies)
{
	for (size_t i = 0; i < size_t(enemies.counter); i++)
	{
		int circle_radius = 10;
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		for (int w = 0; w < circle_radius * 2; w++)
		{
			for (int h = 0; h < circle_radius * 2; h++)
			{
				int dx = circle_radius - w; /*горизонтальное смещение*/
				int dy = circle_radius - h; /*вертикальное смещение*/
				if ((dx * dx + dy * dy) <= (circle_radius * circle_radius))
				{
					SDL_RenderDrawPoint(renderer,
						enemies.mas[i].x * BLOCK_WIDTH + BLOCK_WIDTH / 2 + dx,
						enemies.mas[i].y * BLOCK_WIDTH + BLOCK_WIDTH / 2 + dy);
				}
			}
		}
	}
}

void renderRBox(SDL_Renderer* renderer, RecordsBox& rBox)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderFillRect(renderer, &rBox.border);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0); /*цвет подложки рекордов*/
	SDL_RenderFillRect(renderer, &rBox.box);

	SDL_Texture* tScore = SDL_CreateTextureFromSurface(renderer, rBox.textScore);
	SDL_Rect sRect = { 50, GAME_HEIGHT - RECORDS_OFFSET + 25, 100, 50 };
	SDL_RenderCopy(renderer, tScore, nullptr, &sRect);
	SDL_DestroyTexture(tScore);

	SDL_Texture* tPerc = SDL_CreateTextureFromSurface(renderer, rBox.textPerc);
	SDL_Rect pRect = { GAME_WIDTH / 2, GAME_HEIGHT - RECORDS_OFFSET + 25, 200, 50 };
	SDL_RenderCopy(renderer, tPerc, nullptr, &pRect);
	SDL_DestroyTexture(tPerc);
}

void renderGame(SDL_Renderer* renderer, Map& map, Player& player, Enemies& enemies, RecordsBox& rBox)
{
	renderMap(renderer, map);
	renderPlayer(renderer, player);
	renderEnemies(renderer, enemies);
	renderRBox(renderer, rBox);
}

void renderWinLose(SDL_Renderer* renderer, SDL_Texture* texture)
{
	SDL_Rect rect = { 0,0,GAME_WIDTH, GAME_HEIGHT };
	SDL_RenderCopy(renderer, texture, nullptr, &rect);
	SDL_RenderPresent(renderer);
	SDL_Delay(2900);
}

bool updatePlayer(Player& player, Enemies& enemies, Map& map, RecordsBox& rBox)
{
	SDL_Rect nextPos = { 0,0,0,0 };
	switch (player.dir)
	{
	case dirPlayerType::none:
		return false;
	case dirPlayerType::top:
		nextPos.x = player.x;
		nextPos.y = player.y - 1;
		break;
	case dirPlayerType::down:
		nextPos.x = player.x;
		nextPos.y = player.y + 1;
		break;
	case dirPlayerType::left:
		nextPos.y = player.y;
		nextPos.x = player.x - 1;
		break;
	case dirPlayerType::right:
		nextPos.y = player.y;
		nextPos.x = player.x + 1;
		break;
	}
	if ((nextPos.x >= 0 && nextPos.x < GAME_WIDTH / BLOCK_WIDTH)
		&& (nextPos.y >= 0 && nextPos.y < (GAME_HEIGHT - RECORDS_OFFSET) / BLOCK_WIDTH))
	{
		player.x = nextPos.x;
		player.y = nextPos.y;

		if (map.blocks[player.x][player.y].status == typeBlock::CAPTURED
			&& player.dir != dirPlayerType::none)
		{
			int count = updateMap(map, enemies);
			updateText(count, rBox);
			player.dir = dirPlayerType::none;
			return false;
		}
		else if (map.blocks[player.x][player.y].status == typeBlock::NONCAPTURED)
		{
			map.blocks[player.x][player.y].status = typeBlock::PROCESSED;
			return false;
		}
		else if (map.blocks[player.x][player.y].status == typeBlock::PROCESSED)
		{
			return true;
		}
	}
	return false;
}

void updatePlayerInput(Player& player, SDL_Event* event)
{
	if (event->type == SDL_KEYDOWN)
	{
		if (event->key.keysym.sym == SDLK_UP)
		{
			player.dir = dirPlayerType::top;
		}
		if (event->key.keysym.sym == SDLK_DOWN)
		{
			player.dir = dirPlayerType::down;
		}
		if (event->key.keysym.sym == SDLK_LEFT)
		{
			player.dir = dirPlayerType::left;
		}
		if (event->key.keysym.sym == SDLK_RIGHT)
		{
			player.dir = dirPlayerType::right;
		}
	}
}

bool updateEnemies(Enemies& enemies, Map& map, Player& player)
{
	bool isCollision = false;
	for (size_t i = 0; i < size_t(enemies.counter); i++)
	{
		SDL_Rect nextPos = { 0,0,0,0 };
		switch (enemies.mas[i].dir_hor) /*движение по горизонтали*/
		{
		case dirEnemyType::left:
			nextPos.x = enemies.mas[i].x - 1;
			break;
		case dirEnemyType::right:
			nextPos.x = enemies.mas[i].x + 1;
			break;
		}

		switch (enemies.mas[i].dir_ver) /*движение по вертикали*/
		{
		case dirEnemyType::Top:
			nextPos.y = enemies.mas[i].y - 1;
			break;
		case dirEnemyType::Down:
			nextPos.y = enemies.mas[i].y + 1;
			break;
		}

		if (enemies.mas[i].x == player.x && enemies.mas[i].y == player.y)
			isCollision = true; /*конец игры*/

		/*без столкновения*/
		if (map.blocks[nextPos.x][nextPos.y].status == typeBlock::NONCAPTURED)
		{
			enemies.mas[i].x = nextPos.x;
			enemies.mas[i].y = nextPos.y;
		}
		/*столкновение с уже захваченной территорией*/
		else if (map.blocks[nextPos.x][nextPos.y].status == typeBlock::CAPTURED)
		{
			bool flag = false;
			switch (enemies.mas[i].dir_hor) /*горизонтальная составляющая*/
			{
			case dirEnemyType::left:
				if (map.blocks[enemies.mas[i].x - 1][enemies.mas[i].y].status == typeBlock::CAPTURED)
				{
					enemies.mas[i].dir_hor = dirEnemyType::right;
					flag = true;
				}
				break;
			case dirEnemyType::right:
				if (map.blocks[enemies.mas[i].x + 1][enemies.mas[i].y].status == typeBlock::CAPTURED)
				{
					enemies.mas[i].dir_hor = dirEnemyType::left;
					flag = true;
				}
				break;
			}

			switch (enemies.mas[i].dir_ver) /*вертикальная составляющая*/
			{
			case dirEnemyType::Top:
				if (map.blocks[enemies.mas[i].x][enemies.mas[i].y - 1].status == typeBlock::CAPTURED)
				{
					enemies.mas[i].dir_ver = dirEnemyType::Down;
					flag = true;
				}
				break;
			case dirEnemyType::Down:
				if (map.blocks[enemies.mas[i].x][enemies.mas[i].y + 1].status == typeBlock::CAPTURED)
				{
					enemies.mas[i].dir_ver = dirEnemyType::Top;
					flag = true;
				}
				break;
			}

			if (!flag) /*если угол*/
			{
				if (enemies.mas[i].dir_hor == dirEnemyType::left)
					enemies.mas[i].dir_hor = dirEnemyType::right;
				else
					enemies.mas[i].dir_hor = dirEnemyType::left;

				if (enemies.mas[i].dir_ver == dirEnemyType::Top)
					enemies.mas[i].dir_ver = dirEnemyType::Down;
				else
					enemies.mas[i].dir_ver = dirEnemyType::Top;
			}
		}
		/*столкновение с территорией в процессе захвата*/
		else if (map.blocks[nextPos.x][nextPos.y].status == typeBlock::PROCESSED)
		{
			isCollision = true; /*конец игры*/
		}
	}
	return isCollision;
}

int updateMap(Map& map, Enemies& enemies)
{
	int counter = 0;

	for (size_t i = 0; i < size_t(enemies.counter); i++)
	{
		draw(map, enemies.mas[i].x, enemies.mas[i].y);
	}
	
	for (int i = 0; i < map.row; i++)
	{
		for (int j = 0; j < map.col; j++)
		{
			if (map.blocks[i][j].status == typeBlock::NONDRAW)
			{
				map.blocks[i][j].status = typeBlock::NONCAPTURED;
			}
			else
			{
				if (map.blocks[i][j].status == typeBlock::NONCAPTURED 
					|| map.blocks[i][j].status == typeBlock::PROCESSED)
					counter++;
				map.blocks[i][j].status = typeBlock::CAPTURED;
			}
		}
	}
	return counter;
}

void updateText(int countNew, RecordsBox& rBox)
{
	rBox.score += SCORE_PER_BLOCK * countNew;
	int blocks = rBox.score / SCORE_PER_BLOCK;
	rBox.percent = (blocks * 100) / ((GAME_WIDTH / BLOCK_WIDTH - 2) * ((GAME_HEIGHT - RECORDS_OFFSET) / BLOCK_WIDTH - 2));

	char* score = (char*)calloc(60, sizeof(char));
	strcat(score, "Очки: ");
	char* text = (char*)calloc(20, sizeof(char));
	_itoa(rBox.score, text, 10);
	strcat(score, text);

	SDL_FreeSurface(rBox.textScore);
	SDL_Color color = { 0, 0, 0 };
	rBox.textScore = TTF_RenderUTF8_Blended(rBox.font, score, color);

	char* perc = (char*)calloc(60, sizeof(char));
	_itoa(rBox.percent, text, 10);
	strcat(perc, "Захват: ");
	strcat(perc, text);
	strcat(perc, "/100");
	SDL_FreeSurface(rBox.textPerc);
	rBox.textPerc = TTF_RenderUTF8_Blended(rBox.font, perc, color);

	free(score);
	free(text);
	free(perc);
}

void draw(Map& map, int x, int y)
{
	if (map.blocks[x][y].status == typeBlock::NONCAPTURED) map.blocks[x][y].status = typeBlock::NONDRAW;
	if (map.blocks[x][y - 1].status == typeBlock::NONCAPTURED) draw(map, x, y - 1);
	if (map.blocks[x][y + 1].status == typeBlock::NONCAPTURED) draw(map, x, y + 1);
	if (map.blocks[x - 1][y].status == typeBlock::NONCAPTURED) draw(map, x - 1, y);
	if (map.blocks[x + 1][y].status == typeBlock::NONCAPTURED) draw(map, x + 1, y);
}

bool checkWin(RecordsBox& rBox)
{
	if (rBox.percent >= 80)
		return true;
	return false;
}

void destructorGame(Difficulty& level, RecordsBox& rBox, SDL_Texture* win, SDL_Texture* lose)
{
	TTF_CloseFont(rBox.font);
	TTF_Quit();
	SDL_FreeSurface(rBox.textPerc);
	SDL_FreeSurface(rBox.textScore);
	SDL_DestroyTexture(win);
	SDL_DestroyTexture(lose);
	int easy;
	int medium;
	int hard;

	FILE* file;
	file = fopen("resources/records.txt", "r");
	if (file == nullptr)
	{
		return;
	}
	fscanf(file, "%d", &easy);
	fscanf(file, "%d", &medium);
	fscanf(file, "%d", &hard);

	switch (level)
	{
	case Difficulty::Easy:
		easy = (rBox.score > easy ? rBox.score : easy);
		break;
	case Difficulty::Medium:
		medium = (rBox.score > medium ? rBox.score : medium);
		break;
	case Difficulty::Hard:
		hard = (rBox.score > hard ? rBox.score : hard);
		break;
	}

	fclose(file);
	file = fopen("resources/records.txt", "w");

	fprintf(file, "%d\n%d\n%d", easy, medium, hard);

	fclose(file);
}
