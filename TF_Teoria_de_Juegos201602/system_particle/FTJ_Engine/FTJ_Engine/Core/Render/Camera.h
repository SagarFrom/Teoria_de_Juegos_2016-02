#pragma once
#include "../Entity/IComponent.h"
#include "FTJ_ShaderDefinitions.h"

#include <DirectXMath.h>

using namespace DirectX;

namespace FTJ
{
	__declspec(align(16)) class CCamera : public FTJ::IComponent
	{
	 	ShaderDefinitions::SCENE_BUFFER m_MVP;

	public:
		CCamera(CGameObject* _owner);
		~CCamera();

		void CalculateMVP(XMMATRIX& _rotation, XMMATRIX& _translation);
		void CalculateMVP();

		ShaderDefinitions::SCENE_BUFFER* GetSceneBuffer() { return &m_MVP; }

		void* operator new(size_t i)
		{
			return _mm_malloc(i, 16);
		}

			void operator delete(void* p)
		{
			_mm_free(p);
		}

	};
}