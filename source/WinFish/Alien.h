#pragma once
#include "GameObject.h"
namespace Sexy
{
	class Alien : public GameObject
	{
	public:
		int mSpawnAnimationTimer;
		double mXD;
		double mYD;
		double mVX;
		double mVY;
		double mTargetVX;
		double mTargetVY;
		bool mGusFoundFood;
		int mPsychosquidSwitchTimer;
		int mPsychosquidSwitchThreshold;
		bool mWasPsychosquidHealing;
		int mChaseTargetDelay;
		int mMissleLaunchTimer;
		int mMissleReloadTime;
		int mSpecialAnimTimer;
		int mSwimAnimTimer;
		int mTurnAnimTimer;
		double mSpeedMod;
		int mAnimIndex;
		double mPrevVX;
		uint mMovementState;
		int m0x1d4;
		int mMovementChangeTimer;
		int mCyraxSpawnAlienTimer;
		int mCyraxSpawnMiniTimer;
		int mCyraxSpawnAlienThreshold;
		int mCyraxSpawnMiniThreshold;
		int mAlienType;
		double mHealth;
		double mMaxHealth;
		int mHitFlashTimer;
		bool mIsPsychosquidHealing;

		enum
		{
			ALIEN_NONE,
			ALIEN_WEAK_SYLV,
			ALIEN_STRONG_SYLV,
			ALIEN_BALROG,
			ALIEN_GUS,
			ALIEN_DESTRUCTOR,
			ALIEN_ULYSEES,
			ALIEN_PSYCHOSQUID,
			ALIEN_BILATERUS,
			ALIEN_MINI_SYLV = 20,
			ALIEN_CYRAX
		};

	public:
		Alien();
		Alien(int theX, int theY, int theAlienType);

		virtual void			Update();
		virtual void			Draw(Graphics* g);

		virtual void			CountRequiredFood(int* theFoodReqPtr);			//[70]
		virtual void			VFT74();										//[74]
		virtual void			Remove();										//[75]
		virtual void			OnFoodAte(GameObject* obj);						//[77]
		virtual void			Sync(DataSync* theSync);						//[80]

		bool					Shot(int x, int y);
		bool					TankHasFish();
		bool					AlienUnk01();
		void					CyraxDieCleanup();
		GameObject*				FindObjectToShoot();
		GameObject*				FindNearestFood();
		void					ChaseBehavior();
		void					CheckCollision();
		void					DrawAlien(Graphics* g, bool mirror);
		void					UpdateAnimations();
		void					Remove(bool removeShadow);
		void					SpawnWarp();
		bool					FoodDroppedAtAlien(int theX, int theY);
	};
}

