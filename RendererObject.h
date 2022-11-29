#ifndef RENDEREROBJECT_H
#define RENDEREROBJECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include "RendererObject.h"

// базовый класс для всех рендерируемых объектов
class RendererObject
{
    public:
        RendererObject(SDL_Renderer* renderer);
        ~RendererObject();

        virtual void SetRenderPosition(int x, int y);
        virtual void SetRenderRect(SDL_Rect rect);

        //Gets image dimensions
        int getWidth(){return mWidth;}
        int getHeight(){return mHeight;}
        int GetPosX(){return xPos;}
        int GetPosY(){return yPos;}

        bool HasPoint(int x, int y) {return x >= xPos && x <= xPos+mWidth && y >= yPos && y <= yPos+mHeight;}

        SDL_Rect GetRect() {return SDL_Rect{xPos,yPos,mWidth,mHeight};}
    protected:
        SDL_Renderer* renderer;

        //Image dimensions
        int mWidth;
        int mHeight;
        int xPos, yPos;
};

#endif // RENDEREROBJECT_H
