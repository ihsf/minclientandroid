#ifndef CTEXTUREBASE_H
#define CTEXTUREBASE_H

#include <stdio.h>
#include <iostream>
#include "ForceInline.h"

using namespace std;

class CTexture{
	public:
		virtual void generateTexture() = 0;  

		const char* getTextureName();
		finline unsigned int getID(){ return ID; }

		void setName(const char* const name);
		char* getName(){ return textureName; }

    finline int getSizeX(){ return sizeX; }
    finline int getSizeY(){ return sizeY; }
    finline int getChannels(){ return channels; }

		unsigned char *data;   

	protected:	  
		void init();

		int channels;      
		int sizeX;            
		int sizeY;      

		unsigned int ID;       
  
		char* textureName;	 
};

#endif
