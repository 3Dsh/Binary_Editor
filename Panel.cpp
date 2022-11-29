#include "Panel.h"

Panel::Panel(SDL_Renderer* renderer) : Texture(renderer)
{
    //ctor
}

bool Panel::Create(int width, int height, SDL_TextureAccess access){
    free();

    mTexture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGBA8888, access, width, height );
    SDL_SetTextureBlendMode(mTexture,SDL_BLENDMODE_BLEND);

    if( mTexture == NULL )
    {
        printf( "Unable to create blank texture! SDL Error: %s\n", SDL_GetError() );
    }
    else
    {
        mWidth = width;
        mHeight = height;
    }

    return mTexture != NULL;
}

void Panel::Render(int xOffcet, int yOffcet )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { xPos+xOffcet, yPos+yOffcet, mWidth, mHeight };

    //Render to screen
    SDL_RenderCopy( renderer, mTexture, NULL, &renderQuad);
}

void Panel::SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a){
    setRenderTarget(true);
    SDL_SetRenderDrawColor(renderer, r,g,b,a);
    SDL_RenderClear(renderer);
    setRenderTarget(false);
}

Panel::~Panel()
{
    //dtor
}
