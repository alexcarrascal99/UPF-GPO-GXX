#include "button.h"

bool Button::IsMouseInside(Vector2 mousePosition)
{
    return
        mousePosition.x >= position.x &&
        mousePosition.x < position.x + image.width &&
        mousePosition.y >= position.y &&
        mousePosition.y < position.y + image.height;
}

void Button::Init(const char* imagePath, Vector2 pos, ButtonType btnType)
{
	image.LoadPNG(imagePath);
	position = pos;
	type = btnType;
}