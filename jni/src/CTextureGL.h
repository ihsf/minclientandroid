#ifndef CTEXTUREGL_H
#define CTEXTUREGL_H

#include <stdio.h>
#include <iostream>
#include "CTexture.h"
#include "Engine.h"

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

using namespace std;

class CTextureGL : public CTexture{
   public:
    CTextureGL(unsigned char* data_, int sizeX_, int sizeY_, int channels_);
	  CTextureGL();

	  ~CTextureGL();

	  void generateTexture();	  

   private:

};

#endif
