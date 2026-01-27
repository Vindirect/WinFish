#pragma once

#include "GameObject.h"
#include "Alien.h"
#include "Bilaterus.h"

namespace Sexy
{
	class Bilaterus;

	class BilaterusBone : public GameObject
	{
	public:
		Bilaterus* mBilaterus;
		int mBoneId;
		double mXD;
		double mYD;
		double m0x170;
		double m0x178;
		double m0x180;
		double m0x188;
		double m0x190;
		double m0x198;
		int m0x1a0;
		int m0x1a4;
		int m0x1a8;
		int m0x1ac;
		int m0x1b0;
		int m0x1b4;
		int m0x1b8;
		int m0x1bc;
		int m0x1c0;

		double m0x1c8;
		double m0x1d0;
		double m0x1d8;
		double m0x1e0;
		int m0x1e8;

	public:
		BilaterusBone(Bilaterus* theBilaterus, int theX, int theY, int theBoneId);
		~BilaterusBone();

		virtual void			Draw(Graphics* g);

		virtual void			OnFoodAte(GameObject* obj);						//[77]
		virtual void			Sync(DataSync* theSync);						//[80]

		void					UpdateBone();
		void					UpdateFollowPosAndVel();
		void					CheckCollision();
		void					DrawBone(Graphics* g, bool mirror);
		void					DrawBoneHelper(Graphics* g, Image* theImage, int theSrcX, int theSrcY, bool mirror);
	};
}

