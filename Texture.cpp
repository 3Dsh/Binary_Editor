#include "Texture.h"

Texture::Texture(SDL_Renderer* renderer) : RendererObject(renderer)
{
    mTexture = NULL;
    startWidth = startHeight = 0;
}

Texture::~Texture()
{
    free();
}

void Texture::free(){
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void Texture::setRenderTarget(bool enable){
    if (enable) SDL_SetRenderTarget(renderer, mTexture);
    else SDL_SetRenderTarget(renderer, NULL);
}


void Texture::setAlpha(Uint8 alpha){
    SDL_SetTextureAlphaMod( mTexture, alpha );
}

void Texture::ChangeHeightProportionally(int height){
    float kf = (float)height / startHeight;

    mHeight = startHeight * kf;
    mWidth = startWidth * kf;

    SDL_Texture* tmp = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, mWidth, mHeight);
    SDL_SetTextureBlendMode(tmp, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_NONE);
    SDL_SetRenderTarget(renderer, tmp);
    SDL_RenderCopy(renderer, mTexture, NULL, NULL);
    SDL_SetRenderTarget(renderer, NULL);
    mTexture = tmp;
}
