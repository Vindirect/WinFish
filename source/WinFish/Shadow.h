#pragma once
#include "GameObject.h"
namespace Sexy
{
	class Shadow : public GameObject
	{
	public:
		GameObject* mObjectPtr;
		int mShadowSize;
		int m0x160;
		double m0x168;

	public:
		Shadow();
		Shadow(int theSize, GameObject* theObject);

		virtual void			Update();
		virtual void			Draw(Graphics* g);

		virtual void			VFT74();										//[74]
		virtual void			Remove();										//[75]
		virtual void			Sync(DataSync* theSync);						//[80]
	};

}
