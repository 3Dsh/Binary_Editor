#include "Scrollbar.h"

Scrollbar::Scrollbar(Window* window, ScrollType type, int* val, SDL_Rect rectPos) : RendererObject(window->GetRenderer())
{
    sType = type;
    value = val;
    pressedPoint = NULL;
    xPos = rectPos.x;
    yPos = rectPos.y;
    mWidth = rectPos.w;
    mHeight = rectPos.h;
    if (type == ScrollType::S_Horizntal) sliderPos = SDL_Rect{rectPos.x, rectPos.y, 0.05*rectPos.w, rectPos.h};
    else sliderPos = SDL_Rect{rectPos.x, rectPos.y, rectPos.w, 0.05*rectPos.h};

    background = new Image(window->GetRenderer());
    slider = new Image(window->GetRenderer());
    background->Load("Images/fullbar.bmp");
    slider->Load("Images/slider.bmp");

    background->SetRenderRect(rectPos);
    slider->SetRenderRect(sliderPos);

    minVal = maxVal = 0;

    this->window = window;
}

Scrollbar::~Scrollbar()
{
    SDL_Log("Scrollbar dtor\t%x",this);
    if (background){
        delete(background);
        background = nullptr;
    }
    if (slider){
        delete(slider);
        slider = nullptr;
    }
}

void Scrollbar::SetScrollRange(int min, int max){
    sliderPos = SDL_Rect{xPos, yPos, mWidth, 0.05*mHeight};
    slider->SetRenderRect(sliderPos);

    maxVal = max;
    minVal = min;
    if (maxVal < minVal) maxVal = minVal;
}

void Scrollbar::Show(){
    if (minVal == maxVal) return;

    background->Render();
    slider->Render();
}

void Scrollbar::MoveSlider(int dx, int dy){
    switch (sType){
        case ScrollType::S_Horizntal:
            if (abs(dx) > window->getWidth()) return;
            if (sliderPos.x + dx <= xPos + mWidth - sliderPos.w && sliderPos.x + dx >= xPos) sliderPos.x += dx;
            else if (sliderPos.x + dx > xPos + mWidth - sliderPos.w) sliderPos.x = xPos + mWidth - sliderPos.w;
            else if (sliderPos.x + dx >= xPos) sliderPos.x = xPos;
            *value = minVal + (sliderPos.x)/(((float)mWidth - sliderPos.w)/(maxVal-minVal));
            break;
        case ScrollType::S_Vertical:
            if (abs(dy) > window->getHeight()) return;
            if (sliderPos.y + dy <= yPos + mHeight - sliderPos.h && sliderPos.y + dy >= yPos) sliderPos.y += dy;
            else if (sliderPos.y + dy > yPos + mHeight - sliderPos.h) sliderPos.y = yPos + mHeight - sliderPos.h;
            else if (sliderPos.y + dy < yPos) sliderPos.y = yPos;
            *value = minVal + (sliderPos.y - background->GetPosY())/(((float)mHeight - sliderPos.h)/(maxVal-minVal));
            break;
    }
    slider->SetRenderRect(sliderPos);
}

void Scrollbar::EventHandler(SDL_Event e){
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && (slider->HasPoint(e.button.x, e.button.y)))
        pressedPoint = new SDL_Point{e.button.x, e.button.y};
    else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
        pressedPoint = NULL;

    if (e.type == SDL_MOUSEWHEEL){
        MoveSlider(-e.wheel.x*(window->getHeight()*0.02), -e.wheel.y*(window->getHeight()*0.02));
    }

    if (pressedPoint && e.type == SDL_MOUSEMOTION){
        MoveSlider(e.button.x - pressedPoint->x, e.button.y - pressedPoint->y);
        pressedPoint = new SDL_Point{e.button.x, e.button.y};
    }
}
