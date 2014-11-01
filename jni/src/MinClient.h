#ifndef _MAIN_H
#define _MAIN_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if defined(_WIN32) 
	#include <windows.h>
#endif

#include <SDL.h>
#include "Engine.h"
#include "Camera.h"
#include "OpenGLstuff.h"
#include "SDLstuff.h"
#include "NetworkStuff.h"
#include "Font.h"
#include "Profiler.h"
#include "ProfileLogHandler.h"
#include "OpenGLES2stuff.h"

using namespace std;

int main(int argc, char *argv[]);

void doGameLoop(SDLstuff* sdlstuff, Camera* camera, OpenGLstuff* openglstuff, NetworkStuff* networkStuff);

#endif 

