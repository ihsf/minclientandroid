LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL
SDL_NET_PATH := ../SDL_net

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
  $(LOCAL_PATH)/$(SDL_NET_PATH) 

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
	../../../minclient/Camera.cpp \
	../../../minclient/CTextParser.cpp \
	../../../minclient/CTexture.cpp \
	../../../minclient/CTextureGL.cpp \
	../../../minclient/CVector2.cpp \
	../../../minclient/CVector3.cpp \
	../../../minclient/Engine.cpp \
	../../../minclient/Font.cpp \
	../../../minclient/MinClient.cpp \
	../../../minclient/NetworkStuff.cpp \
	../../../minclient/OpenGLES2stuff.cpp \
	../../../minclient/OpenGLstuff.cpp \
	../../../minclient/SDLstuff.cpp \
	../lz4-r123/lz4.c

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_net 

LOCAL_LDLIBS := -lGLESv2 -llog -lEGL

LOCAL_ARM_MODE := arm
LOCAL_CPPFLAGS := $(LOCAL_CFLAGS) -std=c++11

include $(BUILD_SHARED_LIBRARY)
