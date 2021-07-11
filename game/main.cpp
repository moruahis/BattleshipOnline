#include <iostream>
#include <string>
#include <math.h>

//#include <SDL_ttf.h>
#ifdef __WIN32__
    #include <windows.h>
    #include <SDL.h>
#else
    #include <SDL2/SDL.h>
#endif

#include "graphicEnv.h"
#include "gameObj.h"
#include "package.h"

/***Screen size and scaling functions***/
int SCREEN_WIDTH = 720; //1% = 7.2
int SCREEN_HEIGHT = 540; //1% = 5.4
int scale_x(double value)
{
    return round((value/7.2)*((double)SCREEN_WIDTH/100));
}
int scale_y(double value)
{
    return round((value/5.4)*((double)SCREEN_HEIGHT/100));
}
/***/


/***Describing field***/
typedef struct fieldCell
{
    bool wasShooted;
    states curState;
} cell;

bool init();
void game(SDL_Renderer *ren, SDL_Window *win);
void loadField(SDL_Renderer *ren);

short shipsAmount = 15; //10?

int main(int argc, char* argv[])
{
    SDL_Window *win = NULL;
    SDL_Renderer *ren = NULL;
    //TTF_Font *fnt1 = NULL, *fnt2 = NULL;

    /***Initialization***/
    #ifdef __WIN32__
        SetConsoleOutputCP(65001);
    #endif
    if (!init())
        return 5;
    win = SDL_CreateWindow("Морской бой", 0, 25, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!win)
    {
        std::cerr << "Failed to create Window: " << SDL_GetError() << std::endl;
        return 5;
    }
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (!ren)
    {
        std::cerr << "Failed to create Renderer: " << SDL_GetError() << std::endl;
        return 5;
    }
    //fnt1 = TTF_OpenFont("main.otf", scale_y(70));
    //fnt2 = TTF_OpenFont("base_font.ttf", scale_y(30));
    /***Initializated***/

    game(ren, win);

    /***Memory clean and quit***/
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    //TTF_CloseFont(fnt1);
    //TTF_CloseFont(fnt2);
    SDL_Quit();
    return 0;
}

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO & SDL_INIT_TIMER))
    {
        std::cerr << "SDL wasn't initialised: " << SDL_GetError() << std::endl;
        return 0;
    }
//    if (TTF_Init())
//    {
//        std::cerr << "SDL_ttf wasn't initialised: " << SDL_GetError() << std::endl;
//        return 0;
//    }
    return 1;
}

void game(SDL_Renderer *ren, SDL_Window *win)
{
    SDL_Event evt;
    bool flag = true;
    graphicEnv gameUI;

    loadField(ren);

    while (flag)
    {
        while(SDL_PollEvent(&evt))
        {
            if (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_ESCAPE)
            {
                flag = false;
                break;
            }
            if (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_f) //пофиксить изменение режима экрана
            {
                if (!gameUI.getWindowFlag())
                    SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
                else
                    SDL_SetWindowFullscreen(win, 0);
                SDL_GetWindowSize(win, &SCREEN_WIDTH, &SCREEN_HEIGHT);
                gameUI.invertWindowFlag();
            }
            SDL_RenderPresent(ren); //пофиксить отображение
        }
    }

}

void loadField(SDL_Renderer *ren) //loads background and fields
{
    SDL_Surface *surf = NULL;
    SDL_Texture *txtr = NULL;
    SDL_Rect dst;

    surf = SDL_LoadBMP("background.bmp");
    if (!surf)
        std::cerr << "Image wasn't loaded: " << SDL_GetError() << std::endl;
    txtr = SDL_CreateTextureFromSurface(ren, surf);
    if (!txtr)
        std::cerr << "Texture (background) wasn't created: " << SDL_GetError() << std::endl;
    SDL_RenderCopy(ren, txtr, NULL, NULL);

    surf = SDL_LoadBMP("fields.bmp");
    if (!surf)
        std::cerr << "Image wasn't loaded: " << SDL_GetError() << std::endl;
    txtr = SDL_CreateTextureFromSurface(ren, surf);
    if (!txtr)
        std::cerr << "Texture (background) wasn't created: " << SDL_GetError() << std::endl;
    dst.x = scale_x(30);
    dst.y = scale_y(120);
    dst.h = scale_y(surf->h);
    dst.w = scale_y(surf->w);
    SDL_RenderCopy(ren, txtr, NULL, &dst);

    dst.x = scale_x(30)+scale_x(300)+scale_x(60);
    SDL_RenderCopy(ren, txtr, NULL, &dst);

    SDL_FreeSurface(surf);
    if (txtr)
            SDL_DestroyTexture(txtr);
}
