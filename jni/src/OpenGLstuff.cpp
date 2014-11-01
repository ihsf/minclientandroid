#include "OpenGLstuff.h"

// GL_OES_compressed_ETC1_RGB8_texture 
#ifndef GL_ETC1_RGB8_OES
  #define GL_ETC1_RGB8_OES                                        0x8D64
#endif


OpenGLstuff::OpenGLstuff(SDL_Window* mainWindow_){
  this->mainWindow = mainWindow_;

  frameBufferPointer = NULL;
  framebufferTexID = 0;

  for(int i = 0; i < MAX_SERVERS; i++){
    frameBufferPointerRect[i] = NULL;
    framebufferTexIDRect[i] = 0;
  }

#ifdef _WIN32
  glCompressedTexImage2D = NULL;
  glCompressedTexSubImage2D = NULL;
#endif

  init();
}

OpenGLstuff::~OpenGLstuff(){
}

void OpenGLstuff::init(){
#if defined(_WIN32) 
  glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)wglGetProcAddress("glCompressedTexImage2DARB");
	glCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)wglGetProcAddress("glCompressedTexSubImage2DARB");
#endif 

	int numBytesPerPixel = 4;

  if(!Engine::rectMode){
    if(!Engine::serverUseETC1 && !Engine::serverUseDXT1){
	    frameBufferPointer = new unsigned char[Engine::screenWidthRT * Engine::screenHeightRT * numBytesPerPixel];
	    memset(frameBufferPointer, 0, Engine::screenWidthRT * Engine::screenHeightRT * numBytesPerPixel);
    } else {
#ifdef ANDROID
	      frameBufferPointer = new unsigned char[(Engine::screenWidthRT * Engine::screenHeightRT * numBytesPerPixel) / 8];
	      memset(frameBufferPointer, 0, (Engine::screenWidthRT * Engine::screenHeightRT * numBytesPerPixel) / 8);
#else
	      frameBufferPointer = new unsigned char[Engine::screenWidthRT * Engine::screenHeightRT * numBytesPerPixel];
	      memset(frameBufferPointer, 0, Engine::screenWidthRT * Engine::screenHeightRT * numBytesPerPixel);
#endif
    }
  } else {
    for(int i = 0; i < Engine::numServers; i++){
      if(!Engine::serverUseETC1 && !Engine::serverUseDXT1){
        frameBufferPointerRect[i] = new unsigned char[Engine::rectSizeX[i] * Engine::rectSizeY[i] * numBytesPerPixel];
        memset(frameBufferPointerRect[i], 0, Engine::rectSizeX[i] * Engine::rectSizeY[i] * numBytesPerPixel);
      } else {
#ifdef ANDROID
        // android uses ETC1 nativley so we can assign 1/8 of the frame buffer
        frameBufferPointerRect[i] = new unsigned char[(Engine::rectSizeX[i] * Engine::rectSizeY[i] * numBytesPerPixel) / 8];
        memset(frameBufferPointerRect[i], 0, (Engine::rectSizeX[i] * Engine::rectSizeY[i] * numBytesPerPixel) / 8);
#else
        // everyone else decompresses into RGBA and needs the full frame buffer
        frameBufferPointerRect[i] = new unsigned char[Engine::rectSizeX[i] * Engine::rectSizeY[i] * numBytesPerPixel];
        memset(frameBufferPointerRect[i], 0, Engine::rectSizeX[i] * Engine::rectSizeY[i] * numBytesPerPixel);
#endif
      }
    }
  }

	glViewport(0, 0, Engine::screenWidthGL, Engine::screenHeightGL);

#ifndef ANDROID
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
#endif

  float orthoMatrix[16];
  memset(orthoMatrix, 0, sizeof(orthoMatrix));
  Engine::calculateOrthoMatrix(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f, orthoMatrix);

#ifndef ANDROID
  glMultMatrixf(orthoMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
#endif
  glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	generateFramebufferTexture();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
  swapBuffers();
  glClear(GL_COLOR_BUFFER_BIT);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE); 
}

