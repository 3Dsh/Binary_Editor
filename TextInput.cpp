#include "TextInput.h"

TextInput::TextInput(Window* window, char* fontPath, int maxLength) : RendererObject(window->GetRenderer())
{
    inputText = "";
    this->font = TTF_OpenFont(fontPath, 32);
    this->maxLength = maxLength;

    text = new Text(window->GetRenderer());

    bColor = SDL_Color{255,255,255,255};
    tColor = SDL_Color{0,0,0,255};
    bMode = Frame;

    this->window = window;
}

void TextInput::EventHandler(SDL_Event e){
    //Enable text input
    //SDL_StartTextInput();

    //Special key input
    if( e.type == SDL_KEYDOWN )
    {
        //Handle backspace
        if( e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
        {
            //lop off character
            if (inputText[inputText.size()-1] < 0){
                inputText.pop_back();
            }
            inputText.pop_back();
        }
        //Handle copy
        else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
        {
            SDL_SetClipboardText( inputText.c_str() );
        }
        //Handle paste
        else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
        {
            inputText = SDL_GetClipboardText();
        }
    }
    //Special text input event
    else if( e.type == SDL_TEXTINPUT )
    {
        //Not copy or pasting
        if( !( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) ) )
        {
            char c = *e.text.text;
            if (c >= '0' && c <= '9' || c == '_' || c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z'){
                if (inputText.size() >= 254) return;
                //Append character
                inputText += e.text.text;
            }
        }
    }

    if (inputText.size() == 0) return;

    text->Load(inputText.data(), tColor, font);
    text->ChangeHeightProportionally(mHeight);
    text->SetRenderPosition(xPos + mWidth - text->getWidth(), yPos);
    if (text->getWidth() > mWidth) text->SetClipRect(new SDL_Rect{text->getWidth()-mWidth, 0, mWidth,mHeight});
    else text->SetClipRect(NULL);

    //Disable text input
    //SDL_StopTextInput();
}

void TextInput::Show(){

    SDL_SetRenderDrawColor(window->GetRenderer(), bColor.r, bColor.g, bColor.b, bColor.a);
    switch (bMode){
        case Solid:
            SDL_RenderFillRect(window->GetRenderer(), new SDL_Rect{xPos,yPos,mWidth,mHeight});
            break;
        case Frame:
            SDL_RenderDrawRect(window->GetRenderer(), new SDL_Rect{xPos,yPos,mWidth,mHeight});
            break;
    }

    if (inputText.size() == 0) return;

    text->Render();
}

TextInput::~TextInput()
{
    SDL_Log("TextInput dtor\t%x",this);
    if (font){
        TTF_CloseFont(font);
        font = nullptr;
    }
    if (text){
        delete(text);
        text = nullptr;
    }
}
