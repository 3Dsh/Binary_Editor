#ifndef MAINEDITOR_H
#define MAINEDITOR_H

#include "Window.h"
#include "EditorTools.h"
#include "Scrollbar.h"
#include "Text.h"
#include "EditorHelper.h"

class MainEditor
{
    public:
        MainEditor(Window* window);
        ~MainEditor();

        void Show();
        void EventHandler(SDL_Event e);

        void LoadFile(char* path);
        void SaveFile();
        void AddByte();
        void DeleteByte();
    private:
        void LoadTexture();

        EditorHelper helper;
        EditorTools* tools;

        Text** binaryText;

        Caret caret;

        // Scroll
        Scrollbar* vScroll;
        int scrollVal;

        // binary array
        char* arr;
        int size;
        char* path;

        Window* window;
};

#endif // MAINEDITOR_H
