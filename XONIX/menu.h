#ifndef MENU_H

#include "constants.h"

#define MAX_TEXT_LENGTH 64
#define BUTTON_COUNT 4
#define BUTTON_WIDTH 400
#define BUTTON_HEIGTH 60

/*Струтура кнопки*/
struct Button
{
	SDL_Rect btnBorder;       /*сама кнопка*/
	SDL_Texture* btnBack;     /*картинка кнопки*/
	SDL_Surface* text;        /*экран с текстом рекорда*/
};

/*Структура всех кнопок главного меню*/
struct MenuButton
{
	Button btns[BUTTON_COUNT];
};

/*инициализация background*/
void initBackground(SDL_Texture*& background, SDL_Renderer* renderer);

/*инициализация кнопок*/
void initButtons(MenuButton& menuButton, SDL_Renderer* renderer, TTF_Font* font);

/*отрисовка заднего фона*/
void renderBackground(SDL_Renderer* renderer, SDL_Texture* background);

/*отрисовка кнопок*/
void renderButtons(SDL_Renderer* renderer, MenuButton& menuButton);

/*общая отрисовка меню*/
void renderMenu(SDL_Renderer* renderer, SDL_Texture* background, MenuButton& menuButton);

/*обновление кнопок
1 - легкий уровень
2 - нормальный уровень
3 - сложный уровень
0 - выход
4 - ничего
*/
int updateMenu(SDL_Event* event, MenuButton& menuButton);

/*удаление выделенной памяти*/
void destructorMenu(SDL_Texture* background, TTF_Font* font, MenuButton& menuButton);

#endif // !MENU_H