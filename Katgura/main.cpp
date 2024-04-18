
#include "CommonFunc.h"
#include "BaseObject.h"


#include "GameUtils.h"
#include "Button.h"
#include<bits/stdc++.h>

BaseObject g_background;


SDL_Color textColor = { 0, 0, 0 };
TTF_Font* gFont = nullptr;//
Mix_Music* gMusic = nullptr;
Mix_Music* gMenuMusic = nullptr;
Mix_Chunk* gClick = nullptr;
Mix_Chunk* gJump = nullptr;
Mix_Chunk* gLose = nullptr;

SDL_Rect gPlayButton[BUTTON_TOTAL];
SDL_Rect gHelpButton[BUTTON_TOTAL];
SDL_Rect gExitButton[BUTTON_TOTAL];
SDL_Rect gBackButton[BUTTON_TOTAL];
SDL_Rect gPauseButton[BUTTON_TOTAL];
SDL_Rect gContinueButton[BUTTON_TOTAL];
SDL_Rect gPlayAgainButton[BUTTON_TOTAL];
SDL_Rect gCharacterClips[RUNNING_FRAMES];
SDL_Rect gEnemyClips[FLYING_FRAMES];

BaseObject gMenuTexture;
BaseObject gInstructionTexture;
BaseObject gBackgroundTexture[BACKGROUND_LAYER];
BaseObject gCharacterTexture;
BaseObject gGroundTexture;
BaseObject gPlayButtonTexture;
BaseObject gHelpButtonTexture;
BaseObject gExitButtonTexture;
BaseObject gBackButtonTexture;
BaseObject gPauseButtonTexture;
BaseObject gContinueButtonTexture;
BaseObject gLoseTexture;
BaseObject gText1Texture;
BaseObject gScoreTexture;
BaseObject gText2Texture;
BaseObject gHighScoreTexture;

Button PlayButton(PLAY_BUTON_POSX, PLAY_BUTTON_POSY);
Button HelpButton(HELP_BUTTON_POSX, HELP_BUTTON_POSY);
Button ExitButton(EXIT_BUTTON_POSX, EXIT_BUTTON_POSY);
Button BackButton(BACK_BUTTON_POSX, BACK_BUTTON_POSY);
Button PauseButton(PAUSE_BUTTON_POSX, PAUSE_BUTTON_POSY);

bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0)
        return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("Game Katgura",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH, SCREEN_HEIGHT,
                                SDL_WINDOW_SHOWN);
    if (g_window == NULL)
    {
        success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL)
            success = false;
        else
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                success = false;
            if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
            {
                printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                success = false;
            }

            if (TTF_Init() == -1)
            {
                printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                success = false;
            }
        }
    }

    return success;
}

bool LoadBackground()
{
    bool success = true;

    bool ret = g_background.LoadImg("img//background.jpg", g_screen);
    if (ret == false)
        return false;

    return true;
}

