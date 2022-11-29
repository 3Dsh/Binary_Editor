#include "RendererObject.h"

RendererObject::RendererObject(SDL_Renderer* renderer)
{
    this->renderer = renderer;
    xPos = yPos = 0;
    mWidth = mHeight = 0;
}

RendererObject::~RendererObject()
{
    //dtor
}
void RendererObject::SetRenderPosition(int x, int y){
        xPos = x;
        yPos = y;
}

void RendererObject::SetRenderRect(SDL_Rect rect){
    xPos = rect.x;
    yPos = rect.y;
    mWidth = rect.w;
    mHeight = rect.h;
}
