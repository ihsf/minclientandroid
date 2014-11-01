#ifndef _OPENGLES2STUFF_H
#define _OPENGLES2STUFF_H

// loadShader() and createProgram() from Google Android NDK
// license: Apache 2.0, https://developer.android.com/license.html
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

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <vector>
#include <SDL.h>

using namespace std;

#if defined(_WIN32)
	#include <glext.h>   // used for GL_BGRA
#endif

/*
static const char gVertexShader[] =
"attribute vec4 vPosition;\n"
"void main() {\n"
"  gl_Position = vPosition;\n"
"}\n";

static const char gFragmentShader[] =
"precision mediump float;\n"
"void main() {\n"
"  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
"}\n";
*/

static const char gVertexShader[] = 
    "uniform mediump mat4 u_Matrix;\n"
    "\n"
    "attribute mediump vec4 a_Position;\n"
    "attribute mediump vec2 a_TextureCoordinates;\n"
    "\n"
    "varying mediump vec2 v_TextureCoordinates;\n"
    "\n"
    "void main(){\n"
    "  v_TextureCoordinates = a_TextureCoordinates;\n"
    "  gl_Position = u_Matrix * a_Position;\n"
    "}\n";

static const char gFragmentShader[] = 
    "uniform sampler2D u_TextureUnit;\n"
    "varying mediump vec2 v_TextureCoordinates;\n"
    "\n"
    "void main(){\n"
    "  gl_FragColor = texture2D(u_TextureUnit, v_TextureCoordinates);\n"
    "}";

class OpenGLES2stuff{
	public:
		OpenGLES2stuff();
		~OpenGLES2stuff();

    static void init();
    static unsigned int programID;
    static int gPositionHandle;
    static int gTexCoordHandle;
    static int uMatrixLocation;
    static int uTextureUnitLocation;

  private:
    static GLuint loadShader(GLenum shaderType, const char* pSource);  
    static GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);

#if defined(_WIN32) 
    static PFNGLCREATESHADERPROC glCreateShader;
    static PFNGLDELETESHADERPROC  glDeleteShader;
    static PFNGLSHADERSOURCEARBPROC glShaderSource; 
    static PFNGLCOMPILESHADERARBPROC glCompileShader;
    static PFNGLGETSHADERIVPROC glGetShaderiv;
    static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
    static PFNGLCREATEPROGRAMPROC glCreateProgram;
    static PFNGLATTACHSHADERPROC glAttachShader;
    static PFNGLLINKPROGRAMPROC glLinkProgram;
    static PFNGLGETPROGRAMIVPROC glGetProgramiv;
    static PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
    static PFNGLUSEPROGRAMPROC glUseProgram;
    static PFNGLDELETEPROGRAMPROC glDeleteProgram;
#endif

    static vector<GLuint> programIDs;
    static vector<GLuint> shaderIDs;
};


#endif
