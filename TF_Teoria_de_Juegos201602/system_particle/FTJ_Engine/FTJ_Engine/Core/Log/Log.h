#pragma once

#include "Log.hpp"

//Only sets the object to nil if there are no references
#define SAFE_SHUT(obj) if(obj) { obj->ShutDown(); delete obj; obj = nullptr; }

#ifndef __GOLD__BUILD__
//Send in any number of arguments to get logged to a file, the console window and the Output window
//Use Log("something", vector.size(), argument)
//If there is a linker error or an error where the compiler cannot find the function add the overload to Log.hpp
#define Log(...)		COMBINE(Log, __ID__)::print(##__VA_ARGS__)
//n - Name of the game wide char * or std::wstring
// Creates a file in %appdata% where the log file and any dump files will be placed
//Should be one of the first things called
#define LogSetUp(gameName)	COMBINE(Log, __ID__)::CreateLog(gameName)
//Clean-up the debugger
//Call at the end of the program
#define LogShutDown()		COMBINE(Log, __ID__)::Close()
//Use instead of get last error for window calls
//If there is any error it calls log
#define LogError()		COMBINE(Log, __ID__)::LogLastError()
//m - msg from wndproc
//Used to log what order of messages are sent in wndproc
#define LogWndProc(m)	COMBINE(Log, __ID__)::CheckMessage(m)
//m - Wparam from wndproc
//Used to log the system messages, another case off WM_SYSKEYDOWN messages
#define LogSystem(m)	COMBINE(Log, __ID__)::CheckSystemMessage(m)
#else //!__GOLD__BUILD__
#define LogWndProc __noop
#define LogError() __noop
#define Log(...) __noop
#define LogSetUp() COMBINE(Log, __ID__)::SetDumpFile();
#define LogShutDown() __noop
#endif //__GOLD__BUILD__