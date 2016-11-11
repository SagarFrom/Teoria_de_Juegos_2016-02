#pragma once

#include <string>

#include <DirectXMath.h>

using namespace DirectX;

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

namespace FTJ
{
	typedef unsigned int uint;

	namespace ShaderDefinitions
	{
		enum eDepth
		{
			Playable, HUD
		};

		struct SCENE_BUFFER
		{
			XMMATRIX viewMatrix;
			XMMATRIX projMatrix;
		};

		struct T2DInfo
		{
			XMFLOAT2	vScreenPos;
			XMFLOAT4	v4Color;
			XMFLOAT2	vSize;

			//T2DInfo()
			//{
			//	vScreenPos = PxVec4(-1.0f, 1.0f, 0.5f, 1.0f);
			//	vSize = PxVec2(2.0f, 2.0f);
			//	v4Color = PxVec4(1.f);
			//}
		private:
			//PxVec2		ignore;//padding
		};

		struct T3DVSConstantBuffer
		{
			XMMATRIX WorldMatrix;

			//XMMATRIX* worldMatrix;
			//float	 padding[15];
		};

		//struct TDirectionalLightInfo
		//{
		//	PxVec4		v4Direction;
		//	PxVec4		v4Position;
		//	PxVec4		v4Color;

		//	TDirectionalLightInfo()
		//	{
		//		v4Color = PxVec4(0.5f);
		//		v4Direction = { 0.2f, -1.f, 0.4f, 0.f };
		//		v4Direction.normalize();
		//		v4Position = { 0, 30, 0, 0 };
		//	}
		//};

		//struct TDirectionalLightConstantBuffer
		//{
		//	TDirectionalLightInfo directionalLights[2];
		//};

		struct T2DContextInfo
		{
			eDepth				eDepthFromCamera;
			T2DInfo				tInfo;

			T2DContextInfo()
			{
				eDepthFromCamera = HUD;
			}
		};

		struct T3DContextInfo
		{
			bool				bIsTransparent;
			eDepth				eDepthFromCamera;
			T3DVSConstantBuffer	tConstantBufferInfo;
			std::string			szModelName;
			//eModel				idModel;

			T3DContextInfo()
			{
				szModelName = "Unnamed";
				bIsTransparent = false;
				eDepthFromCamera = Playable;
				//tPSInfo.v4Color = PxVec4(1.f);
			}
		};

		//struct TAnimationContextInfo
		//{
		//	eDepth						eDepthFromCamera;
		//	T3DVSConstantBuffer			tConstantBufferInfo;
		//	T3DPSConstantBuffer			tPSInfo;

		//	std::string			szModelName;
		//	//eModel						idModel;
		//	std::string					szAnimationName;
		//	TObject						tObjectData;

		//	TAnimationContextInfo()
		//	{
		//		szModelName = "Unnamed";
		//		eDepthFromCamera = Playable;
		//		tPSInfo.v4Color = PxVec4(1.f);
		//	}
		//};

	}
}