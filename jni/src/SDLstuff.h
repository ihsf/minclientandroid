#ifndef _SDLSTUFF_H
#define _SDLSTUFF_H

#include <SDL.h>
#include "Engine.h"
#include "Font.h"
#include "Profiler.h"
#include <iostream>

using namespace std;

class SDLstuff{
	public:
		SDLstuff();
		~SDLstuff();

		void init();
		void checkEvents();

		void grabKeyAndMouse();
		void ungrabKeyAndMouse();
		void closeSDL_Net();
    SDL_Window* getMainWindow(){ return mainWindow; }

    void printResolution();
    void tweakFontSize();

	private:

		SDL_Event event;
		SDL_Window* mainWindow;
    SDL_GLContext sdl_glContext;

		void handleKeyPressEvent(SDL_Keysym * keysym);
		void handleKeyReleaseEvent(SDL_Keysym * keysym);
		void handleMousePressEvent(SDL_MouseButtonEvent * mouse);
		void handleMouseReleaseEvent(SDL_MouseButtonEvent * mouse);
    void handleMouseMotionEvent(SDL_MouseMotionEvent* mouseMotion);

    char title[16];
};

#endif
