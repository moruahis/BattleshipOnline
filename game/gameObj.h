#ifndef GAMEOBJ_H_INCLUDED
#define GAMEOBJ_H_INCLUDED

class gameObj
{
protected:
    SDL_Renderer *render;
    SDL_Surface *surf;
    SDL_Texture *txtr;
    SDL_Rect place, deniedCells;
    int cellX, cellY;
public:
    virtual void image() = 0;
};

class ship : public gameObj
{
protected:
    SDL_Event event;
    short cellsValue;
    short HP;
    bool grabbingFlag;
    bool rotationFlag;
public:
    ship()
    {
        surf = NULL;
        txtr = NULL;
        place = {0, 0, 0, 0};
        deniedCells = {-1, -1, -1, -1};
        cellsValue = 0; HP = 0; grabbingFlag = false; rotationFlag = false;
    }
    ~ship()
    {
        SDL_FreeSurface(surf);
        if (txtr)
            SDL_DestroyTexture(txtr);
        std::cerr << "===Ship is destroyed===" << std::endl;
    }
    bool loadSkin(SDL_Renderer *ren, short value)
    {
        cellsValue = value;
        render = ren;

        std::string filename;
        filename = "ship_" + std::to_string(cellsValue) + ".bmp";
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
    void setPlace(SDL_Rect dest)
    {
        place = dest;
    }
    short getHP()
    {
        return HP;
    }
    SDL_Rect getPlace()
    {
        return place;
    }
    bool isDamaged()
    {
        return cellsValue-HP;
    }
    void decreaseHP()
    {
        HP--;
    }
    void handleEvent(SDL_Event evt)
    {
        event = evt;

        SDL_Rect mousePos = {event.motion.x, event.motion.y, 1, 1};

        if (grabbingFlag)
        {
            cellX = (place.x - 30)/30;
            cellY = (place.y - 30)/30;

            cellX = cellX>9 ? 9 : (cellX<0 ? 0 : cellX);
            cellY = cellY>9 ? 9 : (cellY<0 ? 0 : cellY);

            place.x = event.motion.x; place.y = event.motion.y;
        }

        if (!grabbingFlag && event.type == SDL_MOUSEBUTTONDOWN && SDL_HasIntersection(&mousePos, &place))
        {
            std::cout << "Is grabbed" << std::endl;
            grabbingFlag = true;

            if (deniedCells.x != -1)
                for (int i = deniedCells.x; i<(deniedCells.x+deniedCells.w); i++)
                    for (int j = deniedCells.y; j<(deniedCells.y+deniedCells.h); j++)
                        field[i][j].curState = emptyCell;
        }
        else
            if (grabbingFlag && event.type == SDL_MOUSEBUTTONDOWN && checkField())
            {
                std::cout << "Is not grabbed anymore" << std::endl;
                grabbingFlag = !grabbingFlag;
                deniedCells = {cellX-1, cellY-1, cellsValue+2, 3};

                for (int i = deniedCells.x; i<(deniedCells.x+deniedCells.w); i++)
                    for (int j = deniedCells.y; j<(deniedCells.y+deniedCells.h); j++)
                        field[i][j].curState = shipDenied;

                for (int i = cellX; i<(cellX+cellsValue); i++)
                    for (int j = cellY; j<(cellY+1); j++) //предусмотрено для введения поворота корабля
                        field[i][j].curState = (states)cellsValue;


                place.x = cellX*30+30;
                place.y = cellY*30+30;
            }
    }
    bool checkField() //checking cells near choosed cell
    {
        for (int i = cellX; i<(cellX+cellsValue); i++)
            for (int j = cellY; j<(cellY+1); j++)
                if (field[cellX][cellY].curState==shipDenied)
                    return false;
        return true;
    }
    void image() override
    {
        SDL_RenderCopy(render, txtr, NULL, &place);
    }
};

#endif // GAMEOBJ_H_INCLUDED
