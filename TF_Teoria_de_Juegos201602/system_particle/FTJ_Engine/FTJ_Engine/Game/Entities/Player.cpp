#include "Player.h"
#include "../../Core/Events/Event.h"
#include "../../Core/Input/InputManager.h"

#include "../../Core/FTJ_Console.h"

CPlayer::CPlayer() : CListener(this)
{
}

CPlayer::~CPlayer()
{

}

/*virtual*/ void CPlayer::HandleEvent(const CEvent* pEvent) /*override*/
{
}

/*virtual*/ void CPlayer::Update(const float _fDeltaTime) /*override*/
{

}