#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include "Window.h"
#include "GameState.h"

#include "MainEditor.h"
#include "ChoiseFileMenu.h"

class MenuManager
{
    public:

        MenuManager(Window* window);
        ~MenuManager();

        void Update();
        void EventHandler(SDL_Event e);
        void Draw();

    private:
        char* OpenDialog();

        MainEditor* editor;
        ChoiseFileMenu* choiseMenu;

        Window* window;
};

#endif // MENUMANAGER_H
