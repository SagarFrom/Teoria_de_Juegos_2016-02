#pragma once



typedef unsigned int uint;
typedef unsigned int ENTITYID;


class Texture;

namespace FTJ
{

	class IEntity
	{
	protected:
		ENTITYID m_EntityID;

	public:
		IEntity();
		virtual ~IEntity();
	};

}