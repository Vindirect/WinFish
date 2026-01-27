#pragma once

#include "SexyAppFramework/Widget.h"
#include "SexyAppFramework/ButtonListener.h"
#include "SexyAppFramework/Dialog.h"

#include "WinFishApp.h"
#include "DataSync.h"
#include "GameObject.h"
#include "Fish.h"
#include "DeadFish.h"
#include "DeadAlien.h"
#include "Oscar.h"
#include "Food.h"
#include "FishTypePet.h"
#include "OtherTypePet.h"
#include "Alien.h"
#include "Breeder.h"
#include "Grubber.h"
#include "Larva.h"
#include "Gekko.h"
#include "Penta.h"
#include "Ultra.h"
#include "Shot.h"
#include "Shadow.h"
#include "Coin.h"
#include "Missle.h"
#include "Warp.h"
#include "SylvesterFish.h"
#include "BallFish.h"
#include "BiFish.h"
#include "FishSongMgr.h"
#include "BubbleMgr.h"
#include "Bilaterus.h"
#include "MessageWidget.h"

#include "Res.h"

extern bool mBlipInTank;

namespace Sexy
{
	class WinFishApp;
	class ButtonWidget;
	class DialogButton;
	class Graphics;
	class WidgetManager;
	class Screen;
	class BoardOverlay;
	class MenuButtonWidget;
	class MyLabelWidget;
	class StarField;

	typedef std::vector<bool> BoolVector;
	typedef std::vector<int> IntVector;
	typedef std::set<GameObject*> GameObjectSet;

	class Board : public Widget, public ButtonListener
	{
	public:
		WinFishApp*							mApp;
		FishSongMgr*						mFishSongMgr;
		bool								mPause;
		std::vector<DeadFish*>*				mDeadFishList;
		std::vector<DeadAlien*>*			mDeadAlienList;
		std::vector<Fish*>*					mFishList;
		std::vector<Oscar*>*				mOscarList;
		std::vector<Coin*>*					mCoinList1;
		std::vector<Food*>*					mFoodList;
		std::vector<OtherTypePet*>*			mOtherTypePetList;
		std::vector<FishTypePet*>*			mFishTypePetList;
		std::vector<Alien*>*				mAlienList;
		std::vector<Alien*>*				mSmallAlienList;
		std::vector<Breeder*>*				mBreederList;
		std::vector<Grubber*>*				mGrubberList;
		std::vector<Larva*>*				mLarvaList;
		std::vector<Gekko*>*				mGekkoList;
		std::vector<Penta*>*				mPentaList;
		std::vector<Ultra*>*				mUltraList;
		std::vector<Shot*>*					mShotList;
		std::vector<Missle*>*				mMissleList1;
		std::vector<Missle*>*				mMissleList2;
		std::vector<Shadow*>*				mShadowList;
		std::vector<Coin*>*					mCoinList2;
		std::vector<Coin*>*					mCoinList3;
		std::vector<Warp*>*					mWarpList;
		std::vector<Bilaterus*>*			mBilaterusList;
		std::vector<Fish*>*					mSpecialFishList;
		BoardOverlay*						mBoardOverlay1;
		BoardOverlay*						mBoardOverlay2;
		GameObjectSet						mGameObjectSet;
		Alien*								mCyraxPtr;
		ButtonWidget*						mMenuButton;
		MessageWidget*						mMessageWidget;
		BubbleMgr*							mBubbleMgr;
		CheatCode*							mCheatCodes[8];
		StarField*							mStarField;
		int									mPetsInTank[GameObject::PET_END];
		int									mSoundPlayedTimerArray[SOUND_END_ID - SOUND_START_ID + 1];
		bool								mBonusRoundStarted;
		bool								mIsBonusRound;
		bool								m0x2a6;
		bool								m0x2a7;
		int									m0x2a8;
		int									mLastCoinTypeClicked;
		int									mCoinComboCount;
		int									m0x2b4;
		bool								mHasVirtualTankFish;
		int									mAlienExpect;
		int									mAlienTimer;
		int									m0x2c4;
		int									mDropFoodDelay;
		int									mCrosshair1X;
		int									mCrosshair1Y;
		int									mCrosshair2X;
		int									mCrosshair2Y;
		int									m0x2dc;
		int									m0x2e0;
		enum SlotTypes
		{
			SLOT_GUPPY,
			SLOT_BREEDER,
			SLOT_FOODLVL,
			SLOT_FOODLIMIT,
			SLOT_OSCAR,
			SLOT_POTION,
			SLOT_STARCATCHER,
			SLOT_GRUBBER,
			SLOT_GEKKO,
			SLOT_ULTRA,
			SLOT_WEAPON,
			SLOT_EGG,
			SLOT_END
		};
		int									mSlotNumber[SLOT_END];
		int									mSlotPrices[SLOT_END];
		int									m0x344[SLOT_END];
		int									m0x374[SLOT_END];
		bool								mSlotUnlocked[SLOT_END];
		int									m0x3b0;
		int									m0x3b4;
		int									m0x3b8;
		int									m0x3bc;
		int									m0x3c0;
		int									m0x3c4;
		int									mTank;
		int									mLevel;
		int									m0x3d0;
		int									m0x3d4;
		float								mTankLightingSpeeds[3];
		int									m0x3e4;
		int									mCurrentBackgroundId;
		int									m0x3ec;
		int									mMoney;
		int									m0x3f4;
		int									m0x3f8;
		int									m0x3fc;
		int									m0x400;
		MenuButtonWidget*					mMenuButtons[12];
		MenuButtonWidget*					mBackButton;
		MyLabelWidget*						mMoneyLabel;
		int									m0x43c;
		bool								m0x440;
		bool								m0x441;
		int									mGameUpdateCnt;
		int									mBubbulatorTimer;
		int									m0x44c;
		int									m0x450;
		int									m0x454;
		int									m0x458;
		int									m0x45c;
		int									m0x460;
		int									m0x464;
		int									m0x468;
		int									mMaxOscarCountEver;
		int									mMaxUltraCountEver;
		int									m0x474;
		int									m0x478;
		int									m0x47c;
		int									m0x480;
		int									m0x484;
		int									mGuppiesDeadCount;
		int									mMaxFishCountEver;
		int									m0x490;
		int									mMaxPentaCountEver;
		int									m0x498;
		int									m0x49c;
		int									m0x4a0;
		int									m0x4a4;
		int									m0x4a8;
		int									m0x4ac;
		bool								m0x4b0[54];
		bool								m0x4e6;
		int									m0x4e8;
		bool								m0x4ec;
		bool								m0x4ed;
		bool								mShouldSave;
		int									m0x4f0;
		bool								m0x4f4;
		int									m0x4f8;
		bool								m0x4fc;
		bool								mBubbulatorShown;
		bool								mAlienAttractorShown;
		bool								mAlwaysShowWhenHungry;
		bool								m0x500;

