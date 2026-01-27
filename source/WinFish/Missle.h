#pragma once
#include "GameObject.h"
namespace Sexy
{
	class Missle : public GameObject
	{
	public:
		double mXD;
		double mYD;
		double mVX;
		double mVY;
		int m0x178;
		int m0x17c;
		int m0x180;
		double mSpeedMod;
		bool m0x190;
		int m0x194;
		int m0x198;
		int mMissleType;
		int m0x1a0;
		GameObject* mTarget;
		int m0x1a8;
		int m0x1ac;
		int m0x1b0;
		int m0x1b4;

		enum MissleTypes
		{
			MISSLE_CLASSIC,
			MISSLE_ENERGYBALL,
			MISSLE_BALL,
			MISSLE_BONE,
			MISSLE_HEAD1,
			MISSLE_HEAD2
		};

	public:
		Missle();
		Missle(int x, int y, GameObject* theTarget, int theType);

		virtual void			Update();
		virtual void			Draw(Graphics* g);

		virtual void			VFT74();										//[74]
		virtual void			Remove();										//[75]
		virtual void			OnFoodAte(GameObject* obj);						//[77]
		virtual void			Sync(DataSync* theSync);						//[80]

		bool					IsTargetless();
		void					UpdateAnimations();
		void					DrawHelper(Graphics* g, bool flag);
		bool					ChaseTarget();
		bool					CheckCollision();
		bool					Shot(int x, int y);
	};
}

