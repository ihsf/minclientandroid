#include "SDLstuff.h"
#include "SDL_syswm.h"

SDLstuff::SDLstuff(){	
	mainWindow = NULL;
  sdl_glContext = NULL;

  init();
}

SDLstuff::~SDLstuff(){
  //SDL_GL_DeleteContext(sdl_glContext);
  SDL_DestroyWindow(mainWindow);
}

void SDLstuff::init(){
	if( SDL_Init( SDL_INIT_VIDEO  | SDL_INIT_NOPARACHUTE  ) < 0 ) {
		cerr << "Failed initializing SDL Video: " << SDL_GetError() << endl;
		exit(1);
	} 

  SDL_DisplayMode mode;
/*
  int defaultdisplay = SDL_GetDisplayMode(0, 0, &mode);

  // DANIEL: change development 30 inch dell screen to 1920x1080 so the screen is not fully filled
  if(mode.w == 2560 && mode.h == 1600){
    mode.w = 1920;
    mode.h = 1080;
  }

  mode.w = 240;
  mode.h = 240;

  Engine::screenWidthGL = mode.w;
  Engine::screenHeightGL = mode.h;
  */
  cout << "Display mode: " << Engine::screenWidthGL << " x " << Engine::screenHeightGL << endl;
  cout << "Render mode: " << Engine::screenWidthRT << " x " << Engine::screenHeightRT << endl;

  int videoFlags = SDL_WINDOW_OPENGL;
  if(Engine::fullscreen)
    videoFlags |= SDL_WINDOW_FULLSCREEN;

  // SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#ifdef ANDROID
  // force fullscreen on android
  videoFlags |= SDL_WINDOW_FULLSCREEN;
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

  strcpy(title, "MinClient");
  sprintf(title, "MinClient %s", MINCLIENT_VERSION);

  mainWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Engine::screenWidthGL, Engine::screenHeightGL, videoFlags);

  if(!mainWindow){
    cout << "Failed to open window: " << SDL_GetError() << endl;
    exit(0);
  }

  sdl_glContext = SDL_GL_CreateContext(mainWindow);

  if(!sdl_glContext){
    cout << "Failed to create OpenGL Context: " << SDL_GetError() << endl;
    exit(0);
  }
}

void SDLstuff::tweakFontSize(){
  if(Engine::screenWidthGL < 512 || Engine::screenHeightGL < 512){
    Engine::fontSize = 16;
  }
  if (Engine::screenWidthGL < 256 || Engine::screenHeightGL < 256){
    Engine::fontSize = 10;
  }
}

void SDLstuff::printResolution(){
  char output[128];
  sprintf(output, "%ix%i display mode", Engine::screenWidthGL, Engine::screenHeightGL);
  Font::glPrint(10, Font::AUTO, output, true);

  sprintf(output, "%ix%i render mode (2^n)", Engine::screenWidthRT, Engine::screenHeightRT);
  Font::glPrint(10, Font::AUTO, output, true);

  sprintf(output, "%ix%i display mode", Engine::screenWidthGL, Engine::screenHeightGL);
  Font::glPrint(10, Font::AUTO, output, true);
}

void SDLstuff::checkEvents(){
	while( SDL_PollEvent(& event) ) {
    switch(event.type){
      case SDL_KEYDOWN:
        handleKeyPressEvent(&event.key.keysym);
        break;

      case SDL_KEYUP:
        handleKeyReleaseEvent(&event.key.keysym);
        break;

      case SDL_MOUSEBUTTONDOWN:
        handleMousePressEvent(&event.button);
        break;

      case SDL_MOUSEBUTTONUP:
        handleMouseReleaseEvent(&event.button);
        break;

      case SDL_MOUSEMOTION:
        handleMouseMotionEvent(&event.motion);
        break;

      default:
        break;
    } 
	}
}

