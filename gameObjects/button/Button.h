#ifndef ButtonH
#define ButtonH
#include <string>
#include "Entity.h"
#include <SDL_TTF.h>
#include <SDL.h>
class Button: public Entity
{
    std::string text;

public:
    Button(int, int, int, int, std::string);
    Button();
    void setText(std::string);
    std::string getText();

    void draw(SDL_Renderer*, SDL_Color, TTF_Font*, SDL_Color);

};
void drawText(SDL_Renderer* renderer, TTF_Font* font, int x, int y, const char* text, SDL_Color color);
void drawRect(SDL_Renderer* renderer, int x1, int y1, int w, int h, SDL_Color color);
bool positionIsInRect(int x, int y, SDL_Rect rect);
#endif //ButtonH
