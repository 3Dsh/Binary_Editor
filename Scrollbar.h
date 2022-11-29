#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include <cmath>
#include "Window.h"
#include "Image.h"
#include "RendererObject.h"

enum ScrollType{
    S_Horizntal,
    S_Vertical,
};

class Scrollbar : public RendererObject
{
    public:
        Scrollbar(Window* window, ScrollType type, int* val, SDL_Rect rectPos);
        ~Scrollbar();

        void SetScrollRange(int min, int max);

        void Show();
        void EventHandler(SDL_Event e);
        void Shift(int dx, int dy) { MoveSlider(dx,dy); }

        int GetMaxValue() { return maxVal; }
        int GetMinValue() { return minVal; }

        void SetValY(int y){


            sliderPos.y = yPos + y;
            slider->SetRenderRect(sliderPos);
        }
    private:
        void MoveSlider(int dx, int dy);

        ScrollType sType;
        SDL_Rect sliderPos;

        int* value;
        int maxVal, minVal;

        Image *background, *slider;

        SDL_Point *pressedPoint;

        Window *window;
};

#endif // SCROLLBAR_H
