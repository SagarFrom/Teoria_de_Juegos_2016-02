#pragma once

#include <string>

#define __ID__	504

//////////////////////////////////////////////////////
//Don't use these directly
#define COMBINE_ACTION(A,B) A##B
#define COMBINE_STEP(A,B) COMBINE_ACTION(A,B)
//////////////////////////////////////////////////////

//makes one name into one that can be a class or namespace name
#define COMBINE(A,B) COMBINE_STEP(A,B)
//Make a sting out of a class or helpers like __FUNCTION__, __LINE__, __FILE__
#define STRINGIZE(A) #A

namespace COMBINE(Log, __ID__) {

	void DebugPrint(std::string _str);
	void DebugPrint(std::wstring _str);
	void DebugPrint(float _in);
	void DebugPrint(int _in);
	void DebugPrint(unsigned long _in);
	void DebugPrint(unsigned int _in);
	void DebugPrint(void * _in);
	void DebugPrint(char * _in);
	void DebugPrint(wchar_t * _in);
	void DebugPrint(const void * _in);
	void DebugPrint(const char * _in);
	void DebugPrint(const wchar_t * _in);
	void print();

	template <typename work, typename ...type>
	void print(work _in, type...Pass){
		DebugPrint(_in);
		print(Pass...);
	}
	void SetDumpFile();
	void CreateLog(std::wstring _name);
	void Close();
	void LogLastError();
	bool DXFunctionCall(long _hr);
	void CreateDXNaming(void *_context);

	void DebugReport(int _ref);

	void CheckMessage(unsigned int _msg);
	void CheckSystemMessage(unsigned int _msg);

	class DXEvent {
	public:
		DXEvent(wchar_t *_name);
		~DXEvent();
	};
};