void OpenGLstuff::generateFramebufferTexture(){
  if(!Engine::rectMode){
    generateFramebufferTextureNoRect();
  } else {
    generateFramebufferTextureRect();
  }
}

void OpenGLstuff::generateFramebufferTextureNoRect(){
	glGenTextures(1, &framebufferTexID);

	glBindTexture(GL_TEXTURE_2D, framebufferTexID);
   
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if(!Engine::serverUseETC1 && !Engine::serverUseDXT1){
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Engine::screenWidthRT, Engine::screenHeightRT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  } else {
#ifdef ANDROID
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, Engine::screenWidthRT, Engine::screenHeightRT, 0,	(Engine::screenWidthRT * Engine::screenHeightRT) / 2, NULL);  
#else
    if(!Engine::serverUseDXT1){
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Engine::screenWidthRT, Engine::screenHeightRT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    } else {
      glCompressedTexImage2D(GL_TEXTURE_2D, 0, COMPRESSED_RGBA_S3TC_DXT1_EXT, Engine::screenWidthRT, Engine::screenHeightRT, 0,	(Engine::screenWidthRT * Engine::screenHeightRT) / 2, NULL);  
    }
#endif
  }
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLstuff::generateFramebufferTextureRect(){
  for(int i = 0; i < Engine::numServers; i++){
	  glGenTextures(1, &framebufferTexIDRect[i]);

	  glBindTexture(GL_TEXTURE_2D, framebufferTexIDRect[i]);

	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if(!Engine::serverUseETC1 && !Engine::serverUseDXT1){
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Engine::rectSizeX[i], Engine::rectSizeY[i], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    } else {
#ifdef ANDROID
      glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, Engine::rectSizeX[i], Engine::rectSizeY[i], 0,	(Engine::rectSizeX[i] * Engine::rectSizeY[i]) / 2, NULL);  
#else
      if(!Engine::serverUseDXT1){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Engine::rectSizeX[i], Engine::rectSizeY[i], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
      } else {
        glCompressedTexImage2D(GL_TEXTURE_2D, 0, COMPRESSED_RGBA_S3TC_DXT1_EXT, Engine::rectSizeX[i], Engine::rectSizeY[i], 0,	(Engine::rectSizeX[i] * Engine::rectSizeY[i]) / 2, NULL);  
      }
#endif
    }
  }

	glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLstuff::render(){
  glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);

  {
  CProfileSample render1("drawFrameBuffer");
  if(!Engine::rectMode){
    drawFrameBufferNoRect();
  } else {
    drawFrameBufferRect();
  }            
  }

  {
  CProfileSample render2("drawFPS");
  drawFPS();
  }

  {
  CProfileSample render3("drawProfilerOutput");
  drawProfilerOutput();
  }

  {
  CProfileSample render4("drawHUD");
  drawHUD();
  }

  {
  CProfileSample render5("glPrintSavedLines");
  Font::glPrintSavedLines();
  }

  {
  CProfileSample render6("resetSavedLines");
  Font::resetSavedLines();
  }
}

