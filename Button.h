#ifndef BUTTON_H
#define BUTTON_H

#include "RendererObject.h"
#include "Image.h"
#include "Text.h"


class Button : public RendererObject
{
    public:
        enum Event{
            LB_Down, RB_Down, LB_Up, RB_Up, MouseMotion, None,
        };

        Button(SDL_Renderer* renderer, SDL_Rect rect);
        ~Button();
        Event EventHandler(SDL_Event& e);
        void Render( int offcetX = 0, int offcetY = 0);

        void LoadBackImages(char *normalImage, char *pressedImage, char *highlightedImage);
        void LoadBackImages(char *imagePath);
        void CropImages(float x0Prec, float y0Prec, float x1Prec, float y1Prec);
        void LoadText( SDL_Color normalTextColor, SDL_Color pressedTextColor, SDL_Color highlightedTextColor, const char *text, TTF_Font *font);
        void LoadText(SDL_Color color, const char* text, TTF_Font *font, bool scroll = false);

        void SetRenderPosition(int x, int y);
        void SetRenderRect(SDL_Rect rect);
    private:
        void SetNormalColor();
        void SetHighlightedColor();
        void SetPressedColor();

        SDL_Color normalTextColor, pressedTextColor, highlightedTextColor;
        Text *normalText, *pressedText, *highlightedText, *singleText;
        Image *normalImage, *pressedImage, *highlightedImage, *singleImage;
        Text* curText;
        Image* curImage;
};

#endif // BUTTON_H
