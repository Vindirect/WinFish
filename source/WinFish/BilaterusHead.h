#pragma once

#include "Bilaterus.h"

namespace Sexy
{
	class BilaterusHead : public GameObject
	{
	public:
		Bilaterus* mBilaterus;
		bool m0x15c;
		bool m0x15d;
		bool mHeadActive;
		int m0x160;

		double mXD;
		double mYD;
		double mVX;
		double mVY;
		double m0x188;
		double m0x190;
		double m0x198;
		double m0x1a0;
		int m0x1a8;
		int m0x1ac;
		int m0x1b0;
		int m0x1b4;
		int m0x1b8;
		int m0x1bc;
		int m0x1c0;
		int m0x1c4;
		int m0x1c8;
		int m0x1cc;
		int m0x1d0;
		int m0x1d4;
		double m0x1d8;
		double m0x1e0;
		double m0x1e8;
		double m0x1f0;
		double m0x1f8;
		int m0x200;

	public:
		BilaterusHead(Bilaterus* theBilaterus, int theX, int theY, bool active);
		~BilaterusHead();

		virtual void			Draw(Graphics* g);

		virtual void			Remove();										//[75]
		virtual void			OnFoodAte(GameObject* obj);						//[77]
		virtual void			Sync(DataSync* theSync);						//[80]

		void					UpdateHead();
		void					DrawHead(Graphics* g, bool mirror);
		void					DrawHeadHelper(Graphics* g, Image* theImage, Rect& theSrcRect,bool mirror);
		void					UpdateAnimation();
		bool					TakeHit(int theX, int theY);
		bool					ShouldChaseTarget();
		GameObject*				FindNearestTarget();
		void					ChaseTarget();
		void					Die(bool flag);
		void					BHUnk01();
		void					CheckCollision();
	};
}

