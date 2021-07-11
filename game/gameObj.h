#ifndef GAMEOBJ_H_INCLUDED
#define GAMEOBJ_H_INCLUDED

class gameObj
{
protected:
    SDL_Renderer *render;
    SDL_Surface *surf;
    SDL_Texture *txtr;
    SDL_Rect place;
    int FPS;
public:
    gameObj(){FPS = 60;}
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
    ship(SDL_Renderer *ren, short value, SDL_Rect dst)
    {
        std::string filename;
        filename = "ship_" + std::to_string(cellsValue) + ".bmp";
        std::cout << filename << std::endl;

        surf = SDL_LoadBMP(filename.c_str());
        if (!surf)
            std::cerr << "Image wasn't loaded: " << SDL_GetError() << std::endl;
        txtr = SDL_CreateTextureFromSurface(ren, surf);
        if (!txtr)
            std::cerr << "Texture (ship) wasn't created: " << SDL_GetError() << std::endl;

        render = ren;
        place = dst;
        cellsValue = value;
        HP = value;
        rotationFlag = false;
        grabbingFlag = false;

        std::cerr << "===Ship with " << cellsValue << " cells is created===" << std::endl;
    }
    ~ship()
    {
        SDL_FreeSurface(surf);
        if (txtr)
            SDL_DestroyTexture(txtr);
        std::cerr << "===Ship is destroyed===" << std::endl;
    }
    short getHP()
    {
        return HP;
    }
    SDL_Rect shipPlace()
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
        if (event.type == SDL_MOUSEMOTION && grabbingFlag)
        {
            place.x += event.motion.xrel;
            place.y += event.motion.yrel;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN ) //добавить описание координат
        {
            grabbingFlag = !grabbingFlag;
        }

    }
    void moveShip()
    {

    }
};

#endif // GAMEOBJ_H_INCLUDED
