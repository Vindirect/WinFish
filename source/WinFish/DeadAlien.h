#pragma once
#include "GameObject.h"

namespace Sexy
{
	class DeadAlien : public GameObject
	{
	public:
		bool mFacingRight;
		double mYD;
		double mXD;
		double mVX;
		double mVY;
		double m0x180;
		int mDeathAnimationIdx;

		double mOpacity;
		int mTimer;
		int m0x19c;
		int mAlienType;

	public:
		DeadAlien();
		DeadAlien(int theX, int theY, int theAlienType, int theAnimIndex, bool facingRight);

		virtual void			Update();
		virtual void			Draw(Graphics* g);

		virtual void			Remove();										//[75]
		virtual void			Sync(DataSync* theSync);						//[80]
		virtual void			RemoveDeadAlien();								// [81]
		
		bool					DeadAlienUnk01();
	};
}

