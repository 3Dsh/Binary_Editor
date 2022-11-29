#ifndef TEXTURE_H
#define TEXTURE_H

#include "RendererObject.h"

enum Direction{
    Left, Right, Up, Down, None
};


class Texture : public RendererObject
{
    public:
        Texture(SDL_Renderer* );
        ~Texture();

        //Deallocates texture
        void free();

        //Set alpha modulation
        void setAlpha( Uint8 alpha );

        void setRenderTarget(bool enable);

        void RenderCopy(SDL_Renderer* renderer, SDL_Rect* clip, SDL_Rect* scrRect){
            SDL_RenderCopy(renderer, mTexture, clip, scrRect);
        }

        //Renders texture at given point
        virtual void Render(int xOffcet = 0, int yOffcet = 0) = 0;

        void ChangeHeightProportionally(int height);
    protected:
        //The actual hardware texture
        SDL_Texture* mTexture;

        int startWidth, startHeight;

};

#endif // TEXTURE_H
