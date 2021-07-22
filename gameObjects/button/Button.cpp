#include "Button.h"
void drawText(SDL_Renderer* renderer, TTF_Font* font, int x, int y, const char* text, SDL_Color color)
{
	SDL_Surface* surfaceMessage = TTF_RenderUTF8_Solid(font, text, color); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture

	SDL_Rect Message_rect; //create a rect
	Message_rect.x = x;  //controls the rect's x coordinate
	Message_rect.y = y; // controls the rect's y coordinte
	Message_rect.w = surfaceMessage->w; // controls the width of the rect
	Message_rect.h = surfaceMessage->h; // controls the height of the rect

	SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
	SDL_DestroyTexture(Message);
	SDL_FreeSurface(surfaceMessage);
}

void drawRect(SDL_Renderer* renderer, int x1, int y1, int w, int h, SDL_Color color)
{
	SDL_Color temp;
	SDL_GetRenderDrawColor(renderer, &temp.r, &temp.g, &temp.b, &temp.a);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(renderer, x1, y1, x1 + w, y1);
	SDL_RenderDrawLine(renderer, x1 + w, y1, x1 + w, y1 + h);
	SDL_RenderDrawLine(renderer, x1, y1 + h, x1 + w, y1 + h);
	SDL_RenderDrawLine(renderer, x1, y1, x1, y1 + h);
	SDL_SetRenderDrawColor(renderer, temp.r, temp.g, temp.b, temp.a);
}

bool positionIsInRect(int x, int y, SDL_Rect rect)
{
	return ((x > rect.x && x < rect.w + rect.x) && (y > rect.y && y < rect.h + rect.y));
}
Button::Button(int X, int Y, int w, int h, std::string Text)
{
    x=X;
    y=Y;
    width=w;
    height=h;
    text = Text;
}
Button::Button()
{
    x=y=width=height=0;
    text="";
}
void Button::setText(std::string text)
{
    this->text=text;
}
std::string Button::getText()
{
    return this->text;
}

void Button::draw(SDL_Renderer *renderer, SDL_Color color, TTF_Font *font,SDL_Color textColor)
{
    drawRect(renderer,this->x,this->y,this->width,this->height, color);

    SDL_Surface *txt=TTF_RenderUTF8_Solid(font,text.c_str(),textColor);

    drawText(renderer,font,this->x+(this->width-txt->w)/2,this->y+this->height/2-txt->h/2,this->text.c_str(),textColor);
}
