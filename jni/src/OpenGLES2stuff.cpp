#include "OpenGLES2stuff.h"

#ifdef ANDROID
  #include <android/log.h>
  #define  LOG_TAG    "org.ivci.qwrt"
  #define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#endif

unsigned int OpenGLES2stuff::programID = 0;
int OpenGLES2stuff::gPositionHandle = 0;
int OpenGLES2stuff::gTexCoordHandle = 0;
int OpenGLES2stuff::uMatrixLocation = 0;
int OpenGLES2stuff::uTextureUnitLocation = 0;
vector<GLuint> OpenGLES2stuff::programIDs;
vector<GLuint> OpenGLES2stuff::shaderIDs;

#if defined(_WIN32) 
PFNGLCREATESHADERPROC OpenGLES2stuff::glCreateShader = NULL;
PFNGLDELETESHADERPROC  OpenGLES2stuff::glDeleteShader = NULL;
PFNGLSHADERSOURCEARBPROC OpenGLES2stuff::glShaderSource = NULL;
PFNGLCOMPILESHADERARBPROC OpenGLES2stuff::glCompileShader = NULL;
PFNGLGETSHADERIVPROC OpenGLES2stuff::glGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC OpenGLES2stuff::glGetShaderInfoLog = NULL;
PFNGLCREATEPROGRAMPROC OpenGLES2stuff::glCreateProgram = NULL;
PFNGLATTACHSHADERPROC OpenGLES2stuff::glAttachShader = NULL;
PFNGLLINKPROGRAMPROC OpenGLES2stuff::glLinkProgram = NULL;
PFNGLGETPROGRAMIVPROC OpenGLES2stuff::glGetProgramiv = NULL;
PFNGLGETPROGRAMINFOLOGPROC OpenGLES2stuff::glGetProgramInfoLog = NULL;
PFNGLUSEPROGRAMPROC OpenGLES2stuff::glUseProgram = NULL;
PFNGLDELETEPROGRAMPROC OpenGLES2stuff::glDeleteProgram = NULL;
#endif


OpenGLES2stuff::~OpenGLES2stuff(){
  for (int i = 0; i < (int)shaderIDs.size(); i++){
    glDeleteShader(shaderIDs[i]);
  }  

  for (int i = 0; i < (int)programIDs.size(); i++){
    glDeleteProgram(programIDs[i]);
  }
}



void OpenGLES2stuff::init(){
#if defined(_WIN32) 
  // for loading vertex and fragment shaders
  glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
  glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
  glShaderSource = (PFNGLSHADERSOURCEARBPROC)wglGetProcAddress("glShaderSource"); 
  glCompileShader = (PFNGLCOMPILESHADERARBPROC)wglGetProcAddress("glCompileShader");
  glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
  glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
  glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
  glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
  glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
  glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
  glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
  glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
  glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
#endif

#ifdef ANDROID
  programID = createProgram(gVertexShader, gFragmentShader);
 // if(!programID)
//    exit(2);

  gPositionHandle = glGetAttribLocation(programID, "a_Position");
//  if(!gPositionHandle)
//    exit(3);

  gTexCoordHandle = glGetAttribLocation(programID, "a_TextureCoordinates");
//  if(!gTexCoordHandle)
//    exit(4);

  uMatrixLocation = glGetUniformLocation(programID, "u_Matrix");
//  if(!uMatrixLocation)
//    exit(5);

  uTextureUnitLocation = glGetUniformLocation(programID, "u_TextureUnit");
//  if(!uTextureUnitLocation)
//    exit(6);
#endif 
}

GLuint OpenGLES2stuff::loadShader(GLenum shaderType, const char* source) {
  GLuint shader = glCreateShader(shaderType);
  if(shader){
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if(!compiled) {
      GLint infoLength = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);

      if(infoLength) {
        char* buf = (char*)malloc(infoLength);
        if (buf) {
          glGetShaderInfoLog(shader, infoLength, NULL, buf);
          printf("Error compiling %i:\n%s\n", shaderType, buf);
#ifdef ANDROID
          LOGI("Error compiling %i:\n%s\n", shaderType, buf);
#endif
          free(buf);
        }
        glDeleteShader(shader);
        shader = 0;

        SDL_Delay(1000);
        exit(7);
      }
    }
  }

  shaderIDs.push_back(shader);

  return shader;
}

GLuint OpenGLES2stuff::createProgram(const char* pVertexSource, const char* pFragmentSource) {
  GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
  if(!vertexShader){
    return 0;
  }

  GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
  if(!pixelShader){
    return 0;
  }

  GLuint program = glCreateProgram();
  if(program){
    glAttachShader(program, vertexShader);
    glAttachShader(program, pixelShader);

    glLinkProgram(program);
    GLint linkStatus = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

    if(linkStatus != GL_TRUE) {
      exit(8);
      GLint bufLength = 0;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);

      if (bufLength) {
        char* buf = (char*) malloc(bufLength);
        if(buf) {
          glGetProgramInfoLog(program, bufLength, NULL, buf);
          printf("Could not link program: %s\n", buf);
          free(buf);
        }
      }
      glDeleteProgram(program);
      program = 0;
    }
  }

  programIDs.push_back(program);

  return program;
}