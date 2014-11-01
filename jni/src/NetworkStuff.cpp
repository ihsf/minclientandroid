#include "NetworkStuff.h"
#include <math.h>

/* GL_OES_compressed_ETC1_RGB8_texture */
#ifndef GL_ETC1_RGB8_OES
  #define GL_ETC1_RGB8_OES                                        0x8D64
#endif

NetworkStuff::NetworkStuff(Camera* camera_, OpenGLstuff* openglstuff_){
	this->camera = camera_;
	this->openglstuff = openglstuff_;

	serverLastRendered = Engine::numServers - 1;

	//int blackBarAtBottom = Engine::screenHeightRT % 32;
	//realHeightConsideringTileSizes = Engine::screenHeightRT - blackBarAtBottom;

  numBytesToReceive = 0;
  framesToWait = Engine::numServers * Engine::serverFrameBuffers;	
#ifdef NETWORK_ON
	init();	

  initMsgBufferSend();
#endif
}

NetworkStuff::~NetworkStuff(){
}

void NetworkStuff::init() {
	if(SDLNet_Init() < 0){
		cout << "SDLNet_Init: Failed\n" << endl;
		exit(1);
	}
 
  char output[256];
  sprintf(output, "Network protocol version: %i", NETWORK_PROTOCOL_VERSION);
  Font::glPrint(10, Font::AUTO, output, true);
  cout << "Network protocol version: " << NETWORK_PROTOCOL_VERSION << endl;

	for(int i = 0; i < Engine::numServers; i++){
    sprintf(output, "Trying to resolve host %s", Engine::serverName[i]);
    Font::glPrint(10, Font::AUTO, output, true);
		cout << "Trying to resolve host " << Engine::serverName[i] << endl;

		int result = 0;
		do {
      SDL_Delay(5);
			result = SDLNet_ResolveHost(&Engine::serverIP[i], Engine::serverName[i], Engine::serverPort[i]);
		} while (result != 0);
    sprintf(output, "Result ResolveHost %i", result);
    Font::glPrint(10, Font::AUTO, output, true);

    sprintf(output, "Trying to open socket for server %i", i);
    Font::glPrint(10, Font::AUTO, output, true);
		cout << "Trying to open socket for server " << i << endl;
		do {
      SDL_Delay(5);
			Engine::socketDescriptor[i] = SDLNet_TCP_Open(&Engine::serverIP[i]);
		} while (!Engine::socketDescriptor[i]);

    sprintf(output, "Done opening socket for server %i at port %i", i, Engine::serverPort[i]);
    Font::glPrint(10, Font::AUTO, output, true);
		cout << "Done opening socket for server " << i << " at port " << Engine::serverPort[i] << endl;
	}

  determineNumBytesToReceive();
	sendInitPackets();	
}

void NetworkStuff::sendInitPackets(){
	clientMessage msgBuffer;
	memset(&msgBuffer, 0, sizeof (clientMessage));

  // filling in msgBuffer  
  msgBuffer.magic = NETWORK_PROTCOL_MAGICK;
  msgBuffer.clientMessageLength = sizeof(clientMessage);
  msgBuffer.protocolVersion = NETWORK_PROTOCOL_VERSION;

	msgBuffer.cameraPosition = camera->getPosition();	
	msgBuffer.cameraView = msgBuffer.cameraPosition + camera->getDirectionNormalized();
  msgBuffer.cameraUp = camera->getUpVector();

	msgBuffer.screenWidthRT = Engine::screenWidthRT;
	msgBuffer.screenHeightRT = Engine::screenHeightRT;

  msgBuffer.rectLeft = 0;  // this could be overwritten below
  msgBuffer.rectTop = 0;
  msgBuffer.rectRight = 0;
  msgBuffer.rectBottom = 0;

  msgBuffer.currentTime = Engine::currentTime;
  msgBuffer.frameNr = Engine::numFramesRendered;

	for(int i = 0; i < Engine::numServers; i++){
		if(Engine::socketDescriptor[i]){
      msgBuffer.rectLeft = Engine::rectLeftServer[i];
      msgBuffer.rectTop = Engine::rectTopServer[i];
      msgBuffer.rectRight = Engine::rectRightServer[i];
      msgBuffer.rectBottom = Engine::rectBottomServer[i];

			if (SDLNet_TCP_Send(Engine::socketDescriptor[i], (void *)&msgBuffer, sizeof(msgBuffer)) < sizeof(msgBuffer)) {
        Font::glPrint(10, Font::AUTO, "sendInitPacket - SDLNet_TCP_Send: Error", true);
				cout << "sendInitPacket - SDLNet_TCP_Send: Error" << endl;
			}
		}
	}

  Font::glPrint(10, Font::AUTO, "initPackets sent.", true);
  cout << "initPackets sent." << endl;
}

