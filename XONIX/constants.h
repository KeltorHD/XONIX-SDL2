#ifndef CONSTANTS_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#define GAME_WIDTH 800  /*ширина экрана*/
#define GAME_HEIGHT 700 /*высота экрана*/
#define RECORDS_OFFSET 100 /*смещение под рекорды*/

/*инициализация шрифта*/
void initFont(TTF_Font*& font);

#endif // !CONSTANTS_H