#ifndef _ENGINE_H
#define _ENGINE_H

// frame rate calculation based on www.gametutorials.com - see license.txt

#ifdef _WIN32
  #include <windows.h>
#endif

#ifndef ELCH_FFMPEG
  #define ELCH_FFMPEG
#endif

#include <SDL.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <string>
#include <fstream>
#include "CVector2.h" 
#include "CVector3.h" 
#include <vector>
#include <stdlib.h>
#include <SDL_net.h>
#include "CTextParser.h"
#include "ForceInline.h"

using namespace std;

#ifdef _MSC_VER
	#pragma warning(disable: 4244) // conversion from 'const double' to 'float', possible loss of data
	#pragma warning(disable: 4996) // '*' was declared deprecated
#endif

#ifdef __INTEL_COMPILER
  #pragma warning(disable: 3291) // invalid narrowing conversion from "int" to "float"
	#pragma warning(disable: 1786) // function strcpy was declared deprecated ...
	#pragma warning(disable: 279)
	#pragma warning(disable: 1478) //  function "alutLoadWAVFile" was declared "deprecated"
	#pragma warning(disable: 589)  //  transfer of control bypasses initialization of
#endif

#ifndef PI_DEFINED
	#define PI_DEFINED
	const double PI  = 3.1415926535897932384626433832795;
#endif

#ifndef M_180_PI
  #define M_180_PI	57.295779513082320876798154814105f
#endif

#ifndef M_PI_180
  #define M_PI_180	0.017453292519943295769236907684886f
#endif

#ifndef M_1_PI
  #define M_1_PI   0.31830988618379067154f
#endif

#define RAD2DEG(a) (a*M_180_PI)
#define DEG2RAD(a) (a*M_PI_180)

#ifdef ANDROID
  const char * const CONFIG_FILE = "/sdcard/qwrt.cfg";
#else
  const char * const CONFIG_FILE = "Data/qwrt.cfg";
#endif

static CVector3 NULLVECTOR3f(0.0f, 0.0f, 0.0f);

static const int NETWORK_PROTCOL_MAGICK = 123456;
static const int NETWORK_PROTOCOL_VERSION = 10;
static const int MAX_SERVERS = 16;
static const int PORT_TO_SERVER = 2000;
const char * const MINCLIENT_VERSION = "0.11";

#define NETWORK_ON true

class Engine {
	public:
		Engine(){};
		~Engine(){};
    
		static void init();		
		static void calculateFrameRate();
			
		static float getFrameInterval(){ return frameInterval; }

		// Networking stuff
		static TCPsocket socketDescriptor[MAX_SERVERS];
		static IPaddress serverIP[MAX_SERVERS];

    static int rectLeftServer[MAX_SERVERS];
    static int rectTopServer[MAX_SERVERS];
    static int rectRightServer[MAX_SERVERS];
    static int rectBottomServer[MAX_SERVERS];
    static int rectSizeX[MAX_SERVERS];
    static int rectSizeY[MAX_SERVERS];

    static bool rectMode;

		// variables
		static int screenWidthRT; 
		static int screenHeightRT;
		static int screenWidthGL; 
		static int screenHeightGL; 
		static int serverFrameBuffers;

		//static int RENDERTILE_SIZE;

    static bool upscale;

		// keys & mouse
		static bool upKey;
		static bool downKey; 
		static bool leftKey; 
		static bool rightKey;
		//static bool leftButton;
		//static bool rightButton;
		static bool jumpKey;
    static bool crouchKey;

		static bool nextFrameStartProfiler;
		static bool nextFrameStopProfiler;

    static CVector2 relativeMouseMotion;
	
		static bool forceScreenResolution;
		static bool done;

    static bool fullscreen;

    static int fontSize;

		static float angle;

		static float currentTime;
		static float currentTimeLastFrame;
		static float timeWhenMapWasLoaded;
		static float sensitivity;
		static float frameTime;
		static float framesPerSecond;		

		static char* startMap;

		static char* serverName[MAX_SERVERS];
		static int serverPort[MAX_SERVERS];
		static int numServers;

    static void increaseNumFramesRendered(){ numFramesRendered++; }

    static float frameInterval;

    static int numFramesRendered;

    static char strFrameRate[32];

    static void calculateOrthoMatrix(float left, float right, float bottom, float top, float near_, float far_, float result[16]);

    static vector<char*> profilerOutput;

    static bool serverUseDXT1;
    static bool serverUseETC1;

    static CVector2 buttonLeftCenter;
    static CVector2 buttonRightCenter;
    static CVector2 buttonCenter;

    static CVector2 buttonPressed;

    static int clicksPerSecond;

	private:		
		static float lastTime;				// This will hold the time from the last frame		
		static void parseConfigFile();

    static void updateGlobalSinus();
};




#endif