void NetworkStuff::initMsgBufferSend(){
  memset(&msgBufferSend, 0, sizeof(msgBufferSend));

  msgBufferSend.magic = NETWORK_PROTCOL_MAGICK;
  msgBufferSend.clientMessageLength = sizeof(clientMessage);
  msgBufferSend.protocolVersion = NETWORK_PROTOCOL_VERSION;

  msgBufferSend.rectLeft = -1;
  msgBufferSend.rectTop = -1;
  msgBufferSend.rectRight = -1;
  msgBufferSend.rectBottom = -1;
}

void NetworkStuff::sendMessageToRenderServers(){
	msgBufferSend.cameraPosition = camera->getPosition();	
	msgBufferSend.cameraView = msgBufferSend.cameraPosition + camera->getDirectionNormalized();
  msgBufferSend.cameraUp = camera->getUpVector();
	msgBufferSend.screenWidthRT = Engine::screenWidthRT;
	msgBufferSend.screenHeightRT = Engine::screenHeightRT;
	msgBufferSend.currentTime = Engine::currentTime;
  msgBufferSend.frameNr = Engine::numFramesRendered;
  msgBufferSend.doexit = Engine::done;

  bool verbose = true;
  if(Engine::numFramesRendered > 4)
    verbose = false;

	for(int i = 0; i < Engine::numServers; i++){
		if(Engine::socketDescriptor[i]){
      if(verbose)
        Font::glPrint(10, Font::AUTO, "Starting to send message to render server", true);
			if (SDLNet_TCP_Send(Engine::socketDescriptor[i], (void *)&msgBufferSend, sizeof(msgBufferSend)) < sizeof(msgBufferSend)) {
				cout << "sendMessageToRenderServers - SDLNet_TCP_Send: Error" << endl;
			}
      if(verbose)
        Font::glPrint(10, Font::AUTO, "Sent message to render server", true);
		}
	}
}

bool NetworkStuff::determineIfThisFrameShouldBeRendered(int i){
  if(Engine::rectMode)
    return true;

	if(Engine::numFramesRendered % Engine::numServers == i){
		return true;
	} else {
		return false;
	}		
}

void NetworkStuff::receiveMessageFromRenderServer(){
	if(Engine::numServers == 0)
		return;

  if(Engine::numFramesRendered < framesToWait){
		return;
  }

  if(Engine::numFramesRendered <= 3)
    Font::glPrint(10, Font::AUTO, "Starting to receive message from render server", true);

  if(Engine::serverUseETC1 || Engine::serverUseDXT1){
    receiveMessageFromRenderServerETC1();
  } else {
    Font::glPrint(10, Font::AUTO, "Unknown codec from render server", true);
    cout << "Unknown codec from render server" << endl;
    SDL_Delay(1500);
    exit(1);
  }
}

void NetworkStuff::receiveMessageFromRenderServerETC1(){  
  if(!Engine::rectMode){
    receiveMessageFromRenderServerETC1NoRect();
  } else {
    receiveMessageFromRenderServerETC1Rect();
  }
}

