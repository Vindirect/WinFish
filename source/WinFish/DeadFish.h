#pragma once
#include "GameObject.h"
namespace Sexy
{
	class DeadFish : public GameObject
	{
	public:
		bool mFlipped;
		double mYD;
		double mXD;
		double mVX;
		double mVY;
		double mSpeedMod;
		bool m0x188;
		int m0x18c;
		int m0x190;
		int m0x194;
		double m0x198;
		int m0x1a0;
		int mObjType;

	public:
		DeadFish();
		DeadFish(int theX, int theY, double theVX, double theVY, double theSpeedMod, int theObjType, bool facingRight);

		virtual void			Update();
		virtual void			Draw(Graphics* g);

		virtual void			Remove();										//[75]

		virtual void			Sync(DataSync* theSync);						//[80]
		virtual void			RemoveDeadFish(); // [81]
	};
}

