#pragma once
#include "../../Core/Entity/IBehavior.h"

class CameraBehavior : public FTJ::IBehavior
{
	XMMATRIX translationMat;
	XMMATRIX rotationMat;

	virtual void Update(const float fDeltaTime);//gets call through the ObjectManager
public:
	CameraBehavior(FTJ::CGameObject *_pGameObject);
	~CameraBehavior();
};