void NetworkStuff::receiveMessageFromRenderServerETC1Rect(){
  // ToDo: Parallelize with cilk or openmp  - with OpenMP it seems to be getting slower on Android
//#pragma omp parallel for

  char debugString[64];

  for(int i = 0; i < Engine::numServers; i++){
    sprintf(debugString, "Receive TCP server %i", i);

    {
    CProfileSample receiveMessage1(debugString);
    unsigned char* dataPtr = openglstuff->frameBufferPointerRect[i]; 
    unsigned char* dataPtrTmp = dataPtr;

    // ToDo: receiving might be parallelized
    // ToDo: allows parallel uploading of the texture data
	  unsigned int numBytesToReceiveTemp = (Engine::rectSizeX[i] * Engine::rectSizeY[i]) / 2;
    
	  do {
		  int serverIDthatHasTheFrameRenderedForUs = i;

		  int length = SDLNet_TCP_Recv(Engine::socketDescriptor[serverIDthatHasTheFrameRenderedForUs], dataPtrTmp, numBytesToReceiveTemp);
		  if(length > 0){
			  numBytesToReceiveTemp -= length;
			  dataPtrTmp += length;
		  }
	  } while (numBytesToReceiveTemp > 0);	
    }

    sprintf(debugString, "glTexImage server %i", i);
    {
    CProfileSample receiveMessage2(debugString);

    glBindTexture(GL_TEXTURE_2D, openglstuff->framebufferTexIDRect[i]);
#ifdef ANDROID
    if(!Engine::serverUseETC1){
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Engine::rectSizeX[i], Engine::rectSizeY[i], 0, GL_RGBA, GL_UNSIGNED_BYTE, openglstuff->frameBufferPointerRect[i]);  
    } else {
      glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, Engine::rectSizeX[i], Engine::rectSizeY[i], 0,	(Engine::rectSizeX[i] * Engine::rectSizeY[i]) / 2, openglstuff->frameBufferPointerRect[i]);  
    }
#else
    if(!Engine::serverUseDXT1){
      // OPTIMIZE: store copy buffers somewhere else
      unsigned char* copyBuffer = new unsigned char[(Engine::rectSizeX[i] * Engine::rectSizeY[i] * 4) / 8];
      memcpy(copyBuffer, openglstuff->frameBufferPointerRect[i], (Engine::rectSizeX[i] * Engine::rectSizeY[i] * 4) / 8);

      Etc1::convertETC1toRGBA(copyBuffer, openglstuff->frameBufferPointerRect[i], Engine::rectSizeX[i], Engine::rectSizeY[i]);

	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Engine::rectSizeX[i], Engine::rectSizeY[i], 0, GL_BGRA, GL_UNSIGNED_BYTE, openglstuff->frameBufferPointerRect[i]);  
      delete[] copyBuffer;
    } else {
      openglstuff->glCompressedTexImage2D(GL_TEXTURE_2D, 0, COMPRESSED_RGBA_S3TC_DXT1_EXT, Engine::rectSizeX[i], Engine::rectSizeY[i], 0,	(Engine::rectSizeX[i] * Engine::rectSizeY[i]) / 2, openglstuff->frameBufferPointerRect[i]);  
    }
#endif
    }

    // does this help? 
    // glFlush();
  }
}

void NetworkStuff::receiveMessageFromRenderServerETC1NoRect(){
  unsigned char* dataPtr = openglstuff->frameBufferPointer; 
  unsigned char* dataPtrTmp = dataPtr;

	unsigned int numBytesToReceiveTemp = numBytesToReceive;
  
	do {
		int serverIDthatHasTheFrameRenderedForUs = (Engine::numFramesRendered ) % Engine::numServers;

		int length = SDLNet_TCP_Recv(Engine::socketDescriptor[serverIDthatHasTheFrameRenderedForUs], dataPtrTmp, numBytesToReceiveTemp);
		if(length > 0){
			numBytesToReceiveTemp -= length;
			dataPtrTmp += length;
		}
	} while (numBytesToReceiveTemp > 0);	
}

void NetworkStuff::determineNumBytesToReceive(){
  numBytesToReceive	= (Engine::screenWidthRT * Engine::screenHeightRT) / 2;
}

