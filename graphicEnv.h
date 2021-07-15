#ifndef GRAPHICENV_H_INCLUDED
#define GRAPHICENV_H_INCLUDED

#include <SDL.h>
#include <iostream>

class graphicEnv
{
private:
    SDL_Texture* txtr;
    SDL_Surface* surf;
    SDL_Rect dest;
    bool win_flag;
public:
    graphicEnv()
    {
        txtr = NULL;
        dest = {};
        surf = NULL;
        win_flag = 0;
        std::cerr << "===graphicEnv object created===" << std::endl;
    }
    ~graphicEnv()
    {
        if (txtr)
            SDL_DestroyTexture(txtr);
        SDL_FreeSurface(surf);
        std::cerr << "===graphicEnv object was destroyed===" << std::endl;
    }

    SDL_Rect getDest(void)
    {
        return dest;
    }
    void invertWindowFlag(void)
    {
        win_flag = !win_flag;
    }
    bool getWindowFlag(void)
    {
        return win_flag;
    }
};

class renderer
{
    SDL_Renderer* ren;
public:
    renderer(SDL_Window* win)
    {
        ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
        if (!ren)
            std::cerr << "Failed to create Renderer: " << SDL_GetError() << std::endl;
    }
    SDL_Renderer* getRen(void)
    {
        return ren;
    }
};
#endif // GRAPHICENV_H_INCLUDED