// Optimize: Could preallocate vertices[MAX_SERVERS] and render all things while the client
// state is enabled
void OpenGLstuff::drawFrameBufferRect(){
#ifndef ANDROID
  glMatrixMode(GL_PROJECTION);									       
  glPushMatrix();													             
  glLoadIdentity();												             
#endif  

  float orthoMatrix[16];
  memset(orthoMatrix, 0, sizeof(orthoMatrix));
  Engine::calculateOrthoMatrix(0.0f, Engine::screenWidthGL, 0.0f, Engine::screenHeightGL, -1.0f, 1.0f, orthoMatrix);

#ifndef ANDROID
  glMultMatrixf(orthoMatrix);

  glMatrixMode(GL_MODELVIEW);										       
  glPushMatrix();													             
  glLoadIdentity();												             
#endif

  int horizontalOffset = (Engine::screenWidthGL  - Engine::screenWidthRT)  / 2;
  int verticalOffset   = (Engine::screenHeightGL - Engine::screenHeightRT) / 2;

  GLfloat texCoords[] = {0.0f,1.0f,  1.0f,1.0f,  1.0f,0.0f,   1.0f,0.0f,   0.0f,0.0f,  0.0f,1.0f};
  const float aspectRatio = (float)Engine::screenWidthRT / (float)Engine::screenHeightRT;

  for(int i = 0; i < Engine::numServers; i++){
	  // draw ray traced rectangle 
	  glBindTexture(GL_TEXTURE_2D, framebufferTexIDRect[i]);
    
    int startX = Engine::rectLeftServer[i];
    int startY = Engine::rectBottomServer[i];

    GLfloat vertices[]  = {startX + horizontalOffset,                               startY + (float)Engine::rectSizeY[i] + verticalOffset,  
                           startX + (float)Engine::rectSizeX[i] + horizontalOffset, startY + (float)Engine::rectSizeY[i] + verticalOffset,
                           startX + (float)Engine::rectSizeX[i] + horizontalOffset, startY + 0.0f + verticalOffset,
                           startX + (float)Engine::rectSizeX[i] + horizontalOffset, startY + 0.0f + verticalOffset,
                           startX + horizontalOffset,                               startY + 0.0f + verticalOffset,  
                           startX + horizontalOffset,                               startY + (float)Engine::rectSizeY[i] + verticalOffset };
/*
    if(Engine::upscale){      
      float zoomRatio;
      int newHeight;
      int newWidth;
      int newVerticalOffset;
      int newHorizontalOffset;

      if(horizontalOffset > (float)verticalOffset * aspectRatio){
        // use full vertical space
        newHeight = Engine::screenHeightGL;
        zoomRatio = newHeight / (float)Engine::screenHeightRT;
        newWidth = Engine::screenWidthRT * zoomRatio;  

        newVerticalOffset = 0;
        newHorizontalOffset = (Engine::screenWidthGL  - newWidth) / 2;
      } else {
        // use full horizontal space     
        newWidth = Engine::screenWidthGL;
        zoomRatio = newWidth / (float)Engine::screenWidthRT;
        newHeight = Engine::screenHeightRT * zoomRatio;      

        newVerticalOffset = (Engine::screenHeightGL  - newHeight) / 2;
        newHorizontalOffset = 0; 
      }

      vertices[0]  = 0.0f + newHorizontalOffset;                         vertices[1]  = newHeight + newVerticalOffset; 
      vertices[2]  = newWidth + newHorizontalOffset;                     vertices[3]  = newHeight + newVerticalOffset;  
      vertices[4]  = newWidth + newHorizontalOffset;                     vertices[5]  = 0.0f + newVerticalOffset;
      vertices[6]  = newWidth + newHorizontalOffset;                     vertices[7]  = 0.0f + newVerticalOffset;   
      vertices[8]  = 0.0f + newHorizontalOffset;                         vertices[9]  = 0.0f + newVerticalOffset;  
      vertices[10] = 0.0f + newHorizontalOffset;                         vertices[11] = newHeight + newVerticalOffset;
    }   
*/

#ifndef ANDROID
	  glEnableClientState(GL_VERTEX_ARRAY);
	  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   
	  glVertexPointer(2, GL_FLOAT, 0,vertices);
	  glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
   
	  glDrawArrays(GL_TRIANGLES,0,6);  
 
	  glDisableClientState(GL_VERTEX_ARRAY);
	  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#else
    glUseProgram(OpenGLES2stuff::programID);

    glVertexAttribPointer(OpenGLES2stuff::gPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(OpenGLES2stuff::gPositionHandle);

    glVertexAttribPointer(OpenGLES2stuff::gTexCoordHandle, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
    glEnableVertexAttribArray(OpenGLES2stuff::gTexCoordHandle);

    glUniformMatrix4fv(OpenGLES2stuff::uMatrixLocation, 1, false,      orthoMatrix);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(OpenGLES2stuff::uTextureUnitLocation, 0);
   
	  glDrawArrays(GL_TRIANGLES,0,6);  
#endif
  }

#ifndef ANDROID
  glPopMatrix();	               
  glMatrixMode(GL_PROJECTION);									      
  glPopMatrix();								
#endif
}

void OpenGLstuff::drawFrameBufferNoRect(){
#ifndef ANDROID
	glMatrixMode(GL_PROJECTION);									       
	glPushMatrix();													              
	glLoadIdentity();												              
 #endif  

  float orthoMatrix[16];
  memset(orthoMatrix, 0, sizeof(orthoMatrix));
  Engine::calculateOrthoMatrix(0.0f, Engine::screenWidthGL, 0.0f, Engine::screenHeightGL, -1.0f, 1.0f, orthoMatrix);

#ifndef ANDROID
  glMultMatrixf(orthoMatrix);
  glMatrixMode(GL_MODELVIEW);										        
	glPushMatrix();													              
	glLoadIdentity();												            
#endif

  int horizontalOffset = (Engine::screenWidthGL  - Engine::screenWidthRT)  / 2;
  int verticalOffset   = (Engine::screenHeightGL - Engine::screenHeightRT) / 2;

	// draw ray traced rectangle 
	glBindTexture(GL_TEXTURE_2D, framebufferTexID);
#ifdef ANDROID
  if(!Engine::serverUseETC1){
	  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Engine::screenWidthRT, Engine::screenHeightRT, 0, GL_RGBA, GL_UNSIGNED_BYTE, frameBufferPointer);  
  } else {
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, Engine::screenWidthRT, Engine::screenHeightRT, 0,	(Engine::screenWidthRT * Engine::screenHeightRT) / 2, frameBufferPointer);  
  }
#else
  if(!Engine::serverUseDXT1){
    // OPTIMIZE: store copy buffers somewhere else
    unsigned char* copyBuffer = new unsigned char[(Engine::screenWidthRT * Engine::screenHeightRT * 4) / 8];
    memcpy(copyBuffer, frameBufferPointer, (Engine::screenWidthRT * Engine::screenHeightRT * 4) / 8);

    Etc1::convertETC1toRGBA(copyBuffer, frameBufferPointer, Engine::screenWidthRT, Engine::screenHeightRT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Engine::screenWidthRT, Engine::screenHeightRT, 0, GL_BGRA, GL_UNSIGNED_BYTE, frameBufferPointer);  
    delete[] copyBuffer;
  } else {
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, COMPRESSED_RGBA_S3TC_DXT1_EXT, Engine::screenWidthRT, Engine::screenHeightRT, 0,	(Engine::screenWidthRT * Engine::screenHeightRT) / 2, frameBufferPointer); 
		//glCompressedTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Engine::screenWidthRT, Engine::screenHeightRT, COMPRESSED_RGBA_S3TC_DXT1_EXT,	(Engine::screenWidthRT * Engine::screenHeightRT) / 2, frameBufferPointer); 
  }
