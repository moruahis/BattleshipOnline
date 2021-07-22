#ifndef ENTITYH
#define ENTITYH
#include <SDL.h>
class Entity
{
public:
    int x, y, width, height;
    void setPosition(int, int);
    int getX();
    int getY();
    void setSize(int, int);
    int getWidth();
    int getHeight();

    SDL_Rect getRect();

    virtual void draw(SDL_Renderer *renderer){return;}
};

#endif
