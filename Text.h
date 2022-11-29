#ifndef TEXT_H
#define TEXT_H

#include "Texture.h"
#include "string"

class Text : public Texture
{
    public:
        Text(SDL_Renderer* renderer);
        ~Text();

        bool Load( const char* text, SDL_Color textColor, TTF_Font* font, Uint32 width = 0);
        void Render(int xOffcet = 0, int yOffcet = 0 ) override;
        void SetScrollRect (SDL_Rect *scrollRect);
        void SetClipRect (SDL_Rect *rect);

        char* GetText() { return TextureText; }

    private:
        char* TextureText;
        SDL_Rect *scrollRect;
        int scrollRectWidth;
        SDL_Rect *clipRect;
        Uint32 scrollTime;
};

#endif // TEXT_H
