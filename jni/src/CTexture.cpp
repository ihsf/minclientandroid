#include "CTexture.h"

void CTexture::init(){
	sizeX = sizeY = channels = ID = 0; 
	data = NULL;
	textureName = NULL;
}

const char* CTexture::getTextureName(){ 
	return textureName; 
}

void CTexture::setName(const char* const name){
	delete [] textureName;

	textureName = new char[strlen(name) + 1];
	strcpy(textureName, name);
}


