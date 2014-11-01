#include "Camera.h"

Camera::Camera(){
	init();
}

void Camera::init(){
  position = CVector3(0.0f, 10.0f, -20.0f);
  view = CVector3(0.0f, 10.0f, -19.0f);
  upVector = CVector3(0.0f, 1.0f, 0.0f);
  
  // REMOVE BEFORE RELEASING SOURCE
  if(!strcmp(Engine::startMap, "island")){
    position = CVector3(-7659.149902f, 633.134399f, -4382.462402f);                 
    view     = CVector3(-7658.375977f, 632.928223f, -4383.061523f); 
  }  
}

Camera::~Camera(){
}


void Camera::applyMouseLook(){
#if 0
#ifndef ANDROID
  int mouseX, mouseY;                              

  float angleY = 0.0f;                           
  float angleZ = 0.0f;                           
  static float currentRotX = 0.0f;

	if(Engine::matrixCamera){
		setViewMatrixCamera();
		return;
	}

	if( (Engine::relativeMouseMotion.x == 0) && (Engine::relativeMouseMotion.y == 0) ) 
		return;

	if(Engine::currentTime < 0.1f)
		return;

  angleY = (float)( (-Engine::relativeMouseMotion.x)  / 1000.0f);       
  angleZ = (float)( (-Engine::relativeMouseMotion.y)  / 1000.0f);     

  Engine::relativeMouseMotion.x = Engine::relativeMouseMotion.y = 0;

	CVector3 viewMinusPosition = view - position;

  float dotProduct = upVector.DotProduct(&viewMinusPosition);
	
  currentRotX -= angleZ;  

  if(currentRotX > 1.0f)
      currentRotX = 1.0f;
  else if(currentRotX < -1.0f)
      currentRotX = -1.0f;

	if(dotProduct > 0.95f){					
		viewMinusPosition.x += 0.05f;
		viewMinusPosition.y -= 0.1f;
		viewMinusPosition.normalize();
		dotProduct = upVector.DotProduct(&viewMinusPosition);
	}

	if(dotProduct < -0.95f){
		viewMinusPosition.x += 0.05f;
		viewMinusPosition.y += 0.1f;
		viewMinusPosition.normalize();
		dotProduct = upVector.DotProduct(&viewMinusPosition);
	}

	CVector3 axis = axis.cross(viewMinusPosition, upVector);
	axis.normalize();

  rotateView(angleZ, axis.x, axis.y, axis.z);
  rotateView(angleY, 0, 1, 0);
#endif
#endif
}


void Camera::rotateView(float angle, float x, float y, float z){
  CVector3 direction = getDirectionNormalized();

  // mouse/touch look sensitivity
	angle *= Engine::sensitivity;
	
  const float cosTheta = cosf(angle);
  const float sinTheta = sinf(angle);

	CVector3 newView;

  newView.x  = (cosTheta + (1 - cosTheta) * x * x)       * direction.x;
  newView.x += ((1 - cosTheta) * x * y - z * sinTheta)   * direction.y;
  newView.x += ((1 - cosTheta) * x * z + y * sinTheta)   * direction.z;

  newView.y  = ((1 - cosTheta) * x * y + z * sinTheta)   * direction.x;
  newView.y += (cosTheta + (1 - cosTheta) * y * y)       * direction.y;
  newView.y += ((1 - cosTheta) * y * z - x * sinTheta)   * direction.z;

  newView.z  = ((1 - cosTheta) * x * z - y * sinTheta)   * direction.x;
  newView.z += ((1 - cosTheta) * y * z + x * sinTheta)   * direction.y;
  newView.z += (cosTheta + (1 - cosTheta) * z * z)       * direction.z;

  view = position + newView;
}

void Camera::strafeCamera(float speed){   
	speed *= -1.0f; // invert

  position.x += strafe.x * speed;
  position.z += strafe.z * speed;

  view.x += strafe.x * speed;
  view.z += strafe.z * speed;
}

void Camera::moveCamera(float speed){
  CVector3 direction = getDirectionNormalized();

  position += direction * speed;
  view += direction * speed;
}

