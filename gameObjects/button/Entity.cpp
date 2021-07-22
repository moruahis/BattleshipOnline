#include "Entity.h"

void Entity::setPosition(int X, int Y)
{
    this->x=X;
    this->y=Y;
}
int Entity::getX()
{
    return this->x;
}
int Entity::getY()
{
    return this->y;
}
void Entity::setSize(int w, int h)
{
    this->width=w;
    this->height=h;
}
int Entity::getWidth()
{
    return this->width;
}
int Entity::getHeight()
{
    return this->height;
}

SDL_Rect Entity::getRect()
{
    SDL_Rect dst;
    dst.x=this->x;
    dst.y=this->y;
    dst.h=this->height;
    dst.w=this->width;
    return dst;
}
