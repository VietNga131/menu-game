#pragma once

#include "CommonFunc.h"

class BaseObject
{
public:
    BaseObject();
    ~BaseObject();


    virtual bool LoadImg(std::string path, SDL_Renderer* screen);
    void Render(int x, int y, SDL_Renderer* des, const SDL_Rect* clip = NULL);

    void Free();

    bool LoadFromRenderedText(std::string textureText, TTF_Font* gFont, SDL_Color textColor, SDL_Renderer* gRenderer);

    int GetWidth();
    int GetHeight();

protected:
    SDL_Texture* p_object_;


	int mWidth;
	int mHeight;
};

