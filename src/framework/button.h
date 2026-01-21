#pragma once
#include "image.h"


enum ButtonType
{
    BTN_PENCIL,
    BTN_ERASER,
    BTN_LINE,
    BTN_RECT,
    BTN_TRIANGLE,
    BTN_CLEAR,
    BTN_LOAD,
    BTN_SAVE,
    BTN_COLOR_BLACK,
    BTN_COLOR_WHITE,
    BTN_COLOR_RED,
    BTN_COLOR_GREEN,
    BTN_COLOR_BLUE,
    BTN_COLOR_YELLOW,
    BTN_COLOR_CYAN,
    BTN_COLOR_PINK
};

class Button
{
public:
    Image image;
    Vector2 position;
    ButtonType type;

    bool IsMouseInside(Vector2 mousePosition);
	void Init(const char* imagePath, Vector2 pos, ButtonType btnType);
};
