LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL
SDL_NET_PATH := ../SDL_net
#FFMPEG_PATH := ../ffmpeg/ffmpeg/armeabi

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
  $(LOCAL_PATH)/$(SDL_NET_PATH) 
#  \
#  $(LOCAL_PATH)/$(FFMPEG_PATH)/include 

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
	Camera.cpp \
	CTextParser.cpp \
	CTexture.cpp \
	CTextureGL.cpp \
	CVector2.cpp \
	CVector3.cpp \
	Engine.cpp \
	Etc1.cpp \
	Font.cpp \
	MinClient.cpp \
	NetworkStuff.cpp \
	OpenGLES2stuff.cpp \
	OpenGLstuff.cpp \
	ProfileLogHandler.cpp \
	Profiler.cpp	\
	rg_etc1.cpp \
	SDLstuff.cpp

# LOCAL_SHARED_LIBRARIES := SDL2 SDL2_net 
#avcodec avutil avformat avfilter avdevice swscale
 LOCAL_SHARED_LIBRARIES := SDL2 SDL2_net 
 #avcodec avutil avformat

# LOCAL_LDLIBS := -lGLESv1_CM -llog 
LOCAL_LDLIBS := -lGLESv2 -llog 

include $(BUILD_SHARED_LIBRARY)
