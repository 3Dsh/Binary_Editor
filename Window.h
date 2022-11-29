#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Window
{
	public:
		Window(int w, int h);

		bool init(Uint32 font, char* hat);

		void handleEvent( SDL_Event& e );

		void focus();
        void hide();

		void render();

		void free();

		int getWidth();
		int getHeight();

        bool isShown();
        SDL_Window* GetSDLWindow() { return mWindow; }

        SDL_Renderer* GetRenderer(){ return mRenderer; }
	private:
		SDL_Window* mWindow;
		SDL_Renderer* mRenderer;
		int mWindowID;

		int mWidth;
		int mHeight;

		bool mShown;
};
#endif // WINDOW_H
