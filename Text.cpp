#include "Text.h"
#include <iostream>

Text::Text(SDL_Renderer* renderer) : Texture(renderer)
{
    TextureText = NULL;
    scrollRect = nullptr;
    clipRect = nullptr;
    scrollRectWidth = 0;
}

bool Text::Load(const char* text, SDL_Color textColor, TTF_Font* font, Uint32 width)
{

    delete[]TextureText;
    int len = strlen(text);
    TextureText = new char[len+1];
    strcpy(TextureText,text);

    //Get rid of preexisting texture
    free();

    SDL_Surface* textSurface;
    if (!width) textSurface = TTF_RenderUTF8_Blended( font, text, textColor);
    else textSurface = TTF_RenderUTF8_Blended_Wrapped( font, text, textColor,width);
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
        if( mTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = startWidth = textSurface->w;
            mHeight = startHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }

    //Return success
    return mTexture != NULL;
}

void Text::SetScrollRect(SDL_Rect *scrollRect){
    this->scrollRect = scrollRect;
    if (scrollRect) scrollRectWidth = scrollRect->w;
    scrollTime = SDL_GetTicks();
}

void Text::SetClipRect(SDL_Rect *rect){
    if (rect) clipRect = new SDL_Rect{rect->x, rect->y, rect->w, rect->h};
    else clipRect = nullptr;
}

void Text::Render(int xOffcet, int yOffcet )
{
    if (scrollRect){
        //Set rendering space and render to screen
        SDL_Rect renderQuad = { xPos+xOffcet, yPos+yOffcet, scrollRect->w, scrollRect->h};
        //if (scrollRect->x + scrollRect->w > mWidth) renderQuad.w = mWidth - scrollRect->x;
        //Render to screen
        SDL_RenderCopy( renderer, mTexture, scrollRect, &renderQuad);

        if (SDL_GetTicks() - scrollTime >= 16){
            scrollRect->x += 1;
            if (scrollRect->x > mWidth){
                scrollRect->w = scrollRectWidth;
                scrollRect->x = -scrollRectWidth;
            }
            else if (scrollRect->x + scrollRect->w > mWidth) scrollRect->w--;
            scrollTime = SDL_GetTicks();
        }
        return;
    }
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { xPos+xOffcet, yPos+yOffcet, mWidth, mHeight };

    //Render to screen
    if (clipRect) SDL_RenderCopy( renderer, mTexture, clipRect, new SDL_Rect{renderQuad.x+clipRect->x, renderQuad.y+clipRect->y, clipRect->w, clipRect->h});
    else SDL_RenderCopy( renderer, mTexture, NULL, &renderQuad);
}

Text::~Text()
{
    SDL_Log("Text dtor\t%x",this);
    if (TextureText){
        delete[]TextureText;
        TextureText = nullptr;
    }
    if (scrollRect){
        delete(scrollRect);
        scrollRect = nullptr;
    }
    if (clipRect){
        delete(clipRect);
        clipRect = nullptr;
    }
}