void close()
{
    gMenuTexture.Free();
    gInstructionTexture.Free();
    gCharacterTexture.Free();
    gGroundTexture.Free();
    gPlayButtonTexture.Free();
    gHelpButtonTexture.Free();
    gExitButtonTexture.Free();
    gBackButtonTexture.Free();
    gPauseButtonTexture.Free();
    gContinueButtonTexture.Free();
    gLoseTexture.Free();
    gText1Texture.Free();
    gScoreTexture.Free();
    gText2Texture.Free();
    gHighScoreTexture.Free();

    for (int i = 0; i < BACKGROUND_LAYER; ++i)
    {
        gBackgroundTexture[i].Free();
    }

    Mix_FreeMusic(gMusic);
    Mix_FreeMusic(gMenuMusic);
    Mix_FreeChunk(gClick);
    Mix_FreeChunk(gLose);
    Mix_FreeChunk(gJump);
    gMusic = nullptr;
    gMenuMusic = nullptr;
    gClick = nullptr;
    gLose = nullptr;
    gJump = nullptr;

    g_background.Free();

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    if (InitData() == false)
        return -1;

    if (LoadBackground() == false)
        return -1;

    if (LoadMedia() == false)
        return -1;

    bool Quit_Menu = false;
    bool Play_Again = false;
    Mix_PlayMusic(gMenuMusic, IS_REPEATITIVE);

    while (!Quit_Menu)
    {
        SDL_Event e_mouse;

        while (SDL_PollEvent(&e_mouse) != 0)
        {
            if (e_mouse.type == SDL_QUIT)
            {
                Quit_Menu = true;
            }

            bool Quit_Game = false;
            HandlePlayButton(&e_mouse, PlayButton, Quit_Menu, Play_Again, gClick);

            HandleHelpButton(&e_mouse, gBackButton,
                             HelpButton, BackButton,
                             gInstructionTexture, gBackButtonTexture,
                             g_screen, Quit_Game, gClick);

            HandleExitButton(&e_mouse, ExitButton, Quit_Menu, gClick);

            if (Quit_Game == true)
            {
                return 0;
            }
        }

        gMenuTexture.Render(0,0,g_screen);

        SDL_Rect* currentClip_Play = &gPlayButton[PlayButton.currentSprite];

        PlayButton.Render(currentClip_Play, g_screen, gPlayButtonTexture);

        SDL_Rect* currentClip_Help = &gHelpButton[HelpButton.currentSprite];
        HelpButton.Render(currentClip_Help, g_screen, gHelpButtonTexture);

        SDL_Rect* currentClip_Exit = &gExitButton[ExitButton.currentSprite];
        ExitButton.Render(currentClip_Exit, g_screen, gExitButtonTexture);


        SDL_RenderPresent(g_screen);
    }

    while(Play_Again)
    {
        srand(time(NULL));




        bool is_quit = false;
        bool Game_State = true;
        while (!is_quit)
        {
            if (Game_State)
            {
                while (SDL_PollEvent(&g_event)!=0)
                {
                    if (g_event.type == SDL_QUIT)
                    {
                        is_quit = true;
                        Play_Again = false;
                    }


                }

                SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
                SDL_RenderClear(g_screen);

                g_background.Render(0,0,g_screen, NULL);

                SDL_RenderPresent(g_screen);

            }
            //di đến đoạn bấm nút như hqua ik
            //đoạn hiện nút hay render v
            //rendẻ trong main cơ

            if (!Play_Again)
            {

            }
        }
    }

        close();
        return 0;
}
//ưtf nay xoa nham ngoac nao a khum bt nx
    bool LoadMedia()
    {
        bool success = true;

        gMusic = Mix_LoadMUS("sound/bkgr_audio.wav");
        if (gMusic == nullptr)
        {
            LogError("Failed to load background music", MIX_ERROR);
            success = false;
        }

        gMenuMusic = Mix_LoadMUS("sound/menu_audio.wav");
        if (gMenuMusic == nullptr)
        {
            LogError("Failed to load menu music", MIX_ERROR);
            success = false;
        }

        gClick = Mix_LoadWAV("sound/mouse_click.wav");
        if (gClick == nullptr)
        {
            LogError("Failed to load mouse click sound", MIX_ERROR);
            success = false;
        }

        gJump = Mix_LoadWAV("sound/jump_sound.wav");
        if (gJump == nullptr)
        {
            LogError("Failed to load jumping sound", MIX_ERROR);
            success = false;
        }

        gLose = Mix_LoadWAV("sound/lose_sound.wav");
        if (gLose == nullptr)
        {
            LogError("Failed to load lose sound", MIX_ERROR);
            success = false;
        }

        else
        {
            gFont = TTF_OpenFont("font/pixel_font.ttf", 28);
            if (gFont == NULL)
            {
                LogError("Failed to load font", MIX_ERROR);
                success = false;
            }
            else
            {
                if (!gText1Texture.LoadFromRenderedText("Your score: ", gFont, textColor, g_screen))
                {
                    std::cout << "Failed to render text1 texture" << std::endl;
                    success = false;
                }

                if (!gText2Texture.LoadFromRenderedText("High score: ", gFont, textColor, g_screen))
                {
                    std::cout << "Failed to render text2 texture" << std::endl;
                    success = false;
                }

                if (!gMenuTexture.LoadImg("img/background/menu1.png", g_screen))
                {
                    std::cout << "Failed to load menu image" << std::endl;
                    success = false;
                }

                if (!gInstructionTexture.LoadImg("img/background/instruction.png", g_screen))
                {
                    std::cout << "Failed to load instruction image" << std::endl;
                    success = false;
                }

                if (!gPlayButtonTexture.LoadImg("img/button/big_button/play_button.png", g_screen))
                {
                    std::cout << "Failed to load play_button image" << std::endl;
                    success = false;
                }
                else
                {
                    for (int i = 0; i < BUTTON_TOTAL; ++i)
                    {
                        gPlayButton[i].x = 150 * i;
                        gPlayButton[i].y = 0;
                        gPlayButton[i].w = 150;
                        gPlayButton[i].h = 98;
                    }
                }

                if (!gHelpButtonTexture.LoadImg("img/button/big_button/help_button.png", g_screen))
                {
                    std::cout << "Failed to load help_button image" << std::endl;
                    success = false;
                }
                else
                {
                    for (int i = 0; i < BUTTON_TOTAL; ++i)
                    {
                        gHelpButton[i].x = 150 * i;
                        gHelpButton[i].y = 0;
                        gHelpButton[i].w = 150;
                        gHelpButton[i].h = 98;
                    }
                }

                if (!gBackButtonTexture.LoadImg("img/button/big_button/back_button.png", g_screen))
                {
                    std::cout << "Failed to load back_button image" << std::endl;
                    success = false;
                }
                else
                {
                    for (int i = 0; i < BUTTON_TOTAL; ++i)
                    {
                        gBackButton[i].x = 100 * i;
                        gBackButton[i].y = 0;
                        gBackButton[i].w = 100;
                        gBackButton[i].h = 78;
                    }
                }

                if (!gExitButtonTexture.LoadImg("img/button/big_button/exit_button.png", g_screen))
                {
                    std::cout << "Failed to load exit_button image" << std::endl;
                    success = false;
                }
                else
                {
                    for (int i = 0; i < BUTTON_TOTAL; ++i)
                    {
                        gExitButton[i].x = 150 * i;
                        gExitButton[i].y = 0;
                        gExitButton[i].w = 150;
                        gExitButton[i].h = 98;
                    }
                }

                if (!gPauseButtonTexture.LoadImg("img/button/big_button/pause_button.png", g_screen))
                {
                    std::cout << "Failed to load pause_button image " << std::endl;
                    success = false;
                }
                else
                {
                    for (int i = 0; i < BUTTON_TOTAL; ++i)
                    {
                        gPauseButton[i].x = 22 * i;
                        gPauseButton[i].y = 0;
                        gPauseButton[i].w = 22;
                        gPauseButton[i].h = 34;
                    }
                }

                if (!gContinueButtonTexture.LoadImg("img/button/big_button/continue_button.png", g_screen))
                {
                    std::cout << "Failed to load continue_button image " << std::endl;
                    success = false;
                }
                else
                {
                    for (int i = 0; i < BUTTON_TOTAL; ++i)
                    {
                        gContinueButton[i].x = 22 * i;
                        gContinueButton[i].y = 0;
                        gContinueButton[i].w = 22;
                        gContinueButton[i].h = 34;
                    }
                }



                if (!gGroundTexture.LoadImg("img/background/ground.png", g_screen))
                {
                    std::cout << "Failed to load ground image" << std::endl;
                    success = false;
                }


                if (!gLoseTexture.LoadImg("img/background/lose.png", g_screen))
                {
                    std::cout << "Failed to load lose image." << std::endl;
                    success = false;
                }
            }
        }
        return success;
    }
