// THIS FILE WILL BE REMOVED BEFORE RELEASING SOURCE
// THIS FILE WILL BE REMOVED BEFORE RELEASING SOURCE
// THIS FILE WILL BE REMOVED BEFORE RELEASING SOURCE

// LICENSE: http://www.gamedev.net/reference/programming/features/enginuity3/

#if !defined(AFX_PROFILELOGHANDLER_H__CAD57C2F_2BF7_492C_8ED3_EFE606EF3EAC__INCLUDED_)
#define AFX_PROFILELOGHANDLER_H__CAD57C2F_2BF7_492C_8ED3_EFE606EF3EAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _MSC_VER
	#pragma warning( disable : 4244 ) // conversion from 'const double' to 'float', possible loss of data
	#pragma warning( disable : 4996 ) // '*' was declared deprecated
#endif

#ifdef __INTEL_COMPILER
	#pragma warning(disable: 1786) // function strcpy was declared deprecated ...
	#pragma warning(disable: 279)
	#pragma warning(disable: 1478) //  function "alutLoadWAVFile" was declared "deprecated"
#endif

#include "Profiler.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Font.h"

using namespace std;

class CProfileLogHandler : public IProfilerOutputHandler{
	public:
		void BeginOutput(float tTime);
		void EndOutput();
		void Sample(float fMin, float fAvg, float fMax, float tAvg, int callCount, std::string name, int parentCount);
};

#endif // !defined(AFX_PROFILELOGHANDLER_H__CAD57C2F_2BF7_492C_8ED3_EFE606EF3EAC__INCLUDED_)
