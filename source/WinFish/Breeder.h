#pragma once
#include "Fish.h"
namespace Sexy
{
	class Breeder : public GameObject
	{
	public:
		double mXD;
		double mYD;
		double mVX;
		double mVY;
		int mXDirection;
		int m0x17c;
		double mSpeedMod;
		double mPrevVX;
		int mYMax;
		int mXMin;
		int mYMin;
		int mXMax;
		int mSize;
		int m0x1a4;
		int m0x1a8;
		int mGrowthAnimationTimer;
		int mMovementState;
		int mSpecialMovementStateChangeTimer;
		int m0x1b8;
		int mSwimAnimationTimer;
		int mVXAbs;
		int mAnimationIndex;
		int mTurnAnimationTimer;
		int mEatingAnimationTimer;
		int m0x1d0;
		int m0x1d4;
		int mBoughtTimer;
		int m0x1dc;

	public:
		Breeder();
		Breeder(int theX, int theY);
		Breeder(int theX, int theY, int theSize, bool velocityRight);
		virtual ~Breeder();

		virtual void			Update();
		virtual void			Draw(Graphics* g);
		virtual void			MouseDown(int x, int y, int theClickCount);
		virtual void			MouseUp(int x, int y, int theClickCount);

		virtual int				GetShellPrice();								//[72]
		virtual void			Remove();										//[75]
		virtual void			SetPosition(double newX, double newY);			//[76]
		virtual void			OnFoodAte(Food* theFood);						//[77]
		virtual void			UpdateStoreAnimation();							//[78]
		virtual void			DrawStoreAnimation(Graphics* g, int justification);//[79]
		virtual void			Sync(DataSync* theSync);						//[80]

		void					Init(int theX, int theY);
		bool					HungryLogic();
		void					Die(bool playDieSound = false);
		bool					HungryBehavior();
		void					DropCoin();
		void					UpdateAnimations();
		GameObject*				FindNearestFood();
		void					CollideWithFood();
		bool					GiveBirth();
		void					RemoveFromGame(bool removeShadow);
		void					DrawBreeder(Graphics* g, bool mirror);
		int						GetRowToDraw(bool hungry);
	};
}