void SDLstuff::handleMouseMotionEvent(SDL_MouseMotionEvent* mouseMotion){
  Engine::relativeMouseMotion.x = mouseMotion->xrel;
  Engine::relativeMouseMotion.y = mouseMotion->yrel;

  if(Engine::buttonPressed != CVector2(-666.6f, -666.6f)){
    Engine::buttonPressed = Engine::buttonPressed + Engine::relativeMouseMotion;
  }
}

void SDLstuff::handleMousePressEvent(SDL_MouseButtonEvent* mouse){
/*
	if((mouse->button & SDL_BUTTON_LEFT) == SDL_BUTTON_LEFT)
		Engine::leftButton = true;
	else if((mouse->button & SDL_BUTTON_RIGHT) == SDL_BUTTON_RIGHT)
		Engine::rightButton = true;
*/
  Engine::buttonPressed = CVector2(mouse->x, mouse->y);

  Engine::clicksPerSecond++;
  
  // exit if frequently clicked
  if(Engine::clicksPerSecond > 4)
    Engine::done = true;
}

void SDLstuff::handleMouseReleaseEvent(SDL_MouseButtonEvent* mouse){
/*
	if((mouse->button & SDL_BUTTON_LEFT) == SDL_BUTTON_LEFT)
		Engine::leftButton = false;
	else if((mouse->button & SDL_BUTTON_RIGHT) == SDL_BUTTON_RIGHT)
		Engine::rightButton = false;
*/

  Engine::buttonPressed = CVector2(-666.6f, -666.6f);
//#ifdef ANDROID
  Engine::upKey = false;
  Engine::downKey = false; 
  Engine::leftKey = false; 
  Engine::rightKey = false;
  Engine::jumpKey = false;
  Engine::crouchKey = false;
//#endif
}

void SDLstuff::handleKeyPressEvent(SDL_Keysym * keysym){  
	SDL_Keycode key = keysym->sym;
   
	switch(key){
		case SDLK_ESCAPE:                                  
		  Engine::done = true;		
			break;

    case SDLK_UP:                                    
    case SDLK_w:
			Engine::upKey = true;                      
      break;
        
		case SDLK_DOWN:                                   
		case SDLK_s:
			Engine::downKey = true; 
      break;

		case SDLK_RIGHT:                                  
		case SDLK_d:                 
      Engine::leftKey = true;
      break;

		case SDLK_LEFT:                                   
		case SDLK_a:
			Engine::rightKey = true; 
      break;

	  case SDLK_SPACE:
		  Engine::jumpKey = true;
			break;

	  case SDLK_c:
		  Engine::crouchKey = true;
			break;

	  case SDLK_p:
      // profiler
      if(CProfileSample::bProfilerIsRunning)
				Engine::nextFrameStopProfiler = true;
			else
				Engine::nextFrameStartProfiler = true;

      //CProfileSample::ResetAll();

			break;

		default:                                       
			break;                                    
		}
}

void SDLstuff::handleKeyReleaseEvent(SDL_Keysym * keysym){
	SDL_Keycode key = keysym->sym;

  switch(key){
		case SDLK_UP:                       
		case SDLK_w:
				Engine::upKey = false;      
			break;
        
		case SDLK_DOWN:                    
		case SDLK_s:
				Engine::downKey = false;     
			break;

		case SDLK_RIGHT:                   
		case SDLK_d:
			Engine::leftKey = false;    
			break;

		case SDLK_LEFT:                    
		case SDLK_a:
			Engine::rightKey = false;   
			break;

		case SDLK_SPACE:
			Engine::jumpKey = false;
			break;

		case SDLK_c:
			Engine::crouchKey = false;
			break;

		default:                       
			break;                    
	} 
}

void SDLstuff::grabKeyAndMouse(){
  SDL_SetRelativeMouseMode(SDL_TRUE);
}

void SDLstuff::ungrabKeyAndMouse(){
  SDL_SetRelativeMouseMode(SDL_FALSE);
}

void SDLstuff::closeSDL_Net(){
	for(int i = 0; i < Engine::numServers; i++){
		SDLNet_TCP_Close(Engine::socketDescriptor[i]);
	}

	SDLNet_Quit();
}


