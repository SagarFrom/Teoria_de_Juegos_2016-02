#pragma once
#include "../Entity/IComponent.h"
#include "FTJ_ShaderDefinitions.h"

#include <DirectXMath.h>

using namespace DirectX;

namespace FTJ
{
	class CCamera : public FTJ::IComponent
	{
	 	ShaderDefinitions::SCENE_BUFFER m_MVP;

	public:
		CCamera(CGameObject* _owner);
		~CCamera();

		void CalculateMVP(XMMATRIX& _rotation, XMMATRIX& _translation);
		void CalculateMVP();

		ShaderDefinitions::SCENE_BUFFER* GetSceneBuffer() { return &m_MVP; }
	};
}