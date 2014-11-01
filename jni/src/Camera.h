#ifndef _CAMERA_H
#define _CAMERA_H

// class based on www.gametutorials.com - see license.txt

#include "CVector3.h"
#include "Engine.h"
#include <iostream>

using namespace std;

// handles the camera movement
class Camera {
	public:
	  Camera();
		~Camera();

    finline CVector3 getPosition() { return position; }
    finline CVector3 getView()     { return view; }
    finline CVector3 getUpVector() { return upVector; }
		CVector3 getDirectionNormalized();   

		void rotateView(float angle, float X, float Y, float Z);

    void applyMouseLook();
    void applyKeyboardMovements();

		void strafeCamera(float speed);
		void moveCamera(float speed);

		void update();		

	private:
		void init();

		CVector3 position;                   
		CVector3 view;                       
		CVector3 upVector;       
		CVector3 strafe;    
		
		void setViewMatrixCamera();
};

#endif
