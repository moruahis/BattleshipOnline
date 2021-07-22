#ifndef GAMEOBJ_H_INCLUDED
#define GAMEOBJ_H_INCLUDED

#include <SDL.h>
#include <iostream>
#include <string>

class gameObj
{
protected:
    SDL_Renderer* render;
    SDL_Surface* surf;
    SDL_Texture* txtr;
    SDL_Rect place;
    int cellX, cellY;
public:
    virtual void image() = 0;
};

class ship : public gameObj
{
protected:
    SDL_Event event;
    int cellsValue;
    bool grabbingFlag;
    bool rotationFlag;
    SDL_Rect initialPlace;
public:
    ship()
    {
        surf = NULL;
        txtr = NULL;
        initialPlace = place = { 0, 0, 0, 0 };
        cellsValue = 0; grabbingFlag = false; rotationFlag = false;
    }
    ~ship()
    {
        SDL_FreeSurface(surf);
        if (txtr)
            SDL_DestroyTexture(txtr);
        std::cerr << "===Ship is destroyed===" << std::endl;
    }
    bool loadSkin(SDL_Renderer* ren)
    {
        render = ren;

        std::string filename;
        filename = "res/ship_" + std::to_string(cellsValue) + ".bmp";
        std::cout << filename << std::endl;

        surf = SDL_LoadBMP(filename.c_str());
        if (!surf)
        {
            std::cerr << "Image wasn't loaded: " << SDL_GetError() << std::endl;
            return false;
        }
        place.h = surf->h; place.w = surf->w;
        txtr = SDL_CreateTextureFromSurface(ren, surf);
        if (!txtr)
        {
            std::cerr << "Texture (ship) wasn't created: " << SDL_GetError() << std::endl;
            return false;
        }
        return true;
    }
    void saveInitialPos()
    {
        this->initialPlace = this->place;
    }
    void resetPosition()
    {
        this->place = this->initialPlace;
        this->rotationFlag = false;
    }
    void setPlace(SDL_Rect dest)
    {
        place.x = dest.x;
        place.y = dest.y;
    }
    void setPlace()
    {
        place.x = cellX * 30 + 30;
        place.y = cellY * 30 + 30;
    }
    SDL_Rect getPlace()
    {
        return place;
    }
    void handleEvent(SDL_Event evt)
    {
        event = evt;

        SDL_Rect mousePos = { event.motion.x, event.motion.y, 1, 1 };

        if (grabbingFlag)
        {
            cellX = (place.x - 30) / 30;
            cellY = (place.y - 30) / 30;

            cellX = cellX > 9 ? 9 : (cellX < 0 ? 0 : cellX);
            cellY = cellY > 9 ? 9 : (cellY < 0 ? 0 : cellY);

            place.x = event.motion.x; place.y = event.motion.y;
        }

        if (!grabbingFlag && event.type == SDL_MOUSEBUTTONDOWN && SDL_HasIntersection(&mousePos, &place))
        {
            std::cout << "Is grabbed" << std::endl;
            grabbingFlag = true;

        }
        else
            if (grabbingFlag && event.type == SDL_MOUSEBUTTONDOWN)
            {
                place.x = cellX * 30 + 30;
                place.y = cellY * 30 + 30;
            }
    }
    int* getCell()
    {
        int cell[2] = { cellX,cellY };
        return cell;
    }
    void setCell(int _cellX, int _cellY)
    {
        this->cellX = _cellX;
        this->cellY = _cellY;
    }
    void setCellsValue(int _cellsValue)
    {
        this->cellsValue = _cellsValue;
    }
    int getCellsValue()
    {
        return this->cellsValue;
    }
    bool getRotation()
    {
        return this->rotationFlag;
    }
    SDL_Texture* getShipTexture()
    {
        return this->txtr;
    }
    void image() override
    {
        SDL_Point rotationPoint = { 0,0 };
        SDL_Rect renderPlace = place;
        renderPlace.x += 30 * rotationFlag;
        SDL_RenderCopyEx(render, txtr, NULL, &renderPlace, 90*this->rotationFlag, &rotationPoint, SDL_FLIP_NONE);
    }
    void rotate()
    {
        rotationFlag = !rotationFlag;
    }
};

#endif // GAMEOBJ_H_INCLUDED