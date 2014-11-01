#include "MinClient.h"

int main(int argc, char *argv[]){
  Engine::init();

  Camera camera;
  SDLstuff sdlstuff;
  OpenGLstuff openglstuff(sdlstuff.getMainWindow());	
  OpenGLES2stuff::init();

  Font::init(sdlstuff.getMainWindow());

  sdlstuff.tweakFontSize();
  sdlstuff.printResolution();

  SDL_Delay(1000);     // HACKKKKKKKKKK

  openglstuff.printCompressedTextureAvailability();

  NetworkStuff networkStuff(&camera, &openglstuff);

	CProfileLogHandler profileLogHandler;
	CProfileSample::outputHandler= &profileLogHandler;

  // main game loop
  while(!Engine::done)  {
		doGameLoop(&sdlstuff, &camera, &openglstuff, &networkStuff);
	}

	sdlstuff.closeSDL_Net();

  sdlstuff.ungrabKeyAndMouse();

	cout << "MinClient: Leaving now..." << endl;  

  // without the exit here it hangs in android
  exit(1);


  return 0;
}

void doGameLoop(SDLstuff* sdlstuff, Camera* camera, OpenGLstuff* openglstuff, NetworkStuff* networkStuff){	
  {  
    CProfileSample mainLoop("GameLoop"); 

    {
		  CProfileSample sdl("SDL CheckEvents");
	    // check for inputs (e.g. keyboard)
	    sdlstuff->checkEvents();	
    }

    Engine::calculateFrameRate();

    {
		  CProfileSample cam("Camera Updates");
      camera->update();
    }

    {
#ifdef NETWORK_ON
		  CProfileSample sendMessage("SendMessageToRenderServers");
      networkStuff->sendMessageToRenderServers();  
#endif
    }

	  // only display frame after you got one
	  int framesToWait = Engine::numServers * Engine::serverFrameBuffers;

	  if(Engine::numFramesRendered >= framesToWait){
#ifdef NETWORK_ON
      {
		    CProfileSample receiveMessage("ReceiveMessageFromRenderServer");
        networkStuff->receiveMessageFromRenderServer();
      }
#endif

      {
		    CProfileSample render("openglstuff render()");
		    openglstuff->render();
      }

      {
		    CProfileSample swap("swapBuffers()");
		    openglstuff->swapBuffers();
      }
	  }

	  Engine::increaseNumFramesRendered();
  }

	CProfileSample::Output();

	if(Engine::nextFrameStartProfiler){
		CProfileSample::bProfilerIsRunning = true;
		Engine::nextFrameStartProfiler = false;
	} else if(Engine::nextFrameStopProfiler){
		CProfileSample::bProfilerIsRunning = false;
		Engine::nextFrameStartProfiler = false;
	}
}
