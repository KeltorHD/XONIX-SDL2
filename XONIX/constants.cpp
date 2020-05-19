#include "constants.h"

void initFont(TTF_Font*& font)
{
	if (TTF_Init() == -1) /*инициализируем библиотеку для шрифтов*/
	{
		throw "Шрифты не загружены";
	}
	font = TTF_OpenFont("resources/font.ttf", 50);
}
