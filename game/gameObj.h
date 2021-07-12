#ifndef GAMEOBJ_H_INCLUDED
#define GAMEOBJ_H_INCLUDED

class gameObj
{
protected:
    SDL_Renderer *render;
    SDL_Surface *surf;
    SDL_Texture *txtr;
    SDL_Rect place;
    SDL_Rect startPlace;
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
        place = {0, 0, 0, 0}; startPlace = place;
        cellsValue = 0; HP = 0; grabbingFlag = false; rotationFlag = false;
    }
    ~ship()
    {
        SDL_FreeSurface(surf);
        if (txtr)
            SDL_DestroyTexture(txtr);
        std::cerr << "===Ship is destroyed===" << std::endl;
    }
    bool loadSkin(SDL_Renderer *ren, short value, SDL_Rect *dst)
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
        dst->h = surf->h; dst->w = surf->h;
        place = *dst;
        txtr = SDL_CreateTextureFromSurface(ren, surf);
        if (!txtr)
        {
            std::cerr << "Texture (ship) wasn't created: " << SDL_GetError() << std::endl;
            return false;
        }
        return true;
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

        if (grabbingFlag)
        {
            place.x = event.motion.x; place.y = event.motion.y;
            std::cout << "dst.x = " << place.x << "; dst.y = " << place.y << std::endl;
        }

        if (!grabbingFlag && event.type == SDL_MOUSEBUTTONDOWN &&
            (event.motion.x>=place.x && event.motion.x<=(place.x+place.w) &&
            event.motion.y>=place.y && event.motion.y<=(place.y+place.h)))
        {
            std::cout << "Is grabbed" << std::endl;
            grabbingFlag = true;
            startPlace = place;
        }
        if (grabbingFlag && event.type == SDL_MOUSEBUTTONDOWN &&
            ((event.motion.x<startPlace.x || event.motion.x>(startPlace.x+startPlace.w)) &&
            (event.motion.y<startPlace.y || event.motion.y>(startPlace.y+startPlace.h))))
        {
            std::cout << "Is not grabbed anymore" << std::endl;
            grabbingFlag = !grabbingFlag;
        }
    }
    void image() override
    {
        SDL_RenderCopy(render, txtr, NULL, &place);
    }
};

#endif // GAMEOBJ_H_INCLUDED
