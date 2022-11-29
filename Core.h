#ifndef CORE_H
#define CORE_H

#include "Window.h"
#include "MenuManager.h"

class Core
{
    public:
        Core();
        ~Core();

        void MainLoop();

    private:
        bool InitSDL();

        SDL_Event e;
        Window *mainWindow;
        MenuManager *menuManager;

        Uint32 startFrame, endFrame;
};

#endif // CORE_H
