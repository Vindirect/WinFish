#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#pragma once

#include "DataSync.h"
#include "SexyAppFramework/Widget.h"
#include <SexyAppFramework/Buffer.h>

namespace Sexy
{
	class Graphics;
	class WinFishApp;
	class Shadow;
	class Missle;
	class DataSync;
	class Image;
	class MemoryImage;
	class Breeder;
	class Coin;

	class GameObject : public Widget
	{
	public:
		WinFishApp*			mApp;
		int					mType;
		int					mCrosshairAnimationTimer;
		Shadow*				mShadowPtr;
		Missle*				mMisslePtr;
		int					mHunger;
		int					mHungerAnimationTimer;
		int					mHungerShowTreshold;
		bool				mHungerShown;
		int					mVirtualTankId;
		SexyString			mName;
		__time64_t			mTimeBought;
		int					m0xd8;
		int					mHometownIdx;
		int					mShellPrice;
		int					mLikes[3];
		int					mExoticDietFoodType;
		bool				mShown;
		bool				mForwardlyChallenged;
		bool				mSpeedy;
		int					mSpeedySpeedState;
		bool				mInvisible;
		int					mInvisibleTimer;
		bool				mVoracious;
		int					mVoraciousScreamCounter;
		int					m0x10c;
		bool				mSinging;
		bool				mTodayBought;
		int					m0x114;
		int					m0x118;
		int					mSongId;
		int					mCanBeEatenDelay;
		int					mPreNamedTypeId;
		int					m0x128;
		int					mStoreAnimationTimer;
		int					mStoreAnimationIndex;
		long long			m0x138;
		long long			m0x140;
		int					mTimesFedToday;
		int					mMentalState;
		int					m0x150;

		/////////////////////////////////////////////////////////////////////

		enum PreNamedFishTypes
		{
			ROCKY,
			LUDWIG,
			COOKIE,
			JOHNNYV,
			KILGORE,
			SANTA
		};

		enum GameObjectTypes
		{
			TYPE_DEFAULT = -1,
			TYPE_GUPPY,
			TYPE_MEDIUM_GUPPY,
			TYPE_BIG_GUPPY,
			TYPE_STAR_GUPPY,
			TYPE_CROWNED_GUPPY,
			TYPE_OSCAR,
			TYPE_ULTRA,
			TYPE_GEKKO,
			TYPE_PENTA,
			TYPE_GRUBBER,
			TYPE_BREEDER,
			TYPE_MEDIUM_BREEDER,
			TYPE_BIG_BREEDER,
			TYPE_OTHER_TYPE_PET = 20,
			TYPE_FISH_TYPE_PET,
			TYPE_ALIEN,
			TYPE_BILATERUS,
			TYPE_COIN = 25,
			TYPE_DEAD_ALIEN,
			TYPE_DEAD_FISH,
			TYPE_FOOD,
			TYPE_LARVA,
			TYPE_MISSLE,
			TYPE_SHADOW,
			TYPE_SHOT,
			TYPE_WARP,
			TYPE_SYLVESTER_FISH,
			TYPE_BALL_FISH,
			TYPE_BI_FISH,
		};

		enum PetTypes
		{
			PET_STINKY,
			PET_NIKO,
			PET_ITCHY,
			PET_PREGO,
			PET_ZORF,
			PET_CLYDE,
			PET_VERT,
			PET_RUFUS,
			PET_MERYL,
			PET_WADSWORTH,
			PET_SEYMOUR,
			PET_SHRAPNEL,
			PET_GUMBO,
			PET_BLIP,
			PET_RHUBARB,
			PET_NIMBUS,
			PET_AMP,
			PET_GASH,
			PET_ANGIE,
			PET_PRESTO,
			PET_BRINKLEY,
			PET_NOSTRADAMUS,
			PET_STANLEY,
			PET_WALTER,
			PET_END
		};

		enum FishSizes
		{
			SIZE_SMALL,
			SIZE_MEDIUM,
			SIZE_LARGE,
			SIZE_STAR,
			SIZE_CROWNED
		};

	public:
		GameObject();
		virtual ~GameObject();

		virtual void			Update();
		virtual void			UpdateF(float theFrac);
		virtual void			Draw(Graphics* g);

		virtual void			CountRequiredFood(int* theFoodReqPtr);			//[70]
		virtual int				SpecialReturnValue();							//[71]
		virtual int				GetShellPrice();								//[72]
		virtual void			PrestoMorph(int thePetId);						//[73]
		virtual void			VFT74();										//[74]
		virtual void			Remove();										//[75]
		virtual void			SetPosition(double newX, double newY);			//[76]
		virtual void			OnFoodAte(GameObject* obj);						//[77]
		virtual void			UpdateStoreAnimation();							//[78]
		virtual void			DrawStoreAnimation(Graphics* g, int justification);//[79]
		virtual void			Sync(DataSync* theSync);						//[80]

		void					UpdateCounters();
		bool					CoinDropTimerPassed(int theCurrentTime, int theToPassTime);
		bool					RelaxModeCanDrop();
		void					UpdateCrosshairAnimation();
		void					ShowInvisibility();
		void					UpdateInvisible();
		void					UpdateHungerAnimCounter();
		void					UpdateHungerCounter();
		void					UpdateHungerState(bool theHungryShown);
		bool					ShouldDie();
		int						ExoticFoodCollision(int theCenterX, int theCenterY);
		int						CarnivorousExoticFoodCollision(int theCenterX, int theCenterY, int theID);
		void					RemoveHelper02(bool safeDelete);
		void					UpdateFishSongMgr();
		void					Unk02(bool flag);
		void					Unk03(long long theTodayInSec, __time64_t theCurTime);
		bool					UpdateMentalState();
		void					DrawName(Graphics* g, bool flag);
		void					DrawCrosshair(Graphics* g, int theX, int theY);
		bool					IsHungryVisible();
		GameObject*				FindNearestExoticFood(int theX, int theY);
		GameObject*				FindNearestExoticFoodOther(int theX, int theY, int theObjType);
		void					VoraciousScream(int theNum);
		bool					IsHungryBlipPointer(int theTreshold);
		void					SetColorHelper(Graphics* g, Color& theColor);
		void					UpdateHungerStateIfWasHungry(bool wasHungry);
		bool					DrawInvisibleEffect(Graphics* g, Image* theImage, Rect& theSrcRect, bool mirror);
		void					DrawInvisibleEffectHelper(Graphics* g, MemoryImage* theBGImage, int theX, int theY, MemoryImage* theObjImage, Rect& theSrcRect, bool mirror);
		bool					PrestoRightClicked(int theTimer);
		void					DrawPrestoMisc(Graphics* g, int theTimer);
		void					DeterminePetSleepy(bool* sleepy);
		SexyString				GetStoreDescription();
		int						GetAttribute();
		int						GetShellCost();
		void					SetStoreColor(Graphics* g, Color& theColor);
		void					ResetSpecialProperties();
		void					SomeBoughtFunc();
		bool					CanDropCoin();
		int						DetermineCoinDropT(int supposedTime);
		void					CopyBreederDataVT(Breeder* theBreeder);
		bool					RelaxModeCheck(Coin* theCoin);
	};
}

#endif