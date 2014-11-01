// THIS FILE WILL BE REMOVED BEFORE RELEASING SOURCE
// THIS FILE WILL BE REMOVED BEFORE RELEASING SOURCE
// THIS FILE WILL BE REMOVED BEFORE RELEASING SOURCE

#include "Etc1.h"
//#include <omp.h> 
#ifdef __INTEL_COMPILER
  #include <cilk/cilk.h>
#endif


#ifndef ALIGN16
  #ifndef ANDROID
    #define   ALIGN16( x ) __declspec(align(16)) x
  #else
    #define   ALIGN16( x ) x
  #endif  
#endif

#ifndef ByteSwap16
  #define ByteSwap16(n) ( ((((unsigned int) n) << 8) & 0xFF00) | ((((unsigned int) n) >> 8) & 0x00FF) )
#endif

Etc1::Etc1(){
  pack_etc1_block_init();
}

Etc1::~Etc1(){
}

void Etc1::convertETC1toRGBA(unsigned char* compressedData, unsigned char* data, int sizeX, int sizeY){
 if(sizeX < 8 || sizeY < 8){
    cout << "Etc1::convertETC1toRGBA: Invalid image size: " << sizeX << " x " << sizeY << endl;
    return;
  }

  if((sizeX % 8 != 0) || ((sizeY % 8) != 0)){
    cout << "Etc1::convertETC1toRGBA: Invalid image size. Needs to be dividable by 8: " << sizeX << " x " << sizeY << endl;
    return;
  }

  if(!data || !compressedData){
    cout << "Etc1::convertETC1toRGBA: Invalid data or compressedData" << endl;
    return;
  }

  unsigned char* compressedDataPointer = compressedData;
  unsigned char* inBuf = data;

  const int numBlocks = (sizeX * sizeY) / 16; 
  // ToDo: multi-thread
  for(int i = 0; i < numBlocks; i++) {
    etc1helperFunctionDecode(compressedDataPointer, inBuf, sizeX, i);
  }
}

void Etc1::etc1helperFunctionDecode(unsigned char* compressedDataPointer, unsigned char* inBuf, int sizeX, int i){
  // every compressed block has 8 bytes
  unsigned char* actualCompressedDataPointer = compressedDataPointer + (i * 8);  
  ALIGN16( unsigned char block[64] );

  // decode into temporary block first
  unpack_etc1_block(actualCompressedDataPointer, (unsigned int*)block);

  swarRGBtoBGRinBlock(block);
  
  // copy decoded data into the big image
  writeDecodedBlockIntoImage(block, inBuf, sizeX, i);  
}

void Etc1::swarRGBtoBGRinBlock(unsigned char* block){
  unsigned char temp;
  for(int i = 0; i < 64; i+=4){
    temp = block[i];
    block[i] = block[i+2];
    block[i+2] = temp;
  }
}

void Etc1::writeDecodedBlockIntoImage(unsigned char* block, unsigned char* inBuf, int sizeX, int blockNum){
  // blocks are 4*4 pixels
  int positionX = (blockNum * 4) % sizeX;
  int positionY = (((blockNum * 4) - positionX) / sizeX) * 4;   // every block counts for 4 y lines.

  for(int i = 0; i < 4; i++){
    // copy 16 bytes from decoded block into big frame buffer
    memcpy(inBuf + positionX * 4 + (sizeX * 4 * (positionY + i)), block + i * 16, 16);
  }

}

void Etc1::convertRGBAtoETC1(unsigned char* compressedData, unsigned char* data, int sizeX, int sizeY){
  if(sizeX < 8 || sizeY < 8){
    cout << "Etc1::convertRGBAtoETC1: Invalid image size: " << sizeX << " x " << sizeY << endl;
    return;
  }

  if((sizeX % 8 != 0) || ((sizeY % 8) != 0)){
    cout << "Etc1::convertRGBAtoETC1: Invalid image size. Needs to be dividable by 8: " << sizeX << " x " << sizeY << endl;
    return;
  }

  if(!data || !compressedData){
    cout << "Etc1::convertRGBAtoETC1: Invalid data or compressedData" << endl;
    return;
  }

  etc1_pack_params parameters;
  parameters.clear();
  parameters.m_quality = cLowQuality;  // everything else is way too slow

 
  unsigned char* compressedDataPointer = compressedData;
  unsigned char* inBuf = data;

  //WindowsHelper::getMsElapsed();
  //const int numRuns = 100;

  //for( int k = 0; k < numRuns; k++){
    for ( int j = 0; j < sizeY; j += 4) {
#ifdef __INTEL_COMPILER
      cilk_spawn(etc1helperFunction(compressedDataPointer, inBuf, sizeX, &parameters, j));
#else
      cout << "convertRGBAtoETC1: No cilk found. Using single thread" << endl;
      etc1helperFunction(compressedDataPointer, inBuf, sizeX, &parameters, j);
#endif
    }
//  }

//  float timeElapsed = WindowsHelper::getMsElapsed();
//  cout << "Took avg of " << timeElapsed / (float)numRuns << " ms for encoding " << Engine::screenWidthRT << " x " << Engine::screenHeightRT << endl;
//  SDL_Delay(2000);
//  exit(1);
}


void Etc1::etc1helperFunction(unsigned char* compressedDataPointer, unsigned char* inBuf, int sizeX, etc1_pack_params* parameters, int j) {
  unsigned char*  currentInBuf = inBuf + sizeX * 4 * 4 * (j / 4);

  for ( int i = 0; i < sizeX; i += 4) {
    ALIGN16( unsigned char block[64] );
    // copies color values into a block of 4x4 pixels x4 for unsigned char rgba
	  ExtractBlock( currentInBuf + i * 4, sizeX, block );

    unsigned char* currentCompressedDataPointer = compressedDataPointer + 8 * (i / 4) + (8 * sizeX / 4) * (j / 4);
    pack_etc1_block((void*)currentCompressedDataPointer, (unsigned int*)block, *parameters);
  }
}

void Etc1::savePKM(unsigned char* compressedData, int sizeX, int sizeY){
  const int compressedSize = (sizeX * sizeY * 4) / 8;

  ETC1Header header;
  header.tag[0] = 'P';
  header.tag[1] = 'K';
  header.tag[2] = 'M';
  header.tag[3] = ' ';
  header.tag[4] = '1';
  header.tag[5] = '0';
  header.format = 0;
  header.texWidth = ByteSwap16(sizeX);    // big-endian (= non-intel)
  header.texHeight = ByteSwap16(sizeY);
  header.origWidth = ByteSwap16(sizeX);
  header.origHeight = ByteSwap16(sizeY);

  FILE* out = fopen("test.pkm", "wb");
  fwrite(&header, sizeof(ETC1Header), 1, out); 
  size_t writtenBytes = fwrite(compressedData, sizeof(unsigned char), compressedSize, out);
  cout << "writtenBytes: " << writtenBytes << endl;
  fclose(out);

  cout << "Saved test.pkm" << endl;
}
