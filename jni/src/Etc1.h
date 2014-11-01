#ifndef _ETC1_H
#define _ETC1_H

// THIS FILE WILL BE REMOVED BEFORE RELEASING SOURCE
// THIS FILE WILL BE REMOVED BEFORE RELEASING SOURCE
// THIS FILE WILL BE REMOVED BEFORE RELEASING SOURCE

#include <math.h>			
#include <stdio.h>	
#include "Engine.h"
#include <malloc.h>
#include "rg_etc1.h"

using namespace std;
using namespace rg_etc1;

class Etc1 {
	public:
		Etc1();
		~Etc1();

    static void convertRGBAtoETC1(unsigned char* compressedData, unsigned char* data, int sizeX, int sizeY);
    static void convertETC1toRGBA(unsigned char* compressedData, unsigned char* data, int sizeX, int sizeY);

	private:
    finline static void ExtractBlock( const unsigned char *inPtr, int width, unsigned char *colorBlock ){
      for(int j = 0; j < 4; j++) {
        memcpy(&colorBlock[j*4*4], inPtr, 4*4);
        inPtr += width * 4;    
      }
    }

    finline static void setAlphaTo255(unsigned char *colorBlock ){
      for(int j = 0; j < 16; j++) {
        colorBlock[j * 4 + 3] = 255;
      }
    }

    static void writeDecodedBlockIntoImage(unsigned char* block, unsigned char* inBuf, int sizeX, int i);

    static void etc1helperFunction(unsigned char* compressedDataPointer, unsigned char* inBuf, int sizeX, etc1_pack_params* parameters, int j);
    static void etc1helperFunctionDecode(unsigned char* compressedDataPointer, unsigned char* inBuf, int sizeX, int i);

    static void swarRGBtoBGRinBlock(unsigned char* block);

    struct ETC1Header {
      char tag[6];                  // "PKM 10"
      unsigned short format;        // Format == number of mips (== zero)
      unsigned short texWidth;       // Texture dimensions, multiple of 4 (big-endian)
      unsigned short texHeight;
      unsigned short origWidth;      // Original dimensions (big-endian)
      unsigned short origHeight;
    };

    static void savePKM(unsigned char* compressedData, int sizeX, int sizeY);
};

#endif