		enum CheatCodeTypes
		{
			CC_WAVY,
			CC_PREGO,
			CC_VOID,
			CC_SPACE,
			CC_ZOMBIE,
			CC_BETATEST,
			CC_SUPERMEGA,
			CC_TIME
		};

	public:
		Board(WinFishApp* theApp);
		~Board();

		virtual void			AddedToManager(WidgetManager* theWidgetManager);
		virtual void			RemovedFromManager(WidgetManager* theWidgetManager);
		virtual void			Update();
		virtual void			Draw(Graphics* g);
		virtual void			DrawOverlay(Graphics* g, int thePriority);
		virtual void			KeyChar(SexyChar theChar);
		virtual void			KeyDown(KeyCode theKey);
		virtual void			MouseDown(int x, int y, int theClickCount);
		virtual void			MouseUp(int x, int y, int theClickCount);

		virtual void			ButtonPress(int theId);
		virtual void			ButtonDepress(int theId);

		void					DrawOverlay0(Graphics* g);
		void					DrawOverlay1(Graphics* g);

		void					DrawTankBackground(Graphics* g);
		void					DrawTankWaves(Graphics* g, int theY);
		void					DrawAlienAttractorMisc(Graphics* g, int theX, int theY, int theTime, int unk);
		void					DrawBonusRound(Graphics* g);
		void					DrawCoolBonusString(Graphics* g, SexyString& theString, int theX, int theY);

		bool					IsFirstLevel();
		void					ShowText(std::string aText, bool aFlag, int anID);
		bool					AliensInTank();
		bool					RemoveGameObjectFromLists(GameObject* theObject, bool aFlag);
		void					SpawnBubble(int theX, int theY);
		void					SpawnRandomBubble();
		void					AddGameObject(GameObject* theObject);
		void					MakeShadowForGameObject(GameObject* theObject);
		void					SortGameObjects();
		void					PauseGame(bool shouldPause);
		void					StartGame();
		void					SaveCurrentGame();
		void					SaveGame(const SexyString& theSavePath);
		bool					SyncGameData(DataSync& aSync);
		GameObject*				CreateGameObject(int theType);
		bool					LoadGame(SexyString theSavePath);
		void					InitLevel();
		void					InitBonusLevel();
		void					StartMusic();
		int						Unk01();
		int						Unk02();
		void					Unk03();
		void					Unk04();
		void					Unk05();
		void					Unk06(Graphics* g, Image* theImage, int theX, int theY, float theVal);
		void					Unk07(int unk);
		bool					Unk08(int x, int y);
		bool					Unk09(Coin* theCoin);
		int						Unk10();
		bool					Unk11(int x, int y);
		void					Unk12();
		void					Unk13();
		void					Unk14(bool unk);
		void					Unk15();
		void					DetermineAlienSpawnCoordsVT();
		int						GetNextVirtualTankId();
		bool					HasAnyFish();
		void					HandleBuyEgg();
		void					HandleFeedButton();
		void					UpdateHasVirtualTankFish();
		void					BonusRoundDropShell();
		void					NostradamusSneezeEffect();

		void					PlayChompSound(bool flag);
		void					PlaySlurpSound(bool isVoracious);
		void					PlayPointsSound();
		void					PlaySplashSound();
		void					PlayDieSound(int theObjType);
		void					PlayBirthSound(bool flag);
		void					PlayPunchSound(int theDelay);
		void					PlayZapSound();
		bool					CanPlaySound(int theSoundId, int theTimePassed);

