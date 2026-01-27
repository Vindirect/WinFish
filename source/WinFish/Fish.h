#ifndef __Fish_H__
#define __Fish_H__

#pragma once

#include "GameObject.h"

namespace Sexy
{
	class Food;

	class Fish : public GameObject
	{
	public:
		double mXD, mYD;
		double mVX, mVY;
		int mXDirection;
		int m0x17c;
		double mSpeedMod;
		double mPrevVX;
		int mXMin, mXMax, mYMin, mYMax;
		int mSize;
		int mFoodAte;
		int mFoodNeededToGrow;
		int mMovementState;
		int mSpecialMovementStateChangeTimer;
		int mMovementStateChangeTimer;
		int mSwimFrameCounter;
		int mVXAbs;
		int mAnimationFrameIndexFish;
		int mTurnAnimationTimer;
		int mEatingAnimationTimer;
		int mGrowthAnimationTimer;
		bool mHasSpecialColors;
		bool mRainbowFish;
		int mRainbowFishDeterminant;
		Color mColors[3];
		int mBoughtTimer;
		int mWadsworthVXModCounter;
		int mUnusedTimer;
		double mUnusedVXWadsworthAddon;
		int mCoinDropTimer;
		int mCoinDropT;
		bool mBeginner;
		bool mIsGuppy;
		bool mVirtualFish;

	public:
		Fish();
		Fish(int theX, int theY);
		Fish(int theX, int theY, int theSize, bool theVelDirection);
		virtual ~Fish();

		virtual void			Update();
		virtual void			Draw(Graphics* g);

		virtual void			MouseDown(int x, int y, int theClickCount);
		virtual void			MouseUp(int x, int y, int theClickCount);

		// GameObject VFT
																				//[70]
		virtual int				SpecialReturnValue();							//[71]
		virtual int				GetShellPrice();								//[72]
																				//[73]
																				//[74]
		virtual void			Remove();										//[75]
		virtual void			SetPosition(double newX, double newY);			//[76]
		virtual void			OnFoodAte(GameObject* obj);						//[77]
		virtual void			UpdateStoreAnimation();							//[78]
		virtual void			DrawStoreAnimation(Graphics* g, int justification);//[79]
		virtual void			Sync(DataSync* theSync);						//[80]

		// Fish VFT
		virtual void			DropCoin();										//[81]
		virtual bool			Hungry();										//[82]
		virtual void			DrawFish(Graphics* g, bool mirror);				//[83]
		virtual bool			HungryBehavior();								//[84]
		virtual GameObject*		FindNearestFood();								//[85]
		virtual void			CollideWithFood();								//[86]
		virtual void			RemoveFromGame(bool aRemoveShadow);				//[87]
		virtual void			Die(bool flag = false);							//[88]
		virtual void			FishUpdateAnimation();							//[89]
		virtual void			SetFishColors(int theRandIdx, bool rainbow);	//[90]

		void					Init(int theX, int theY);
		void					UpdateRainbow();
		bool					CanEatFood(Food* theFood);
		Image*					GetImageToDraw(bool theFlag);
		void					ClickedBehavior(int theX, int theY);
		void					FishOnGrow();
		bool					WadsworthActive();
	};
}

#endif