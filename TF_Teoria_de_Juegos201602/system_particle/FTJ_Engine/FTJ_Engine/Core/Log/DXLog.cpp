#include "DXLog.h"
#include <Initguid.h>
#include <dxgidebug.h >
#include <d3d11_1.h>
#include <comdef.h>
#include "Log.h"

#pragma comment( lib, "dxguid.lib")

typedef HRESULT(__stdcall *GetDXGIFunction)(const IID&, void**);
GetDXGIFunction test;
HMODULE						m_hModule = nullptr;
IDXGIDebug *				m_pDXGIDebug = nullptr;
ID3DUserDefinedAnnotation * m_pAnnotation = nullptr;

namespace COMBINE(Log, __ID__) {
	//--------------------------------------------------------------------------------
	void CreateDXNaming(void *_context) {
		DXCall(static_cast<ID3D11DeviceContext *>(_context)->QueryInterface(__uuidof(ID3DUserDefinedAnnotation), (void **)&m_pAnnotation) );

		//Done this way so the current device does not get an extra reference 
		m_hModule = LoadLibraryEx(L"Dxgidebug.dll", 0, LOAD_LIBRARY_AS_IMAGE_RESOURCE | LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE);
		if (nullptr == m_hModule) {
			Log("Could not find Dxgidebug.dll");
			return;
		}

		GetDXGIFunction GetDXGI = reinterpret_cast<GetDXGIFunction>(GetProcAddress(m_hModule, "DXGIGetDebugInterface") );
		if (GetDXGI)
			GetDXGI(__uuidof(IDXGIDebug), (void**)&m_pDXGIDebug);
		else {
			Log("Warning: Live objects not collected, D3D11_CREATE_DEVICE_DEBUG flag not set");
		}
	}
	//--------------------------------------------------------------------------------
	bool DXFunctionCall(long _hr) {
		if (_hr == S_OK)
			return true;

		_com_error err(_hr);
		Log(err.ErrorMessage() );
		return false;
	}
	//--------------------------------------------------------------------------------
	void DebugReport(int _ref) {
		//m_pAnnotation has a ref to the context that throws off the report
		if (nullptr != m_pAnnotation) {
			SAFE_RELEASE(m_pAnnotation);
			_ref -= 1;
		}

		if (_ref > 0) {
			Log("\n** DX LIVE OBJECT REPORT **\n");
			if (m_pDXGIDebug)
				m_pDXGIDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
			else
				Log("DXGI Module was not found");
			Log("\n** END OF OBJECT REPORT **\n");
		}
	}
	//--------------------------------------------------------------------------------
	void DXRelease() {
		//SAFE_RELEASE(m_pAnnotation);
		//DXMemoryCheck(m_pDXGIDebug->Release());
		SAFE_RELEASE(m_pDXGIDebug);
		FreeLibrary(m_hModule);
	}
	//--------------------------------------------------------------------------------
	DXEvent::DXEvent(wchar_t * _name) {
		if (nullptr != m_pAnnotation)
			m_pAnnotation->BeginEvent(_name);
		else
			Log("No Pointer cannot log");
	}
	//--------------------------------------------------------------------------------
	DXEvent::~DXEvent() {
		if (nullptr != m_pAnnotation)
			m_pAnnotation->EndEvent();
		else
			Log("No Pointer cannot end log");

	};
	//--------------------------------------------------------------------------------
};