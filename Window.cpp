#include "Window.h"

Window::Window(int width, int height)
{
    //Initialize non-existant window
	mWindow = NULL;
	mRenderer = NULL;

	mWindowID = -1;
    mShown = false;

	mWidth = width;
	mHeight = height;
}

bool Window::init(Uint32 flags, char* hat){
    //Create window
    mWindow = SDL_CreateWindow( hat, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWidth, mHeight, flags);
    if( mWindow != NULL )
    {
        //Create renderer for window
        mRenderer = SDL_CreateRenderer( mWindow, -1, SDL_RENDERER_ACCELERATED );
        if( mRenderer == NULL )
        {
            printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
            SDL_DestroyWindow( mWindow );
            mWindow = NULL;
        }
        else
        {
            //Initialize renderer color
            SDL_SetRenderDrawColor( mRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

            //Grab window identifier
            mWindowID = SDL_GetWindowID( mWindow );

            mShown = flags == SDL_WINDOW_SHOWN;
        }
    }
    else
    {
        printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
    }

    return mWindow != NULL && mRenderer != NULL;
}

void Window::focus(){
    //Restore window if needed
    if( !mShown )
    {
        SDL_ShowWindow( mWindow );
        mShown = true;
    }

    //Move window forward
    //SDL_RaiseWindow( mWindow );
}

void Window::hide(){
    SDL_HideWindow( mWindow );
    mShown = false;
}

void Window::handleEvent(SDL_Event& e){
    //If an event was detected for this window
	if( e.type == SDL_WINDOWEVENT && e.window.windowID == mWindowID )
	{
		//Caption update flag
		bool updateCaption = false;

		switch( e.window.event )
		{
            //Window appeared
			case SDL_WINDOWEVENT_SHOWN:
			break;

			//Hide on close
			case SDL_WINDOWEVENT_CLOSE:
			SDL_HideWindow( mWindow );
			mShown = false;
			break;
		}
	}
}

void Window::free()
{
	if( mWindow != NULL )
	{
		SDL_DestroyWindow( mWindow );
	}

	mWidth = 0;
	mHeight = 0;
}

void Window::render()
{
    //Clear screen
    SDL_SetRenderDrawColor( mRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( mRenderer );

    //Update screen
    SDL_RenderPresent( mRenderer );
}

int Window::getWidth()
{
	return mWidth;
}

int Window::getHeight()
{
	return mHeight;
}

bool Window::isShown()
{
	return mShown;
}
