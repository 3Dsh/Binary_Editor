#ifndef CHOISELEVELMENU_H

#include "Button.h"
#include <string>
#include <stdio.h>
#include "List.h"
#include "GameState.h"
#include "Window.h"

#ifdef _WIN32
#include <windows.h>
#else
#ifdef __unix__
#include <sys/types.h>
#include <dirent.h>
#else
#ifdef __TURBOC__
#include <dir.h>
#else
#error Unsupported Implementation
#endif
#endif
#endif


#define CHOISELEVELMENU_H


class ChoiseFileMenu
{
    public:
        ChoiseFileMenu(Window* window, char* dir);
        ~ChoiseFileMenu();

        void Reload();
        void Show();
        char* EventHandler(SDL_Event e);

    private:
        void LoadPaths(char* dir);
        void InitButtons();
        char* GetFullPath(char* level);

        List<char*> paths;
        List<Button*> buttons;

        Button *backButton;
        Button *nextPage, *prevPage;

        int countOnPage, curPage, pageCount;

        char* levelDir;
        TTF_Font* font;
        Window* window;
};

#endif // CHOISELEVELMENU_H
