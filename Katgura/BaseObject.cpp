#include "BaseObject.h"

BaseObject::BaseObject()
{
    p_object_ = NULL;

}

BaseObject::~BaseObject()
{
    Free();
}

bool BaseObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    Free();

    SDL_Texture* new_texture = NULL;

    SDL_Surface* load_surface = IMG_Load(path.c_str());

    if (load_surface != NULL)
    {
        SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
        new_texture = SDL_CreateTextureFromSurface(screen, load_surface);
        if (new_texture != NULL)
        {
            mWidth = load_surface->w;
            mHeight = load_surface->h;
        }
        else LogError("Can not create texture from surface.", SDL_ERROR);

        SDL_FreeSurface(load_surface);
    }
    else LogError("Can not load image.", IMG_ERROR);

    p_object_ = new_texture;

    return p_object_ !=NULL;
}

void BaseObject::Render(int x, int y, SDL_Renderer* des, const SDL_Rect* clip )
{

    SDL_Rect renderquad = { x, y, mWidth, mHeight };

    if (clip != nullptr)
	{

		renderquad.w = clip->w;
		renderquad.h = clip->h;
	}

    SDL_RenderCopy(des, p_object_, clip, &renderquad);
}

void BaseObject::Free()
{
    if (p_object_ != NULL)
    {

        p_object_ = NULL;

    }
}

bool BaseObject::LoadFromRenderedText(std::string textureText, TTF_Font *gFont, SDL_Color textColor, SDL_Renderer *gRenderer)
{
    Free();

    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
    if (textSurface == NULL)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else
    {
        p_object_ = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if (p_object_ == NULL)
        {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        SDL_FreeSurface(textSurface);
    }

    return p_object_ != NULL;
}
