#include "Image.h"

Image::Image(SDL_Renderer* renderer, char* filePath) : Texture(renderer)
{
    if (filePath) Load(filePath);
}

bool Image::Load(char* path){
    free();
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    //SDL_Surface* loadedSurface = IMG_Load( path );
    SDL_Surface* loadedSurface = SDL_LoadBMP( path );
    if( loadedSurface == NULL )
    {
        //printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }
    else
    {
        //Color key image
        //SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 255, 255, 255 ) );
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = startWidth = loadedSurface->w;
            mHeight = startHeight = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    mTexture = newTexture;
    return mTexture != NULL;
}

void Image::Crop(float x0Prec, float y0Prec, float x1Prec, float y1Prec){
    if (x0Prec > 1 || x0Prec < 0 || y0Prec > 1 || y0Prec < 0 ||
        x1Prec > 1 || x1Prec < 0 || y1Prec > 1 || y1Prec < 0) return;

    SDL_Texture *tmp = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (x1Prec - x0Prec)*startWidth, (y1Prec-y0Prec)*startHeight);
    SDL_SetTextureBlendMode(tmp, SDL_BLENDMODE_BLEND);
    SDL_Rect rect = SDL_Rect{x0Prec*startWidth, y0Prec*startHeight, (x1Prec - x0Prec)*startWidth, (y1Prec-y0Prec)*startHeight};
    SDL_SetRenderTarget(renderer, tmp);
    SDL_RenderCopy(renderer, mTexture, &rect,NULL);
    SDL_SetRenderTarget(renderer, NULL);

    SDL_DestroyTexture(mTexture);
    mTexture = tmp;
}

void Image::Render(int xOffcet, int yOffcet )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { xPos+xOffcet, yPos+yOffcet, mWidth, mHeight };

    //Render to screen
    SDL_RenderCopy( renderer, mTexture, NULL, &renderQuad);
}

void Image::RenderClip(SDL_Rect* clip){
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { xPos, yPos, mWidth, mHeight };

    //Render to screen
    SDL_RenderCopy( renderer, mTexture, clip, &renderQuad);
}


Image::~Image()
{
    //dtor
}
