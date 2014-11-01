#include "CTextureGL.h"

CTextureGL::CTextureGL(){
	init();
}

CTextureGL::CTextureGL(unsigned char* data_, int sizeX_, int sizeY_, int channels_){
	init();

  this->data = data_;
  this->sizeX = sizeX_;
  this->sizeY = sizeY_;
  this->channels = channels_;

	generateTexture();
}

CTextureGL::~CTextureGL(){
	//delete [] data;
	delete [] textureName;
}

void CTextureGL::generateTexture(){
	if(!data){
		cout << "CTextureGL::GenerateTexture(): No data has been assigned yet!" << endl;
		return;
	}

	glGenTextures(1, &ID);

	glBindTexture(GL_TEXTURE_2D, ID);

	if (channels == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	else if (channels == 4) 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sizeX, sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//delete [] data;
	//data = NULL;
}

