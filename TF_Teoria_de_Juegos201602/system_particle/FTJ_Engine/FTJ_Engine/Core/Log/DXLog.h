#pragma once

#include "Log.h"			//For Log calls when only including DXLog.h
#include"DXLog.hpp"			//For DX Events
#include <d3d11.h>			//For setting resource names

//Will not set pointer to null if there are still refrences on the object
#define SAFE_RELEASE(com) { int keeper = 0; if(com)  { if (0 == (keeper = com->Release() ) ) com = nullptr; } Log(keeper); }

#ifndef __GOLD__BUILD__
	//Wrap everything that returns an HRESULT
	//Logs out the com error if there is one
	//returns true if it is S_OK else false
	#define DXCall(hr)			COMBINE(Log, __ID__)::DXFunctionCall(hr)
	//Set-up function: Call right after error checking, and there is a valid context
	//Nothing will do DX logs without calling this function
	#define DXNameInit(context) COMBINE(Log, __ID__)::CreateDXNaming(context)
	//Name objects for Graphics Object Table while visual debugging
	#define DXName(obj, str)	{ std::string s = str; D3D_SET_OBJECT_NAME_A(obj, s.c_str() ); }
	//n - wide char *
	//Call to group DX calls for Graphic Event List while visual debugging
	//Suggestions group HUD, particles, animated objects static objects as groups
	#define DXDrawName(n)		COMBINE(Log, __ID__)::DXEvent COMBINE(Log,__LINE__)(n)
	//Use: DXMemoryCheck(Device->Release() );
	#define DXMemoryCheck(ref)  COMBINE(Log, __ID__)::DebugReport(ref)
	//Call last before end of renderer shutdown
	#define DXDebugRelease()	COMBINE(Log, __ID__)::DXRelease()
#else
	//Needs to be an empty function so the enclosed function will still work
	#define DXCall(hr) {}
	#define DXMemoryCheck() _noop
	#define DXName(obj, str) __noop
	#define DXNameInit(context) __noop
	#define DXDrawName(n) __noop
#endif