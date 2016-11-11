#pragma once
#include "../../Core/Entity/GameObject.h"
#include "../../Core/Events/Listener.h"


class FTJ::CEntityManager;

using namespace FTJ;

class CPlayer : public FTJ::CListener
{

public:
	CPlayer();
	~CPlayer();

	virtual void	HandleEvent(const CEvent* pEvent) override;

	virtual void	Update(const float _fDeltaTime);

};

