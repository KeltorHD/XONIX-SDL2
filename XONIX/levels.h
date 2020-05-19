#ifndef LEVELS_H

#include "game.h"
#include "menu.h"

/*
меню выбора уровня, просмотр наилучшего результата в процентах и очках
true - если выход из приложения
*/
bool menu(SDL_Window* window, SDL_Renderer* renderer, Difficulty& level);

/*
Игра, принимает уровень сложности
*/
void game(SDL_Window* window, SDL_Renderer* renderer, Difficulty& level);

#endif // !LEVELS_H