		void					RemoveWidgetHelper(Widget* theWidget);
		void					ResetLevel(WidgetManager* theWidgetManager);
		void					DeterminePricesAndSlots();
		void					HandleBuySlotPressed(int theSlotId);
		void					RelaxModeConfig();
		void					MakeVirtualTankButtons();
		void					StartVirtualTank();
		void					SetAlienExpectVT();
		void					WidgetSetupVT();
		void					ChangeBackground(int theBGId);
		bool					DoCheatCode(int theCheatCode);
		GameObject*				GetPrestoPet();
		void					ApplyShadowsIf3D();
		void					UpdateNikoPosition();
		MenuButtonWidget*		MakeAndUnlockMenuButton(int theBtnId, bool flag);
		void					MenuButtonSetupNoVT(int theBtnId, bool flag);
		MenuButtonWidget*		GetMenuButtonById(int theBtnId);
		void					PlaySample(int theSoundId, int theUpdatesBeforePlayingAgain, double theVolume);
		void					GetBubbleSpawnCoords(int& theX, int& theY);
		void					GetAlienAttractorSpawnCoords(int& theX, int& theY);
		void					UpdateMoneyLabelText();
		bool					Buy(int theFoodCost, bool playBuzzer);
		void					DropFood(int theX, int theY, int unk1, bool unk2, int theFoodCantEatTimer, int theFoodTypeOverride);
		void					NostradamusDropFood(int theX, int theY);
		void					DropCoin(int theX, int theY, int theType, OtherTypePet* thePet, double theSpeed, int theUpdateCnt);
		bool					IsTankAndLevelNB(int theTank, int theLevel);
		void					CheckMouseDown(int theX, int theY);
		void					UpdateSlotPrice(int theSlotId, int thePrice);
		void					ResetMessageWidget(int unk);

		void					MakeNote(int theX, int theY, int unk, const SexyString& theText);
		int						PlayFishSong(int theObjType, int theSpecObjType);
		bool					IsSongPlaying(int theSongId);
		void					FishSongMgrUpdate();
		void					StopFishSong(int theSongId);

		void					DoVirtualTankDialog();
		void					GetExoticFoodsRequiredInTank(int* theInfoArray);
		void					GetExoticFoodsInTank(int* theInfoArray);
		GameObject*				GetGameObjectByVirtualId(int theId);
		void					HideObject(GameObject* theObj, bool hide);

		Fish*					SpawnGuppy(int theX, int theY);
		GameObject*				SpawnGuppyBought();
		GameObject*				SpawnStarGuppyBought();
		GameObject*				SpawnBreeder(int theX, int theY);
		GameObject*				SpawnBreederBought();
		GameObject*				SpawnOscarBought();
		GameObject*				SpawnUltraBought();
		GameObject*				SpawnGekkoBought();
		GameObject*				SpawnPentaBought();
		GameObject*				SpawnGrubberBought();
		GameObject*				SpawnPet(int thePetType, int theX, int theY, bool flag1, bool flag2);
		GameObject*				SpawnLarva(int theX, int theY);
		void					SpawnAlien(int theType, bool unk);
		void					SpawnAlien(int theType, int theX, int theY, bool unk);
		void					SpawnShot(int theX, int theY, int theType);
		void					SpawnLaserShot(int theX, int theY);
		void					SpawnMissle(int theX, int theY, GameObject* theTarget, int theType);
		void					SpawnShadow(int theSize, GameObject* theObject);
		void					SpawnDeadFish(int theX, int theY, double theVX, double theVY, double theSpeedMod, int theType, bool facingRight, Shadow* theShadowPtr);
		void					SpawnDeadAlien(int theX, int theY, int theAnimationIndex, int theAlienType, bool facingRight);
		void					SpawnGameObject(GameObject* theObject, bool randomPosition);
		void					SpawnVirtualTankFood(int theFoodType);
		GameObject*				SpawnGuppyAsFood();
		GameObject*				SpawnStarAsFood();
		GameObject*				SpawnLarvaAsFood();
		GameObject*				SpawnOscarAsFood();
		GameObject*				SpawnUltraAsFood();
		GameObject*				SpawnExoticFood(int theType);
		void					CyraxSpawnMiniAlien(int theX, int theY);

		void					RessurectFish(int theX, int theY, int theSize, bool flipped);
		void					RessurectOscar(int theX, int theY, bool flipped);
		void					RessurectUltra(int theX, int theY, bool flipped);
		void					RessurectGekko(int theX, int theY, bool flipped);
		void					RessurectPenta(int theX);
		void					RessurectGrubber(int theX);
		void					RessurectBreeder(int theX, int theY, int theSize, bool flipped);
	};

	class BoardOverlay : public Widget
	{
	public:
		Board* mBoard;
		int mPriority;

	public:
		BoardOverlay(Board* theBoard, int thePriority);
		~BoardOverlay();

		virtual void			Draw(Graphics* g);
	};
}
