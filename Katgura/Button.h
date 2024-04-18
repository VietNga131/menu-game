#ifndef BUTTON_H_
#define BUTTON_H_

#include "CommonFunc.h"
#include "BaseObject.h"

class Button
{
public:
	ButtonSprite currentSprite;

	Button();

	Button(int x, int y);

	void SetPosition(int x, int y);

	bool IsInside(SDL_Event *e, int size);

	void Render(SDL_Rect* currentClip, SDL_Renderer* gRenderer, BaseObject gButtonTexture);

private:
	SDL_Point position;
};

#endif // !BUTTON_H_
