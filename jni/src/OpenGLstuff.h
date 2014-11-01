#ifndef _OPENGLSTUFF_H
#define _OPENGLSTUFF_H

#define GL_GLEXT_PROTOTYPES

#if defined(_WIN32) 
	#include <windows.h>
#endif

#ifdef ANDROID
//  #include <GLES/gl.h>
  #include <GLES2/gl2.h>
  #include <GLES2/gl2ext.h>
  #include <android/log.h>
#else
  #include <GL/gl.h>
#endif

#include <SDL.h>					
#include "Engine.h"
#include "Font.h"
#include "Profiler.h"
#include "Etc1.h"
#include "OpenGLES2stuff.h"

#if defined(_WIN32)
	#include <glext.h>   // used for GL_BGRA
#endif

#ifndef COMPRESSED_RGBA_S3TC_DXT1_EXT
	#define COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#endif
class OpenGLstuff{
	public:
		OpenGLstuff(SDL_Window* mainWindow_);
		~OpenGLstuff();

		void init();

		void render();
		void swapBuffers();
    void printCompressedTextureAvailability();

		void setExitKeyPressedTime(float time);	

		unsigned char *frameBufferPointer;
    unsigned char *frameBufferPointerRect[MAX_SERVERS];
    unsigned int framebufferTexIDRect[MAX_SERVERS];  

#if defined(_WIN32) 
		PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
    PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D;
#endif

	private:		
    SDL_Window* mainWindow;

		void generateFramebufferTexture();
    void generateFramebufferTextureNoRect();
    void generateFramebufferTextureRect();
		unsigned int framebufferTexID;  
    

    void drawFrameBufferNoRect();
    void drawFrameBufferRect();
    void drawFPS();
    void drawProfilerOutput();
    void drawHUD();
    
    bool isPVRTCSupported();
    bool isPVRTC2Supported();
    bool isS3TCSupported();
    bool isETCSupported();
    bool isDXT1Supported();
};


#endif