#endif
  
  GLfloat vertices[]  = {0.0f + horizontalOffset,                  (float)Engine::screenHeightRT + verticalOffset,  
                  (float)Engine::screenWidthRT + horizontalOffset, (float)Engine::screenHeightRT + verticalOffset,
                  (float)Engine::screenWidthRT + horizontalOffset, 0.0f + verticalOffset,
                  (float)Engine::screenWidthRT + horizontalOffset, 0.0f + verticalOffset,
                  0.0f + horizontalOffset,                         0.0f + verticalOffset,  
                  0.0f + horizontalOffset,                         (float)Engine::screenHeightRT + verticalOffset };

  if(Engine::upscale){
    const float aspectRatio = (float)Engine::screenWidthRT / (float)Engine::screenHeightRT;
    float zoomRatio;
    int newHeight;
    int newWidth;
    int newVerticalOffset;
    int newHorizontalOffset;

    if(horizontalOffset > (float)verticalOffset * aspectRatio){
      // use full vertical space
      newHeight = Engine::screenHeightGL;
      zoomRatio = newHeight / (float)Engine::screenHeightRT;
      newWidth = Engine::screenWidthRT * zoomRatio;  

      newVerticalOffset = 0;
      newHorizontalOffset = (Engine::screenWidthGL  - newWidth) / 2;
    } else {
      // use full horizontal space     
      newWidth = Engine::screenWidthGL;
      zoomRatio = newWidth / (float)Engine::screenWidthRT;
      newHeight = Engine::screenHeightRT * zoomRatio;      

      newVerticalOffset = (Engine::screenHeightGL  - newHeight) / 2;
      newHorizontalOffset = 0; 
    }

    vertices[0]  = 0.0f + newHorizontalOffset;                         vertices[1]  = newHeight + newVerticalOffset; 
    vertices[2]  = newWidth + newHorizontalOffset;                     vertices[3]  = newHeight + newVerticalOffset;  
    vertices[4]  = newWidth + newHorizontalOffset;                     vertices[5]  = 0.0f + newVerticalOffset;
    vertices[6]  = newWidth + newHorizontalOffset;                     vertices[7]  = 0.0f + newVerticalOffset;   
    vertices[8]  = 0.0f + newHorizontalOffset;                         vertices[9]  = 0.0f + newVerticalOffset;  
    vertices[10] = 0.0f + newHorizontalOffset;                         vertices[11] = newHeight + newVerticalOffset;
  }

  GLfloat texCoords[] = {0.0f,1.0f,  1.0f,1.0f,  1.0f,0.0f,   1.0f,0.0f,   0.0f,0.0f,  0.0f,1.0f};

