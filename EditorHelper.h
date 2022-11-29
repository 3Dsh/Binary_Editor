#ifndef EDITORHELPER_H
#define EDITORHELPER_H

#include "Window.h"
#include "Text.h"

class EditorHelper
{
    public:
        EditorHelper(Window* window);
        ~EditorHelper();

        int Createx2Texture(char* arr, int size, Text** binaryArr);
        int Createx8Texture(char* arr, int size, Text** binaryArr);
        int Createx16Texture(char* arr, int size, Text** binaryArr);

        char Deletex2(char c, char num);
        char Deletex8(char c, char num);
        char Deletex16(char c, char num);

        char Addx2(char c, char num, char sym);
        char Addx8(char c, char num, char sym);
        char Addx16(char c, char num, char sym);

        char* Getx16Code(char c);
        char* Getx8Code(char c);
        char* Getx2Code(char c);

    private:
        TTF_Font* font;
        Window* window;
};

#endif // EDITORHELPER_H
