// THIS FILE WILL BE REMOVED BEFORE RELEASING SOURCE
// THIS FILE WILL BE REMOVED BEFORE RELEASING SOURCE
// THIS FILE WILL BE REMOVED BEFORE RELEASING SOURCE

// ProfileLogHandler.cpp: implementation of the CProfileLogHandler class.
#include "ProfileLogHandler.h"

const char * const PROFILER_INTRO1= "  Min :   Avg :   Max :   # : Profile Name  ";
const char * const PROFILER_INTRO2 = "--------------------------------------------";

void CProfileLogHandler::BeginOutput(float tTime){
  char* buffer = new char[128];
  strcpy(buffer, PROFILER_INTRO1);
  Engine::profilerOutput.push_back(buffer);

  buffer = new char[128];
  strcpy(buffer, PROFILER_INTRO2);
  Engine::profilerOutput.push_back(buffer);

	//cout << endl;
	//cout << "  Min :   Avg :   Max :   # : Profile Name  " << endl;
	//cout << "--------------------------------------------" << endl;
}

void CProfileLogHandler::Sample(float fMin, float fAvg, float fMax, float tAvg, int callCount, std::string name, int parentCount){
	char namebuf[256], indentedName[256];
	char avg[16], minChar[16], maxChar[16], num[16], time[16];

	if(fAvg < 0.0f)
		fAvg = 0.0f;

	if(fMin < 0.0f)
		fMin = 0.0f;

	if(fMax < 0.0f)
		fMax = 0.0f;

  if(fAvg >= 10.0f){
    sprintf(avg, " %3.1f", fAvg);
  } else {
    sprintf(avg, "  %3.1f", fAvg);
  }

  if(fMin >= 10.0f){
    sprintf(minChar, " %3.1f", fMin);
  } else {
    sprintf(minChar, "  %3.1f", fMin);
  }

  if(fMax >= 10.0f){
    sprintf(maxChar, " %3.1f", fMax);
  } else {
    sprintf(maxChar, "  %3.1f", fMax);
  }

	sprintf(time,"%3.1f", tAvg);
	sprintf(num, "%3d",   callCount);

	strcpy( indentedName, name.c_str());
	for( int indent=0; indent < parentCount; ++indent ){
		sprintf(namebuf, " %s", indentedName);
		strcpy( indentedName, namebuf);
	}

  char* buffer = new char[128];
  sprintf(buffer, "%s   %s   %s   %s     %s", minChar, avg, maxChar, num, indentedName); 
  Engine::profilerOutput.push_back(buffer);
}

void CProfileLogHandler::EndOutput(){
	//cout << endl;
}