#ifndef ANDROID
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 
	glVertexPointer(2, GL_FLOAT, 0,vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
 
	glDrawArrays(GL_TRIANGLES,0,6);  
 
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#else
  glUseProgram(OpenGLES2stuff::programID);

  glVertexAttribPointer(OpenGLES2stuff::gPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, vertices);
  glEnableVertexAttribArray(OpenGLES2stuff::gPositionHandle);

  glVertexAttribPointer(OpenGLES2stuff::gTexCoordHandle, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
  glEnableVertexAttribArray(OpenGLES2stuff::gTexCoordHandle);

  glUniformMatrix4fv(OpenGLES2stuff::uMatrixLocation, 1, false, orthoMatrix);

  glActiveTexture(GL_TEXTURE0);
  glUniform1i(OpenGLES2stuff::uTextureUnitLocation, 0);
   
	glDrawArrays(GL_TRIANGLES,0,6);  
#endif

#ifndef ANDROID
  glPopMatrix();	               
	glMatrixMode(GL_PROJECTION);									      
	glPopMatrix();								  
#endif
}

void OpenGLstuff::drawProfilerOutput(){
  Font::resetNumLinesPrinted();

  for(int i = 0; i < (int)Engine::profilerOutput.size(); i++){
    Font::glPrint(10, Font::AUTO, Engine::profilerOutput[i], false);
    delete [] Engine::profilerOutput[i];
  }

  Engine::profilerOutput.clear();
}

void OpenGLstuff::drawHUD(){
  Engine::buttonLeftCenter.x = 150.0f;
  Engine::buttonLeftCenter.y = 100.0f;

  Engine::buttonRightCenter.x = 400.0f;
  Engine::buttonRightCenter.y = 100.0f;

  // lenovo k900  
  if(Engine::screenHeightGL == 1920 && Engine::screenWidthGL == 1080){
    Engine::buttonLeftCenter.x = 175.0f;
    Engine::buttonLeftCenter.y = 175.0f;

    Engine::buttonRightCenter.x =  Engine::screenWidthGL - 200.0f;
    Engine::buttonRightCenter.y = 175.0f;
  }

  if(Engine::screenWidthGL > 512){
    Font::glPrint(Engine::buttonLeftCenter.x, Engine::buttonLeftCenter.y + Engine::fontSize, " o ", false);
    Font::glPrint(Engine::buttonLeftCenter.x, Engine::buttonLeftCenter.y, "ooo", false);
    Font::glPrint(Engine::buttonLeftCenter.x, Engine::buttonLeftCenter.y - Engine::fontSize, " o ", false);
  }

  //Font::glPrint(Engine::buttonRightCenter.x, Engine::buttonRightCenter.y + Engine::fontSize, " o ", false);
  //Font::glPrint(Engine::buttonRightCenter.x, Engine::buttonRightCenter.y,                    "ooo", false);
  //Font::glPrint(Engine::buttonRightCenter.x, Engine::buttonRightCenter.y - Engine::fontSize, " o ", false);
}

void OpenGLstuff::swapBuffers(){
	SDL_GL_SwapWindow(mainWindow); 
}

void OpenGLstuff::drawFPS(){
	const int width = Engine::screenWidthGL;
	const int height = Engine::screenHeightGL;

  int widthToPrint = width - width/10;
  if(width < 512)
    widthToPrint = width - width/5;

  Font::glPrint(widthToPrint, height - height/24, Engine::strFrameRate, 0);
}

bool OpenGLstuff::isETCSupported(){
	int count = 0;
	int GLEW_OES_compressed_ETC1_RGB8_texture = 0;

	glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &count);

	if(count > 0){
    GLint* formats = (GLint*)calloc(count, sizeof(GLint));
		glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, formats);

    int index;
    for(index = 0; index < count; index++){
			switch(formats[index]){
				case GL_ETC1_RGB8_OES:
					GLEW_OES_compressed_ETC1_RGB8_texture = 1;
					break;
			}
		}

		free(formats);
	}

  if(GLEW_OES_compressed_ETC1_RGB8_texture)
    return true;
  else
    return false;
}

