#include "Core.h"

Core::Core()
{
    if (!InitSDL())
        throw -10;

    mainWindow = new Window(1280,768);
    mainWindow->init(SDL_WINDOW_SHOWN, "Binary editor");

    menuManager = new MenuManager(mainWindow);
}

bool Core::InitSDL(){
    bool success = true;
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}
		if (TTF_Init()<0){
            printf("SDL_TTF could not initialize!\n");
            success = false;
		}
	}
	return success;
}

void Core::MainLoop(){
    SDL_Renderer* renderer = mainWindow->GetRenderer();
    State::curState = State::MainEditor;

    while (mainWindow->isShown()){
        startFrame = SDL_GetTicks();
        e.type = SDL_MOUSEMOTION;

        SDL_PollEvent(&e);

        menuManager->EventHandler(e);
        mainWindow->handleEvent(e);
        SDL_SetRenderDrawColor(renderer,90,90,90,255);
        SDL_RenderClear(renderer);

        menuManager->Draw();
        SDL_RenderPresent(renderer);
        endFrame = SDL_GetTicks();

        //cout << endFrame - startFrame << endl;
        //if (endFrame - startFrame < delay)
        //   SDL_Delay(delay - (endFrame - startFrame));
    }
}

Core::~Core()
{
    SDL_Log("Core dtor\t%x",this);
    delete(menuManager);
}
