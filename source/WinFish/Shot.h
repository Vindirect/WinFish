#pragma once
#include "GameObject.h"
namespace Sexy
{
	class Shot : public GameObject
	{
	public:
		int m0x158;
		int m0x15c;
		int mShotType;
		int m0x164;
		int m0x168;

	public:
		Shot();
		Shot(int theX, int theY);
		Shot(int theX, int theY, int theType);

		virtual void			Update();
		virtual void			Draw(Graphics* g);

		virtual void			Remove();										//[75]
		virtual void			Sync(DataSync* theSync);						//[80]

		void					SetRespectiveColor(Graphics* g, int theAlpha);
	};
}