bool OpenGLstuff::isPVRTCSupported(){
  bool PVRTCSupported = false;

  const GLubyte* pExtensions = glGetString(GL_EXTENSIONS);
  PVRTCSupported = (strstr((char*)pExtensions, "GL_IMG_texture_compression_pvrtc") != NULL);

  return PVRTCSupported;
}

bool OpenGLstuff::isPVRTC2Supported(){
  bool PVRTC2Supported = false;

  const GLubyte* pExtensions = glGetString(GL_EXTENSIONS);
  PVRTC2Supported = (strstr((char*)pExtensions, "GL_IMG_texture_compression_pvrtc2") != NULL);

  return PVRTC2Supported;
}

bool OpenGLstuff::isS3TCSupported(){
  bool S3TCSupported = false;

  const GLubyte* pExtensions = glGetString(GL_EXTENSIONS);
  S3TCSupported = (strstr( (char*)pExtensions, "GL_EXT_texture_compression_s3tc" ) != NULL);
  
  return S3TCSupported;
}

bool OpenGLstuff::isDXT1Supported(){
  bool DXT1Supported = false;

  const GLubyte* pExtensions = glGetString(GL_EXTENSIONS);
  DXT1Supported = (strstr( (char*)pExtensions, "GL_EXT_texture_compression_dxt1" ) != NULL);
  
  return DXT1Supported;
}

void OpenGLstuff::printCompressedTextureAvailability(){
  char output[64];
  strcpy(output, "Comp. tex: ");
  if(isETCSupported()){
    strcat(output, "ETC1 ");
  }
  if(isPVRTCSupported()){
    strcat(output, "PVRTC ");
  }
  if(isPVRTC2Supported()){
    strcat(output, "PVRTC2 ");
  }
  if(isS3TCSupported()){
    strcat(output, "S3TC ");
  }
  if(isDXT1Supported()){
    strcat(output, "DXT1 ");
  }

  Font::glPrint(10, Font::AUTO, output, true);

  if(Engine::serverUseETC1){
    if(!isETCSupported()){
      Font::glPrint(10, Font::AUTO, "Trying to use ETC1, but not supported in hardware.", true);
#ifdef ANDROID      
      //SDL_Delay(2000);
      //exit(1);
#endif
    }
  }
}
