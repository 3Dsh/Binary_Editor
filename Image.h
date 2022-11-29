#ifndef IMAGE_H
#define IMAGE_H

#include "Texture.h"

class Image : public Texture
{
    public:
        Image(SDL_Renderer* renderer, char* filePath = NULL);
        ~Image();

        virtual bool Load(char* path);
        void Render(int xOffcet = 0, int yOffcet = 0) override;
        void RenderClip(SDL_Rect* clip);

        // обрезание изображения в процентах
        void Crop(float x0Prec, float y0Prec, float x1Prec, float y1Prec);

};

#endif // IMAGE_H
