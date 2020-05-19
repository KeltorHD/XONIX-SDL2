#ifndef XONIX_H

#include "constants.h"

#define BLOCK_WIDTH 20    /*ширина блока*/
#define MAX_ENEMY 5       /*максимально количество противников*/
#define SCORE_PER_BLOCK 5 /*очков за один захваченный блок*/

/*тип блока*/
enum class typeBlock 
{
	CAPTURED = 0, /*захвачено*/
	PROCESSED,    /*в процессе захвата*/
	NONCAPTURED,  /*не захвачено*/
	NONDRAW       /*не закрашивать*/
};

/*структура блока*/
struct Block
{
	int x; /*положение по иксу*/
	int y; /*положение по игреку*/
	typeBlock status; /*тип клетки*/
};

/*структура карты*/
struct Map
{
	int col; /*количество столбцов*/
	int row; /*количество строк*/
	Block blocks[GAME_WIDTH / BLOCK_WIDTH][(GAME_HEIGHT - RECORDS_OFFSET) / BLOCK_WIDTH]; /*массив блоков*/
};

/*направления движения для игрока*/
enum class dirPlayerType
{
	left,
	right,
	top,
	down,
	none
};

/*структура игрока*/
struct Player
{
	int x; /*положение по иксу*/
	int y; /*положение по игреку*/
	dirPlayerType dir; /*направление движения*/
};

/*направления движения*/
enum class dirEnemyType
{
	left = 0, /*лево*/
	right,    /*право*/
	Top,      /*вверх*/
	Down      /*вниз*/
};

/*перечисление разных уровней*/
enum class Difficulty
{
	Easy = 1,   /*легко*/
	Medium = 3, /*средне*/
	Hard = 5    /*сложно*/
};

/*структура противника*/
struct Enemy
{
	int x; /*положение по иксу*/
	int y; /*положение по игреку*/
	dirEnemyType dir_hor; /*движение по горионтали*/
	dirEnemyType dir_ver; /*движение по вертикали*/
};

/*структура набора противников*/
struct Enemies
{
	Enemy mas[MAX_ENEMY]; /*массив противников*/
	Difficulty counter;   /*количество противников*/
};

/*структура полоски текущего рекорда*/
struct RecordsBox
{
	SDL_Rect border;
	SDL_Rect box;
	int score; /*очки*/
	int percent;     /*процент захвата карты*/
	SDL_Surface* textPerc;
	SDL_Surface* textScore;
	TTF_Font* font;
};

/*инициализация карты*/
void initMap(Map& map);

/*инициализация игрока*/
void initPlayer(Player& player);

/*инициализация противников*/
void initEnemies(Enemies& enemies, Difficulty diff);

/*иниализация рекордов*/
void initRecordsBox(RecordsBox& rBox);

/*инициализация экрана выигрыша, проигрыша*/
void initWinLose(SDL_Texture*& win, SDL_Texture*& lose, SDL_Renderer* renderer);

/*отрисовка карты*/
void renderMap(SDL_Renderer* renderer, Map& map);

/*отрисовка игрока*/
void renderPlayer(SDL_Renderer* renderer, Player& player);

/*отрисовка противников*/
void renderEnemies(SDL_Renderer* renderer, Enemies& enemies);

/*отрисовка рекорда*/
void renderRBox(SDL_Renderer* renderer, RecordsBox& rBox);

/*отрисовка всего*/
void renderGame(SDL_Renderer* renderer, Map& map, Player& player, Enemies& enemies, RecordsBox& rBox);

/*отрисовка экрана выигрыша и проигрыша*/
void renderWinLose(SDL_Renderer* renderer, SDL_Texture* texture);

/*обновление игрока, 1 - столкновение*/
bool updatePlayer(Player& player, Enemies& enemies, Map& map, RecordsBox& rBox);

/*ввод для игрока*/
void updatePlayerInput(Player& player, SDL_Event* event);

/*обновление шаров. True - если произошло столкновение*/
bool updateEnemies(Enemies& enemies, Map& map, Player& player);

/*
обновление карты, закрашивается захваченная территория, 
возвращает количество захваченных блоков
*/
int updateMap(Map& map, Enemies& enemies);

/*обновление текста*/
void updateText(int countNew, RecordsBox& rBox);

/*закрашивание территории*/
void draw(Map& map, int x, int y);

/*проверка выигрыша, true - если выигрыш*/
bool checkWin(RecordsBox& rBox);

/*очищение памяти*/
void destructorGame(Difficulty& level, RecordsBox& rBox, SDL_Texture* win, SDL_Texture* lose);

#endif // !XONIX_H