void Camera::applyKeyboardMovements(){
  const float speedUp = 30.0f;
  float speed = speedUp * Engine::frameInterval;

	if(speed < 0.0f)
		speed = -speed;

//#ifdef ANDROID
  if(Engine::buttonPressed != CVector2(-666.6f, -666.6f)){
    Engine::upKey = false;
    Engine::downKey = false; 
    Engine::leftKey = false; 
    Engine::rightKey = false;
    Engine::jumpKey = false;
    Engine::crouchKey = false;    

    CVector2 translatedButtonPressed = CVector2(Engine::buttonPressed.x - ((float)Engine::fontSize * 1.32f), Engine::screenHeightGL - Engine::buttonPressed.y - ((float)Engine::fontSize * (4.0f / 8.0f)));

    CVector2 distanceToLeftButton = Engine::buttonLeftCenter - translatedButtonPressed;
    float distanceLeft = distanceToLeftButton.magnitude();

    const float radiusForButton = 125.0f;
    if(distanceLeft < radiusForButton && distanceLeft > 1.0f){
      distanceToLeftButton.normalize();

      int maxComponent = distanceToLeftButton.maxComponentAbs();
      if(maxComponent == CVector2::X_COMPONENT){
        if(distanceToLeftButton.x > 0.0f){
          Engine::rightKey = true; // swapped left/right on purpose!
        } else {
          Engine::leftKey = true;           
        }
      } else {
        if(distanceToLeftButton.y > 0.0f){
          Engine::downKey = true; 
        } else {
          Engine::upKey = true;
        }
      }
    }

    // Handle right button field for mouse view
    CVector2 distanceToRightButton = Engine::buttonRightCenter - translatedButtonPressed;
    float distanceRight = distanceToRightButton.magnitude();
    if(distanceRight < radiusForButton){
      float angleY = 0.0f;                
      float angleZ = 0.0f;                       
      static float currentRotX = 0.0f;

      // mouse look
      CVector2 mouseMotion = CVector2(-distanceToRightButton.x, distanceToRightButton.y);
      angleY = (float)( (-mouseMotion.x)  / 3500.0f);       
      angleZ = (float)( (-mouseMotion.y)  / 3500.0f);     

	    CVector3 viewMinusPosition = view - position;

      float dotProduct = upVector.dotProduct(&viewMinusPosition);
    	
      currentRotX -= angleZ; 

      if(currentRotX > 1.0f)
        currentRotX = 1.0f;
      else if(currentRotX < -1.0f)
        currentRotX = -1.0f;

	    if(dotProduct > 0.95f){					
		    viewMinusPosition.x += 0.05f;
		    viewMinusPosition.y -= 0.1f;
		    viewMinusPosition.normalize();
		    dotProduct = upVector.dotProduct(&viewMinusPosition);
	    }

	    if(dotProduct < -0.95f){
		    viewMinusPosition.x += 0.05f;
		    viewMinusPosition.y += 0.1f;
		    viewMinusPosition.normalize();
		    dotProduct = upVector.dotProduct(&viewMinusPosition);
	    }

	    CVector3 axis = axis.cross(viewMinusPosition, upVector);
	    axis.normalize();

      rotateView(angleZ, axis.x, axis.y, axis.z);
      rotateView(angleY, 0, 1, 0);
    }
  }
//#endif
 
	if(Engine::crouchKey){
		position.y -= speed;
		view.y -= speed;
	}
	if(Engine::jumpKey){
		position.y += speed;
		view.y += speed;
	}

	if(Engine::upKey) {             
	  moveCamera(speed);              
	}

	if(Engine::downKey) {           
	  moveCamera(-speed);             
	}

	if(Engine::leftKey) {   
    strafeCamera(-speed);
	}

	if(Engine::rightKey) {   
		strafeCamera(speed);
	}   
}

void Camera::update() {
  strafe = strafe.cross(view - position, upVector);
  strafe.normalize();

  applyMouseLook();
  applyKeyboardMovements();
}

CVector3 Camera::getDirectionNormalized(){
	CVector3 dir = view - position;
  if(dir.magnitude() < 0.01f) {
    cerr << "Camera direction is too small..." << endl;
  }
	dir.normalize();

	return dir;
}
