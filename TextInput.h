#ifndef TEXTINPUT_H
#define TEXTINPUT_H

#include <string>
#include "Text.h"
#include "Window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// выводит текст в прямоугольной области
class TextInput : public RendererObject
{
    public:
        enum BackMode{
            Transperent, Solid, Frame,
        };

        TextInput(Window* window, char* fontPath, int maxLength);
        ~TextInput();

        void EventHandler(SDL_Event e);
        void Show();

        const char* GetText() { return inputText.data();}

        void SetBackMode(BackMode bMode) { this->bMode = bMode; }
        void SetBackColor(SDL_Color color) { bColor = color; }
        void SetTextColor(SDL_Color color) { tColor = color; }

    private:
        std::string inputText;
        TTF_Font *font;
        int maxLength;

        SDL_Color bColor, tColor;
        BackMode bMode;

        Text* text;
        Window* window;
};

#endif // TEXTINPUT_H
