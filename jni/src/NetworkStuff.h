#ifndef _NETWORKSTUFF_H
#define _NETWORKSTUFF_H

#include "Engine.h"
#include "Camera.h"
#include "OpenGLstuff.h"
#include "Font.h"
#include "Profiler.h"

using namespace std;

struct clientMessage{
  int magic;
  int clientMessageLength;
  int protocolVersion;

  CVector3 cameraPosition;
  CVector3 cameraView;
  CVector3 cameraUp;

  int screenWidthRT;
  int screenHeightRT;

  int rectLeft;
  int rectTop;
  int rectRight;
  int rectBottom;

  float currentTime;
  int frameNr;

  bool doexit;
};

class NetworkStuff {
	public:
		NetworkStuff(Camera* camera_, OpenGLstuff* openglstuff_);
		~NetworkStuff();

		void sendMessageToRenderServers();
    void receiveMessageFromRenderServer();

	private:
		Camera* camera;
		OpenGLstuff* openglstuff; 

		void init();
    void determineNumBytesToReceive();
		bool determineIfThisFrameShouldBeRendered(int i);
		void sendInitPackets();

		void receiveMessageFromRenderServerETC1();
    void receiveMessageFromRenderServerETC1NoRect();
    void receiveMessageFromRenderServerETC1Rect();

		unsigned int numBytesToReceive;
		//int realHeightConsideringTileSizes;

		int serverLastRendered;

    int framesToWait;

    clientMessage msgBufferSend;
    void initMsgBufferSend();
};

#endif
