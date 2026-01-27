#pragma once
#include "GameObject.h"
#include "Coin.h"

namespace Sexy
{
	class OtherTypePet : public GameObject
	{
	public:
		double					mXD;
		double					mYD;
		double					mVX;
		double					mVY;
		double					mPrevVX;
		double					mTargetVX;
		double					mSpeedMod;
		int						mMovementState;
		int						mChaseEntityTimer;
		int						mMovementStateChangeTimer;
		int						mTurnAnimationTimer;
		int						mMovementAnimationTimer;
		int						mAnimationIndex;
		int						mPrestoTransformAnimTimer;
		int						mPrestoTimer;
		bool					mSleepy;
		int						mPetSpecialtyTimer;
		int						m0x1b8;
		int						mPetAngryTimer;
		bool					m0x1c0;
		int						mOtherTypePetType;
		bool					mIsPresto;

	public:
		OtherTypePet();
		OtherTypePet(int theX, int theY, int thePetType, int theBackgroundId, bool flag);

		virtual void			Update();
		virtual void			Draw(Graphics* g);
		virtual void			MouseDown(int x, int y, int theClickCount);

		virtual void			PrestoMorph(int thePetId);						//[73]
		virtual void			Remove();										//[75]
		virtual void			Sync(DataSync* theSync);						//[80]

		void					UpdateNikoPosition(int theBgId);
		void					UpdatePetAnimations();
		void					UpdatePetSpecialAnimations();
		bool					ChaseEnemyBehavior();
		bool					ChaseEntity();
		GameObject*				GetEntityToChase();
		void					DrawHelper(Graphics* g, bool mirror);
		void					DrawStinky(Graphics* g, bool mirror);
		void					DrawNiko(Graphics* g);
		void					DrawClyde(Graphics* g, bool mirror);
		bool					HandlePrestoClick(int x, int y, int theClickCount);
		void					CollideWithObject();
	};
}

