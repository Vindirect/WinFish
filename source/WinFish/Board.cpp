#include "Board.h"

#include "SexyAppFramework/ButtonWidget.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/SexyMatrix.h"
#include "SexyAppFramework/trivertex.h"
#include "SexyAppFramework/DialogButton.h"
#include "SexyAppFramework/ImageFont.h"
#include "SexyAppFramework/DDImage.h"
#include "SexyAppFramework/BassMusicInterface.h"
#include "SexyAppFramework/SoundManager.h"
#include "SexyAppFramework/SoundInstance.h"
#include "SexyAppFramework/Buffer.h"

#include "ImageLib/ImageLib.h"

#include "Res.h"
#include "Bilaterus.h"
#include "BilaterusHead.h"
#include "MenuButtonWidget.h"
#include "StarField.h"
#include "MyLabelWidget.h"

#include "WinFishAppCommon.h"
#include "HighScoreMgr.h"

#include <chrono>
#include <ctime>

using namespace Sexy;

const double PI = 3.141590118408203;

Board::Board(WinFishApp* theApp)
{
	mApp = theApp;

	// Decomp
	m0x2a6 = true;
	m0x2a7 = false;
	mStarField = new StarField();
	if ((mApp->mCurrentProfile->mCheatCodeFlags >> 3 & 1) != 0)
		mStarField->Init(1000);

	mCheatCodes[CC_WAVY] = new CheatCode("wavy");
	mCheatCodes[CC_PREGO] = new CheatCode("prego");
	mCheatCodes[CC_VOID] = new CheatCode("void");
	mCheatCodes[CC_SPACE] = new CheatCode("space");
	mCheatCodes[CC_ZOMBIE] = new CheatCode("zombie");
	mCheatCodes[CC_BETATEST] = new CheatCode("welovebetatesters");
	mCheatCodes[CC_SUPERMEGA] = new CheatCode("supermegaultra");
	mCheatCodes[CC_TIME] = new CheatCode("time");
	
	mFishSongMgr = new FishSongMgr();
	mFishSongMgr->mTone = SOUND_TONE;
	mFishSongMgr->mToneHi = SOUND_TONEHI;
	mFishSongMgr->mToneSuperHi = SOUND_TONESUPERHI;
	mFishSongMgr->mToneLo = SOUND_TONELO;

	mDeadFishList = new std::vector<DeadFish*>();
	mDeadAlienList = new std::vector<DeadAlien*>();
	mFishList = new std::vector<Fish*>();
	mOscarList = new std::vector<Oscar*>();
	mFoodList = new std::vector<Food*>();
	mOtherTypePetList = new std::vector<OtherTypePet*>();
	mFishTypePetList = new std::vector<FishTypePet*>();
	mAlienList = new std::vector<Alien*>();
	mSmallAlienList = new std::vector<Alien*>();
	mBreederList = new std::vector<Breeder*>();
	mGrubberList = new std::vector<Grubber*>();
	mLarvaList = new std::vector<Larva*>();
	mGekkoList = new std::vector<Gekko*>();
	mPentaList = new std::vector<Penta*>();
	mUltraList = new std::vector<Ultra*>();
	mShotList = new std::vector<Shot*>();
	mShadowList = new std::vector<Shadow*>();
	mCoinList1 = new std::vector<Coin*>();
	mCoinList2 = new std::vector<Coin*>();
	mCoinList3 = new std::vector<Coin*>();
	mMissleList1 = new std::vector<Missle*>();
	mMissleList2 = new std::vector<Missle*>();
	mWarpList = new std::vector<Warp*>();
	mBilaterusList = new std::vector<Bilaterus*>();
	mSpecialFishList = new std::vector<Fish*>();

	mGameObjectSet.clear();

	mBoardOverlay1 = new BoardOverlay(this, 0);
	mBoardOverlay2 = new BoardOverlay(this, 1);

	mBubbleMgr = new BubbleMgr();
	mBubbleMgr->mBubbleBounds = Rect(0, 82, 640, 398);
	mBubbleMgr->SetBubbleConfig(0, 0);
	gZombieMode = mApp->mCurrentProfile->mCheatCodeFlags >> 4 & 1;
	mCyraxPtr = nullptr;
	memset(m0x4b0, 0, sizeof(m0x4b0));
	memset(mPetsInTank, 0, sizeof(mPetsInTank));

	mCurrentBackgroundId = 0;
	m0x3ec = -1;

	int aVal = Unk01();
	m0x3b4 = aVal;
	m0x3b8 = aVal;
	m0x3bc = 0;
	m0x3b0 = 0;
	m0x3c0 = 0;
	m0x3c4 = 0;

	mPause = false;

	for (int i = 0; i < 3; i++)
		mTankLightingSpeeds[i] = mApp->mSeed->Next() % 640;

	mMenuButtons[SLOT_GUPPY] = NULL;
	mMenuButtons[SLOT_BREEDER] = NULL;
	mMenuButtons[SLOT_FOODLVL] = NULL;
	mMenuButtons[SLOT_FOODLIMIT] = NULL;
	mMenuButtons[SLOT_OSCAR] = NULL;
	mMenuButtons[SLOT_POTION] = NULL;
	mMenuButtons[SLOT_STARCATCHER] = NULL;
	mMenuButton = NULL;
	mMessageWidget = NULL;
	mMenuButtons[SLOT_GRUBBER] = NULL;
	mMenuButtons[SLOT_GEKKO] = NULL;
	mMenuButtons[SLOT_ULTRA] = NULL;
	mMenuButtons[SLOT_WEAPON] = NULL;
	mMenuButtons[SLOT_EGG] = NULL;
	mBackButton = NULL;
	mMoneyLabel = NULL;

	m0x4ec = false;
	m0x4ed = false;
	m0x4f0 = 0;

	mDropFoodDelay = 0;

	m0x4f4 = false;
	m0x4f8 = 0;

	mMenuButton = new ButtonWidget(123, this);
	mMenuButton->mDoFinger = true;
	mMenuButton->mButtonImage = IMAGE_BLANK;
	mMenuButton->mOverImage = IMAGE_OPTIONSBUTTON;
	mMenuButton->mDownImage = IMAGE_OPTIONSBUTTOND;
	mMenuButton->Resize(525, 3, 101, 29);

	mMoneyLabel = new MyLabelWidget();
	mMoneyLabel->mMouseVisible = false;
	mMoneyLabel->mAlignment = 2;
	mMoneyLabel->mX = 535;
	mMoneyLabel->mY = 40;
	mMoneyLabel->mLabelFont = FONT_CONTINUUMBOLD12;
	mMoneyLabel->mHeight = mMoneyLabel->mLabelFont->GetHeight();
	mMoneyLabel->mWidth = 80;

	Color aMoneyLabelColor = Color(0, 0, 0);
	if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
		aMoneyLabelColor = Color(0xfa, 0x9b, 0x96);
	else
		aMoneyLabelColor = Color(0xb4, 0xff, 0x5a);

	mMoneyLabel->mLabelColor = aMoneyLabelColor;

	mMessageWidget = new MessageWidget(mApp, "");

	mShouldSave = false;
	mTank = 0;
	mLevel = 0;
	mIsBonusRound = false;
	mGameUpdateCnt = 0;
	mBubbulatorTimer = 0;
	m0x44c = 0;
	m0x2c4 = 0;
	m0x3f4 = 0;
	m0x3f8 = 0;
	m0x3fc = 0;
	mLastCoinTypeClicked = -1;
	mCoinComboCount = 0;
	m0x400 = 0;
	m0x4fc = false;
	mBubbulatorShown = false;
	mAlienAttractorShown = false;
	mAlwaysShowWhenHungry = false;
	m0x500 = true;
	m0x441 = false;
	m0x2b4 = 0;
	for (int i = 0; i < 64; i++)
		mSoundPlayedTimerArray[i] = -1000;
	mHasVirtualTankFish = false;
}

Board::~Board()
{
	if(mMoneyLabel)
		delete mMoneyLabel;
	if (mMenuButton)
		delete mMenuButton;
	if (mMessageWidget)
		delete mMessageWidget;

	delete mDeadFishList;
	delete mDeadAlienList;
	delete mFishList;
	delete mOscarList;
	delete mUltraList;
	delete mFoodList;
	delete mCoinList1;
	delete mOtherTypePetList;
	delete mFishTypePetList;
	delete mAlienList;
	delete mSmallAlienList;
	delete mShotList;
	delete mPentaList;
	delete mGrubberList;
	delete mLarvaList;
	delete mGekkoList;
	delete mMissleList1;
	delete mMissleList2;
	delete mBreederList;
	delete mShadowList;
	delete mCoinList2;
	delete mCoinList3;
	delete mWarpList;
	delete mBilaterusList;
	delete mSpecialFishList;

	if (mFishSongMgr)
		mFishSongMgr->ClearSongs();

	if (mBoardOverlay1)
		delete mBoardOverlay1;
	if (mBoardOverlay2)
		delete mBoardOverlay2;
	if (mBubbleMgr)
		delete mBubbleMgr;
}

bool Board::IsFirstLevel()
{
	if (mLevel == 1 && mTank == 1 && !mApp->mCurrentProfile->mFinishedGame)
		return true;
	return false;
}

void Board::ShowText(std::string aText, bool aFlag, int anID)
{
	if (anID == -1)
	{
		mMessageWidget->m0xd8 = -1;
		mMessageWidget->mMessage = aText;
		mMessageWidget->m0xd4 = aFlag;
		if (!aFlag)
			mMessageWidget->m0xd0 = 100;
		else
			mMessageWidget->m0xd0 = 500;
	}
	else if (!m0x4b0[anID])
	{
		mMessageWidget->m0xd8 = anID;
		mMessageWidget->mMessage = aText;
		mMessageWidget->m0xd4 = aFlag;
		if (aFlag)
			mMessageWidget->m0xd0 = 500;
		m0x4b0[anID] = true;
		mMessageWidget->m0xd0 = 185;
	}
	mMessageWidget->mColor1 = Color(0xb4, 0xfa, 0x5a);
	mMessageWidget->mColor2 = Color(0, 0x4b, 0);
}

bool Sexy::Board::AliensInTank()
{
	return !mAlienList->empty() || !mBilaterusList->empty();
}

bool Board::RemoveGameObjectFromLists(GameObject* theObject, bool aFlag)
{
	bool unkbool = true;
	switch (theObject->mType)
	{
	case GameObject::TYPE_GUPPY:
		mFishList->erase(std::remove(mFishList->begin(), mFishList->end(), (Fish*)theObject), mFishList->end());
		break;
	case GameObject::TYPE_OSCAR:
		mOscarList->erase(std::remove(mOscarList->begin(), mOscarList->end(), (Oscar*)theObject), mOscarList->end());
		break;
	case GameObject::TYPE_ULTRA:
		mUltraList->erase(std::remove(mUltraList->begin(), mUltraList->end(), (Ultra*)theObject), mUltraList->end());
		break;
	case GameObject::TYPE_GEKKO:
		mGekkoList->erase(std::remove(mGekkoList->begin(), mGekkoList->end(), (Gekko*)theObject), mGekkoList->end());
		break;
	case GameObject::TYPE_PENTA:
		mPentaList->erase(std::remove(mPentaList->begin(), mPentaList->end(), (Penta*)theObject), mPentaList->end());
		break;
	case GameObject::TYPE_GRUBBER:
		mGrubberList->erase(std::remove(mGrubberList->begin(), mGrubberList->end(), (Grubber*)theObject), mGrubberList->end());
		break;
	case GameObject::TYPE_BREEDER:
		mBreederList->erase(std::remove(mBreederList->begin(), mBreederList->end(), (Breeder*)theObject), mBreederList->end());
		break;
	case GameObject::TYPE_OTHER_TYPE_PET:
	{
		OtherTypePet* aPet = (OtherTypePet*)theObject;
		if (aPet->mOtherTypePetType >= GameObject::PET_STINKY && aPet->mOtherTypePetType < GameObject::PET_END)
			mPetsInTank[aPet->mOtherTypePetType]--;
		mOtherTypePetList->erase(std::remove(mOtherTypePetList->begin(), mOtherTypePetList->end(), (OtherTypePet*)theObject), mOtherTypePetList->end());
		break;
	}
	case GameObject::TYPE_FISH_TYPE_PET:
	{
		FishTypePet* aPet = (FishTypePet*)theObject;
		if (aPet->mFishTypePetType >= GameObject::PET_STINKY && aPet->mFishTypePetType < GameObject::PET_END)
			mPetsInTank[aPet->mFishTypePetType]--;
		mFishTypePetList->erase(std::remove(mFishTypePetList->begin(), mFishTypePetList->end(), (FishTypePet*)theObject), mFishTypePetList->end());
		break;
	}
	case GameObject::TYPE_ALIEN:
	{
		Alien* anAlien = (Alien*)theObject;
		if (anAlien == mCyraxPtr) return false;
		if(anAlien->mAlienType == Alien::ALIEN_MINI_SYLV)
			mSmallAlienList->erase(std::remove(mSmallAlienList->begin(), mSmallAlienList->end(), (Alien*)theObject), mSmallAlienList->end());
		else
			mAlienList->erase(std::remove(mAlienList->begin(), mAlienList->end(), (Alien*)theObject), mAlienList->end());
		break;
	}
	case GameObject::TYPE_BILATERUS:
		mBilaterusList->erase(std::remove(mBilaterusList->begin(), mBilaterusList->end(), (Bilaterus*)theObject), mBilaterusList->end());
		break;
	case GameObject::TYPE_COIN:
	{
		Coin* aCoin = (Coin*)theObject;
		if (aCoin->m0x1a0 > 1 || aCoin->mCoinType == 15)
			mCoinList3->erase(std::remove(mCoinList3->begin(), mCoinList3->end(), aCoin), mCoinList3->end());
		if (aCoin->mCoinType == 16)
			mCoinList2->erase(std::remove(mCoinList2->begin(), mCoinList2->end(), aCoin), mCoinList2->end());
		else
			mCoinList1->erase(std::remove(mCoinList1->begin(), mCoinList1->end(), aCoin), mCoinList1->end());
		break;
	}
	case GameObject::TYPE_DEAD_ALIEN:
		mDeadAlienList->erase(std::remove(mDeadAlienList->begin(), mDeadAlienList->end(), (DeadAlien*)theObject), mDeadAlienList->end());
		break;
	case GameObject::TYPE_DEAD_FISH:
		mDeadFishList->erase(std::remove(mDeadFishList->begin(), mDeadFishList->end(), (DeadFish*)theObject), mDeadFishList->end());
		break;
	case GameObject::TYPE_FOOD:
		mFoodList->erase(std::remove(mFoodList->begin(), mFoodList->end(), (Food*)theObject), mFoodList->end());
		break;
	case GameObject::TYPE_LARVA:
		mLarvaList->erase(std::remove(mLarvaList->begin(), mLarvaList->end(), (Larva*)theObject), mLarvaList->end());
		break;
	case GameObject::TYPE_MISSLE:
	{
		Missle* aMis = (Missle*)theObject;
		if (!aMis->IsTargetless())
		{
			mMissleList1->erase(std::remove(mMissleList1->begin(), mMissleList1->end(), (Missle*)theObject), mMissleList1->end());
			unkbool = false;
		}
		else
		{
			mMissleList2->erase(std::remove(mMissleList2->begin(), mMissleList2->end(), (Missle*)theObject), mMissleList2->end());
			unkbool = false;
		}
		break;
	}
	case GameObject::TYPE_SHADOW:
		mShadowList->erase(std::remove(mShadowList->begin(), mShadowList->end(), (Shadow*)theObject), mShadowList->end());
		unkbool = false;
		break;
	case GameObject::TYPE_SHOT:
		mShotList->erase(std::remove(mShotList->begin(), mShotList->end(), (Shot*)theObject), mShotList->end());
		unkbool = false;
		break;
	case GameObject::TYPE_WARP:
		mWarpList->erase(std::remove(mWarpList->begin(), mWarpList->end(), (Warp*)theObject), mWarpList->end());
		unkbool = false;
		break;
	case GameObject::TYPE_SYLVESTER_FISH:
		mSpecialFishList->erase(std::remove(mSpecialFishList->begin(), mSpecialFishList->end(), (SylvesterFish*)theObject), mSpecialFishList->end());
		break;
	case GameObject::TYPE_BALL_FISH:
		mSpecialFishList->erase(std::remove(mSpecialFishList->begin(), mSpecialFishList->end(), (BallFish*)theObject), mSpecialFishList->end());
		break;
	case GameObject::TYPE_BI_FISH:		
		mSpecialFishList->erase(std::remove(mSpecialFishList->begin(), mSpecialFishList->end(), (BiFish*)theObject), mSpecialFishList->end());
		break;
	}

	int anErased = mGameObjectSet.erase(theObject);
	if (anErased || !aFlag)
	{
		if (unkbool)
			m0x2a7 = true;
		return true;
	}
	return false;
}

void Sexy::Board::SpawnDeadFish(int theX, int theY, double theVX, double theVY, double theSpeedMod, int theType, bool facingRight, Shadow* theShadowPtr)
{
	DeadFish* aDeadFish = new DeadFish(theX, theY, theVX, theVY, theSpeedMod, theType, facingRight);
	AddGameObject(aDeadFish);
	mWidgetManager->AddWidget(aDeadFish);
	aDeadFish->mShadowPtr = theShadowPtr;
	if (theShadowPtr)
		theShadowPtr->mObjectPtr = aDeadFish;
	SortGameObjects();
}

void Sexy::Board::SpawnDeadAlien(int theX, int theY, int theAnimationIndex, int theAlienType, bool facingRight)
{
	DeadAlien* anAlien = new DeadAlien(theX, theY, theAlienType, theAnimationIndex, facingRight);
	AddGameObject(anAlien);
	mWidgetManager->AddWidget(anAlien);
	SortGameObjects();
}

void Sexy::Board::SpawnGameObject(GameObject* theObject, bool randomPosition)
{
	if (randomPosition)
		theObject->SetPosition(mApp->mSeed->Next() % 520 + 20, mApp->mSeed->Next() % 265 + 105);
	AddGameObject(theObject);
	mWidgetManager->AddWidget(theObject);
	MakeShadowForGameObject(theObject);
}

void Sexy::Board::SpawnVirtualTankFood(int theFoodType)
{
	GameObject* aFood = nullptr;
	switch (theFoodType)
	{
	case 0:
		aFood = SpawnGuppyAsFood();
		break;
	case 1:
		aFood = SpawnStarAsFood();
		break;
	case 2:
		aFood = SpawnLarvaAsFood();
		break;
	case 3:
		aFood = SpawnOscarAsFood();
		break;
	case 4:
		aFood = SpawnUltraAsFood();
		break;
	case 5:
		aFood = SpawnExoticFood(Food::EXO_FOOD_CHICKEN);
		break;
	case 6:
		aFood = SpawnExoticFood(Food::EXO_FOOD_PIZZA);
		break;
	case 7:
		aFood = SpawnExoticFood(Food::EXO_FOOD_ICE_CREAM);
		break;
	default:
		return;
	}
	if (aFood)
		aFood->mCanBeEatenDelay = 40;
}

GameObject* Sexy::Board::SpawnGuppyAsFood()
{
	PlaySample(SOUND_GROW_ID, 3, 1.0);
	PlaySplashSound();
	return SpawnGuppyBought();
}

GameObject* Sexy::Board::SpawnStarAsFood()
{
	PlaySample(SOUND_GROW_ID, 3, 1.0);
	Coin* aCoin = new Coin(mApp->mSeed->Next() % 520 + 20, Rand() % 50 + 105, 3, nullptr, -1.0);
	aCoin->mMouseVisible = false;
	AddGameObject(aCoin);
	mWidgetManager->AddWidget(aCoin);
	SortGameObjects();
	return aCoin;
}

GameObject* Sexy::Board::SpawnLarvaAsFood()
{
	PlaySample(SOUND_GROW_ID, 3, 1.0);
	Larva* aLarva = new Larva(mApp->mSeed->Next() % 520 + 20,360 - Rand() % 50);
	aLarva->m0x175 = true;
	AddGameObject(aLarva);
	mWidgetManager->AddWidget(aLarva);
	SortGameObjects();
	return aLarva;
}

GameObject* Sexy::Board::SpawnOscarAsFood()
{
	PlaySample(SOUND_GROW_ID, 3, 1.0);
	PlaySplashSound();
	return SpawnOscarBought();
}

GameObject* Sexy::Board::SpawnUltraAsFood()
{
	PlaySample(SOUND_GROW_ID, 3, 1.0);
	PlaySample(SOUND_SPLASHBIG_ID, 3, 1.0);
	return SpawnUltraBought();
}

GameObject* Sexy::Board::SpawnExoticFood(int theType)
{
	PlaySample(SOUND_GROW_ID, 3, 1.0);
	Food* aFood = new Food(mApp->mSeed->Next() % 450 + 50, 115, 0, 0, theType);
	AddGameObject(aFood);
	mWidgetManager->AddWidget(aFood);
	SortGameObjects();
	return aFood;
}

void Sexy::Board::CyraxSpawnMiniAlien(int theX, int theY)
{
	Alien* aMiniAlien = new Alien(theX + 40, theY + 40, Alien::ALIEN_MINI_SYLV);
	AddGameObject(aMiniAlien);
	mWidgetManager->AddWidget(aMiniAlien);
	MakeShadowForGameObject(aMiniAlien);
	SortGameObjects();
}

void Sexy::Board::AddedToManager(WidgetManager* theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mBoardOverlay1);
	theWidgetManager->AddWidget(mBoardOverlay2);
}

void Board::RemovedFromManager(WidgetManager* theWidgetManager)
{
	Widget::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mBoardOverlay1);
	theWidgetManager->RemoveWidget(mBoardOverlay2);
	RemoveWidgetHelper(mMenuButton);
	mMenuButton = 0;
	RemoveWidgetHelper(mMoneyLabel);
	mMoneyLabel = 0;
	RemoveWidgetHelper(mMessageWidget);
	mMessageWidget = 0;
	RemoveWidgetHelper(mMenuButtons[SLOT_GRUBBER]);
	mMenuButtons[SLOT_GRUBBER] = 0;
	RemoveWidgetHelper(mMenuButtons[SLOT_GEKKO]);
	mMenuButtons[SLOT_GEKKO] = 0;
	RemoveWidgetHelper(mMenuButtons[SLOT_ULTRA]);
	mMenuButtons[SLOT_ULTRA] = 0;
	RemoveWidgetHelper(mMenuButtons[SLOT_WEAPON]);
	mMenuButtons[SLOT_WEAPON] = 0;
	RemoveWidgetHelper(mMenuButtons[SLOT_EGG]);
	mMenuButtons[SLOT_EGG] = 0;
	RemoveWidgetHelper(mBackButton);
	mBackButton = 0;
	ResetLevel(theWidgetManager);
}

void Board::Update()
{
	Widget::Update();
	mMoney = 99999; // TODO REMOVE THESE PARTS
	if(!mSlotUnlocked[SLOT_EGG] && mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)
		MakeAndUnlockMenuButton(SLOT_EGG, true);


	int aLastMX = mApp->mWidgetManager->mLastMouseX;
	int aLastMY = mApp->mWidgetManager->mLastMouseY;

	if (mDropFoodDelay != 0)
		mDropFoodDelay--;
	if (mPause || (gUnkBool02 = true, mFishSongMgr->mSongList.size() < 1))
		gUnkBool02 = false;
	if (mPause)
		return;

	mFishSongMgr->Update();

	if (((mApp->mCurrentProfile->mCheatCodeFlags >> 3) & 1))
		mStarField->Update();
	mBubbleMgr->Update();
	bool isScrSvr = mApp->IsScreenSaver();
	if (isScrSvr && mUpdateCnt % 10800 == 0 && mApp->mScreenSaverRotateBackdrops) // 72 line ghidra
	{
		std::vector<int> aCandidateBGS;

		for (int i = 0; i < 6; i++)
			if (mApp->mCurrentProfile->mUnlockedBackgrounds[i] && (i + 1 != mCurrentBackgroundId))
				aCandidateBGS.push_back(i);

		if (!aCandidateBGS.empty())
		{
			int aRandIdx = Rand() % aCandidateBGS.size();
			if (m0x3ec == -1)
				m0x3ec = mCurrentBackgroundId;
			ChangeBackground(aCandidateBGS[aRandIdx] + 1);
			UpdateNikoPosition();
		}
	}
	
	mApp->mPlaytimeCounter++;

	if (mApp->mGameMode == WinFishApp::GAMEMODE_CHALLENGE && m0x2e0 < m0x2dc && mGameUpdateCnt > 10800) // 105
	{
		mApp->PlaySample(SOUND_BUTTONCLICK);
		for (int i = 0; i < SLOT_END; i++) MakeAndUnlockMenuButton(i, true);
	}

	if (m0x2a7) // 113
	{
		m0x2a7 = false;
		if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
			WidgetSetupVT();
		Unk14(m0x2a6);
	}

	if (m0x2b4 > 0)
		m0x2b4--;

	if (mBubbulatorShown) //123
	{
		int aTimer = mBubbulatorTimer % 500;
		int aTimer2 = 0;
		if (aTimer < 60)
			aTimer2 = aTimer % 20;
		m0x44c = aTimer2;

		if (aTimer2 == 13)
		{
			if (aTimer < 20)
				PlaySample(SOUND_BUBBLES_ID, 3, 1.0);
			int aX, aY;
			GetBubbleSpawnCoords(aX, aY);
			int aNumOfBubbles = Rand() % 5 + 5;
			for (int i = 0; i < aNumOfBubbles; i++)
				SpawnBubble(aX + Rand() % 41 + 5, (aY - Rand() % 10) - 15);
		}
		mBubbulatorTimer++;
	}

	if (mApp->mRelaxMode) //149
	{
		if (gFoodType != 2)
		{
			if (mFishList->size() > 4)
				gFoodType = 2;
		}
		if (gFoodLimit < 9)
		{
			if (gFoodLimit < mFishList->size())
			{
				if (mFishList->empty())
					gFoodLimit = 0;
				else
					gFoodLimit = mFishList->size();
			}
		}
	}

	if (mApp->mGameMode == WinFishApp::GAMEMODE_TIME_TRIAL) //176 for highscores
	{
		m0x3b0 = (Unk01() - m0x3b8) / 1000;
		if (m0x3bc - m0x3b0 < 0)
		{
			mShouldSave = false;
			Unk04();
			mApp->mHighScoreMgr->RecordTimeTrialHighScore(mTank, mApp->mCurrentProfile, mMoney);
			mApp->DoTimesUpDialog();
			return;
		}
	}

	if (mApp->mGameMode == WinFishApp::GAMEMODE_CHALLENGE) //190
		m0x3b0 = (Unk01()-m0x3b8)/1000;

	if (mIsBonusRound)
	{
		if (!mBonusRoundStarted)
		{
			if (mGameUpdateCnt - m0x2a8 > 159)
				Unk05();
		}
		else // 200
		{
			m0x3b0 = (Unk01() - m0x3b8) / 1000;
			if (m0x3bc - m0x3b0 < 0)
			{
				if (mCoinList1->empty() && m0x3f8++ > 100)
				{
					mApp->mCurrentProfile->NextLevel();
					if (mLevel < 6)
						mApp->mHighScoreMgr->RecordAdventureHighScore(mTank, mLevel, mApp->mCurrentProfile, m0x3fc);
					mApp->SwitchToBonusScreen();
					return;
				}
			}
			else if(mGameUpdateCnt % 10 == 0)
			{
				BonusRoundDropShell();
			}
		}
	}

	if (mApp->mGameMode == WinFishApp::GAMEMODE_CHALLENGE && (mSlotUnlocked[0] || mSlotUnlocked[1])) // 223
	{
		int aUnk01 = 0;
		if (mTank == 1)
			aUnk01 = 150;
		else if(mTank == 2)
			aUnk01 = 150;
		else if(mTank == 3)
			aUnk01 = 175;
		else if(mTank == 4)
			aUnk01 = 200;

		if (m0x454 < 1 && m0x43c < 6)
		{
			if (mGameUpdateCnt % aUnk01 == 0)
			{
				for (int i = 0; i < SLOT_END; i++)
				{
					MenuButtonWidget* aBtn = GetMenuButtonById(i);
					if (aBtn)
					{
						mSlotPrices[i] = mSlotPrices[i] + m0x374[i];
						if (mSlotPrices[i] > 99999)
							mSlotPrices[i] = 99999;
						aBtn->SetSlotPrice(mSlotPrices[i]);
					}
				}
				for (int i = 0; i < 7; i++)
					if (mMenuButtons[i] && mMenuButtons[i]->mMouseVisible)
						mMenuButtons[i]->mPriceTextColor = Color(0xfa, 0x9b, 0x6e);
			}
			else if (aUnk01 - 10 < mGameUpdateCnt % aUnk01)
			{
				for (int i = 0; i < 7; i++)
					if (mMenuButtons[i] && mMenuButtons[i]->mMouseVisible)
						mMenuButtons[i]->mPriceTextColor = Color(0xfa, 0x6e, 0x37);
			}
			else
			{
				for (int i = 0; i < 7; i++)
					if (mMenuButtons[i] && mMenuButtons[i]->mMouseVisible)
						mMenuButtons[i]->mPriceTextColor = Color(0xfa, 0x9b, 0x6e);
			}
		}
		else
		{
			int aVal = m0x454 % 14;
			if (aVal > 6)
				aVal = 14 - aVal;

			for (int i = 0; i < 7; i++)
				if (mMenuButtons[i] && mMenuButtons[i]->mMouseVisible)
					mMenuButtons[i]->mPriceTextColor = Color(aVal*14+110, 250, aVal * 14 + 110);
		}
	}

	if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK && mApp->mCurrentProfile->mShells == 0 && mGameUpdateCnt == 150) // 286
		ShowText("Here you\'ll be able to build your own custom fish tank!", false, 44);

	if (m0x4ec)
	{
		bool isLeftBtnDown = mWidgetManager->IsLeftButtonDown();
		int aVal = Unk01();
		int aLastMouseX = mWidgetManager->mLastMouseX;
		int aLastMouseY = mWidgetManager->mLastMouseY;
		if (!isLeftBtnDown)
			m0x4ec = false;
		else if ((mGameUpdateCnt % (16 - gFoodLimit) == 0) && (m0x3c0 + 200 < aVal) && 
			(aLastMouseX >= 31 && aLastMouseX <= 586) && (aLastMouseY >= 61 && aLastMouseY <= 399) && Buy(m0x4ac, false))
		{ // 301
			DropFood(aLastMouseX - 10, aLastMouseY - 10, 0, false, 20, -1);
		}
	}

	if (m0x4ed) // 305
	{
		bool isLeftBtnDown = mWidgetManager->IsLeftButtonDown();
		int aVal = Unk01();
		if (!isLeftBtnDown)
			m0x4ed = false;
		else if(m0x3e4 > 11 && mGameUpdateCnt%(11 - m0x3e4 / 2) == 0 && m0x3c4 + 100 < aVal && mWidgetManager->mLastMouseY > 40)
		{ // 311
			for (int i = 0; i < mBilaterusList->size(); i++)
			{
				Bilaterus* aBil = mBilaterusList->at(i);
				if (aBil->Shoot(aLastMX, aLastMY))
					break;
			}

			for (int i = 0; i < mMissleList1->size(); i++)
			{
				Missle* aMissle = mMissleList1->at(i);
				if (aMissle->Shot(aLastMX, aLastMY))
					break;
			}

			for (int i = 0; i < mAlienList->size(); i++)
			{
				Alien* anAlien = mAlienList->at(i);
				if (anAlien->Shot(aLastMX, aLastMY))
					break;
			}

			SpawnLaserShot(aLastMX - 40, aLastMY - 40);
			PlayZapSound();
		}
	}

	if (m0x450 > 0)
		m0x450--;
	if (m0x454 > 0)
		m0x454--;
	if (m0x458 > 0)
		m0x458--;

	mTankLightingSpeeds[0] -= 1.6;
	mTankLightingSpeeds[1] += 1.8;
	mTankLightingSpeeds[2] += 2.2;

	for (int i = 0; i < 3; i++)
	{
		if (mTankLightingSpeeds[i] > 640)
			mTankLightingSpeeds[i] = 0;
		else if (mTankLightingSpeeds[i] < 0)
			mTankLightingSpeeds[i] = 640;
	}

	if (mApp->mGameMode == WinFishApp::GAMEMODE_SANDBOX && m0x2e0 < m0x2dc && mTank != 5) // 424
	{
		for (int i = 0; i < SLOT_END; i++)
			MakeAndUnlockMenuButton(i, true);
		UpdateSlotPrice(SLOT_EGG, 0);
	}
	SortGameObjects();

	if (!mPause)
	{
		mGameUpdateCnt++;
		if (mGameUpdateCnt < 0)
			mGameUpdateCnt = 0x40000000;

		if (mGameUpdateCnt % 2048 == 0)
			Unk13();

		if (!mBubbulatorShown && mApp->mSeed->Next() % 1000 == 0)
		{
			int aNumOfBubbles = mApp->mSeed->Next() % 3 + 2;
			for (int i = 0; i < aNumOfBubbles; i++)
				SpawnRandomBubble();
			PlaySample(SOUND_BUBBLES_ID, 3, 1.0);
		}
	}

	if (m0x2c4 != 0) // 458
		m0x2c4--;


	if (mPause || (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK && !mAlienAttractorShown))
	{
		if (mLevel == 1 && mTank == 1 && m0x4e6)
		{
			if (mSlotPrices[SLOT_EGG] < mMoney)
				m0x4e8++;
			if (m0x4e8 > 1500)
				m0x4b0[40] = true;
		}
		if (mTank == 5 && mCyraxPtr == 0 && m0x4f4 && !mIsBonusRound && !mSlotUnlocked[SLOT_EGG])
			MakeAndUnlockMenuButton(SLOT_EGG, true);
		MarkDirty();
		return;
	}

	if (mAlienList->empty() && mBilaterusList->empty() && mMissleList1->empty())
	{ // 469
		if (mAlienExpect != 0 && mCyraxPtr == nullptr && !m0x4f4)
		{
			mAlienTimer--;
			if (mAlienAttractorShown && mAlienTimer == 33)
				PlaySample(SOUND_UNLEASH_ID, 3, 1.0);
		}
		if (mAlienTimer == 30 && mPetsInTank[GameObject::PET_NOSTRADAMUS] != 0 && mTank != 5)
			NostradamusSneezeEffect();

		if (mApp->mGameMode == WinFishApp::GAMEMODE_CHALLENGE && mAlienTimer == 2999)
		{ // 480
			int aDifIncreaseProb = 1;
			if (mTank == 1)
				aDifIncreaseProb = 4;
			else if (mTank == 2 || mTank == 3)
				aDifIncreaseProb = 5;
			else if (mTank == 4)
				aDifIncreaseProb = 6;

			if (m0x45c % aDifIncreaseProb == 0 && m0x45c != 0)
			{
				mMessageWidget->m0xd4 = true;
				mMessageWidget->m0xd0 = 274;
				mMessageWidget->mColor1 = Color(255, 50, 50, 255);
				mMessageWidget->mColor2 = Color(100, 20, 20, 255);
			}
		}
		else
		{ // 516
			if (mAlienTimer == 276)
			{
				if (!mApp->mRelaxMode && mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)
				{
					if (!m0x4b0[3] && IsTankAndLevelNB(1, 2))
					{
						mApp->DoDialogUnkF(WinFishApp::DIALOG_INFO, true, "DANGER!", "A vicious alien is about to enter your tank! Defeat it with your laser weapon by clicking on it!", "Click to Continue", Dialog::BUTTONS_FOOTER);
						m0x4b0[3] = true;
					}
					else if (!m0x4b0[4] && IsTankAndLevelNB(1, 2))
					{
						mApp->DoDialogUnkF(WinFishApp::DIALOG_INFO, true, "BATTLE TIP", "Shoot the alien\'s head to push it downwards! Shoot its tail to deflect it upwards!", "Click to Continue", Dialog::BUTTONS_FOOTER);
						m0x4b0[4] = true;
					}
					else if (!m0x4b0[17] && IsTankAndLevelNB(2, 3))
					{
						mApp->DoDialogUnkF(WinFishApp::DIALOG_INFO, true, "WARNING!", "A new breed of alien is fast approaching!  Lasers can\'t hurt this baddie, so find another way to defeat it!", "Click to Continue", Dialog::BUTTONS_FOOTER);
						m0x4b0[17] = true;
					}
				}
			}
			else if (mAlienTimer == 275)
			{
				if(!m0x4b0[46] && mLevel == 4 && mTank == 2 && (mAlienExpect == 9 || mAlienExpect == 10 
					|| mAlienExpect == 11 || mAlienExpect == 12))
					mMessageWidget->mMessage = "FOURTEEN ALIEN SIGNATURES DETECTED";
				else
				{
					if (mAlienExpect == Alien::ALIEN_GUS)
						mMessageWidget->mMessage = "ALIEN SIGNATURE TYPE-G DETECTED";
					else if (mAlienExpect == Alien::ALIEN_DESTRUCTOR)
						mMessageWidget->mMessage = "ALIEN SIGNATURE TYPE-D DETECTED";
					else if (mAlienExpect == Alien::ALIEN_ULYSEES)
						mMessageWidget->mMessage = "ALIEN SIGNATURE TYPE-U DETECTED";
					else if (mAlienExpect == Alien::ALIEN_PSYCHOSQUID)
						mMessageWidget->mMessage = "ALIEN SIGNATURE TYPE-P DETECTED";
					else if (mAlienExpect == Alien::ALIEN_BILATERUS)
						mMessageWidget->mMessage = "ALIEN SIGNATURE TYPE-II DETECTED";
					else if (mAlienExpect == 9 || mAlienExpect == 10 || mAlienExpect == 11 || mAlienExpect == 12)
						mMessageWidget->mMessage = "MULTIPLE ALIEN SIGNATURES DETECTED";
					else if (mAlienExpect == Alien::ALIEN_CYRAX)
						mMessageWidget->mMessage = StrFormat("%s OF DOOM APPROACHING", GetCyraxEndGameString(mApp->mCurrentProfile->mCyraxNum + 1).c_str());
					else
						mMessageWidget->mMessage = "ENEMY APPROACHING";
				}
				mMessageWidget->m0xd4 = false;
				mMessageWidget->m0xd0 = 274;
				mMessageWidget->mColor1 = Color(255, 50, 50, 255);
				mMessageWidget->mColor2 = Color(100, 20, 20, 255);
				mApp->SomeMusicFunc(true);
				PlaySample(SOUND_AWOOGA_ID, 3, 1.0);
				if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
					DetermineAlienSpawnCoordsVT();
				else
				{
					mCrosshair1X = mApp->mSeed->Next() % 450 + 20;
					mCrosshair1Y = mApp->mSeed->Next() % 195 + 105;
					mCrosshair2X = mApp->mSeed->Next() % 450 + 20;
					mCrosshair2Y = mApp->mSeed->Next() % 195 + 105;
				}
			}
			else if (mAlienTimer < 1)
			{
				m0x2c4 = 35;
				m0x4ed = false;
				m0x4ec = false;
				SpawnAlien(mAlienExpect, mCrosshair1X, mCrosshair1Y, true);
				if (mApp->mGameMode == WinFishApp::GAMEMODE_CHALLENGE)
				{
					int aDifIncreaseProb = 1;
					if (mTank == 1)
						aDifIncreaseProb = 4;
					else if (mTank == 2 || mTank == 3)
						aDifIncreaseProb = 5;
					else if (mTank == 4)
						aDifIncreaseProb = 6;

					int aVal = m0x45c / aDifIncreaseProb;
					if (mAlienExpect == 12)
						aVal--;

					for (int i = 0; i < aVal; i++)
					{
						mCrosshair1X = mApp->mSeed->Next() % 450 + 20;
						mCrosshair1Y = mApp->mSeed->Next() % 195 + 105;
						mCrosshair2X = mApp->mSeed->Next() % 450 + 20;
						mCrosshair2Y = mApp->mSeed->Next() % 195 + 105;
						SpawnAlien(mAlienExpect, mCrosshair1X, mCrosshair1Y, false);
					}
				}

				if (!mApp->mRelaxMode) // 754
				{
					if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
						SetAlienExpectVT();
					else
					{
						if (mTank == 1 && mLevel == 5)
							mAlienExpect = ((mApp->mSeed->Next() % 2 != 0) ? Alien::ALIEN_BALROG : 9);
						else if (mTank == 2 && mLevel == 5)
						{
							if(mAlienExpect == 9)
								mAlienExpect = ((mApp->mSeed->Next() % 2 != 0) ? Alien::ALIEN_GUS : Alien::ALIEN_DESTRUCTOR);
							if (mApp->mSeed->Next() % 10 == 0)
								mAlienExpect = (mAlienExpect != Alien::ALIEN_DESTRUCTOR) + Alien::ALIEN_GUS;
							else if (mApp->mSeed->Next() % 20 == 0)
								mAlienExpect = 9;
						}
						else if (mTank == 3)
						{
							if (mLevel == 2)
							{
								if(mApp->mSeed->Next() % 10 == 0)
									mAlienExpect = (mAlienExpect != Alien::ALIEN_DESTRUCTOR) + Alien::ALIEN_GUS;
							}
							else if(mLevel == 5)
							{
								mAlienExpect = (mApp->mSeed->Next() % 2 != 0 ? Alien::ALIEN_PSYCHOSQUID : Alien::ALIEN_ULYSEES);
							}
						}
						else if (mTank == 4)
						{
							if (mLevel == 3)
							{
								mAlienExpect = (mApp->mSeed->Next() % 2 != 0 ? Alien::ALIEN_GUS : 10);
							}
							else if (mLevel == 4)
							{
								mAlienExpect = (mApp->mSeed->Next() % 2 != 0 ? Alien::ALIEN_BILATERUS : 11);
							}
							else if (mLevel == 5)
							{
								int anAlienChance = mApp->mSeed->Next() % 5;
								if (anAlienChance < 2)
									mAlienExpect = 10;
								else
									mAlienExpect = (anAlienChance > 3 ? Alien::ALIEN_BILATERUS : 11);
								if (mApp->mGameMode == WinFishApp::GAMEMODE_CHALLENGE && m0x45c > 4 && mAlienExpect == Alien::ALIEN_BILATERUS)
									mAlienExpect = 12;
							}
						}
					}
				} // 818
				else
				{
					RelaxModeConfig();
				}
				m0x45c++;
				mAlienTimer = 3000;
			}
			else if (mAlienTimer < 275)
			{
				if (!m0x4b0[46] && mLevel == 4 && mTank == 4 && (mAlienExpect == 9 || mAlienExpect == 10 ||
					mAlienExpect == 11 || mAlienExpect == 12) && mAlienTimer == 100)
				{
					mMessageWidget->mMessage = "JUST KIDDING. ONLY TWO!";
					m0x4b0[46] = true;
				}
				if (mAlienTimer == 1)
					mApp->PlayMusic(1, 1);
			}
		}
	}
	
	if (!mIsBonusRound && mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)
	{
		if (!HasAnyFish() && mTank != 5)
		{
			PauseGame(true);
			if (!IsFirstLevel())
			{
				mShouldSave = false;
				Unk04();
				mApp->DoDialogUnkF(WinFishApp::DIALOG_GAME_OVER, true, "GAME OVER", "Oops!  All of your fish have died!", "Click to Continue", Dialog::BUTTONS_FOOTER);
			}
			else
			{
				mApp->DoDialogUnkF(WinFishApp::DIALOG_FIRST_LVL_GAME_OVER, true, "YOUR LAST FISH HAS DIED!", "Normally this would end your game, but this time we\'ll give you another fish to keep playing! Make sure you keep it well fed!", "Click to Continue", Dialog::BUTTONS_FOOTER);
			}
		}
		if(mTank == 5)
		{
			if (mFishTypePetList->empty() && mOtherTypePetList->empty())
			{
				PauseGame(true);
				mShouldSave = false;
				if (mCyraxPtr != nullptr && mCyraxPtr->mMaxHealth - mCyraxPtr->mHealth > 1000.0)
					mApp->mCurrentProfile->mFinishedGameCount++;
				mApp->DoDialogUnkF(WinFishApp::DIALOG_GAME_OVER, true, "GAME OVER", "Oops!  All of your pets have died!", "Click to Continue", Dialog::BUTTONS_FOOTER);

			}
		}
	}

	if (mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)
	{
		if (mApp->mGameMode == WinFishApp::GAMEMODE_ADVENTURE && IsTankAndLevelNB(1, 1) && mGameUpdateCnt == 150)
			ShowText("Here are your first fish! Take good care of them!", false, 1);
		else if(mApp->mGameMode == WinFishApp::GAMEMODE_ADVENTURE && IsTankAndLevelNB(1,2) && mGameUpdateCnt == 150)
			ShowText("On this level you will meet your 1st alien opponent!", false, 20);
		else if(mApp->mGameMode == WinFishApp::GAMEMODE_TIME_TRIAL && mGameUpdateCnt == 150)
			ShowText("How much money can you earn before time runs out?", false, 53);
		else if(mApp->mGameMode == WinFishApp::GAMEMODE_CHALLENGE && mGameUpdateCnt == 150)
			ShowText("Can you fend off the increasingly hungry aliens?", false, 51);
		else if(mApp->mGameMode == WinFishApp::GAMEMODE_SANDBOX && mGameUpdateCnt == 150)
			ShowText("Make sure caps-lock is off and GO TO TOWN!!", false, 48);
	}

	if (mLevel == 1 && mTank == 1 && m0x4e6)
	{
		if (mSlotPrices[SLOT_EGG] < mMoney)
			m0x4e8++;
		if (m0x4e8 > 1500)
			m0x4b0[40] = true;
	}
	
	if (mTank == 5 && mCyraxPtr == nullptr && m0x4f4 && !mIsBonusRound && !mSlotUnlocked[SLOT_EGG])
		MakeAndUnlockMenuButton(SLOT_EGG, true);

	MarkDirty();
}

void Board::Draw(Graphics* g)
{
	DrawTankBackground(g);

	if (mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)
		g->DrawImage(IMAGE_MENUBAR, 0, 0);
	else if(!mApp->IsScreenSaver())
		g->DrawImage(IMAGE_TROPHYBAR, 0, 0);

	if (m0x450 > 0 && m0x450 % 20 < 10)
		g->DrawImage(IMAGE_MONEYFLASH, 545, 39);

	g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
	g->SetColor(Color(0x6e, 0xfa, 0x6e));
	if (m0x4b0[2] && mLevel == 1 && mTank == 1 && !mApp->mCurrentProfile->mFinishedGame) // 52
	{
		g->DrawImage(IMAGE_HELPARROW, 40, 70);
		g->DrawString("Click here to", 65, 95);
		g->DrawString("buy fish!", 83, 107);
	} // 65
	else
	{
		if (!m0x4b0[40] || mLevel != 1 || mTank != 1 || mApp->mCurrentProfile->mFinishedGame) //66
		{
			if (m0x4b0[15] && mLevel == 2 && mTank == 1 && !mApp->mCurrentProfile->mFinishedGame)
			{
				g->DrawImage(IMAGE_HELPARROW, 110, 70);
				g->DrawString("Click here to", 135, 95);
				g->DrawString("upgrade!", 152, 107);
			}
			else if (mTank == 5 && m0x4f4 && !mIsBonusRound)
			{
				g->DrawImage(IMAGE_HELPARROW, 460, 70);
				g->DrawString("Click here to", 485, 95);
				g->DrawString("end the level!", 480, 107);
			}
			else if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK && !mHasVirtualTankFish && !mApp->IsScreenSaver() && !mApp->GetDialog(39))
			{
				g->DrawImage(IMAGE_HELPARROW, 100, 70);
				g->DrawString("Click here to", 125, 95);
				g->DrawString("buy fish!", 140, 107);
			}
		}
		else // 18
		{
			g->DrawImage(IMAGE_HELPARROW, 460, 70);
			g->DrawString("Click here to", 485, 95);
			g->DrawString("buy egg piece!", 480, 107);
		}
	} // 132

	if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
		return;

	g->SetFont(FONT_CONTINUUMBOLD12);
	Color aTextColor = Color::White;
	if (mCurrentBackgroundId == 2)
		aTextColor = Color(0xdc, 0x96, 0x96);
	else if (mCurrentBackgroundId == 4)
		aTextColor = Color(0x3c, 0xb4, 0x50);
	else if (mCurrentBackgroundId == 3)
		aTextColor = Color(0x7d, 200, 0xd7);
	else
		aTextColor = Color(0xa5, 0x8c, 0x50);
	g->SetColor(aTextColor);

	if (!mApp->mRelaxMode)
	{
		if (mApp->mGameMode == WinFishApp::GAMEMODE_CHALLENGE)
			g->DrawString("Challenge", 15, 470);
		else if(mApp->mGameMode == WinFishApp::GAMEMODE_TIME_TRIAL)
			g->DrawString("Time Trial", 15, 470);
		else if(mApp->mGameMode == WinFishApp::GAMEMODE_SANDBOX)
			g->DrawString("Sandbox", 15, 470);
		else if(!mIsBonusRound || mApp->mCurrentProfile->mFinishedGame && mTank != 5)
			g->DrawString(StrFormat("Tank %d-%d", mTank, mLevel), 15, 470);
		else if(mTank != 5)
			g->DrawString("Bonus Round", 15, 470);
	}
	else
	{
		g->DrawString("Relax", 15, 470);
	}

	if (mApp->mGameMode == WinFishApp::GAMEMODE_TIME_TRIAL)
	{
		int aTime = m0x3bc - m0x3b0;
		int aMins = aTime / 60;
		int aSecs = aTime % 60;
		if (aMins < 0) aMins = 0; if (aSecs < 0) aSecs = 0;
		if (mGameUpdateCnt < 347)
			g->DrawString(StrFormat("%d:%02d", aMins, aSecs), g->GetFont()->StringWidth("Time Remaining: ") + 465, 470);
		else
			g->DrawString(StrFormat("Time Remaining: %d:%02d", aMins, aSecs), 465, 470);

		if (mIsBonusRound)
			DrawBonusRound(g);
	}
	else
	{
		if (((mApp->mCurrentProfile->mCheatCodeFlags >> 7) & 1) != 0)
		{
			if (!mIsBonusRound)
			{
				if (mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)
				{
					g->DrawString(StrFormat("Time: %d:%02d", (Unk01() - m0x3b8) / 60000, (Unk01() - m0x3b8) / 1000 % 60), 540, 470);
				}
			}
		}
		else if (mIsBonusRound)
			DrawBonusRound(g);
	} // 276

	if (mAlienTimer < 1 || mAlienTimer > 225 || mPetsInTank[13] == 0 || mTank == 5 || mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
		return;
	g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
	mCrosshair1Y = 280;
	g->DrawImageCel(IMAGE_CROSSHAIR, mCrosshair1X + 40, mCrosshair1Y + 40, (mGameUpdateCnt / 4) % 5);
	if (mAlienExpect == 9 || mAlienExpect == 10 || mAlienExpect == 11 || mAlienExpect == 12)
	{
		if (mAlienExpect == 11)mCrosshair2Y = 280;
		g->DrawImageCel(IMAGE_CROSSHAIR, mCrosshair2X + 40, mCrosshair2Y + 40, (mGameUpdateCnt / 4 + 2) % 5);
	}
	g->SetDrawMode(Graphics::DRAWMODE_NORMAL);

	if (mAlienTimer == 225)
		PlaySample(SOUND_SONAR_ID, 3, 1.0);
}

void Board::DrawOverlay(Graphics* g, int thePriority)
{
	if (thePriority == 0)
		DrawOverlay0(g);
	else if (thePriority == 1)
		DrawOverlay1(g);
}
void Sexy::Board::KeyChar(SexyChar theChar)
{
	// TODO REMOVE THESE IN FUTURE
	if (theChar == 'W')
		Unk07(2500);
	if (theChar == 'U')
		SpawnUltraBought();
	if (theChar == 'G')
		SpawnGekkoBought();
	if (theChar == 'Q')
	{
		mApp->mCurrentProfile->m0x98 = Rand();
		memset(mApp->mCurrentProfile->mStoreScreenBought, 0, 8);
	}

	if (mApp->mGameMode != WinFishApp::GAMEMODE_SANDBOX) // TODO == not !=
	{
		bool gusInTank = false;
		if (!mAlienList->empty())
			if (mAlienList->at(0)->mAlienType == Alien::ALIEN_GUS)
				gusInTank = true;

		if (theChar == '1')
			SpawnGuppyBought();
		else if (theChar == '2')
			SpawnOscarBought();
		else if (theChar == '3')
			SpawnStarGuppyBought();
		else if (theChar == '4')
			SpawnPentaBought();
		else if (theChar == '5')
			SpawnGrubberBought();
		else if (theChar == '6')
			SpawnGekkoBought();
		else if (theChar == '7')
			SpawnBreederBought();
		else if (theChar == '8')
			SpawnUltraBought();
		else if (theChar == 'z')
		{
			if (!gusInTank)
			{
				mAlienExpect = Alien::ALIEN_STRONG_SYLV;
				SpawnAlien(Alien::ALIEN_STRONG_SYLV, true);
			}
		}
		else if (theChar == 'x')
		{
			if (!gusInTank)
			{
				mAlienExpect = Alien::ALIEN_BALROG;
				SpawnAlien(Alien::ALIEN_BALROG, true);
			}
		}
		else if (theChar == 'c')
		{
			mAlienExpect = Alien::ALIEN_GUS;
			SpawnAlien(Alien::ALIEN_GUS, true);
		}
		else if (theChar == 'v')
		{
			if (!gusInTank)
			{
				mAlienExpect = Alien::ALIEN_DESTRUCTOR;
				SpawnAlien(Alien::ALIEN_DESTRUCTOR, true);
			}
		}
		else if (theChar == 'b')
		{
			if (!gusInTank)
			{
				mAlienExpect = Alien::ALIEN_ULYSEES;
				SpawnAlien(Alien::ALIEN_ULYSEES, true);
			}
		}
		else if (theChar == 'n')
		{
			if (!gusInTank)
			{
				mAlienExpect = Alien::ALIEN_PSYCHOSQUID;
				SpawnAlien(Alien::ALIEN_PSYCHOSQUID, true);
			}
		}
		else if (theChar == 'm')
		{
			if (!gusInTank)
			{
				mAlienExpect = Alien::ALIEN_BILATERUS;
				SpawnAlien(Alien::ALIEN_BILATERUS, true);
			}
		}
		else if (theChar == 'q')
			SpawnPet(GameObject::PET_STINKY, -1, -1, false, false);
		else if (theChar == 'w')
			SpawnPet(GameObject::PET_NIKO, -1, -1, false, false);
		else if (theChar == 'e')
			SpawnPet(GameObject::PET_ITCHY, -1, -1, false, false);
		else if (theChar == 'r')
			SpawnPet(GameObject::PET_PREGO, -1, -1, false, false);
		else if (theChar == 't')
			SpawnPet(GameObject::PET_ZORF, -1, -1, false, false);
		else if (theChar == 'y')
			SpawnPet(GameObject::PET_CLYDE, -1, -1, false, false);
		else if (theChar == 'u')
			SpawnPet(GameObject::PET_VERT, -1, -1, false, false);
		else if (theChar == 'i')
			SpawnPet(GameObject::PET_RUFUS, -1, -1, false, false);
		else if (theChar == 'o')
			SpawnPet(GameObject::PET_MERYL, -1, -1, false, false);
		else if (theChar == 'p')
			SpawnPet(GameObject::PET_WADSWORTH, -1, -1, false, false);
		else if (theChar == 'a')
			SpawnPet(GameObject::PET_SEYMOUR, -1, -1, false, false);
		else if (theChar == 's')
			SpawnPet(GameObject::PET_SHRAPNEL, -1, -1, false, false);
		else if (theChar == 'd')
			SpawnPet(GameObject::PET_GUMBO, -1, -1, false, false);
		else if (theChar == 'f')
			SpawnPet(GameObject::PET_BLIP, -1, -1, false, false);
		else if (theChar == 'g')
			SpawnPet(GameObject::PET_RHUBARB, -1, -1, false, false);
		else if (theChar == 'h')
			SpawnPet(GameObject::PET_NIMBUS, -1, -1, false, false);
		else if (theChar == 'j')
			SpawnPet(GameObject::PET_AMP, -1, -1, false, false);
		else if (theChar == 'k')
			SpawnPet(GameObject::PET_GASH, -1, -1, false, false);
		else if (theChar == 'l')
			SpawnPet(GameObject::PET_ANGIE, -1, -1, false, false);
		else if (theChar == ';')
			SpawnPet(GameObject::PET_PRESTO, -1, -1, false, false);
		else if (theChar == 'A')
			SpawnPet(GameObject::PET_BRINKLEY, -1, -1, false, false);
		else if (theChar == 'S')
			SpawnPet(GameObject::PET_NOSTRADAMUS, -1, -1, false, false);
		else if (theChar == 'D')
			SpawnPet(GameObject::PET_WALTER, -1, -1, false, false);
		else if (theChar == 'F')
			SpawnPet(GameObject::PET_STANLEY, -1, -1, false, false);
		else if (theChar == '+' || theChar == '-')
		{
			ChangeBackground(theChar == '+' ? mCurrentBackgroundId + 1 : mCurrentBackgroundId - 1);
			UpdateNikoPosition();
		}
	}
	else
	{
		for (int i = 0; i < sizeof(mCheatCodes) / 4; i++)
			if (mCheatCodes[i]->CheckCodeActivated(theChar))
				if (DoCheatCode(i))
					return;
		if (theChar == 'b' || theChar == 'B')
		{
			if (!mBubbulatorShown)
			{
				int aNumOfBubbs = mApp->mSeed->Next() % 3 + 2;
				for (int i = 0; i < aNumOfBubbs; i++)
					SpawnRandomBubble();
			}
			else
				mBubbulatorTimer = 40;
		}
	}
	if (theChar == ' ')
		mApp->DoLostFocusDialog();
}

void Board::KeyDown(KeyCode theKey)
{
	if (!mApp->mDebugKeysEnabled)
		for (int i = 0; i < (sizeof(mCheatCodes) / 4); i++)
			if (mCheatCodes[i]->CheckCodeActivated(theKey))
				DoCheatCode(i);
}

void Board::MouseDown(int x, int y, int theClickCount)
{
	Widget::MouseDown(x, y, theClickCount);

	if (mIsBonusRound)
	{
		if (theClickCount > -1)
		{
			if (!mBonusRoundStarted)
				Unk05();
			return;
		}
	}

	if (theClickCount < 0)
	{
		if (y > 40)
			CheckMouseDown(x, y);
		return;
	}
	// 39
	int aX = m0x3d0 - x;
	int aY = m0x3d4 - y;
	if (mDropFoodDelay > 0 && (aX * aX + aY * aY) > 2500)
		mDropFoodDelay = 0;

	bool playZapSound = false;
	if (!mSmallAlienList->empty() && mCyraxPtr && y > 40)
	{
		for (int i = 0; i < mSmallAlienList->size(); ++i)
		{
			if (mSmallAlienList->at(i)->Shot(x, y)) break;
		}
		playZapSound = true;
		SpawnLaserShot(x - 40, y - 40);
	}

	if (mCyraxPtr)
	{
		mCyraxPtr->Shot(x, y);
		playZapSound = true;
		SpawnLaserShot(x - 40, y - 40);
	}

	bool aliensExist = !mAlienList->empty();
	bool bilaterusExists = !mBilaterusList->empty();
	bool missilesExist = !mMissleList1->empty();

	if (aliensExist || bilaterusExists || (mCyraxPtr && !missilesExist)) // 128
	{
		if (bilaterusExists && y > 40)
		{
			for (int i = 0; i < mBilaterusList->size(); ++i)
			{
				Bilaterus* aBil = mBilaterusList->at(i);
				if (aBil->Shoot(x, y)) break;
			}
			playZapSound = true;
			SpawnLaserShot(x - 40, y - 40);
			m0x4ed = true;
			m0x3c4 = Unk01();
		}

		if (aliensExist)
		{
			if (mAlienList->at(0)->mAlienType == Alien::ALIEN_GUS && mCyraxPtr == nullptr)
			{
				if (x >= 31 && x <= 586 && y >= 61 && y <= 379)
				{
					int aEatDelay = 0;
					if (mAlienList->at(0)->FoodDroppedAtAlien(x, y))
						aEatDelay = 20;
					DropFood(x - 10, y - 10, 0, false, aEatDelay, -1);
					m0x4ec = true;
					m0x3c0 = Unk01();
				}
			}
			else
			{
				if (y > 40)
				{
					for (int i = 0; i < mAlienList->size(); ++i)
					{
						Alien* anAlien = mAlienList->at(i);
						if (anAlien->Shot(x, y)) break;
					}
					for (int i = 0; i < mMissleList1->size(); ++i)
					{
						Missle* aMissle = mMissleList1->at(i);
						if (aMissle->Shot(x, y)) break;
					}

					SpawnLaserShot(x - 40, y - 40);
					m0x4ed = true;
					m0x3c4 = Unk01();
					PlayZapSound();
					return;
				}
			}
		}
	}
	else // 83
	{
		if (missilesExist && y > 40)
		{
			for (int i = 0; i < mMissleList1->size(); i++)
			{
				Missle* aMissle = mMissleList1->at(i);
				if (aMissle->Shot(x, y)) break;
			}

			SpawnLaserShot(x - 40, y - 40);
			m0x4ed = true;
			m0x3c4 = Unk01();
			playZapSound = true;
		}
		else
		{
			if (x > 30 && x < 587 && y > 60 && y < 400 && 
				mDropFoodDelay <= 0 && (mTank != 5 || mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK) &&
				mBilaterusList->empty() && mCyraxPtr == nullptr && mMissleList1->empty())
			{
				if (m0x440 || Buy(m0x4ac, true))
				{
					DropFood(x - 10, y - 10, 0, false, 0, -1);
					m0x4ec = true;
					m0x3c0 = Unk01();
				}
			}
		}
	}

	if (playZapSound)
		PlayZapSound();
}

void Board::MouseUp(int x, int y, int theClickCount)
{
	Widget::MouseUp(x, y, theClickCount);
	m0x4ec = false;
	m0x4ed = false;
}

void Sexy::Board::ButtonPress(int theId)
{
	mApp->PlaySample(SOUND_BUTTONCLICK);
	if (mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK && theId < SLOT_END)
	{
		HandleBuySlotPressed(theId);
		return;
	}
	switch (theId)
	{
	case 10:
		mApp->SwitchToStoreScreen();
		return;
	case 11:
		mApp->SwitchToSimFishScreen();
		return;
	case 12:
		mApp->SwitchToPetsScreen();
		return;
	case 13:
		mApp->SwitchToSimSetupScreen();
		return;
	case 14:
		if (!mApp->KillDialog(32))
		{
			HandleFeedButton();
			return;
		}
		break;
	case 15:
		mApp->LeaveGameBoard();
		return;
	}
}

void Sexy::Board::ButtonDepress(int theId)
{
	if (theId == 123)
		mApp->DoOptionsDialog(false);
}

void Board::DrawOverlay0(Graphics* g)
{
	if (mBubbulatorShown)
	{
		int anX, anY;
		GetBubbleSpawnCoords(anX, anY);
		g->SetColorizeImages(true);
		g->SetColor(Color::Black);
		g->DrawImage(IMAGE_SHADOW, anX + 10, anY + 64);
		g->SetColorizeImages(false);
		g->DrawImageCel(IMAGE_BUBBULATOR, anX, anY, m0x44c);
	}
	if (mAlienAttractorShown)
	{
		int anX, anY;
		GetAlienAttractorSpawnCoords(anX, anY);
		g->SetColor(Color(255, 255, 128, 160));
		if (mAlienTimer < 30 || m0x2c4 > 0)
		{
			int aRow = m0x2c4;
			if (m0x2c4 < 1)
				aRow = 30 - m0x2c4;
			if (aRow > 5)
				aRow = 5;
			g->DrawImageBox(Rect((anX - IMAGE_AA_BEAM->mWidth / 2) + 40, (anY - IMAGE_AA_BEAM->mHeight) + 40, IMAGE_AA_BEAM->mWidth, aRow * 450 / 5), IMAGE_AA_BEAM);
		}
		DrawAlienAttractorMisc(g, anX, anY, mAlienTimer, m0x2c4);
	}
	if (m0x4fc)
	{
		for (GameObjectSet::iterator it = mGameObjectSet.begin(); it != mGameObjectSet.end(); ++it)
		{
			GameObject* anObj = *it;
			if (anObj->mShown && anObj->mInvisible && anObj->mInvisibleTimer == 0)
			{
				g->Translate(anObj->mX, anObj->mY);
				anObj->DrawName(g, true);
				g->Translate(-anObj->mX, -anObj->mY);
			}
		}
	}
}

void Board::SpawnBubble(int theX, int theY)
{
	if (mBubbleMgr->mBubbleList.size() < 30)
		mBubbleMgr->SpawnBubble(theX, theY);
}

void Sexy::Board::SpawnRandomBubble()
{
	if (mBubbleMgr->mBubbleList.size() < 50) 
		mBubbleMgr->SpawnBubble(mApp->mSeed->Next() % 22 + 150, mApp->mSeed->Next() % 6 + 400);
}

void Board::AddGameObject(GameObject* theObject)
{
	bool someFlag = true;
	switch (theObject->mType)
	{
	case GameObject::TYPE_GUPPY:
		mFishList->push_back((Fish*) theObject);
		break;
	case GameObject::TYPE_OSCAR:
		mOscarList->push_back((Oscar*)theObject);
		break;
	case GameObject::TYPE_ULTRA:
		mUltraList->push_back((Ultra*)theObject);
		break;
	case GameObject::TYPE_GEKKO:
		mGekkoList->push_back((Gekko*)theObject);
		break;
	case GameObject::TYPE_PENTA:
		mPentaList->push_back((Penta*)theObject);
		break;
	case GameObject::TYPE_GRUBBER:
		mGrubberList->push_back((Grubber*)theObject);
		break;
	case GameObject::TYPE_BREEDER:
		mBreederList->push_back((Breeder*)theObject);
		break;
	case GameObject::TYPE_OTHER_TYPE_PET:
	{
		OtherTypePet* aPet = (OtherTypePet*)theObject;
		if (aPet->mOtherTypePetType >= GameObject::PET_STINKY && aPet->mOtherTypePetType < GameObject::PET_END)
			mPetsInTank[aPet->mOtherTypePetType]++;
		mOtherTypePetList->push_back(aPet);
		break;
	}
	case GameObject::TYPE_FISH_TYPE_PET:
	{
		FishTypePet* aPet = (FishTypePet*)theObject;
		if (aPet->mFishTypePetType >= GameObject::PET_STINKY && aPet->mFishTypePetType < GameObject::PET_END)
			mPetsInTank[aPet->mFishTypePetType]++;
		mFishTypePetList->push_back(aPet);
		break;
	}
	case GameObject::TYPE_ALIEN:
	{
		Alien* anAlien = (Alien*)theObject;
		if (anAlien->mAlienType == Alien::ALIEN_MINI_SYLV)
			mSmallAlienList->push_back(anAlien);
		else
			mAlienList->push_back(anAlien);
		break;
	}
	case GameObject::TYPE_BILATERUS:
		mBilaterusList->push_back((Bilaterus*) theObject);
		break;
	case GameObject::TYPE_COIN:
	{
		Coin* aCoin = (Coin*)theObject;
		if (aCoin->m0x1a0 < 2 && aCoin->mCoinType != 15)
		{
			if (aCoin->mCoinType == 16)
				mCoinList2->push_back(aCoin);
			else
				mCoinList1->push_back(aCoin);
		}
		else
			mCoinList3->push_back(aCoin);
		break;
	}
	case GameObject::TYPE_DEAD_ALIEN:
		mDeadAlienList->push_back((DeadAlien*)theObject);
		break;
	case GameObject::TYPE_DEAD_FISH:
		mDeadFishList->push_back((DeadFish*)theObject);
		break;
	case GameObject::TYPE_FOOD:
		mFoodList->push_back((Food*)theObject);
		break;
	case GameObject::TYPE_LARVA:
		mLarvaList->push_back((Larva*)theObject);
		break;
	case GameObject::TYPE_MISSLE:
	{
		Missle* aMis = (Missle*)theObject;
		someFlag = false;
		if (!aMis->IsTargetless())
			mMissleList1->push_back((Missle*)theObject);
		else
			mMissleList2->push_back((Missle*)theObject);
		break;
	}
	case GameObject::TYPE_SHADOW:
		mShadowList->push_back((Shadow*)theObject);
		someFlag = false;
		break;
	case GameObject::TYPE_SHOT:
		mShotList->push_back((Shot*)theObject);
		someFlag = false;
		break;
	case GameObject::TYPE_WARP:
		mWarpList->push_back((Warp*)theObject);
		someFlag = false;
		break;
	case GameObject::TYPE_SYLVESTER_FISH:
		mSpecialFishList->push_back((SylvesterFish*)theObject);
		break;
	case GameObject::TYPE_BALL_FISH:
		mSpecialFishList->push_back((BallFish*)theObject);
		break;
	case GameObject::TYPE_BI_FISH:
		mSpecialFishList->push_back((BiFish*)theObject);
		break;
	}

	mGameObjectSet.insert(theObject);
	if (someFlag)
		m0x2a7 = true;
}

void Board::MakeShadowForGameObject(GameObject* theObject)
{
	if (theObject->mShadowPtr == nullptr)
	{
		switch (theObject->mType)
		{
		case GameObject::TYPE_GUPPY:
		case GameObject::TYPE_OSCAR:
		case GameObject::TYPE_GEKKO:
		case GameObject::TYPE_BREEDER:
		case GameObject::TYPE_DEAD_FISH:
		case GameObject::TYPE_BALL_FISH:
		case GameObject::TYPE_BI_FISH:
			SpawnShadow(0, theObject);
			break;
		case GameObject::TYPE_ULTRA:
			SpawnShadow(2, theObject);
			break;
		case GameObject::TYPE_PENTA:
		case GameObject::TYPE_GRUBBER:
			SpawnShadow(1, theObject);
			break;
		case GameObject::TYPE_OTHER_TYPE_PET:
		{
			OtherTypePet* aPet = (OtherTypePet*)theObject;
			if(aPet->mOtherTypePetType == GameObject::PET_CLYDE)
				SpawnShadow(0, theObject);
			else if(aPet->mOtherTypePetType != GameObject::PET_NIKO)
				SpawnShadow(1, theObject);
			break;
		}
		case GameObject::TYPE_FISH_TYPE_PET:
		{
			FishTypePet* aPet = (FishTypePet*)theObject;
			if(aPet->mFishTypePetType == GameObject::PET_AMP)
				SpawnShadow(2, theObject);
			else
				SpawnShadow(aPet->mFishTypePetType == GameObject::PET_BRINKLEY, theObject);
			break;
		}
		case GameObject::TYPE_ALIEN:
		{
			Alien* anAlien = (Alien*)theObject;
			SpawnShadow(anAlien->mAlienType == Alien::ALIEN_MINI_SYLV ? 0 : 2, theObject);
			break;
		}
		case GameObject::TYPE_BILATERUS:
		case GameObject::TYPE_COIN:
		case GameObject::TYPE_DEAD_ALIEN:
		case GameObject::TYPE_FOOD:
		case GameObject::TYPE_LARVA:
		case GameObject::TYPE_MISSLE:
		case GameObject::TYPE_SHADOW:
		case GameObject::TYPE_SHOT:
		case GameObject::TYPE_WARP:
			break;
		case GameObject::TYPE_SYLVESTER_FISH:
		{
			SylvesterFish* aFish = (SylvesterFish*)theObject;
			SpawnShadow(aFish->mSize == 1 ? 0 : 2, theObject);
			break;
		}
		default:
			SpawnShadow(0, theObject);
		}
	}
}

void Board::SortGameObjects()
{
	if (mWidgetManager)
	{
		mWidgetManager->MarkAllDirty();
		mWidgetManager->BringToFront(mBoardOverlay1);
	}

	for (int i = 0; i < mShadowList->size(); i++) // 21
	{
		GameObject* anObj = mShadowList->at(i);
		if (anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
			mWidgetManager->BringToFront(anObj);
	}

	for (int i = 0; i < mDeadFishList->size(); i++) // 60
	{
		GameObject* anObj = mDeadFishList->at(i);
		if(anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
			mWidgetManager->BringToFront(anObj);
	}

	for (int i = 0; i < mDeadAlienList->size(); i++) // 99
	{
		GameObject* anObj = mDeadAlienList->at(i);
		if(anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
			mWidgetManager->BringToFront(anObj);
	}

	for (int i = 0; i < mCoinList2->size(); i++) // 138
	{
		GameObject* anObj = mCoinList2->at(i);
		if(anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
			mWidgetManager->BringToFront(anObj);
	}

	for (int i = 0; i < mWarpList->size(); i++) // 177
	{
		GameObject* anObj = mWarpList->at(i);
		if(anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
			mWidgetManager->BringToFront(anObj);
	}

	for (int i = 0; i < mPentaList->size(); i++) // 216
	{
		GameObject* anObj = mPentaList->at(i);
		if(anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
			mWidgetManager->BringToFront(anObj);
	}

	for (int i = 0; i < mGrubberList->size(); i++) // 255
	{
		GameObject* anObj = mGrubberList->at(i);
		if(anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
			mWidgetManager->BringToFront(anObj);
	}

	for (int i = 0; i < mFoodList->size(); i++) // 294
	{
		GameObject* anObj = mFoodList->at(i);
		if(anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
			mWidgetManager->BringToFront(anObj);
	}

	for (int i = 0; i < mFishList->size(); i++) // 333
	{
		GameObject* anObj = mFishList->at(i);
		if(anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
			mWidgetManager->BringToFront(anObj);
	}

	for (int i = 0; i < mOscarList->size(); i++) // 372
	{
		GameObject* anObj = mOscarList->at(i);
		if(anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
			mWidgetManager->BringToFront(anObj);
	}

	for (int i = 0; i < mLarvaList->size(); i++) // 411
	{
		GameObject* anObj = mLarvaList->at(i);
		if(anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
			mWidgetManager->BringToFront(anObj);
	}

	for (int i = 0; i < mGekkoList->size(); i++) // 455
	{
		GameObject* anObj = mGekkoList->at(i);
		if(anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
			mWidgetManager->BringToFront(anObj);
	}

	for (int i = 0; i < mUltraList->size(); i++) // 489
	{
		GameObject* anObj = mUltraList->at(i);
		if(anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
			mWidgetManager->BringToFront(anObj);
	}

	for (int i = 0; i < mBreederList->size(); i++) // 528
	{
		GameObject* anObj = mBreederList->at(i);
		if(anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
			mWidgetManager->BringToFront(anObj);
	}

	for (int i = 0; i < mSpecialFishList->size(); i++) // 567
	{
		GameObject* anObj = mSpecialFishList->at(i);
		if(anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
			mWidgetManager->BringToFront(anObj);
	}

	for (int i = 0; i < mFishList->size(); i++) // 606
	{
		Fish* anObj = mFishList->at(i);
		if(anObj && anObj->mSize == 4)
			mWidgetManager->BringToFront(anObj);
	}

	if (!mCyraxPtr) // 627
	{
		for (int i = 0; i < mAlienList->size(); i++) // 633
		{
			GameObject* anObj = mAlienList->at(i);
			if (anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
				mWidgetManager->BringToFront(anObj);
		}
		for (int i = 0; i < mBilaterusList->size(); i++) // 672
		{
			GameObject* anObj = mBilaterusList->at(i);
			if (anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
				mWidgetManager->BringToFront(anObj);
		}
	}

	for (int i = 0; i < mOtherTypePetList->size(); i++) // 712
	{
		OtherTypePet* anObj = mOtherTypePetList->at(i);
		if (anObj)
		{
			if(anObj->mOtherTypePetType != 1)
				mWidgetManager->BringToFront(anObj);
			else
			{
				if(anObj->mIsPresto)
					mWidgetManager->BringToFront(anObj);
				else
					mWidgetManager->PutBehind(anObj, mBoardOverlay1);
			}
			anObj->MarkDirty();
		}
	}

	for (int i = 0; i < mFishTypePetList->size(); i++) // 759
	{
		GameObject* anObj = mFishTypePetList->at(i);
		if (anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
			mWidgetManager->BringToFront(anObj);
	}

	for (int i = 0; i < mMissleList1->size(); i++) // 798
	{
		GameObject* anObj = mMissleList1->at(i);
		if (anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
			mWidgetManager->BringToFront(anObj);
	}

	for (int i = 0; i < mMissleList2->size(); i++) // 837
	{
		GameObject* anObj = mMissleList2->at(i);
		if (anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
			mWidgetManager->BringToFront(anObj);
	}

	if (mCyraxPtr) //871
	{
		mWidgetManager->BringToFront(mCyraxPtr);
		mCyraxPtr->MarkDirty();

		for (int i = 0; i < mAlienList->size(); i++) // 879
		{
			GameObject* anObj = mAlienList->at(i);
			if (anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
				mWidgetManager->BringToFront(anObj);
		}

		for (int i = 0; i < mBilaterusList->size(); i++) // 918
		{
			GameObject* anObj = mBilaterusList->at(i);
			if (anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
				mWidgetManager->BringToFront(anObj);
		}
	}


	for (int i = 0; i < mSmallAlienList->size(); i++) // 958
	{
		GameObject* anObj = mSmallAlienList->at(i);
		if (anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
			mWidgetManager->BringToFront(anObj);
	}

	for (int i = 0; i < mShotList->size(); i++) // 997
	{
		GameObject* anObj = mShotList->at(i);
		if (anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
			mWidgetManager->BringToFront(anObj);
	}

	for (int i = 0; i < mCoinList1->size(); i++) // 1036
	{
		GameObject* anObj = mCoinList1->at(i);
		if (anObj && (!anObj->mInvisible || (anObj->mInvisible && anObj->mInvisibleTimer != 0)))
			mWidgetManager->BringToFront(anObj);
	}

	if (mLastCoinTypeClicked > -1)
		for (int i = 0; i < mCoinList1->size(); i++) // 1076
		{
			Coin* anObj = mCoinList1->at(i);
			if (anObj && mLastCoinTypeClicked == anObj->mCoinType)
				mWidgetManager->BringToFront(anObj);
		}

	for (int i = 0; i < mCoinList3->size(); i++) // 1103
	{
		Coin* anObj = mCoinList3->at(i);
		if (anObj && mLastCoinTypeClicked == anObj->mCoinType)
			mWidgetManager->BringToFront(anObj);
	}

	if(mMessageWidget)
		mWidgetManager->BringToFront(mMessageWidget);
	mWidgetManager->BringToFront(mBoardOverlay2);

	std::list<Dialog*>& aDiaList = mApp->mDialogList;
	for (Dialog* aDia : aDiaList)
	{
		if (aDia != nullptr)
		{
			mWidgetManager->BringToFront(aDia);
			aDia->MarkDirty();
		}
	}
}

void Sexy::Board::PauseGame(bool shouldPause)
{
	if (mPause != shouldPause)
	{
		if (shouldPause)
		{
			if (!mApp->IsScreenSaver())
				mPause = true;
		}
		else
		{
			m0x4ec = false;
			m0x4ed = false;
			UpdateMoneyLabelText();
			WidgetSetupVT();
			Unk13();
			DetermineAlienSpawnCoordsVT();
			UpdateNikoPosition();
			if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
				UpdateHasVirtualTankFish();
			mFishSongMgr->PausedGameDelaySong();
			mPause = false;
		}
	}
}

void Sexy::Board::StartGame()
{
	mPause = false;
	PlaySample(SOUND_BUBBLES_ID, 3, 1.0);
	int aRandVal = (mApp->mSeed->Next() & 3) + 2;
	while (aRandVal > -1)
	{
		SpawnRandomBubble();
		aRandVal--;
	}
	UserProfile* aCurProf = mApp->mCurrentProfile;
	if ((!aCurProf->mFinishedGame || (mApp->mGameMode != WinFishApp::GAMEMODE_TIME_TRIAL &&
		mApp->mGameMode != WinFishApp::GAMEMODE_CHALLENGE)) && aCurProf->mNumOfUnlockedPets < 4)
	{
		for (int i = GameObject::PET_STINKY; i < GameObject::PET_PRESTO; i++)
			if (aCurProf->IsPetUnlocked(i))
				SpawnPet(i, -1, -1, false, false);
	}
	else
	{
		for (int i = GameObject::PET_STINKY; i < GameObject::PET_END; i++)
			if (aCurProf->m0x5a[i])
				SpawnPet(i, -1, -1, false, false);
	}

	if (!mApp->mRelaxMode)
	{
		if (mTank == 4)
		{
			SpawnBreeder(mApp->mSeed->Next() % 520 + 20, mApp->mSeed->Next() % 265 + 105);
			if (!mBreederList->empty())
				mBreederList->at(0)->m0x1a4 = 2;
		}
		else if(mTank == 5)
		{
			mAlienTimer = 300;
			for (int i = GameObject::PET_STINKY; i < GameObject::PET_GASH; i++)
				SpawnPet(i, -1, -1, false, false);

			SpawnPet(GameObject::PET_GASH, -1, -1, false, false);
		}
		else
		{
			for (int i = 0; i < 2; i++)
				SpawnGuppy(mApp->mSeed->Next() % 520 + 20, mApp->mSeed->Next() % 265 + 105);

			mFishList->at(0)->mFoodAte = 2;
			mFishList->at(1)->mFoodAte = 2;

			if (mTank == 1 && mLevel == 1)
			{
				mFishList->at(0)->mBeginner = true;
				mFishList->at(1)->mBeginner = true;
			}
		}
	}
	else
	{
		for (int i = 0; i < 2; i++)
			SpawnGuppy(mApp->mSeed->Next() % 520 + 20, mApp->mSeed->Next() % 265 + 105);
	}

	if (mApp->mGameMode == WinFishApp::GAMEMODE_TIME_TRIAL)
	{
		if (mTank == 1)
			m0x3bc = 300;
		else if(mTank == 2 ||mTank == 3 ||mTank == 4)
			m0x3bc = 600;
	}
	bool aGameFin = mApp->mCurrentProfile->mFinishedGame;
	SexyString aMsg = "";
	int anId = -1;
	if (mApp->mGameMode == WinFishApp::GAMEMODE_ADVENTURE)
	{
		if (mLevel == 1 && mTank == 1)
		{
			if (!aGameFin)
			{
				anId = 0;
				aMsg = "Welcome to the Insaniquarium!";
			}
			else
			{
				anId = 0;
				aMsg = "Welcome to the Bonus Adventure!";
			}
		}
		else if (mLevel == 2 && mTank == 1 && !aGameFin)
		{
			anId = 16;
			aMsg = "Welcome to the next level!";
		}
		else if (mLevel == 1 && mTank == 2 && !aGameFin)
		{
			anId = 19;
			aMsg = "Welcome to the second tank! Now things get tricky!";
		}
	}
	else if (mApp->mGameMode == WinFishApp::GAMEMODE_TIME_TRIAL)
	{
		anId = 52;
		aMsg = "Welcome to Time Trial mode!";
	}
	else if (mApp->mGameMode == WinFishApp::GAMEMODE_SANDBOX)
	{
		anId = 47;
		aMsg = "You\'ve discovered the top-secret SANDBOX mode!";
	}
	else if (mApp->mGameMode == WinFishApp::GAMEMODE_CHALLENGE)
	{
		anId = 50;
		aMsg = "Welcome to Challenge mode!";
	}
	if (!aMsg.empty() && anId != -1)
		ShowText(aMsg, false, anId);

	mGameUpdateCnt = 0;
	m0x450 = 0;
	m0x454 = 0;
	m0x45c = 0;
	m0x458 = 0;
	m0x4e8 = 0;
	m0x3b4 = Unk01();
	m0x3b8 = Unk01();
	m0x4e6 = false;
}

void Sexy::Board::SaveCurrentGame()
{
	SexyString aFileStr = mApp->mCurrentProfile->GetSaveGameFilePath(mApp->mGameMode, mApp->mCurrentProfile->mId);

	if (!(mApp->mBoard->mShouldSave && mApp->mGameMode != WinFishApp::GAMEMODE_SANDBOX))
		mApp->EraseFile(aFileStr);
	else
	{
		MkDir(GetAppDataFolder().append("userdata"));
		SaveGame(aFileStr);
	}
}

void Sexy::Board::SaveGame(const SexyString& theSavePath)
{
	DataWriter aDW;
	aDW.OpenMemory(32);
	DataSync aDS(aDW);
	int aVersion = 54;
	aDW.WriteLong((int)aVersion); // Game Save File Version
	aDS.mVersion = aVersion;
	SyncGameData(aDS);
	aDW.WriteLong(_time64(NULL));
	aDS.SyncPointers();
	mApp->WriteBytesToFile(theSavePath, aDW.mMemoryHandle, aDW.mMemoryPosition);
	mApp->ClearUpdateBacklog();
	if (mApp->IsScreenSaver())
	{
		mApp->mCurrentProfile->SaveScreenSaver(m0x400);
		m0x400 = 0;
	}
}

bool Sexy::Board::SyncGameData(DataSync& theSync)
{
	theSync.SyncLong(mTank);
	theSync.SyncLong(mLevel);

	if (mApp->mGameMode == WinFishApp::GAMEMODE_ADVENTURE && theSync.mReader != nullptr
		&& (mTank != mApp->mCurrentProfile->mTank || mLevel != mApp->mCurrentProfile->mLevel))
		return false;

	theSync.SyncBool(mIsBonusRound);
	theSync.SyncBool(m0x2a6);

	DataReader* aDR = theSync.mReader;
	DataWriter* aDW = theSync.mWriter;

	int aBGId = -1;

	if (mApp->IsScreenSaver() && aDW && m0x3ec != -1)
	{
		aBGId = mCurrentBackgroundId;
		mCurrentBackgroundId = m0x3ec;
		UpdateNikoPosition();
	}

	if (aDW)
	{
		ulong aNumOfObjects = 0;
		ulong aPos = aDW->mMemoryPosition;
		aDW->WriteLong(0);

		for (GameObjectSet::iterator it = mGameObjectSet.begin(); it != mGameObjectSet.end(); ++it)
		{
			GameObject* anObj = *it;
			if (anObj->mType != GameObject::TYPE_SHADOW)
			{
				aNumOfObjects++;
				anObj->Sync(&theSync);
			}
		}

		if (aDW->mMemoryHandle && aPos + 4U <= aDW->mMemoryLength)
			memcpy((char*)aDW->mMemoryHandle + aPos, &aNumOfObjects, sizeof(aNumOfObjects));
	}
	else
	{
		ResetLevel(mWidgetManager);
		ulong aNumOfObjects = aDR->ReadLong();
		for (ulong i = 0; i < aNumOfObjects; i++)
		{
			int aType = aDR->ReadLong();
			aDR->RollbackBytes(4);
			GameObject* anObj = CreateGameObject(aType);

			if (!anObj)
				throw DataReaderException(); // Usually when aType is -1

			anObj->Sync(&theSync);

			if (!anObj->mShown)
			{
				mGameObjectSet.insert(anObj);
			}
			else
			{
				AddGameObject(anObj);
				mWidgetManager->AddWidget(anObj);
				MakeShadowForGameObject(anObj);
			}
		}
	}

	if (!aDR)
	{
		aDW->WriteBool(mCyraxPtr != nullptr);
		if (mCyraxPtr)
			mCyraxPtr->Sync(&theSync);
	}
	else
	{
		if (mCyraxPtr)
			delete mCyraxPtr;

		if (aDR->ReadBool())
		{
			mCyraxPtr = new Alien();
			mCyraxPtr->Sync(&theSync);
		}
		else
		{
			mCyraxPtr = nullptr;
		}
	}

	if (mCyraxPtr)
		mWidgetManager->AddWidget(mCyraxPtr);

	mMessageWidget->Sync(theSync);

	for (int i = 0; i < 24; i++)
		theSync.SyncLong(mPetsInTank[i]);

	theSync.SyncBool(mBonusRoundStarted);
	theSync.SyncLong(m0x2a8);
	theSync.SyncLong(mAlienExpect);
	theSync.SyncLong(mAlienTimer);
	theSync.SyncLong(m0x2c4);
	theSync.SyncLong(mDropFoodDelay);
	theSync.SyncLong(mCrosshair1X);
	theSync.SyncLong(mCrosshair1Y);
	theSync.SyncLong(mCrosshair2X);
	theSync.SyncLong(mCrosshair2Y);
	theSync.SyncLong(m0x2dc);
	theSync.SyncLong(m0x3b0);
	theSync.SyncLong(m0x3b4);
	theSync.SyncLong(m0x3b8);
	theSync.SyncLong(m0x3bc);
	theSync.SyncLong(m0x3c0);
	theSync.SyncLong(m0x3c4);
	theSync.SyncLong(m0x3e4);
	theSync.SyncLong(mCurrentBackgroundId);
	theSync.SyncLong(mMoney);
	theSync.SyncLong(m0x3f4);
	theSync.SyncLong(m0x3f8);
	theSync.SyncLong(m0x3fc);
	theSync.SyncLong(mLastCoinTypeClicked);
	theSync.SyncLong(mCoinComboCount);
	theSync.SyncLong(m0x43c);
	theSync.SyncBool(m0x440);
	theSync.SyncLong(mGameUpdateCnt);
	theSync.SyncLong(mBubbulatorTimer);
	theSync.SyncLong(m0x44c);
	theSync.SyncLong(m0x450);
	theSync.SyncLong(m0x454);
	theSync.SyncLong(m0x458);
	theSync.SyncLong(m0x45c);
	theSync.SyncLong(m0x460);
	theSync.SyncLong(m0x464);
	theSync.SyncLong(m0x468);
	theSync.SyncLong(mMaxOscarCountEver);
	theSync.SyncLong(mMaxUltraCountEver);
	theSync.SyncLong(m0x474);
	theSync.SyncLong(m0x478);
	theSync.SyncLong(m0x47c);
	theSync.SyncLong(m0x480);
	theSync.SyncLong(m0x484);
	theSync.SyncLong(mGuppiesDeadCount);
	theSync.SyncLong(mMaxFishCountEver);
	theSync.SyncLong(m0x490);
	theSync.SyncLong(mMaxPentaCountEver);
	theSync.SyncLong(m0x498);
	theSync.SyncLong(m0x49c);
	theSync.SyncLong(m0x4a0);
	theSync.SyncLong(m0x4a4);
	theSync.SyncLong(m0x4a8);
	theSync.SyncLong(m0x4ac);
	theSync.SyncBool(m0x4fc);
	theSync.SyncBool(mBubbulatorShown);
	theSync.SyncBool(mAlienAttractorShown);
	theSync.SyncBool(mAlwaysShowWhenHungry);
	theSync.SyncBool(m0x500);
	for (int i = 0; i < sizeof(m0x4b0); i++)
		theSync.SyncBool(m0x4b0[i]);
	theSync.SyncBool(m0x4e6);
	theSync.SyncLong(m0x4e8);
	theSync.SyncLong(m0x4f0);
	theSync.SyncBool(m0x4f4);
	theSync.SyncLong(gWadsworthTimer);
	theSync.SyncLong(gWadsworthX);
	theSync.SyncLong(gWadsworthY);
	theSync.SyncLong(gUnkInt02);
	theSync.SyncLong(gUnkInt05);
	theSync.SyncLong(gUnkInt06);
	theSync.SyncBool(gMerylActive);
	theSync.SyncLong(gFoodType);
	theSync.SyncLong(gFoodLimit);
	theSync.SyncColor(gUnkColor01);
	if (aDR)
		m0x2e0 = 0;

	for (int i = 0; i < sizeof(mSlotPrices) / sizeof(int); i++)
	{
		theSync.SyncLong(mSlotNumber[i]);
		theSync.SyncLong(mSlotPrices[i]);
		theSync.SyncLong(m0x344[i]);
		theSync.SyncLong(m0x374[i]);
		theSync.SyncBool(mSlotUnlocked[i]);

		if (aDR && mSlotUnlocked[i])
			MakeAndUnlockMenuButton(i, false);
	}

	if (aBGId != -1)
	{
		mCurrentBackgroundId = aBGId;
		UpdateNikoPosition();
	}

	bool resetTimer = true;
	if (gWadsworthTimer > 0)
		if (mAlienList->size() > 0 || mBilaterusList->size() > 0)
			resetTimer = false;
	if(resetTimer)
		gWadsworthTimer = 0;
	if (gMerylActive && mPetsInTank[8] == 0)
		gMerylActive = false;
	Unk03();
	return true;
}

GameObject* Sexy::Board::CreateGameObject(int theType)
{
	switch (theType)
	{
	case GameObject::TYPE_GUPPY:
		return new Fish();
		break;
	case GameObject::TYPE_OSCAR:
		return new Oscar();
		break;
	case GameObject::TYPE_ULTRA:
		return new Ultra();
		break;
	case GameObject::TYPE_GEKKO:
		return new Gekko();
		break;
	case GameObject::TYPE_PENTA:
		return new Penta();
		break;
	case GameObject::TYPE_GRUBBER:
		return new Grubber();
		break;
	case GameObject::TYPE_BREEDER:
		return new Breeder();
		break;
	case GameObject::TYPE_OTHER_TYPE_PET:
		return new OtherTypePet();
		break;
	case GameObject::TYPE_FISH_TYPE_PET:
		return new FishTypePet();
		break;
	case GameObject::TYPE_ALIEN:
		return new Alien();
		break;
	case GameObject::TYPE_BILATERUS:
		return new Bilaterus();
		break;
	case GameObject::TYPE_COIN:
		return new Coin();
		break;
	case GameObject::TYPE_DEAD_ALIEN:
		return new DeadAlien();
		break;
	case GameObject::TYPE_DEAD_FISH:
		return new DeadFish();
		break;
	case GameObject::TYPE_FOOD:
		return new Food();
		break;
	case GameObject::TYPE_LARVA:
		return new Larva();
		break;
	case GameObject::TYPE_MISSLE:
		return new Missle();
		break;
	case GameObject::TYPE_SHADOW:
		return new Shadow();
		break;
	case GameObject::TYPE_SHOT:
		return new Shot();
		break;
	case GameObject::TYPE_WARP:
		return new Warp();
		break;
	case GameObject::TYPE_SYLVESTER_FISH:
		return new SylvesterFish();
		break;
	case GameObject::TYPE_BALL_FISH:
		return new BallFish();
		break;
	case GameObject::TYPE_BI_FISH:
		return new BiFish();
		break;
	}
	return nullptr;
}

bool Sexy::Board::LoadGame(SexyString theSavePath)
{
	if (!mApp->IsScreenSaver())
		mApp->mCurrentProfile->Unk01();


	Buffer aBuf;
	if (mApp->ReadBufferFromFile(theSavePath, &aBuf, false))
	{
		DataReader aDR;
		aDR.OpenMemory(aBuf.GetDataPtr(), aBuf.GetDataLen(), false);
		DataSync aSync(aDR);

		int aVer = aDR.ReadLong();
		aSync.mVersion = aVer;
		if (aVer > 52 && SyncGameData(aSync))
		{
			int aVal = aDR.ReadLong();
			aSync.SyncPointers();

			if(mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
			{
				__time64_t currentTime = _time64(nullptr);
				__time64_t timeOfSave = aVal;
				if (currentTime - timeOfSave > 120) 
					Unk15();
			}
			StartMusic();
			mWidgetManager->AddWidget(mMenuButton);
			mWidgetManager->AddWidget(mMoneyLabel);
			mWidgetManager->AddWidget(mMessageWidget);
			UpdateMoneyLabelText();
			ApplyShadowsIf3D();
			mApp->ClearUpdateBacklog();
			mShouldSave = true;
			SortGameObjects();
			if (mApp->IsScreenSaver())
			{
				int aTimeVal = mGameUpdateCnt / 36;
				int aNewVal = (aTimeVal / 172.8) + 5000.0;
				if (aNewVal > 50000)
					aNewVal = 50000;
				else if (aNewVal < 5000)
					aNewVal = 5000;
				gUnkInt07 = aNewVal;
			}
			if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
			{
				MakeVirtualTankButtons();
				WidgetSetupVT();
				Unk13();
				Unk14(m0x2a6);
				DetermineAlienSpawnCoordsVT();
				DoVirtualTankDialog();
			}
			return true;
		}
	}
	return false;
}

void Sexy::Board::InitLevel()
{
	mGameUpdateCnt = 0;

	for (int i = 0; i < SLOT_END; i++)
	{
		mSlotNumber[i] = -1;
		mSlotPrices[i] = 0;
		m0x374[i] = -1;
		mSlotUnlocked[i] = false;
	}

	if (mApp->mGameMode == WinFishApp::GAMEMODE_ADVENTURE)
	{
		mTank = mApp->mCurrentProfile->mTank;
		mLevel = mApp->mCurrentProfile->mLevel;
	}
	else
	{
		mTank = mApp->mTankGameModeChoose;
		mLevel = 5;
	}

	if (mApp->mGameMode == WinFishApp::GAMEMODE_SANDBOX)
		mMoney = 999999;
	else
		mMoney = 200;

	m0x4ac = 5;
	mAlienTimer = 3000;
	mBonusRoundStarted = false;
	mDropFoodDelay = 0;
	m0x2a8 = 0;
	m0x43c = 1;
	m0x3e4 = 2;
	mAlienExpect = 2;
	gFoodType = 0;
	gFoodLimit = 1;
	gMerylActive = false;
	gWadsworthTimer = 0;
	gUnkInt02 = 0;
	m0x440 = false;
	if (mApp->mRelaxMode)
	{
		gFoodType = 0;
		gFoodLimit = 3;
		m0x3e4 = 8;
		mMoney = 200;
		gUnkInt03 = 0;
	}
	m0x460 = 0;
	m0x464 = 0;
	m0x468 = 0;
	mMaxOscarCountEver = 0;
	mMaxUltraCountEver = 0;
	m0x474 = 0;
	m0x478 = 0;
	m0x47c = 0;
	m0x480 = 0;
	m0x484 = 0;
	mGuppiesDeadCount = 0;
	mMaxFishCountEver = 0;
	m0x490 = 0;
	mMaxPentaCountEver = 0;
	m0x498 = 0;
	m0x49c = 0;
	m0x4a0 = 0;
	m0x4a4 = 0;
	m0x4a8 = 0;
	mWidgetManager->AddWidget(mMenuButton);
	mWidgetManager->AddWidget(mMoneyLabel);
	mWidgetManager->AddWidget(mMessageWidget);
	UpdateMoneyLabelText();
	memset(m0x4b0, 0, 54);
	DeterminePricesAndSlots();
	m0x2dc = 0;
	m0x2e0 = 0;
	for (int i = 0; i < SLOT_END; i++)
	{
		if (mSlotNumber[i] > -1)
			m0x2dc++;
		if (m0x374[i] == -1)
			m0x374[i] = mSlotPrices[i] / 100;
		m0x344[i] = mSlotPrices[i];
	}

	if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
		MakeVirtualTankButtons();
	mWidgetManager->MarkAllDirty();
}

void Sexy::Board::InitBonusLevel()
{
	mMessageWidget->m0xd0 = 0;
	m0x2a8 = mGameUpdateCnt;
	mAlienExpect = 0;
	mPause = false;
	mIsBonusRound = true;
	mBonusRoundStarted = false;
	gFoodType = 0;
	gFoodLimit = 1;
	gMerylActive = false;
	gWadsworthTimer = 0;
	gUnkInt02 = 0;
	m0x440 = false;
	m0x450 = 0;
	m0x454 = 0;
	m0x458 = 0;
	m0x45c = 0;
	switch (mTank)
	{
	case 2:
		m0x3bc = 15;
		break;
	case 3:
		m0x3bc = 20;
		break;
	case 4:
		m0x3bc = 25;
		break;
	default:
		m0x3bc = 10;
	}

	int aLvl = mLevel;
	if (aLvl < 1)
		aLvl = 1;
	if(aLvl >= 7)
		aLvl = 6;

	m0x3bc = m0x3bc + aLvl - 1;
	m0x3f4 = 0;
	m0x3f8 = 0;
	mLastCoinTypeClicked = -1;
	mCoinComboCount = 0;
	int aVal = Unk01();
	m0x3b4 = aVal;
	m0x3b8 = aVal;
}

void Sexy::Board::StartMusic()
{
	if (mAlienTimer != 1)
	{
		if (mAlienList->empty() && mBilaterusList->empty() && !mCyraxPtr)
		{
			if (mAlienTimer < 2 || mAlienTimer > 275)
			{
				if (mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)
				{
					mApp->StartGameMusic();
					return;
				}
				mApp->StopMusic();
				return;
			}
			mApp->StopMusic();
			if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK && !mAlienAttractorShown)
				return;
			mApp->SomeMusicFunc(false);
			return;
		}
	}

	mApp->StopMusic();
	mApp->PlayMusic(1, 3);
}

int Sexy::Board::Unk01()
{
	return mApp->mFrameTime * mGameUpdateCnt;
}

int Sexy::Board::Unk02()
{
	return (Unk01() - m0x3b8) / 1000;
}

void Sexy::Board::Unk03()
{
	switch (mCurrentBackgroundId)
	{
	case 1:
		gUnkColor01 = Color(66, 48, 41);
		break;
	case 2:
		gUnkColor01 = Color(154, 77, 111);
		break;
	case 3:
		gUnkColor01 = Color(47, 39, 124);
		break;
	case 4:
		gUnkColor01 = Color(15, 73, 58);
		break;
	case 5:
		gUnkColor01 = Color(110, 83, 72);
		break;
	case 6:
		gUnkColor01 = Color(50, 0, 59);
		break;
	default:
		break;
	}

	if ((mApp->mCurrentProfile->mCheatCodeFlags >> 2 & 1))
		gUnkColor01 = Color(0x333333);
}

void Sexy::Board::Unk04()
{
	int something = Unk10();
	mMoney += something;
	if (mMoney < 0)
		mMoney = 0;
	if (mMoney > 9999999)
		mMoney = 9999999;
	UpdateMoneyLabelText();
}

void Sexy::Board::Unk05()
{
	if (mLevel != 6)
		mApp->StartGameMusic();
	mBonusRoundStarted = true;
	m0x3b4 = Unk01();
	m0x3b8 = Unk01();
	m0x3b0 = 0;
	m0x2a8 = mGameUpdateCnt;
}

void Sexy::Board::Unk06(Graphics* g, Image* theImage, int theX, int theY, float theVal)
{
	int aStartY = -81 - theY;
	int aCounter = 0;
	for (int i = 0; i < 180; i += 9)
	{
		if (aStartY + theY < 359)
		{
			bool isAccel = mApp->Is3DAccelerated();
			double aRad = (((mGameUpdateCnt + i) * 5) * PI) / 180.0;
			float aSinVal = sinf(aRad) * theVal;
			if (!isAccel)
				g->DrawImage(theImage, theX, aSinVal + theY, Rect(0, aCounter, theImage->mWidth, 24));
			else
				g->DrawImageF(theImage, theX, aSinVal + theY, Rect(0, aCounter, theImage->mWidth, 24));
		}
		theY += 24;
		aCounter += 24;
	}
}

void Sexy::Board::Unk07(int unk)
{
	if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
	{
		if (!mApp->IsScreenSaver())
			mApp->mCurrentProfile->AddShells(unk);
		m0x400 += unk;
		UpdateMoneyLabelText();
		return;
	}
	if (mIsBonusRound)
	{
		m0x3f4 += unk;
		UpdateMoneyLabelText();
		return;
	}
	mMoney += unk;
	m0x460 += unk;
	if (mMoney > 9999999)
		mMoney = 9999999;
	UpdateMoneyLabelText();
}

bool Sexy::Board::Unk08(int x, int y)
{
	if (gUnkBool03)
		return false;

	gUnkBool03 = true;
	GameObject* aPetGotIt = nullptr;

	for(int i = 0; i < mFishTypePetList->size(); i++)
	{
		FishTypePet* aPet = mFishTypePetList->at(i);
		if (aPet->m0x230 && aPet->Contains(x, y))
		{
			aPetGotIt = aPet;
			break;
		}
	}

	for(int i = 0; i < mOtherTypePetList->size(); i++)
	{
		OtherTypePet* aPet = mOtherTypePetList->at(i);
		if (aPet->mIsPresto && aPet->Contains(x, y))
		{
			aPetGotIt = aPet;
			break;
		}
	}

	if(aPetGotIt)
		aPetGotIt->MouseDown(x - aPetGotIt->mX, y - aPetGotIt->mY, -1);

	gUnkBool03 = false;
	return aPetGotIt != nullptr;
}

bool Sexy::Board::Unk09(Coin* theCoin)
{
	if (!mIsBonusRound)
		return false;

	if (theCoin->mCoinType != mLastCoinTypeClicked)
	{
		mLastCoinTypeClicked = theCoin->mCoinType;
		mCoinComboCount = 0;
	}

	int aSound = 0;
	int aUnk01 = 0;
	int aValue = 0;
	switch (theCoin->mCoinType - 1)
	{
	case 0:
		aSound = SOUND_POINTS;
		aUnk01 = 0;
		aValue = 1;
		break;
	case 1:
		aSound = SOUND_POINTS;
		aUnk01 = 1;
		aValue = 2;
		break;
	case 3:
		aSound = SOUND_DIAMOND;
		aUnk01 = 2;
		aValue = 5;
		break;
	case 5:
		aSound = SOUND_PEARL;
		aUnk01 = 3;
		aValue = 10;
		break;
	case 6:
		aSound = SOUND_BONUSCOLLECT;
		aUnk01 = 4;
		aValue = 20;
		break;
	default:
		return false;
	}

	mCoinComboCount++;
	if (mCoinComboCount < 10)
	{
		theCoin->mComboCount = mCoinComboCount;
		MakeNote(theCoin->mX, theCoin->mY, aUnk01 + 3, StrFormat("+%d", mCoinComboCount* aValue));
		SoundInstance* aSndInst = mApp->mSoundManager->GetSoundInstance(aSound);

		if (aSndInst)
		{
			aSndInst->SetVolume(1.0);
			aSndInst->AdjustPitch(mCoinComboCount - 1);
			aSndInst->Play(false, true);
		}
	}
	else
	{
		theCoin->mComboCount = 25;
		MakeNote(theCoin->mX, theCoin->mY, aUnk01 + 3, StrFormat("+%d (MAX CHAIN!!)", mCoinComboCount));

		FishSong* aFishSong = new FishSong();
		aFishSong->mTone = aSound;
		for (int i = 0; i < 5; i++)
		{
			NoteData aNote;
			aNote.mPitch = 9;
			aNote.mVolume = 1.0;
			aNote.mDuration = 200;
			aFishSong->mNoteDataVector.push_back(aNote);
		}
		mFishSongMgr->AddSong(aFishSong);
		mCoinComboCount = 1;
		mLastCoinTypeClicked = -1;
	}
	return true;
}

int Sexy::Board::Unk10()
{
	int aValue = 0;
	for (int i = 0; i < mCoinList1->size(); i++)
	{
		Coin* aCoin = mCoinList1->at(i);
		if (aCoin->m0x198)
			aValue += aCoin->GetValue();
	}

	for (int i = 0; i < mCoinList3->size(); i++)
	{
		Coin* aCoin = mCoinList3->at(i);
		if (aCoin->m0x198)
			aValue += aCoin->GetValue();
	}

	for (int i = 0; i < mLarvaList->size(); i++)
	{
		Larva* aLarva = mLarvaList->at(i);
		if (aLarva->mPickedUp)
			aValue += aLarva->GetValue();
	}
	return aValue;
}

bool Sexy::Board::Unk11(int x, int y)
{
	for (int i = 0; i < mFoodList->size(); i++)
	{
		Food* aFood = mFoodList->at(i);
		if (aFood->mExoticFoodType == 2 && !aFood->mPickedUp)
		{
			if (aFood->Contains(x, y))
			{
				aFood->PickUp();
				return true;
			}
		}
	}
	return false;
}

void Sexy::Board::Unk12()
{
	if (mMissleList1->empty() && mAlienList->empty() && mBilaterusList->empty())
	{
		m0x4ed = false;
		m0x4ec = false;
		mDropFoodDelay = 36;
		if (mCyraxPtr == nullptr)
			mApp->SomeMusicPlayFunc(true);
		Unk14(true);
	}
}

void Sexy::Board::Unk13()
{
	long long aTodaySecs = GetTodayStartSeconds();
	long long aCurrentTime = time(NULL);

	for (GameObjectSet::iterator it = mGameObjectSet.begin(); it != mGameObjectSet.end(); ++it)
	{
		GameObject* anObj = *it;

		anObj->Unk03(aTodaySecs, aCurrentTime);
	}
}

void Sexy::Board::Unk14(bool unk)
{
	m0x2a6 = unk;
	for (int i = 0; i < mFishList->size(); i++)
	{
		Fish* aFish = mFishList->at(i);
		aFish->mMouseVisible = unk;
	}
	for (int i = 0; i < mBreederList->size(); i++)
	{
		Breeder* aBreeder = mBreederList->at(i);
		aBreeder->mMouseVisible = unk;
	}

	if (mTank != 5 || mLevel != 1)
	{
		for (int i = 0; i < mFishTypePetList->size(); i++)
		{
			FishTypePet* aPet = mFishTypePetList->at(i);
			if(aPet->mFishTypePetType == GameObject::PET_AMP && aPet->mCoinDropT <= aPet->mCoinDropTimer)
				aPet->mMouseVisible = unk;
			else if(aPet->mFishTypePetType == GameObject::PET_WALTER)
				aPet->mMouseVisible = unk;
		}
	}
}

void Sexy::Board::Unk15()
{
	GameObjectSet aSet;
	
	for (GameObjectSet::iterator it = mGameObjectSet.begin(); it != mGameObjectSet.end(); ++it)
	{
		GameObject* anObj = *it;
		if (anObj->mType != GameObject::TYPE_SHADOW && 
			(anObj->mType != GameObject::TYPE_COIN || ((Coin*)anObj)->m0x190 == 0) && 
			anObj->mVirtualTankId < 0)
			aSet.insert(anObj);
	}

	for (GameObjectSet::iterator it = aSet.begin(); it != aSet.end(); ++it)
	{
		GameObject* anObj = *it;
		anObj->RemoveHelper02(true);
	}
}

void Sexy::Board::DetermineAlienSpawnCoordsVT()
{
	if (mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)
		return;

	int aX, aY;
	GetAlienAttractorSpawnCoords(aX, aY);
	mCrosshair1X = aX - 39;
	mCrosshair1Y = aY - 180;
	if (mAlienExpect == 8)
		mCrosshair1X = aX + 1;

	mCrosshair2X = mApp->mSeed->Next() % 450 + 20;
	mCrosshair2Y = mApp->mSeed->Next() % 195 + 105;
}

int Sexy::Board::GetNextVirtualTankId()
{
	int aMaxObjects = mApp->mCurrentProfile->mBubbulatorBought != 0 ? 20 : 10;

	bool occupied[20] = { false };

	for (GameObjectSet::iterator it = mGameObjectSet.begin(); it != mGameObjectSet.end(); ++it)
	{
		GameObject* anObj = *it;
		if (anObj->mVirtualTankId >= 0 && anObj->mVirtualTankId < aMaxObjects)
			occupied[anObj->mVirtualTankId] = true;
	}

	for (int i = 0; i < aMaxObjects; i++)
		if (!occupied[i])
			return i;
	return -1;
}

bool Sexy::Board::HasAnyFish()
{
	if (mFishList->empty() && mOscarList->empty() && mPentaList->empty() && mUltraList->empty()
		&& mGrubberList->empty() && mGekkoList->empty() && mBreederList->empty())
		return false;
	return true;
}

void Sexy::Board::HandleBuyEgg()
{
	MenuButtonWidget* aBtn = GetMenuButtonById(SLOT_EGG);
	if (!aBtn)
		return;

	if (mApp->mGameMode == WinFishApp::GAMEMODE_SANDBOX)
	{
		mApp->LeaveGameBoard();
		return;
	}

	if (!Buy(mSlotPrices[SLOT_EGG], true))
		return;

	if (mApp->mGameMode == WinFishApp::GAMEMODE_TIME_TRIAL)
	{
		std::vector<int> anAvailablePets;
		for (int i = 0; i < GameObject::PET_END; i++)
		{
			if (mPetsInTank[i] == 0 && i != GameObject::PET_PRESTO && mApp->mCurrentProfile->IsPetUnlocked(i))
				anAvailablePets.push_back(i);
		}

		if (anAvailablePets.size() != 0)
		{
			int aRandPetIdx = Rand() % anAvailablePets.size();
			SpawnPet(anAvailablePets[aRandPetIdx], -1, -1, false, false);
			m0x498++;
			UpdateSlotPrice(SLOT_EGG, mSlotPrices[SLOT_EGG]);
		}

		if (anAvailablePets.size() <= 1)
			aBtn->SetMaxedOut();

		PlaySample(SOUND_BUY_ID, 3, 1.0);

		return;
	}

	if (m0x4b0[40])
	{
		m0x4b0[40] = false;
		m0x4e6 = true;
		m0x4e8 = 0;
	}
	
	if (IsTankAndLevelNB(1, 1) && mMessageWidget->m0xd0 < 1 && m0x43c == 1)
		ShowText("Collect 2 more egg pieces to finish level!", true, 10);

	m0x43c++;

	if (mApp->mGameMode == WinFishApp::GAMEMODE_CHALLENGE)
	{
		m0x454 = 500;
		for (int i = 0; i < SLOT_END; i++)
		{
			if (i != SLOT_EGG)
				UpdateSlotPrice(i, m0x344[i]);
		}
		if(m0x43c < 4)
			ShowText("Prices have been reset!", false, -1);
	}

	MenuButtonSetupNoVT(SLOT_EGG, true);
	PlaySample(SOUND_BUY_ID, 3, 1.0);

	if (m0x43c < 4)
		return;

	m0x3fc = Unk02();
	mShouldSave = false;
	Unk04();

	if (mApp->mGameMode == WinFishApp::GAMEMODE_CHALLENGE)
	{
		if (mTank > 0 && mTank < 5)
			mApp->mCurrentProfile->m0x78[mTank-1] = true;
		mApp->mHighScoreMgr->RecordChallengeHighScore(mTank, mApp->mCurrentProfile, m0x3fc);
		mApp->SwitchToBonusScreen();
		return;
	}

	int aScore = m0x3fc;
	if (mTank == 5)
	{
		int aCnt = 0;
		gPetsDiedOnBossLevel = 0;
		for (int i = 0; i < 18; i++)
		{
			if (mPetsInTank[i] == 0)
			{
				gDeadPetsIdArray[aCnt] = i;
				aCnt++;
				gPetsDiedOnBossLevel = aCnt;
			}
		}

		aScore = mOtherTypePetList->size() + mFishTypePetList->size();
	}
	mApp->mHighScoreMgr->RecordAdventureHighScore(mTank, mLevel, mApp->mCurrentProfile, aScore);

	if (mApp->mCurrentProfile->mFinishedGame)
	{
		if (mTank == 5 && mLevel == 1)
		{
			mApp->mCurrentProfile->NextLevel();
			mApp->SwitchToBonusScreen();
			return;
		}
		mShouldSave = true;
		ResetLevel(mWidgetManager);
		mApp->StopMusic();
		mApp->PlayMusic(2, 54);
		InitBonusLevel();
		return;
	}

	mApp->mCurrentProfile->NextLevel();
	int aPetId = -1;
	if (mTank == 4 && mLevel == 5)
		aPetId = 999;
	else if(mTank == 5)
	{
		aPetId = GameObject::PET_PRESTO;
		mApp->mCurrentProfile->UnlockPet(aPetId, true);
	}
	else
	{
		aPetId = mTank * 5 - 6 + mLevel;
		if(aPetId >= GameObject::PET_STINKY && aPetId < GameObject::PET_END)
			mApp->mCurrentProfile->UnlockPet(aPetId, true);
	}

	if (aPetId == GameObject::PET_PRESTO)
		mApp->mCurrentProfile->AddShells(5000);

	mApp->SwitchToHatchScreen(aPetId);
}

void Sexy::Board::HandleFeedButton()
{
	int aReqFoods[8] = {0};
	int aFoodsInTank[8] = { 0 };
	GetExoticFoodsRequiredInTank(aReqFoods);
	GetExoticFoodsInTank(aFoodsInTank);
	int aNumOfButtonsToShow = 0;
	int ivar4 = 0;
	for (int i = 0; i < 8; i++)
	{
		if (aReqFoods[i] > 0)
			aNumOfButtonsToShow++;
		if (aReqFoods[i] != aFoodsInTank[i] && aReqFoods[i] - aFoodsInTank[i] > -1)
			ivar4++;
	}

	if (ivar4 != 0)
	{
		if (aNumOfButtonsToShow > 1)
		{
			mApp->DoFoodDialog();
		}
		else
		{
			for (int i = 0;i < 8;i++)
				if (aReqFoods[i] != aFoodsInTank[i] && aReqFoods[i] - aFoodsInTank[i] > -1)
					mApp->mBoard->SpawnVirtualTankFood(i);
		}
	}
}

void Sexy::Board::UpdateHasVirtualTankFish()
{
	mHasVirtualTankFish = false;
	for (GameObjectSet::iterator it = mGameObjectSet.begin(); it != mGameObjectSet.end(); ++it)
	{
		if ((*it)->mVirtualTankId > -1)
		{
			mHasVirtualTankFish = true;
			return;
		}
	}
}

void Sexy::Board::BonusRoundDropShell()
{
	int aCount = (mApp->mSeed->Next() & 1) + 1;
	int aProbs[] = { 20, 30, 20, 10 };

	for (int i = 0; i < aCount; i++)
	{
		int aCoinType = 1;

		int aRandVal = Rand() % 100;
		int aProbIndex = 0;
		int aChance = 100;
		while (aProbIndex < 4)
		{
			aChance -= aProbs[aProbIndex];
			if (aChance <= aRandVal) break;
			aProbIndex++;
		}

		switch (aProbIndex)
		{
		case 0: aCoinType = 2; break;
		case 1: aCoinType = 4; break;
		case 2: aCoinType = 6; break;
		case 3: aCoinType = 7; break;
		default: aCoinType = 1; break;
		}

		int aX = (mApp->mSeed->Next() % 520) + 20;
		int aY = mApp->mSeed->Next() % 10 + 50;

		double aVY = (double)(Rand() % 10) / 10.0 * 3.0 + 1.0;

		DropCoin(aX, aY, aCoinType, nullptr, aVY, 0);
	}
	m0x454 = 500;
}

void Sexy::Board::NostradamusSneezeEffect()
{
	if (Rand() % 100 >= 50)
		return;

	int aNostradamusCnt = 0;
	for (int i = 0; i < mFishTypePetList->size(); i++)
	{
		FishTypePet* aPet = mFishTypePetList->at(i);
		if (aPet->mFishTypePetType == GameObject::PET_NOSTRADAMUS)
		{
			aNostradamusCnt++;
			for (int i = 0; i < 5; i++)
				SpawnBubble(Rand() % 21 + 30 + aPet->mX, Rand() % 21 + 40 + aPet->mY);
		}
	}

	m0x2b4 = 30;
	PlaySample(SOUND_BUBBLES_ID, 3, 1.0);

	FishSong* aFishSong = new FishSong();
	aFishSong->mTone = SOUND_SNEEZE;

	if (aNostradamusCnt > 5)
		aNostradamusCnt = 5;

	for (int i = 0; i < aNostradamusCnt; i++)
	{
		NoteData aNote;
		aNote.mPitch = 0;
		aNote.mVolume = 1.0;
		aNote.mDuration = Rand() % 150 + 50;
		aFishSong->mNoteDataVector.push_back(aNote);
	}
	mFishSongMgr->AddSong(aFishSong);

	mAlienTimer = 635;
	mApp->SomeMusicPlayFunc(false);
	ShowText("Attack Postponed by Sneeze of Power!", false, -1);
}

void Sexy::Board::PlayChompSound(bool flag)
{
	if (!CanPlaySound(SOUND_CHOMP_ID, 3))
		return;

	int aSoundId = SOUND_CHOMP2_ID - ((mApp->mSeed->Next() % 3) != 0);
	if (flag)
		aSoundId = SOUND_SLURP_ID;

	int aSndInstId = GetSoundById(aSoundId);
	SoundInstance* aSndInst = mApp->mSoundManager->GetSoundInstance(aSndInstId);
	if (aSndInst)
	{
		if (flag)
			aSndInst->AdjustPitch(-18);
		aSndInst->Play(false, true);
	}
}

void Sexy::Board::PlaySlurpSound(bool isVoracious)
{
	if (!CanPlaySound(SOUND_SLURP_ID, 3))
		return;

	int aSound = SOUND_SLURP3;
	if (mApp->mSeed->Next() % 5)
	{
		aSound = SOUND_SLURP2;
		if (mApp->mSeed->Next() % 5 != 1)
			aSound = SOUND_SLURP;
	}

	SoundInstance* aSndInst = mApp->mSoundManager->GetSoundInstance(aSound);
	if (aSndInst)
	{
		if (isVoracious)
			aSndInst->AdjustPitch(-14);
		aSndInst->Play(false, true);
	}
}

void Sexy::Board::PlayPointsSound()
{
	if (!CanPlaySound(SOUND_POINTS_ID, 3))
		return;

	int aSoundChoose = mApp->mSeed->Next() % 4;
	if (aSoundChoose != 0 && !mIsBonusRound)
	{
		if (aSoundChoose == 1)
			mApp->PlaySample(SOUND_POINTS2);
		else if(aSoundChoose == 2)
			mApp->PlaySample(SOUND_POINTS3);
		else
			mApp->PlaySample(SOUND_POINTS4);
		return;
	}
	mApp->PlaySample(SOUND_POINTS);
}

void Sexy::Board::PlaySplashSound()
{
	unsigned long aRand = mApp->mSeed->Next();
	if(aRand % 3 == 0)
		mApp->PlaySample(SOUND_SPLASH);
	else if(aRand % 3 == 1)
		mApp->PlaySample(SOUND_SPLASH2);
	else 
		mApp->PlaySample(SOUND_SPLASH3);
}

void Sexy::Board::PlayDieSound(int theObjType)
{
	if (!CanPlaySound(SOUND_DIE_ID, 3))
		return;

	int aPitch = 0;
	switch (theObjType)
	{
	case GameObject::TYPE_OSCAR:
	case GameObject::TYPE_GRUBBER:
		aPitch = -6;
		break;
	case GameObject::TYPE_ULTRA:
		aPitch = -12;
		break;
	case GameObject::TYPE_GEKKO:
	case GameObject::TYPE_SYLVESTER_FISH:
	case GameObject::TYPE_BALL_FISH:
	case GameObject::TYPE_BI_FISH:
		aPitch = -3;
		break;
	case GameObject::TYPE_PENTA:
		aPitch = 4;
		break;
	case GameObject::TYPE_BREEDER:
		aPitch = 5;
		break;
	}

	SoundInstance* inst = mApp->mSoundManager->GetSoundInstance(SOUND_DIE);
	if (inst)
	{
		inst->AdjustPitch(aPitch);
		inst->Play(false, true);
	}
}

void Sexy::Board::PlayBirthSound(bool flag)
{
	int aFlags = mApp->mCurrentProfile->mCheatCodeFlags;
	if ((aFlags >> 6 & 1))
		PlaySample(SOUND_FART2_ID, 3, 1.0);
	else if (!flag)
	{
		if ((aFlags >> 1 & 1) == 0)
			PlaySample(SOUND_BABY_ID, 3, 1.0);
		else
			PlaySample(SOUND_FART_ID, 3, 1.0);
	}
	else if((aFlags >> 5 & 1) == 0)
		PlaySample(SOUND_SFX_ID, 3, 1.0);
	else
		PlaySample(SOUND_FART_ID, 3, 1.0);
}

void Sexy::Board::PlayPunchSound(int theDelay)
{
	PlaySample(SOUND_PUNCH_ID, theDelay, 1.0);
}

void Sexy::Board::PlayZapSound()
{
	if ((mApp->mSeed->Next() & 1) != 0)
	{
		PlaySample(SOUND_ZAP_ID, 3, 1.0);
		PlaySample(SOUND_ZZAM_ID, 3, 1.0);
	}
	else
		PlaySample(SOUND_ZAP_ID, 3, 1.0);

	if (mApp->mSeed->Next() % 10 == 0)
		PlaySample(SOUND_RICOCHET_ID, 3, 1.0);
}

bool Sexy::Board::CanPlaySound(int theSoundId, int theTimePassed)
{
	int aNum = theSoundId - SOUND_START_ID;
	int aNumOfSounds = SOUND_END_ID - SOUND_START_ID;
	if (aNum <= aNumOfSounds && mUpdateCnt - mSoundPlayedTimerArray[aNum] >= theTimePassed)
	{
		mSoundPlayedTimerArray[aNum] = mUpdateCnt;
		return true;
	}
	return false;
}

void Sexy::Board::RemoveWidgetHelper(Widget* theWidget)
{
	if (theWidget)
	{
		mApp->mWidgetManager->RemoveWidget(theWidget);
		mApp->SafeDeleteWidget(theWidget);
	}
}

void Sexy::Board::ResetLevel(WidgetManager* theWidgetManager)
{
	mGameObjectSet.clear();
	memset(mPetsInTank, 0, sizeof(mPetsInTank));
	for (int i = 0; i < mFishList->size(); i++)
	{
		theWidgetManager->RemoveWidget(mFishList->at(i));
		mApp->SafeDeleteWidget(mFishList->at(i));
	}
	mFishList->clear();

	for (int i = 0; i < mDeadFishList->size(); i++)
	{
		theWidgetManager->RemoveWidget(mDeadFishList->at(i));
		mApp->SafeDeleteWidget(mDeadFishList->at(i));
	}
	mDeadFishList->clear();

	for (int i = 0; i < mDeadAlienList->size(); i++)
	{
		theWidgetManager->RemoveWidget(mDeadAlienList->at(i));
		mApp->SafeDeleteWidget(mDeadAlienList->at(i));
	}
	mDeadAlienList->clear();

	for (int i = 0; i < mCoinList1->size(); i++)
	{
		theWidgetManager->RemoveWidget(mCoinList1->at(i));
		mApp->SafeDeleteWidget(mCoinList1->at(i));
	}
	mCoinList1->clear();

	for (int i = 0; i < mOscarList->size(); i++)
	{
		theWidgetManager->RemoveWidget(mOscarList->at(i));
		mApp->SafeDeleteWidget(mOscarList->at(i));
	}
	mOscarList->clear();

	for (int i = 0; i < mUltraList->size(); i++)
	{
		theWidgetManager->RemoveWidget(mUltraList->at(i));
		mApp->SafeDeleteWidget(mUltraList->at(i));
	}
	mUltraList->clear();

	for (int i = 0; i < mGekkoList->size(); i++)
	{
		theWidgetManager->RemoveWidget(mGekkoList->at(i));
		mApp->SafeDeleteWidget(mGekkoList->at(i));
	}
	mGekkoList->clear();

	for (int i = 0; i < mGrubberList->size(); i++)
	{
		theWidgetManager->RemoveWidget(mGrubberList->at(i));
		mApp->SafeDeleteWidget(mGrubberList->at(i));
	}
	mGrubberList->clear();

	for (int i = 0; i < mPentaList->size(); i++)
	{
		theWidgetManager->RemoveWidget(mPentaList->at(i));
		mApp->SafeDeleteWidget(mPentaList->at(i));
	}
	mPentaList->clear();

	for (int i = 0; i < mBreederList->size(); i++)
	{
		theWidgetManager->RemoveWidget(mBreederList->at(i));
		mApp->SafeDeleteWidget(mBreederList->at(i));
	}
	mBreederList->clear();

	for (int i = 0; i < mShotList->size(); i++)
	{
		theWidgetManager->RemoveWidget(mShotList->at(i));
		mApp->SafeDeleteWidget(mShotList->at(i));
	}
	mShotList->clear();

	for (int i = 0; i < mFoodList->size(); i++)
	{
		theWidgetManager->RemoveWidget(mFoodList->at(i));
		mApp->SafeDeleteWidget(mFoodList->at(i));
	}
	mFoodList->clear();

	for (int i = 0; i < mShadowList->size(); i++)
	{
		theWidgetManager->RemoveWidget(mShadowList->at(i));
		mApp->SafeDeleteWidget(mShadowList->at(i));
	}
	mShadowList->clear();

	for (int i = 0; i < mCoinList2->size(); i++)
	{
		theWidgetManager->RemoveWidget(mCoinList2->at(i));
		mApp->SafeDeleteWidget(mCoinList2->at(i));
	}
	mCoinList2->clear();

	for (int i = 0; i < mCoinList3->size(); i++)
	{
		theWidgetManager->RemoveWidget(mCoinList3->at(i));
		mApp->SafeDeleteWidget(mCoinList3->at(i));
	}
	mCoinList3->clear();

	for (int i = 0; i < mWarpList->size(); i++)
	{
		theWidgetManager->RemoveWidget(mWarpList->at(i));
		mApp->SafeDeleteWidget(mWarpList->at(i));
	}
	mWarpList->clear();

	for (int i = 0; i < mMissleList1->size(); i++)
	{
		theWidgetManager->RemoveWidget(mMissleList1->at(i));
		mApp->SafeDeleteWidget(mMissleList1->at(i));
	}
	mMissleList1->clear();

	for (int i = 0; i < mMissleList2->size(); i++)
	{
		theWidgetManager->RemoveWidget(mMissleList2->at(i));
		mApp->SafeDeleteWidget(mMissleList2->at(i));
	}
	mMissleList2->clear();

	for (int i = 0; i < mFishTypePetList->size(); i++)
	{
		theWidgetManager->RemoveWidget(mFishTypePetList->at(i));
		mApp->SafeDeleteWidget(mFishTypePetList->at(i));
	}
	mFishTypePetList->clear();

	for (int i = 0; i < mOtherTypePetList->size(); i++)
	{
		theWidgetManager->RemoveWidget(mOtherTypePetList->at(i));
		mApp->SafeDeleteWidget(mOtherTypePetList->at(i));
	}
	mOtherTypePetList->clear();

	for (int i = 0; i < mLarvaList->size(); i++)
	{
		theWidgetManager->RemoveWidget(mLarvaList->at(i));
		mApp->SafeDeleteWidget(mLarvaList->at(i));
	}
	mLarvaList->clear();

	for (int i = 0; i < mAlienList->size(); i++)
	{
		theWidgetManager->RemoveWidget(mAlienList->at(i));
		mApp->SafeDeleteWidget(mAlienList->at(i));
	}
	mAlienList->clear();

	for (int i = 0; i < mSmallAlienList->size(); i++)
	{
		theWidgetManager->RemoveWidget(mSmallAlienList->at(i));
		mApp->SafeDeleteWidget(mSmallAlienList->at(i));
	}
	mSmallAlienList->clear();

	for (int i = 0; i < mBilaterusList->size(); i++)
	{
		theWidgetManager->RemoveWidget(mBilaterusList->at(i));
		mApp->SafeDeleteWidget(mBilaterusList->at(i));
	}
	mBilaterusList->clear();

	for (int i = 0; i < mSpecialFishList->size(); i++)
	{
		theWidgetManager->RemoveWidget(mSpecialFishList->at(i));
		mApp->SafeDeleteWidget(mSpecialFishList->at(i));
	}
	mSpecialFishList->clear();

	RemoveWidgetHelper(mCyraxPtr);
	mCyraxPtr = nullptr;
	for (int i = 0; i < 7; i++)
	{
		RemoveWidgetHelper(mMenuButtons[i]);
		mMenuButtons[i] = 0;
	}
	m0x2dc = 0;
	m0x2e0 = 0;
	memset(mSlotUnlocked, 0, sizeof(mSlotUnlocked));	
}

void Sexy::Board::DeterminePricesAndSlots()
{
	if (!mApp->mRelaxMode)
	{
		if (mTank == 1)
		{
			mSlotPrices[SLOT_OSCAR] = 1000;
			mSlotPrices[SLOT_WEAPON] = 1000;
			mSlotNumber[SLOT_GUPPY] = 0;
			mSlotNumber[SLOT_FOODLVL] = 1;
			mSlotNumber[SLOT_FOODLIMIT] = 2;
			mSlotNumber[SLOT_OSCAR] = 3;
			mSlotNumber[SLOT_WEAPON] = 5;
			mSlotNumber[SLOT_EGG] = 6;
			mSlotPrices[SLOT_GUPPY] = 100;
			mSlotPrices[SLOT_FOODLVL] = 200;
			mSlotPrices[SLOT_FOODLIMIT] = 300;
			switch (mLevel)
			{
			case 1:
				mAlienExpect = Alien::ALIEN_NONE;
				mSlotPrices[SLOT_EGG] = 150;
				mSlotNumber[SLOT_FOODLVL] = -1;
				mSlotNumber[SLOT_FOODLIMIT] = -1;
				mSlotNumber[SLOT_OSCAR] = -1;
				mSlotNumber[SLOT_WEAPON] = -1;
				break;
			case 2:
				mAlienExpect = Alien::ALIEN_WEAK_SYLV;
				mSlotPrices[SLOT_EGG] = 500;
				mAlienTimer = 1750;
				mSlotNumber[SLOT_OSCAR] = -1;
				mSlotNumber[SLOT_WEAPON] = -1;
				break;
			case 3:
				mAlienExpect = Alien::ALIEN_STRONG_SYLV;
				mSlotPrices[SLOT_EGG] = 2000;
				break;
			case 4:	
				mAlienExpect = Alien::ALIEN_BALROG;
				mSlotPrices[SLOT_EGG] = 3000;
				break;
			case 5:	
				mAlienExpect = Alien::ALIEN_BALROG;
				mSlotPrices[SLOT_EGG] = 5000;
				break;
			}
		}
		else if (mTank == 2)
		{
			mSlotNumber[SLOT_GUPPY] = 0;
			mSlotNumber[SLOT_FOODLVL] = 1;
			mSlotNumber[SLOT_FOODLIMIT] = 2;
			mSlotNumber[SLOT_POTION] = 3;
			mSlotNumber[SLOT_STARCATCHER] = 4;
			mSlotNumber[SLOT_WEAPON] = 5;
			mSlotNumber[SLOT_EGG] = 6;
			mSlotPrices[SLOT_GUPPY] = 100;
			mSlotPrices[SLOT_FOODLVL] = 200;
			mSlotPrices[SLOT_FOODLIMIT] = 300;
			mSlotPrices[SLOT_POTION] = 250;
			mSlotPrices[SLOT_STARCATCHER] = 750;
			mSlotPrices[SLOT_WEAPON] = 1000;
			switch (mLevel)
			{
			case 1:
				mAlienExpect = Alien::ALIEN_STRONG_SYLV;
				mSlotPrices[SLOT_EGG] = 750;
				mSlotNumber[SLOT_WEAPON] = -1;
				mSlotNumber[SLOT_STARCATCHER] = -1;
				break;
			case 2:
				mAlienExpect = Alien::ALIEN_BALROG;
				mSlotPrices[SLOT_EGG] = 3000;
				break;
			case 3:
				mAlienExpect = Alien::ALIEN_GUS;
				mSlotPrices[SLOT_EGG] = 5000;
				break;
			case 4:
				mAlienExpect = Alien::ALIEN_DESTRUCTOR;
				mSlotPrices[SLOT_EGG] = 7500;
				break;
			case 5:
				mAlienExpect = mApp->mSeed->Next() % 2 + Alien::ALIEN_GUS;
				mSlotPrices[SLOT_EGG] = 10000;
				break;
			}
		}
		else if (mTank == 3)
		{
			mSlotNumber[SLOT_GUPPY] = 0;
			mSlotPrices[SLOT_GEKKO] = 2000;
			mSlotPrices[SLOT_WEAPON] = 2000;
			mSlotNumber[SLOT_FOODLVL] = 1;
			mSlotNumber[SLOT_FOODLIMIT] = 2;
			mSlotNumber[SLOT_GRUBBER] = 3;
			mSlotNumber[SLOT_GEKKO] = 4;
			mSlotNumber[SLOT_WEAPON] = 5;
			mSlotNumber[SLOT_EGG] = 6;
			mSlotPrices[SLOT_GUPPY] = 100;
			mSlotPrices[SLOT_FOODLVL] = 200;
			mSlotPrices[SLOT_FOODLIMIT] = 300;
			mSlotPrices[SLOT_GRUBBER] = 750;
			switch (mLevel)
			{
			case 1:
				mAlienExpect = Alien::ALIEN_BALROG;
				mSlotPrices[SLOT_EGG] = 1000;
				mSlotNumber[SLOT_WEAPON] = -1;
				mSlotNumber[SLOT_GEKKO] = -1;
				break;
			case 2:
				mAlienExpect = Alien::ALIEN_GUS + mApp->mSeed->Next() % 2;
				mSlotPrices[SLOT_EGG] = 5000;
				break;
			case 3:
				mAlienExpect = Alien::ALIEN_PSYCHOSQUID;
				mSlotPrices[SLOT_EGG] = 7500;
				break;
			case 4:
				mAlienExpect = Alien::ALIEN_ULYSEES;
				mSlotPrices[SLOT_EGG] = 10000;
				break;
			case 5:
				mAlienExpect = Alien::Alien::ALIEN_ULYSEES + mApp->mSeed->Next() % 2;
				mSlotPrices[SLOT_EGG] = 15000;
				break;
			}
		}
		else if (mTank == 4)
		{
			mSlotNumber[SLOT_BREEDER] = 0;
			mSlotPrices[SLOT_BREEDER] = 200;
			mSlotPrices[SLOT_FOODLVL] = 200;
			mSlotNumber[SLOT_FOODLVL] = 1;
			mSlotNumber[SLOT_FOODLIMIT] = 2;
			mSlotNumber[SLOT_OSCAR] = 3;
			mSlotNumber[SLOT_ULTRA] = 4;
			mSlotNumber[SLOT_WEAPON] = 5;
			mSlotNumber[SLOT_EGG] = 6;
			mSlotPrices[SLOT_FOODLIMIT] = 300;
			mSlotPrices[SLOT_OSCAR] = 1000;
			mSlotPrices[SLOT_ULTRA] = 10000;
			mSlotPrices[SLOT_WEAPON] = 5000;
			switch (mLevel)
			{
			case 1:
				mAlienExpect = Alien::ALIEN_BALROG;
				mSlotPrices[SLOT_EGG] = 3000;
				mSlotNumber[SLOT_WEAPON] = -1;
				mSlotNumber[SLOT_ULTRA] = -1;
				break;
			case 2:
				mAlienExpect = 8;
				mSlotPrices[SLOT_EGG] = 25000;
				break;
			case 3:
				mAlienExpect = Alien::ALIEN_GUS;
				mSlotPrices[SLOT_EGG] = 50000;
				break;
			case 4:
				mAlienExpect = 8;
				mSlotPrices[SLOT_EGG] = 75000;
				break;
			case 5:
				mAlienExpect = 8;
				mSlotPrices[SLOT_EGG] = 99999;
				break;
			}
		}
		else if (mTank == 5)
		{
			mSlotNumber[SLOT_EGG] = 6;
			mSlotPrices[SLOT_EGG] = 0;
			m0x43c = 3;
			mAlienExpect = Alien::ALIEN_CYRAX;
		}
	}
	else // Is relax mode
	{
		mSlotPrices[SLOT_GUPPY] = 100;
		mSlotPrices[SLOT_OSCAR] = 1000;
		mSlotPrices[SLOT_ULTRA] = 10000;
		mSlotPrices[SLOT_STARCATCHER] = 750;
		mSlotPrices[SLOT_GRUBBER] = 750;
		mSlotPrices[SLOT_GEKKO] = 2000;
		mSlotNumber[SLOT_EGG] = 6;
		switch (mTank)
		{
		case 1:
			mSlotPrices[SLOT_EGG] = 5000;
			mSlotNumber[SLOT_OSCAR] = 3;
			mSlotNumber[SLOT_GUPPY] = 0;
			break;
		case 2:
			mSlotPrices[SLOT_EGG] = 5000;
			mSlotNumber[SLOT_STARCATCHER] = 3;
			mSlotNumber[SLOT_GUPPY] = 0;
			break;
		case 3:
			mSlotPrices[SLOT_EGG] = 10000;
			mSlotNumber[SLOT_GRUBBER] = 3;
			mSlotNumber[SLOT_GEKKO] = 5;
			mSlotNumber[SLOT_GUPPY] = 0;
			break;
		case 4:
			mSlotPrices[SLOT_EGG] = 25000;
			mSlotNumber[SLOT_ULTRA] = 5;
			mSlotNumber[SLOT_GUPPY] = 0;
			break;
		default:
			break;
		}
		for (int i = 0; i < SLOT_END; i++)
			MakeAndUnlockMenuButton(i, true);
		RelaxModeConfig();
	}

	if (mApp->mGameMode == WinFishApp::GAMEMODE_CHALLENGE)
	{
		if (mTank == 2)
			mSlotPrices[SLOT_EGG] = 5000;
		else if (mTank == 3)
			mSlotPrices[SLOT_EGG] = 10000;
		else if (mTank == 4)
			mSlotPrices[SLOT_EGG] = 25000;
	}
	else if (mApp->mGameMode == WinFishApp::GAMEMODE_TIME_TRIAL)
	{
		mSlotPrices[SLOT_EGG] = 100;
	}
	else if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
	{
		ChangeBackground(mApp->mCurrentProfile->m0x8c + 1);
		return;
	}

	if (mTank == 1)
		ChangeBackground(1);
	else if (mTank == 2)
		ChangeBackground(2);
	else if (mTank == 3)
		ChangeBackground(4);
	else if (mTank == 4)
		ChangeBackground(5);
	else if (mTank == 5)
		ChangeBackground(6);
}

void Sexy::Board::HandleBuySlotPressed(int theSlotId)
{
	MenuButtonWidget* aBtn = GetMenuButtonById(theSlotId);
	if (!aBtn)
		return;

	int aPrice = mSlotPrices[theSlotId];

	switch (theSlotId)
	{
	case SLOT_GUPPY:
		if (IsTankAndLevelNB(1, 1) && mMoney < aPrice)
			ShowText("You can\'t afford new fish yet! Collect more money!", false, 14);

		if (!Buy(aPrice, true))
			return;

		SpawnGuppyBought();
		if (m0x4b0[2]) m0x4b0[2] = false;

		PlaySample(SOUND_GROW_ID, 3, 1.0);
		PlaySplashSound();
		break;
	case SLOT_BREEDER:
		if (!Buy(aPrice, true))
			return;
		SpawnBreederBought();
		break;
	case SLOT_FOODLVL:
		if (gFoodType < 2 && Buy(aPrice, true))
		{
			if (m0x4b0[15])
				m0x4b0[15] = false;
			gFoodType++;
			MenuButtonSetupNoVT(SLOT_FOODLVL, true);

			if (!mSlotUnlocked[SLOT_FOODLIMIT] && mTank == 1 && mLevel == 2)
			{
				MakeAndUnlockMenuButton(SLOT_FOODLIMIT, true);
				MakeAndUnlockMenuButton(SLOT_EGG, true);
				if (!mApp->mCurrentProfile->mFinishedGame)
					ShowText("Upgrade Food Quantity to drop more food at once!", false, 12);
			}

			PlaySample(SOUND_BUY_ID, 3, 1.0);
		}
		break;
	case SLOT_FOODLIMIT:
		if (gFoodLimit < 9 && Buy(aPrice, true))
		{
			if(mTank == 1 && gFoodLimit == 1 && !mApp->mCurrentProfile->mFinishedGame)
				ShowText("Hold down mouse button to Auto-Feed!", false, -1);

			gFoodLimit++;
			MenuButtonSetupNoVT(SLOT_FOODLIMIT, true);

			PlaySample(SOUND_BUY_ID, 3, 1.0);
		}
		break;
	case SLOT_OSCAR:
		if (!Buy(aPrice, true))
			return;
		SpawnOscarBought();
		if (mTank != 4)
			MakeAndUnlockMenuButton(SLOT_WEAPON, true);
		if (mTank == 1 && mLevel > 1)
		{
			MakeAndUnlockMenuButton(SLOT_ULTRA, true);
			MakeAndUnlockMenuButton(SLOT_WEAPON, true);
		}
		MakeAndUnlockMenuButton(SLOT_EGG, true);
		PlaySample(SOUND_GROW_ID, 3, 1.0);
		PlaySplashSound();
		break;
	case SLOT_POTION:
		if (!m0x440 && Buy(aPrice, true))
		{
			if (!m0x441)
			{
				m0x441 = true;
				if (mLevel < 3)
				{
					ShowText("Click on tank to drop star potion!", 0, -1);
					mMessageWidget->m0xd0 = 180;
				}
			}
			m0x440 = true;
			PlaySample(SOUND_BUY_ID, 3, 1.0);
		}
		break;
	case SLOT_STARCATCHER:
		if (!Buy(aPrice, true))
			return;
		SpawnPentaBought();
		MakeAndUnlockMenuButton(SLOT_WEAPON, true);
		MakeAndUnlockMenuButton(SLOT_EGG, true);
		PlaySample(SOUND_GROW_ID, 3, 1.0);
		PlaySplashSound();
		break;
	case SLOT_GRUBBER:
		if (!Buy(aPrice, true))
			return;
		SpawnGrubberBought();
		PlaySample(SOUND_GROW_ID, 3, 1.0);
		PlaySplashSound();
		if(mLevel != 1)
			MakeAndUnlockMenuButton(SLOT_GEKKO, true);
		else
			MakeAndUnlockMenuButton(SLOT_EGG, true);
		break;
	case SLOT_GEKKO:
		if (!Buy(aPrice, true))
			return;
		m0x4a4++;
		SpawnGekkoBought();
		MakeAndUnlockMenuButton(SLOT_WEAPON, true);
		MakeAndUnlockMenuButton(SLOT_EGG, true);
		PlaySample(SOUND_GROW_ID, 3, 1.0);
		PlaySplashSound();
		break;
	case SLOT_ULTRA:
		if (!Buy(aPrice, true))
			return;
		SpawnUltraBought();
		MakeAndUnlockMenuButton(SLOT_WEAPON, true);
		MakeAndUnlockMenuButton(SLOT_EGG, true);
		PlaySample(SOUND_GROW_ID, 3, 1.0);
		PlaySample(SOUND_SPLASHBIG_ID, 3, 1.0);
		break;
	case SLOT_WEAPON:
		if (m0x3e4 < 12 && Buy(aPrice, true))
		{
			m0x3e4++;
			MenuButtonSetupNoVT(SLOT_WEAPON, true);
			PlaySample(SOUND_BUY_ID, 3, 1.0);
		}
		break;
	case SLOT_EGG:
		HandleBuyEgg();
		break;
	}
}

void Sexy::Board::RelaxModeConfig()
{
	if (mTank == 1)
	{
		mAlienExpect = ((Rand() % 2 == 0) ? Alien::ALIEN_STRONG_SYLV : Alien::ALIEN_BALROG);
	}
	else if (mTank == 2)
	{
		int aPossibleAliens[] = { Alien::ALIEN_STRONG_SYLV, Alien::ALIEN_BALROG, Alien::ALIEN_GUS, Alien::ALIEN_DESTRUCTOR};
		mAlienExpect = aPossibleAliens[Rand() % 4];
	}
	else if (mTank == 3)
	{
		int aPossibleAliens[] = {  Alien::ALIEN_BALROG, Alien::ALIEN_GUS, Alien::ALIEN_DESTRUCTOR, \
			Alien::ALIEN_PSYCHOSQUID, Alien::ALIEN_ULYSEES};
		mAlienExpect = aPossibleAliens[Rand() % 5];
	}
	else if (mTank == 4)
	{
		int aPossibleAliens[] = { Alien::ALIEN_BALROG, Alien::ALIEN_GUS, Alien::ALIEN_DESTRUCTOR, \
			Alien::ALIEN_PSYCHOSQUID, Alien::ALIEN_ULYSEES, Alien::ALIEN_BILATERUS };
		mAlienExpect = aPossibleAliens[Rand() % 6];
	}
}

void Sexy::Board::MakeVirtualTankButtons()
{
	if (mMenuButtons[SLOT_GRUBBER])
		return;

	if (mMessageWidget)
	{
		mMessageWidget->mX = 20;
		mMessageWidget->mWidth = 600;
		if (mApp->IsScreenSaver())
			mMessageWidget->mY = 60;
	}

	mMenuButtons[SLOT_GRUBBER] = new MenuButtonWidget(mWidgetManager, 10, this, "Visit Store");
	mMenuButtons[SLOT_GRUBBER]->mButtonImage = IMAGE_MENUBTNU;
	mMenuButtons[SLOT_GRUBBER]->mOverImage = IMAGE_MENUBTNO;
	mMenuButtons[SLOT_GRUBBER]->mDownImage = IMAGE_MENUBTND2;
	mMenuButtons[SLOT_GRUBBER]->Resize(0x46, 2, 0x3a, 60);
	mWidgetManager->AddWidget(mMenuButtons[SLOT_GRUBBER]);

	mMenuButtons[SLOT_GRUBBER]->mPriceText = "STORE";
	mMenuButtons[SLOT_GRUBBER]->Configure(IMAGE_TROPHYBUTTONS, 5, 1, 0, 2);

	mMenuButtons[SLOT_GEKKO] = new MenuButtonWidget(mWidgetManager, 11, this, "Add/Remove Fish");
	mMenuButtons[SLOT_GEKKO]->mButtonImage = IMAGE_MENUBTNU;
	mMenuButtons[SLOT_GEKKO]->mOverImage = IMAGE_MENUBTNO;
	mMenuButtons[SLOT_GEKKO]->mDownImage = IMAGE_MENUBTND2;
	mMenuButtons[SLOT_GEKKO]->Resize(0x90, 2, 0x3a, 60);
	mWidgetManager->AddWidget(mMenuButtons[SLOT_GEKKO]);
	mMenuButtons[SLOT_GEKKO]->mPriceText = "FISH";
	mMenuButtons[SLOT_GEKKO]->Configure(IMAGE_TROPHYBUTTONS, 5, 1, 0, 3);

	mMenuButtons[SLOT_ULTRA] = new MenuButtonWidget(mWidgetManager, 12, this, "Add/Remove Pets");
	mMenuButtons[SLOT_ULTRA]->mButtonImage = IMAGE_MENUBTNU;
	mMenuButtons[SLOT_ULTRA]->mOverImage = IMAGE_MENUBTNO;
	mMenuButtons[SLOT_ULTRA]->mDownImage = IMAGE_MENUBTND2;
	mMenuButtons[SLOT_ULTRA]->Resize(0xd8, 2, 0x3a, 60);
	mWidgetManager->AddWidget(mMenuButtons[SLOT_ULTRA]);
	mMenuButtons[SLOT_ULTRA]->mPriceText = "PETS";
	mMenuButtons[SLOT_ULTRA]->Configure(IMAGE_TROPHYBUTTONS, 5, 1, 0, 4);

	mMenuButtons[SLOT_WEAPON] = new MenuButtonWidget(mWidgetManager, 13, this, "Tank Options");
	mMenuButtons[SLOT_WEAPON]->mButtonImage = IMAGE_MENUBTNU;
	mMenuButtons[SLOT_WEAPON]->mOverImage = IMAGE_MENUBTNO;
	mMenuButtons[SLOT_WEAPON]->mDownImage = IMAGE_MENUBTND2;
	mMenuButtons[SLOT_WEAPON]->Resize(0x16c, 2, 0x3a, 60);
	mWidgetManager->AddWidget(mMenuButtons[SLOT_WEAPON]);
	mMenuButtons[SLOT_WEAPON]->mPriceText = "TANK";
	mMenuButtons[SLOT_WEAPON]->Configure(IMAGE_TROPHYBUTTONS, 5, 1, 0, 5);

	mMenuButtons[SLOT_EGG] = new MenuButtonWidget(mWidgetManager, 14, this, "Special Food");
	mMenuButtons[SLOT_EGG]->mButtonImage = IMAGE_MENUBTNU;
	mMenuButtons[SLOT_EGG]->mOverImage = IMAGE_MENUBTNO;
	mMenuButtons[SLOT_EGG]->mDownImage = IMAGE_MENUBTND2;
	mMenuButtons[SLOT_EGG]->Resize(0x122, 2, 0x3a, 60);
	mWidgetManager->AddWidget(mMenuButtons[SLOT_EGG]);
	mMenuButtons[SLOT_EGG]->mPriceText = "FEED";
	mMenuButtons[SLOT_EGG]->Configure(IMAGE_TROPHYBUTTONS, 5, 1, 0, 1);
	
	mBackButton = new MenuButtonWidget(mWidgetManager, 15, this, "Back to Main Menu");
	mBackButton->mButtonImage = IMAGE_MENUBTNU;
	mBackButton->mOverImage = IMAGE_MENUBTNO;
	mBackButton->mDownImage = IMAGE_MENUBTND2;
	mBackButton->Resize(0x1b6, 2, 0x3a, 60);
	mWidgetManager->AddWidget(mBackButton);
	mBackButton->mPriceText = "BACK";
	mBackButton->Configure(IMAGE_TROPHYBUTTONS, 5, 1, 0, 0);
}

void Sexy::Board::StartVirtualTank()
{
	mAlienExpect = 0;
	mPause = false;
	mShouldSave = true;

	gFoodType = 0;
	gFoodLimit = 1;
	gMerylActive = false;
	gWadsworthTimer = 0;
	gUnkInt02 = 0;

	m0x440 = false;
	mGameUpdateCnt = 0;
	m0x450 = 0;
	m0x454 = 0;
	m0x458 = 0;
	m0x45c = 0;

	int aVal = Unk01();
	m0x3b4 = aVal;
	m0x3b8 = aVal;

	m0x4fc = true;
	SetAlienExpectVT();
	WidgetSetupVT();
	DoVirtualTankDialog();
}

void Sexy::Board::SetAlienExpectVT()
{
	int aPossibleAliens[7] = {2,3,4,5,7,6,8};
	mAlienExpect = aPossibleAliens[Rand() % 7];
	DetermineAlienSpawnCoordsVT();
}

void Sexy::Board::WidgetSetupVT()
{
	if (mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)
		return;

	m0x3e4 = 8;

	int fishListFreeCapacity = mFishList->size();
	int breederListFreeCapacity = mBreederList->size(); // May be incorrect
	gFoodLimit = fishListFreeCapacity + breederListFreeCapacity;
	if (gFoodLimit < 5)
		gFoodLimit = 5;
	else if (gFoodLimit > 9)
		gFoodLimit = 9;

	if (mApp->mCurrentProfile->mBubbulatorBought == 0)
		mBubbulatorShown = false;
	if (mApp->mCurrentProfile->mAlienAttractorBought == 0)
		mAlienAttractorShown = false;

	if (mMenuButtons[SLOT_EGG])
	{
		bool aVisible = false;
		int foods[8] = {0};
		GetExoticFoodsRequiredInTank(foods);
		for (int i = 0; i < 8; i++)
			if (foods[i] > 0)
				aVisible = true;
		mMenuButtons[SLOT_EGG]->SetVisible(aVisible);
	}
	if (mMenuButtons[SLOT_GEKKO])
	{
		bool aVisible = false;
		for (GameObjectSet::iterator it = mGameObjectSet.begin(); it != mGameObjectSet.end(); ++it)
		{
			GameObject* anObj = *it;
			if (anObj->mVirtualTankId > -1 && anObj->mVirtualTankId < 108)
			{
				aVisible = true;
				break;
			}
		}
		mMenuButtons[SLOT_GEKKO]->SetVisible(aVisible);
	}

	if (mMenuButtons[SLOT_ULTRA])
		mMenuButtons[SLOT_ULTRA]->SetVisible(mApp->mCurrentProfile->mNumOfUnlockedPets > 0);

	if (mApp->IsScreenSaver())
	{
		mMenuButtons[SLOT_GRUBBER]->SetVisible(false);
		mMenuButtons[SLOT_GEKKO]->SetVisible(false);
		mMenuButtons[SLOT_ULTRA]->SetVisible(false);
		mMenuButtons[SLOT_WEAPON]->SetVisible(false);
		mMenuButtons[SLOT_EGG]->SetVisible(false);
		mBackButton->SetVisible(false);
		mMoneyLabel->SetVisible(false);
		mMenuButton->SetVisible(false);
	}
}

void Sexy::Board::ChangeBackground(int theBGId)
{
	if (theBGId < 1)
		theBGId = 1;
	else if (theBGId > 6)
		theBGId = 6;

	mCurrentBackgroundId = theBGId;
	Unk03();
}

bool Sexy::Board::DoCheatCode(int theCheatCode)
{
	bool aStatus = mApp->mCurrentProfile->ToggleCheatFlag(theCheatCode);
	if (theCheatCode == CC_WAVY)
		ShowText(aStatus ? "Wavy Mode Enabled" : "Wavy Mode Disabled", false, -1);
	else if (theCheatCode == CC_PREGO)
		ShowText(aStatus ? "Original Prego Sound Enabled" : "Original Prego Sound Disabled", false, -1);
	else if (theCheatCode == CC_BETATEST)
		ShowText(aStatus ? "Original Breeder Sound Enabled" : "Original Breeder Sound Disabled", false, -1);
	else if (theCheatCode == CC_SUPERMEGA)
		ShowText(aStatus ? "Ultra Prego Sound Enabled" : "Ultra Prego Sound Disabled", false, -1);
	else if (theCheatCode == CC_VOID)
	{
		mApp->mCurrentProfile->SetCheatFlag(CC_SPACE, false);
		ApplyShadowsIf3D();
		Unk03();
		ShowText(aStatus ? "Void Mode Enabled" : "Void Mode Disabled", false, -1);
	}
	else if (theCheatCode == CC_SPACE)
	{
		mApp->mCurrentProfile->SetCheatFlag(CC_VOID, false);
		ApplyShadowsIf3D();
		Unk03();
		ShowText(aStatus ? "Space Mode Enabled" : "Space Mode Disabled", false, -1);
		if (aStatus)
			mStarField->Init(1000);
	}
	else if (theCheatCode == CC_ZOMBIE)
	{
		ShowText(aStatus ? "Zombie Mode Enabled" : "Zombie Mode Disabled", false, -1);
		gZombieMode = aStatus;
	}
	return true;
}

GameObject* Sexy::Board::GetPrestoPet()
{
	GameObject* aPet = nullptr;
	for (int i = 0; i < mFishTypePetList->size(); i++)
	{
		FishTypePet* aFPet = mFishTypePetList->at(i);

		if (aFPet->m0x230)
			return aFPet;
	}

	for (int i = 0; i < mOtherTypePetList->size(); i++)
	{
		OtherTypePet* aOPet = mOtherTypePetList->at(i);

		if (aOPet->mIsPresto)
			return aOPet;
	}

	return nullptr;
}

void Sexy::Board::ApplyShadowsIf3D()
{
	bool isVisible = false;
	if (mApp->Is3DAccelerated() || ((mApp->mCurrentProfile->mCheatCodeFlags >> 2) & 1) != 0)
		isVisible = true;
	if(((mApp->mCurrentProfile->mCheatCodeFlags >> 3) & 1) != 0)
		isVisible = false;

	for (int i = 0; i < mShadowList->size(); i++)
	{
		Shadow* aShad = mShadowList->at(i);
		aShad->mVisible = isVisible;
	}
}

void Sexy::Board::UpdateNikoPosition()
{
	for (int i = 0; i < mOtherTypePetList->size(); i++)
	{
		OtherTypePet* aPet = mOtherTypePetList->at(i);

		aPet->UpdateNikoPosition(mCurrentBackgroundId);
	}
}

MenuButtonWidget* Sexy::Board::MakeAndUnlockMenuButton(int theBtnId, bool flag)
{
	const int aBtnXCoords[] = { 18, 87, 144, 217, 290, 363, 436 };
	int aBtnSlot = mSlotNumber[theBtnId];

	if (aBtnSlot < 0)
		return nullptr;

	SexyString aToolTipStr = "";
	switch (theBtnId)
	{
	case 0:
		aToolTipStr = "buy guppy";
		break;
	case 1:
		aToolTipStr = "buy breeder";
		break;
	case 2:
		aToolTipStr = "upgrade food quality";
		break;
	case 3:
		aToolTipStr = "upgrade food quantity";
		break;
	case 4:
		aToolTipStr = "buy carnivore";
		break;
	case 5:
		aToolTipStr = "buy star potion";
		break;
	case 6:
		aToolTipStr = "buy starcatcher";
		break;
	case 7:
		aToolTipStr = "buy guppycruncher";
		break;
	case 8:
		aToolTipStr = "buy beetlemuncher";
		break;
	case 9:
		aToolTipStr = "buy ultravore";
		break;
	case 10:
		aToolTipStr = "upgrade weapon";
		break;
	case 11:
		if (mApp->mGameMode == WinFishApp::GAMEMODE_TIME_TRIAL)
			aToolTipStr = "buy random pet";
		else if (mApp->mGameMode == WinFishApp::GAMEMODE_SANDBOX)
			aToolTipStr = "end level";
		else
			aToolTipStr = "buy egg piece";
		break;
	}

	if (mMenuButtons[aBtnSlot] == nullptr)
	{
		mMenuButtons[aBtnSlot] = new MenuButtonWidget(mWidgetManager, theBtnId, this, aToolTipStr);

		mWidgetManager->AddWidget(mMenuButtons[aBtnSlot]);
		mMenuButtons[aBtnSlot]->mButtonImage = IMAGE_MENUBTNU;
		mMenuButtons[aBtnSlot]->mDownImage = IMAGE_MENUBTND2;
		mMenuButtons[aBtnSlot]->mOverImage = IMAGE_MENUBTNO;
		mMenuButtons[aBtnSlot]->Resize(aBtnXCoords[aBtnSlot], 3, 58, 60);
		MenuButtonSetupNoVT(theBtnId, flag);
		mSlotUnlocked[theBtnId] = true;
		m0x2e0++;
	}

	return mMenuButtons[aBtnSlot];
}

void Sexy::Board::MenuButtonSetupNoVT(int theBtnId, bool flag)
{
	MenuButtonWidget* aBtn = GetMenuButtonById(theBtnId);
	if (!aBtn)
		return;

	aBtn->SetSlotPrice(mSlotPrices[theBtnId]);
	Image* aSlotImg = nullptr;
	int aSlotImgX = 0;
	int aSlotImgY = 0;
	int aSlotImgRow = 0;
	int aSlotImgCol = 0;
	bool dontConfigure = false;
	switch (theBtnId)
	{
	case 0:
		aSlotImg = IMAGE_SMALLSWIM;
		aSlotImgX = -12;
		aSlotImgY = -18;
		aSlotImgRow = 0;
		aSlotImgCol = -1;
		break;
	case 1:
		aSlotImg = IMAGE_SCL_BREEDER;
		aSlotImgX = 8;
		aSlotImgY = 2;
		aSlotImgRow = 0;
		aSlotImgCol = -1;
		break;
	case 2:
		if (gFoodType == 0)
		{
			aSlotImg = IMAGE_FOOD;
			aSlotImgX = 9;
			aSlotImgY = 3;
			aSlotImgRow = 1;
			aSlotImgCol = 0;
		}
		else if (gFoodType != 1)
		{
			aBtn->SetMaxedOut();
			dontConfigure = true;
		}
		else
		{
			aSlotImg = IMAGE_FOOD;
			aSlotImgX = 9;
			aSlotImgY = 3;
			aSlotImgRow = 2;
			aSlotImgCol = 0;
		}
		break;
	case 3:
		if (gFoodLimit < 9)
			aBtn->m0x15c = StrFormat("%d", gFoodLimit+1);
		else
			aBtn->SetMaxedOut();
		dontConfigure = true;
		break;
	case 4:
		aSlotImg = IMAGE_SCL_OSCAR;
		aSlotImgX = 8;
		aSlotImgY = 3;
		aSlotImgRow = 0;
		aSlotImgCol = -1;
		break;
	case 5:
		aSlotImg = IMAGE_FOOD;
		aSlotImgX = 10;
		aSlotImgY = 1;
		aSlotImgRow = 3;
		aSlotImgCol = 0;
		break;
	case 6:
		aSlotImg = IMAGE_SCL_STARCATCHER;
		aSlotImgX = 8;
		aSlotImgY = 2;
		aSlotImgRow = 0;
		aSlotImgCol = -1;
		break;
	case 7:
		aSlotImg = IMAGE_SCL_GRUBBER;
		aSlotImgX = 8;
		aSlotImgY = 1;
		aSlotImgRow = 0;
		aSlotImgCol = -1;
		break;
	case 8:
		aSlotImg = IMAGE_SCL_GEKKO;
		aSlotImgX = 10;
		aSlotImgY = 2;
		aSlotImgRow = 0;
		aSlotImgCol = -1;
		break;
	case 9:
		aSlotImg = IMAGE_SCL_ULTRA;
		aSlotImgX = 10;
		aSlotImgY = 2;
		aSlotImgRow = 0;
		aSlotImgCol = -1;
		break;
	case 10:
		aBtn->Configure(IMAGE_LASERUPGRADES, 7, 3, 0, m0x3e4 - 2);
		if (m0x3e4 > 11)
		{
			if (flag)
			{
				ShowText("Hold down mouse button for Rapid Fire!", false, -1);
			}
			aBtn->SetMaxedOut();
		}
		dontConfigure = true;
		break;
	case 11:
		if (mApp->mGameMode == WinFishApp::GAMEMODE_TIME_TRIAL)
		{
			aBtn->Configure(IMAGE_EGGPIECES, 7, 3, 0, 2);
			aBtn->m0x15c = "?";
			int i;
			for (i = 0; i < 24; i++)
			{
				if (mPetsInTank[i] == 0 && i != 19 && mApp->mCurrentProfile->IsPetUnlocked(i))
					break;
			}
			dontConfigure = true;
			if (i != 24)
				break;
			aBtn->SetMaxedOut();
			break;
		}
		aSlotImg = IMAGE_EGGPIECES;
		if (mApp->mGameMode == WinFishApp::GAMEMODE_SANDBOX)
		{
			aSlotImgCol = 2;
			aSlotImgRow = 0;
			aSlotImgX = 7;
			aSlotImgY = 3;
		}
		else
		{
			if (m0x43c < 1 || m0x43c > 3) { dontConfigure; break; }
			aSlotImgCol = m0x43c - 1;
			aSlotImgRow = 0;
			aSlotImgY = 3;
			aSlotImgX = 7;
		}
		break;
	}
	if (!dontConfigure)
		aBtn->Configure(aSlotImg, aSlotImgX, aSlotImgY, aSlotImgRow, aSlotImgCol);
	if (flag)
		aBtn->m0x13c = 8;
}

MenuButtonWidget* Sexy::Board::GetMenuButtonById(int theBtnId)
{
	int aSlotId = mSlotNumber[theBtnId];
	if (aSlotId < 0)
		return nullptr;
	return mMenuButtons[aSlotId];
}

void Sexy::Board::PlaySample(int theSoundId, int theUpdatesBeforePlayingAgain, double theVolume)
{
	int theIdx = theSoundId - 262;
	if (theIdx < 64 && theUpdatesBeforePlayingAgain <= mUpdateCnt - mSoundPlayedTimerArray[theIdx])
	{
		mSoundPlayedTimerArray[theIdx] = mUpdateCnt;
		int aSound = GetSoundById(theSoundId);
		SoundInstance* anInstance = mApp->mSoundManager->GetSoundInstance(aSound);
		if (anInstance)
		{
			anInstance->SetVolume(theVolume);
			anInstance->Play(false, true);
		}
	}
}

void Sexy::Board::GetBubbleSpawnCoords(int& theX, int& theY)
{
	switch (mCurrentBackgroundId)
	{
	case 1:
		theX = 470;
		theY = 300;
		return;
	case 2:
	case 3:
		theX = 550;
		theY = 315;
		return;
	case 4:
		theX = 10;
		theY = 325;
		return;
	case 5:
		theX = 540;
		theY = 315;
		return;
	case 6:
		theX = 360;
		theY = 315;
		return;
	default:
		theX = 480;
		theY = 300;
		return;
	}
}

void Sexy::Board::GetAlienAttractorSpawnCoords(int& theX, int& theY)
{
	switch (mCurrentBackgroundId)
	{
	case 1:
		theX = 267;
		theY = 320;
		return;
	case 2:
		theX = 255;
		theY = 310;
		return;
	case 3:
		theX = 260;
		theY = 310;
		return;
	case 4:
		theX = 270;
		theY = 345;
		return;
	case 5:
		theX = 292;
		theY = 320;
		return;
	case 6:
		theX = 210;
		theY = 352;
		return;
	default:
		theX = 480;
		theY = 300;
		return;
	}
}

void Sexy::Board::UpdateMoneyLabelText()
{
	if (!mMoneyLabel)
		return;

	SexyString aStr = StrFormat("%d", mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK ? mApp->mCurrentProfile->mShells : mMoney);
	mMoneyLabel->SetLabel(aStr);
}

bool Sexy::Board::Buy(int theCost, bool playBuzzer)
{
	if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
		return true;

	if (mApp->mRelaxMode && mMoney < 5)
		mMoney = 5;

	if (mMoney < theCost)
	{
		int aVal = Unk10();
		if (mMoney + aVal < theCost)
		{
			if (playBuzzer)
			{
				PlaySample(SOUND_BUZZER_ID, 3, 1.0);
				m0x450 = 70;
			}
			else if(m0x450 == 0)
				m0x450 = 70;

			if (mMoney == 0 && IsFirstLevel())
				mApp->DoDialogUnkF(WinFishApp::DIALOG_OUT_OF_MONEY_LOAN, true, "OUT OF MONEY!", "You\'ve run out of money!  Because you\'re new, we\'ll float you a loan, but be careful next time!", "Click to Continue", Dialog::BUTTONS_FOOTER);

			return false;
		}
	}

	mMoney -= theCost;
	if (mApp->mRelaxMode && mMoney < 5 && mMoney > -1)
		mMoney = 5;
	UpdateMoneyLabelText();

	return true;
}

void Sexy::Board::DropFood(int theX, int theY, int unk1, bool unk2, int theFoodCantEatTimer, int theFoodTypeOverride)
{
	if (unk1 == 0)
	{
		int aCnt = 0;
		for (int i = 0; i < mFoodList->size(); i++)
		{
			if (mFoodList->at(i)->m0x18c == 0 && mFoodList->at(i)->mExoticFoodType != 0)
				aCnt++;
		}
		if (gFoodLimit + aCnt <= mFoodList->size())
		{
			if (mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)
			{
				if (!m0x440 && !AliensInTank())
						mMoney += m0x4ac;
				if (mMoney > 9999999)
					mMoney = 9999999;
				UpdateMoneyLabelText();
			}
			if (IsTankAndLevelNB(1, 1) && mMessageWidget->m0xd0 <= 0)
				ShowText("You can only drop 1 food pellet at a time for now", false, 6);
			else if(IsTankAndLevelNB(1, 2) && mMessageWidget->m0xd0 <= 0 && mSlotUnlocked[3])
				ShowText("Upgrade Food Quantity to drop more food at once!", false, 13);
			return;
		}
	}

	Food* aFood = new Food(theX, theY, unk1, unk2, 0);
	aFood->mFoodType = gFoodType;
	if (unk1 != 0)
		aFood->mFoodType = 1;

	if (unk1 == 0)
	{
		if (!m0x440)
		{
			PlaySample(SOUND_DROPFOOD_ID, 3, 1.0);
		}
		else if (unk2)
		{
			PlaySample(SOUND_DROPFOOD_ID, 3, 1.0);
		}
		else
		{
			aFood->mFoodType = 3;
			m0x440 = false;
		}
	}

	if(theFoodTypeOverride != -1)
		aFood->mFoodType = theFoodTypeOverride;
	if(theFoodCantEatTimer > -1)
		aFood->mCantEatTimer = theFoodCantEatTimer;

	AddGameObject(aFood);
	mWidgetManager->AddWidget(aFood);
	SortGameObjects();
}

void Sexy::Board::NostradamusDropFood(int theX, int theY)
{
	Food* aFood = new Food(theX, theY, 0, 0, 2);
	AddGameObject(aFood);
	mWidgetManager->AddWidget(aFood);
	SortGameObjects();
}

void Sexy::Board::DropCoin(int theX, int theY, int theType, OtherTypePet* thePet, double theSpeed, int theUpdateCnt)
{
	Coin* aCoin = new Coin(theX, theY, theType, thePet, theSpeed);
	aCoin->mUpdateCnt = theUpdateCnt;
	AddGameObject(aCoin);
	mWidgetManager->AddWidget(aCoin);
	SortGameObjects();
}

bool Sexy::Board::IsTankAndLevelNB(int theTank, int theLevel)
{
	return mTank == theTank && mLevel == theLevel && !mApp->mCurrentProfile->mFinishedGame;
}

void Sexy::Board::CheckMouseDown(int theX, int theY)
{
	bool isSomething = Unk08(theX, theY);
	if (isSomething)
		return;
	mApp->PlaySample(SOUND_TAPGLASS);
	if (mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)
		return;

	for (int i = 0; i < mFishList->size(); i++)
	{
		Fish* aFish = mFishList->at(i);
		aFish->ClickedBehavior(theX, theY);
	}

	for (int i = 0; i < mOscarList->size(); i++)
	{
		Oscar* anOscar = mOscarList->at(i);
		anOscar->ClickedBehavior(theX, theY);
	}

	for (int i = 0; i < mGekkoList->size(); i++)
	{
		Gekko* aGekko = mGekkoList->at(i);
		aGekko->ClickedBehavior(theX, theY);
	}

	for (auto it = mGameObjectSet.begin(); it != mGameObjectSet.end(); ++it)
	{
		GameObject* aCurObj = *it;
		if (aCurObj->mSongId != -1 && aCurObj->Contains(theX, theY))
			mFishSongMgr->ClearSongs();
	}

}

void Sexy::Board::UpdateSlotPrice(int theSlotId, int thePrice)
{
	if (thePrice > 99999)
		thePrice = 99999;
	mSlotPrices[theSlotId] = thePrice;
	MenuButtonWidget* aBtn = GetMenuButtonById(theSlotId);
	if (aBtn) aBtn->SetSlotPrice(thePrice);
}

void Sexy::Board::ResetMessageWidget(int unk)
{
	if (mMessageWidget && mMessageWidget->m0xd8 == unk)
		mMessageWidget->m0xd0 = 0;
}

void Sexy::Board::MakeNote(int theX, int theY, int unk, const SexyString& theText)
{
	Coin* aCoin = new Coin(theX, theY, Coin::COIN_NOTE, nullptr, -1.0);
	aCoin->m0x1a0 = unk;
	aCoin->m0x1a4 = theText;
	AddGameObject(aCoin);
	mWidgetManager->AddWidget(aCoin);
	SortGameObjects();
}

int Sexy::Board::PlayFishSong(int theObjType, int theSpecObjType)
{
	FishSongMgr* aMgr = mFishSongMgr;

	if (aMgr->mSongList.size() != 0)
		return -1;

	int aSongId = aMgr->mCounter;
	aMgr->mCounter++;

	FishSongData* aSongData = GetSongData(theSpecObjType);

	if (aSongData == nullptr)
		return -1;
		
	SexyString aTitle = "";
	bool titleFound = aSongData->GetProperty("title", &aTitle);

	if (titleFound)
	{
		if (aSongData->GetProperty("long", nullptr))
			aTitle.append(" (extended)");

		ShowText(aTitle, false, -1);
		mMessageWidget->m0xd0 = 180;
	}

	gUnkBool02 = true;

	FishSong* aSong = aMgr->PlayFishSong(aSongData, aSongId);

	if (aSong != nullptr)
	{
		if (theSpecObjType != 4 && theSpecObjType != 5)
		{
			bool shouldAddApplause = aSongData->GetProperty("long", nullptr);

			if (shouldAddApplause)
				aSong->mApplause = SOUND_APPLAUSE;
		}
	}

	return aSongId;
}

bool Sexy::Board::IsSongPlaying(int theSongId)
{
	return mFishSongMgr->IsSongInList(theSongId);
}

void Sexy::Board::FishSongMgrUpdate()
{
	mFishSongMgr->Update();
}

void Sexy::Board::StopFishSong(int theSongId)
{
	if (mFishSongMgr)
		mFishSongMgr->StopFishSong(theSongId);
}

void Sexy::Board::DoVirtualTankDialog()
{
	if (!mApp->IsScreenSaver() && mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
	{
		UpdateHasVirtualTankFish();
		if (!mHasVirtualTankFish)
			mApp->DoVirtualDialog();
	}
}

void Sexy::Board::GetExoticFoodsRequiredInTank(int* theInfoArray)
{
	memset(theInfoArray, 0, 8 * sizeof(int));
	for (GameObjectSet::iterator it = mGameObjectSet.begin(); it != mGameObjectSet.end(); ++it)
	{
		GameObject* anObj = *it;

		if (anObj->mShown)
		{
			int anExDiet = anObj->mExoticDietFoodType;
			if (anExDiet == 0)
				anObj->CountRequiredFood(theInfoArray);
			else if (anExDiet < Food::EXO_FOOD_OBJECTS_START)
			{
				if (anExDiet == Food::EXO_FOOD_PIZZA)
					theInfoArray[6]++;
				else if (anExDiet == Food::EXO_FOOD_ICE_CREAM)
					theInfoArray[7]++;
				else if (anExDiet == Food::EXO_FOOD_CHICKEN)
					theInfoArray[5]++;
			}
			else if (anExDiet == Food::EXO_FOOD_GUPPY)
				theInfoArray[0]++;
			else if (anExDiet == Food::EXO_FOOD_OSCAR)
				theInfoArray[3]++;
			else if(anExDiet == Food::EXO_FOOD_ULTRA)
				theInfoArray[4]++;
		}
	}
}

void Sexy::Board::GetExoticFoodsInTank(int* theInfoArray)
{
	for (int i = 0; i < mFishList->size(); i++)
	{
		Fish* anObj = mFishList->at(i);
		if (anObj->mVirtualTankId < 0 && anObj->mSize == GameObject::TYPE_GUPPY)
			theInfoArray[0]++;
	}
	for (int i = 0; i < mOscarList->size(); i++)
	{
		Oscar* anObj = mOscarList->at(i);
		if (anObj->mVirtualTankId < 0)
			theInfoArray[3]++;
	}
	for (int i = 0; i < mUltraList->size(); i++)
	{
		Ultra* anObj = mUltraList->at(i);
		if (anObj->mVirtualTankId < 0)
			theInfoArray[4]++;
	}
	for (int i = 0; i < mCoinList1->size(); i++)
	{
		Coin* anObj = mCoinList1->at(i);
		if (anObj->mCoinType == 3)
			theInfoArray[1]++;
	}

	theInfoArray[2] = mLarvaList->size();

	for (int i = 0; i < mFoodList->size(); i++)
	{
		Food* anObj = mFoodList->at(i);
		if (anObj->mExoticFoodType == 3)
			theInfoArray[6]++;
		else if (anObj->mExoticFoodType == 4)
			theInfoArray[7]++;
		else if (anObj->mExoticFoodType == 5)
			theInfoArray[5]++;
	}
}

GameObject* Sexy::Board::GetGameObjectByVirtualId(int theId)
{
	for (GameObjectSet::iterator it = mGameObjectSet.begin(); it != mGameObjectSet.end(); ++it)
	{
		GameObject* anObj = *it;
		if (anObj->mVirtualTankId == theId)
			return anObj;
	}
	return nullptr;
}

void Sexy::Board::HideObject(GameObject* theObj, bool hide)
{
	if (theObj->mType == GameObject::TYPE_BREEDER)
	{
		GameObject* anObj = GetGameObjectByVirtualId(100 + theObj->mVirtualTankId);
		if (anObj)
			HideObject(anObj, hide);
	}
	if (theObj->mShown != hide)
	{
		if (hide)
		{
			int anErased = mGameObjectSet.erase(theObj);
			AddGameObject(theObj);
			mWidgetManager->AddWidget(theObj);
			MakeShadowForGameObject(theObj);
			theObj->mShown = true;
			m0x2a7 = true;
		}
		else
		{
			if (theObj->mSongId != -1)
				StopFishSong(theObj->mSongId);
			theObj->RemoveHelper02(false);
			mGameObjectSet.insert(theObj);
			theObj->mShown = false;
			m0x2a7 = true;
		}
	}
}

Fish* Sexy::Board::SpawnGuppy(int theX, int theY)
{
	Fish* aFish = new Fish(theX, theY);
	AddGameObject(aFish);
	mApp->mWidgetManager->AddWidget(aFish);
	if (mMaxFishCountEver < mFishList->size())
		mMaxFishCountEver = mFishList->size();
	MakeShadowForGameObject(aFish);
	SortGameObjects();
	return aFish;
}

GameObject* Sexy::Board::SpawnGuppyBought()
{
	Fish* aFish = new Fish(mApp->mSeed->Next() % 520 + 20, mApp->mSeed->Next() % 265 + 105);
	aFish->mVY = mApp->mSeed->Next() % 5 + 18;
	aFish->mYD = 30;
	aFish->mY = 30;
	aFish->mBoughtTimer = mApp->mSeed->Next() % 10 + 45;
	AddGameObject(aFish);
	mWidgetManager->AddWidget(aFish);
	if (mMaxFishCountEver < mFishList->size())
		mMaxFishCountEver = mFishList->size();
	MakeShadowForGameObject(aFish);
	SortGameObjects();
	return aFish;
}

GameObject* Sexy::Board::SpawnStarGuppyBought()
{
	Fish* aFish = new Fish(mApp->mSeed->Next() % 520 + 20, mApp->mSeed->Next() % 265 + 105);
	aFish->mVY = mApp->mSeed->Next() % 5 + 18;
	aFish->mYD = 30;
	aFish->mY = 30;
	aFish->mBoughtTimer = mApp->mSeed->Next() % 10 + 45;
	AddGameObject(aFish);
	mWidgetManager->AddWidget(aFish);
	if (mMaxFishCountEver < mFishList->size())
		mMaxFishCountEver = mFishList->size();
	aFish->mSize = GameObject::SIZE_STAR;
	MakeShadowForGameObject(aFish);
	SortGameObjects();
	return aFish;
}

GameObject* Sexy::Board::SpawnBreeder(int theX, int theY)
{
	Breeder* aBreeder = new Breeder(theX, theY);
	AddGameObject(aBreeder);
	mApp->mWidgetManager->AddWidget(aBreeder);
	MakeShadowForGameObject(aBreeder);
	SortGameObjects();
	return aBreeder;
}

GameObject* Sexy::Board::SpawnBreederBought()
{
	Breeder* aBreeder = new Breeder(mApp->mSeed->Next() % 520 + 20, mApp->mSeed->Next() % 265 + 105);
	aBreeder->mVY = mApp->mSeed->Next() % 5 + 18;
	aBreeder->mYD = 30;
	aBreeder->mY = 30;
	aBreeder->mBoughtTimer = mApp->mSeed->Next() % 10 + 45;
	AddGameObject(aBreeder);
	mWidgetManager->AddWidget(aBreeder);
	MakeShadowForGameObject(aBreeder);
	SortGameObjects();
	return aBreeder;
}

GameObject* Sexy::Board::SpawnOscarBought()
{
	Oscar* anOscar = new Oscar(mApp->mSeed->Next() % 520 + 20, mApp->mSeed->Next() % 265 + 105);
	anOscar->mVY = mApp->mSeed->Next() % 5 + 23;
	anOscar->mYD = 40;
	anOscar->mY = 40;
	anOscar->mBoughtTimer = mApp->mSeed->Next() % 10 + 45;
	AddGameObject(anOscar);
	mWidgetManager->AddWidget(anOscar);
	if (mMaxOscarCountEver < mOscarList->size())
		mMaxOscarCountEver = mOscarList->size();
	MakeShadowForGameObject(anOscar);
	SortGameObjects();
	return anOscar;
}

GameObject* Sexy::Board::SpawnUltraBought()
{
	Ultra* anUltra = new Ultra(mApp->mSeed->Next() % 520 + 20, mApp->mSeed->Next() % 265 + 105);
	anUltra->mVY = mApp->mSeed->Next() % 5 + 25;
	anUltra->mYD = 40;
	anUltra->mY = 40;
	anUltra->mBoughtTimer = mApp->mSeed->Next() % 10 + 45;
	AddGameObject(anUltra);
	mWidgetManager->AddWidget(anUltra);
	if (mMaxUltraCountEver < mUltraList->size())
		mMaxUltraCountEver = mUltraList->size();
	MakeShadowForGameObject(anUltra);
	SortGameObjects();
	return anUltra;
}

GameObject* Sexy::Board::SpawnGekkoBought()
{
	Gekko* aGekko = new Gekko(mApp->mSeed->Next() % 520 + 20, mApp->mSeed->Next() % 265 + 105);
	aGekko->mVY = mApp->mSeed->Next() % 5 + 23;
	aGekko->mYD = 40;
	aGekko->mY = 40;
	aGekko->mBoughtTimer = mApp->mSeed->Next() % 10 + 45;
	AddGameObject(aGekko);
	mWidgetManager->AddWidget(aGekko);
	MakeShadowForGameObject(aGekko);
	SortGameObjects();
	return aGekko;
}

GameObject* Sexy::Board::SpawnPentaBought()
{
	Penta* aPenta = new Penta(mApp->mSeed->Next() % 520 + 20);
	aPenta->mYD = 65;
	aPenta->mY = 65;
	AddGameObject(aPenta);
	mWidgetManager->AddWidget(aPenta);
	if (mMaxPentaCountEver < mPentaList->size())
		mMaxPentaCountEver = mPentaList->size();
	MakeShadowForGameObject(aPenta);
	SortGameObjects();
	return aPenta;
}

GameObject* Sexy::Board::SpawnGrubberBought()
{
	Grubber* aGrubber = new Grubber(mApp->mSeed->Next() % 520 + 20);
	aGrubber->mYD = 65;
	aGrubber->mY = 65;
	AddGameObject(aGrubber);
	mWidgetManager->AddWidget(aGrubber);
	MakeShadowForGameObject(aGrubber);
	SortGameObjects();
	return aGrubber;
}

GameObject* Sexy::Board::SpawnPet(int thePetType, int theX, int theY, bool flag1, bool flag2)
{
	if (thePetType == GameObject::PET_PRESTO)
		flag1 = true;
	if (theX == -1)
	{
		theX = mApp->mSeed->Next() % 265 + 105;
		theY = mApp->mSeed->Next() % 520 + 20;
	}
	GameObject* aPet = nullptr;
	if (thePetType == GameObject::PET_STINKY || thePetType == GameObject::PET_NIKO ||
		thePetType == GameObject::PET_RUFUS || thePetType == GameObject::PET_CLYDE || thePetType == GameObject::PET_RHUBARB)
		aPet = new OtherTypePet(theX, theY, thePetType, mCurrentBackgroundId, flag1);
	else
		aPet = new FishTypePet(theX, theY, thePetType, flag1);
	if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK && !flag2)
	{
		aPet->mVirtualTankId = flag1 ? GameObject::PET_PRESTO : thePetType + 1000;
	}
	AddGameObject(aPet);
	mWidgetManager->AddWidget(aPet);
	MakeShadowForGameObject(aPet);
	SortGameObjects();
	return aPet;
}

GameObject* Sexy::Board::SpawnLarva(int theX, int theY)
{
	Larva* aLarva = new Larva(theX, theY);
	AddGameObject(aLarva);
	mWidgetManager->AddWidget(aLarva);
	SortGameObjects();
	return aLarva;
}

void Sexy::Board::SpawnAlien(int theType, bool unk)
{
	SpawnAlien(theType, mApp->mSeed->Next() % 450 + 20, mApp->mSeed->Next() % 195 + 105, unk);
}

void Sexy::Board::SpawnAlien(int theType, int theX, int theY, bool unk)
{
	if (theType == 9)
	{
		SpawnAlien(Alien::ALIEN_WEAK_SYLV, theX, theY, unk);
		SpawnAlien(Alien::ALIEN_BALROG, mCrosshair2X, mCrosshair2Y, unk);
	}
	else if (theType == 10)
	{
		SpawnAlien(Alien::ALIEN_PSYCHOSQUID, theX, theY, unk);
		SpawnAlien(Alien::ALIEN_BALROG, mCrosshair2X, mCrosshair2Y, unk);
	}
	else if (theType == 11)
	{
		SpawnAlien(Alien::ALIEN_DESTRUCTOR, theX, theY, unk);
		SpawnAlien(Alien::ALIEN_ULYSEES, mCrosshair2X, mCrosshair2Y, unk);
	}
	else if (theType == 12)
	{
		SpawnAlien(Alien::ALIEN_BALROG, theX, theY, unk);
		SpawnAlien(Alien::ALIEN_BILATERUS, mCrosshair2X, mCrosshair2Y, unk);
	}
	else if (theType == Alien::ALIEN_BILATERUS)
	{
		Bilaterus* aBil = new Bilaterus(theX, theY);
		aBil->SpawnWarp();
		mWidgetManager->AddWidget(aBil);
		MakeShadowForGameObject(aBil);
		AddGameObject(aBil);
	}
	else if (theType == Alien::ALIEN_CYRAX)
	{
		mShouldSave = true;
		Alien* anAlien = new Alien(theX, theY, Alien::ALIEN_CYRAX);
		anAlien->SpawnWarp();
		mCyraxPtr = anAlien;
		mWidgetManager->AddWidget(anAlien);
		MakeShadowForGameObject(anAlien);
		m0x3e4 = 10;
	}
	else
	{
		Alien* anAlien = new Alien(theX, theY, theType);
		anAlien->SpawnWarp();
		AddGameObject(anAlien);
		mWidgetManager->AddWidget(anAlien);
		MakeShadowForGameObject(anAlien);
	}
	Unk14(false);
	SortGameObjects();
	if (unk)
	{
		if (theType == Alien::ALIEN_GUS)
			mApp->PlaySample(SOUND_GUFFAW);
		else if(theType == Alien::ALIEN_DESTRUCTOR)
			mApp->PlaySample(SOUND_INTERFER);
		else if(theType == Alien::ALIEN_ULYSEES)
			mApp->PlaySample(SOUND_ROAR2);
		else if(theType == Alien::ALIEN_PSYCHOSQUID)
			mApp->PlaySample(SOUND_ROAR3);
		else if(theType == Alien::ALIEN_CYRAX)
			mApp->PlaySample(SOUND_EVILLAFF);
		else if(theType == Alien::ALIEN_WEAK_SYLV || theType == Alien::ALIEN_STRONG_SYLV || theType == Alien::ALIEN_BALROG)
			mApp->PlaySample(SOUND_ROAR3);
	}
	return;
}

void Sexy::Board::SpawnShot(int theX, int theY, int theType)
{
	if (mShotList->size() >= 20)
		return;

	Shot* aShot = new Shot(theX, theY, theType);
	AddGameObject(aShot);
	mWidgetManager->AddWidget(aShot);
	mWidgetManager->BringToFront(aShot);
	SortGameObjects();
}

void Sexy::Board::SpawnLaserShot(int theX, int theY)
{
	Shot* aShot = new Shot(theX, theY);
	AddGameObject(aShot);
	mWidgetManager->AddWidget(aShot);
	mWidgetManager->BringToFront(aShot);
	SortGameObjects();
	m0x3d0 = theX + 40;
	m0x3d4 = theY + 40;
}

void Sexy::Board::SpawnMissle(int theX, int theY, GameObject* theTarget, int theType)
{
	Missle* aMissle = new Missle(theX, theY, theTarget, theType);
	AddGameObject(aMissle);
	mWidgetManager->AddWidget(aMissle);
	mWidgetManager->BringToFront(aMissle);
	mWidgetManager->BringToFront(mBoardOverlay2);
	if (!aMissle->IsTargetless())
		theTarget->mMisslePtr = aMissle;
}

void Sexy::Board::SpawnShadow(int theSize, GameObject* theObject)
{
	if (mShadowList->size() >= 40)
		return;

	Shadow* aShad = new Shadow(theSize, theObject);
	aShad->mVisible = mApp->Is3DAccelerated();
	AddGameObject(aShad);
	mWidgetManager->AddWidget(aShad);
	SortGameObjects();
}

void Sexy::Board::RessurectFish(int theX, int theY, int theSize, bool flipped)
{
	Fish* aFish = new Fish(theX, theY, theSize, flipped);
	AddGameObject(aFish);
	mWidgetManager->AddWidget(aFish);
	if (mMaxFishCountEver < mFishList->size())
		mMaxFishCountEver = mFishList->size();
	MakeShadowForGameObject(aFish);
	SortGameObjects();
}

void Sexy::Board::RessurectOscar(int theX, int theY, bool flipped)
{
	Oscar* anOscar = new Oscar(theX, theY, flipped);
	AddGameObject(anOscar);
	mWidgetManager->AddWidget(anOscar);
	if (mMaxOscarCountEver < mOscarList->size())
		mMaxOscarCountEver = mOscarList->size();
	MakeShadowForGameObject(anOscar);
	SortGameObjects();
}

void Sexy::Board::RessurectUltra(int theX, int theY, bool flipped)
{
	Ultra* anUltra = new Ultra(theX, theY, flipped);
	AddGameObject(anUltra);
	mWidgetManager->AddWidget(anUltra);
	MakeShadowForGameObject(anUltra);
	SortGameObjects();
}

void Sexy::Board::RessurectGekko(int theX, int theY, bool flipped)
{
	Gekko* aGekko = new Gekko(theX, theY, flipped);
	AddGameObject(aGekko);
	mWidgetManager->AddWidget(aGekko);
	MakeShadowForGameObject(aGekko);
	SortGameObjects();
}

void Sexy::Board::RessurectPenta(int theX)
{
	Penta* aPenta = new Penta(theX);
	AddGameObject(aPenta);
	mWidgetManager->AddWidget(aPenta);
	if (mMaxPentaCountEver < mPentaList->size())
		mMaxPentaCountEver = mPentaList->size();
	MakeShadowForGameObject(aPenta);
	SortGameObjects();
}

void Sexy::Board::RessurectGrubber(int theX)
{
	Grubber* aGrubber = new Grubber(theX);
	AddGameObject(aGrubber);
	mWidgetManager->AddWidget(aGrubber);
	MakeShadowForGameObject(aGrubber);
	SortGameObjects();
}

void Sexy::Board::RessurectBreeder(int theX, int theY, int theSize, bool flipped)
{
	Breeder* aBreeder = new Breeder(theX, theY, theSize, flipped);
	AddGameObject(aBreeder);
	mWidgetManager->AddWidget(aBreeder);
	MakeShadowForGameObject(aBreeder);
	SortGameObjects();
}

void Board::DrawOverlay1(Graphics* g)
{
	mBubbleMgr->Draw(g);
	if (!mApp->IsScreenSaver())
		return;

	g->SetColor(Color::Black);
	g->FillRect(0, 0, 640, 60);
	g->FillRect(0, 445, 640, 35);
	g->SetFont(FONT_CONTINUUMBOLD14);
	g->SetColor(Color::White);
	SexyString aWarning = "Warning: Insaniquarium is running.  Shells collected here may not be saved.";
	int aStrWdth = g->GetFont()->StringWidth(aWarning);
	if (mApp->mScreenSaverUnk01)
	{
		int aY = 465;
		if ((mUpdateCnt / 1000 % 2) == 0)
			aY = 40;
		DrawStringWithOutline(g, aWarning, 320 - aStrWdth / 2, aY, FONT_CONTINUUMBOLD14OUTLINE, 0x29558c);
	}
	g->SetFont(FONT_CONTINUUMBOLD14);
	g->SetColor(Color(0xffffff));

	SexyString aShellsCollectedStr = StrFormat("%d Shells Collected", m0x400);
	aStrWdth = g->GetFont()->StringWidth(aShellsCollectedStr);
	int aStrY = 0;
	int aStrX = 0;
	switch (mUpdateCnt / 1000 % 4)
	{
	case 0:
		aStrX = 630 - aStrWdth;
		aStrY = 468;
		break;
	case 1:
		aStrX = 630 - aStrWdth;
		aStrY = 40;
		break;
	case 2:
		aStrX = 10;
		aStrY = 468;
		break;
	case 3:
		aStrX = 10;
		aStrY = 40;
		break;
	}
	if (mApp->mScreenSaverShowMoney)
	{
		g->SetFont(FONT_CONTINUUMBOLD14);
		g->SetColor(Color::White);
		DrawStringWithOutline(g, aShellsCollectedStr, aStrX, aStrY, FONT_CONTINUUMBOLD14OUTLINE, 0x29558c);
	}
	if (mApp->mScreenSaverPeriodicDim)
	{
		int aTimer = mUpdateCnt % 3000;
		if (aTimer >= 1320 && aTimer <= 1680)
		{
			int anAlpha = 200;
			if (aTimer < 1351)
			{
				anAlpha = InterpolateInt(0, 200, aTimer - 1320, 30, false);
			}
			else if (aTimer > 1649)
			{
				anAlpha = InterpolateInt(200, 0, aTimer - 1650, 30, false);
			}
			g->SetColor(Color(0, 0, 0, anAlpha));
			g->FillRect(0, 60, 640, 385);
		}
	}
}

void Sexy::Board::DrawTankBackground(Graphics* g)
{
	UserProfile* aCurProf = mApp->mCurrentProfile;
	if (aCurProf->mCheatCodeFlags >> 2 & 1)
	{
		g->SetColor(Color(0xffffff));
		g->FillRect(0, 0, 640, 480);
		return;
	}
	if (aCurProf->mCheatCodeFlags >> 3 & 1)
	{
		mStarField->Draw(g, aCurProf->mCheatCodeFlags & 1);
		return;
	}
	int aRandTransX = 0;
	int aRandTransY = 0;
	if (m0x2b4 != 0 && m0x2b4 < 20 && !mPause)
	{
		aRandTransX = rand() % 5 - 2;
		aRandTransY = rand() % 5 - 2;
		g->SetColor(Color::Black);
		g->FillRect(0, 0, 640, 480);
		g->Translate(aRandTransX, aRandTransY);
	}
	Image* anAquariumImg = GetImageById(mCurrentBackgroundId + IMAGE_AQUARIUM1_ID - 1);
	g->DrawImage(anAquariumImg, 0, 0);
	bool isAccel = mApp->Is3DAccelerated();
	if (aCurProf->mCheatCodeFlags & 1)
	{
		float aVal;
		if (isAccel)
			aVal = 2.0f;
		else
			aVal = 1.2f;

		Unk06(g, anAquariumImg, 0, 0, aVal);
	}

	DrawTankWaves(g, 88);

	if (isAccel)
	{
		g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
		g->SetColorizeImages(true);
		g->SetColor(Color(255, 255, 255, 40));
		g->DrawImage(IMAGE_TANKLIGHTING, mTankLightingSpeeds[0], 365);
		g->DrawImage(IMAGE_TANKLIGHTING, mTankLightingSpeeds[0] - 640.0, 365);

		g->SetColor(Color(255, 255, 255, 100));
		g->DrawImage(IMAGE_TANKLIGHTING, mTankLightingSpeeds[1], 365);
		g->DrawImage(IMAGE_TANKLIGHTING, mTankLightingSpeeds[1] - 640.0, 365);

		g->SetColor(Color(255, 255, 255, 70));
		g->DrawImageMirror(IMAGE_TANKLIGHTING, mTankLightingSpeeds[2], 365, true);
		g->DrawImageMirror(IMAGE_TANKLIGHTING, mTankLightingSpeeds[2] - 640.0, 365, true);
		g->SetColorizeImages(false);
		g->SetDrawMode(Graphics::DRAWMODE_NORMAL);

		g->DrawImage(GetImageById(mCurrentBackgroundId - 1 + IMAGE_TANKMASK1_ID), 0, 365);
	}
	g->Translate(-aRandTransX, -aRandTransY);
}

void Sexy::Board::DrawTankWaves(Graphics* g, int theY)
{
	g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
	int aCel = IMAGE_WAVE->GetAnimCel(mGameUpdateCnt);
	Rect aRect = IMAGE_WAVE->GetCelRect(aCel);
	g->DrawImageMirror(IMAGE_WAVESIDE, 0, theY, aRect, false);
	g->DrawImageCel(IMAGE_WAVE, 160, theY, aCel);
	g->DrawImageCel(IMAGE_WAVE, 320, theY, aCel);
	g->DrawImageMirror(IMAGE_WAVESIDE, 480, theY, aRect, true);
	g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
}

void Sexy::Board::DrawAlienAttractorMisc(Graphics* g, int theX, int theY, int theTime, int unk)
{
	if (unk != 0)
		theTime = 0;
	g->DrawImage(IMAGE_AA_BOTTOM, theX, theY);

	int aNewX = theX + 25;
	int aNewY = theY - 26;
	int aNewX2 = theX - 11;
	int aNewY2 = theY - 50;
	if (theTime < 30)
	{
		g->DrawImage(IMAGE_AA_GLOW, aNewX2, aNewY2);
	}
	else if (theTime < 40)
	{
		g->DrawImage(IMAGE_AA_TOP2, aNewX, aNewY);
		g->SetColorizeImages(true);
		g->SetColor(Color(255, 255, 255, ((30 - theTime) * 255) / 20 + 255));
		g->DrawImage(IMAGE_AA_GLOW, aNewX2, aNewY2);
		g->SetColorizeImages(false);
	}
	else if (theTime < 300)
	{
		g->DrawImage(IMAGE_AA_TOP2, aNewX, aNewY);
		int anAlphaMod = (300 - theTime) % 40;
		if (anAlphaMod > 20)
			anAlphaMod = 40 - anAlphaMod;
		g->SetColorizeImages(true);
		g->SetColor(Color(255, 255, 255, (anAlphaMod * 255) / 20));
		g->DrawImage(IMAGE_AA_TOP2, aNewX, aNewY);
		g->SetColorizeImages(false);
	}
	else
	{
		g->DrawImage(IMAGE_AA_TOP1, aNewX, aNewY);
	}
}

void Sexy::Board::DrawBonusRound(Graphics* g)
{
	int aBonusTimer = mGameUpdateCnt - m0x2a8; // ivar11
	Color aColor = g->GetColor();//loc1a4, 1a0, 19c, 198

	int anXBucketPos = 260; // loc16c
	int anYBucketPos = 265; // loc168
	if (!mBonusRoundStarted)
	{
		if (aBonusTimer > 129)
		{
			if (aBonusTimer < 150)
			{
				float anEndYPos = anYBucketPos + 5;
				float aProgress = (float)(aBonusTimer - 130) / 20.0f;
				aProgress *= aProgress;
				g->DrawImage(IMAGE_BONUSBUCKET, anXBucketPos, anEndYPos * aProgress);
			}
			else
			{
				int anYBucketPos2 = anYBucketPos;
				if (aBonusTimer < 152)
				{
					float anEndYPos = anYBucketPos + 5;
					float aProgress = (float)(aBonusTimer - 150) * 2.5;
					anYBucketPos2 = anEndYPos - aProgress;
				}
				g->DrawImage(IMAGE_BONUSBUCKET, anXBucketPos, anYBucketPos2);
			}
		}
	}
	else
	{
		g->DrawImage(IMAGE_BONUSBUCKET, anXBucketPos, anYBucketPos);
		if (m0x3f8 > 59 || (m0x3f8 / 10) % 2 == 0)
		{
			g->SetFont(FONT_CONTINUUMBOLD14);
			SexyString aStr = StrFormat("%d", m0x3f4);
			int aStrWdth = g->GetFont()->StringWidth(aStr);
			g->SetColor(Color(0xf0a33b));
			DrawStringWithOutline(g, aStr, (anXBucketPos - aStrWdth / 2) + 55, anYBucketPos + 151, FONT_CONTINUUMBOLD14OUTLINE, 0x7b2000);
		}
	}

	if (!mBonusRoundStarted || aBonusTimer < 20)
	{
		float anAlphaVal = 255;
		if (!mBonusRoundStarted && aBonusTimer < 10)
			anAlphaVal = (aBonusTimer * 255) / 10;
		else if(mBonusRoundStarted)
			anAlphaVal = 255 - (aBonusTimer * 255) / 20;
		g->SetFont(FONT_JUNGLEFEVER17OUTLINE);
		g->SetColor(Color(255, 255, 0, anAlphaVal));

		SexyString aStr = "BONUS ROUND";
		int aStrWdth = g->GetFont()->StringWidth(aStr);
		int aCenteredStrX = (mWidth - aStrWdth) / 2;
		int anXStrOffset = 0;
		if (!mBonusRoundStarted && aBonusTimer < 8)
			anXStrOffset = ((8 - aBonusTimer) * aStrWdth * -2) / 8;
		DrawCoolBonusString(g, aStr, aCenteredStrX + anXStrOffset, 235);
		g->SetFont(FONT_CONTINUUMBOLD14);
		g->SetColor(Color(180, 250, 90, anAlphaVal));

		aStr = "Collect as many shells as you can!";
		aStrWdth = g->GetFont()->StringWidth(aStr);
		aCenteredStrX = (mWidth - aStrWdth) / 2;
		anXStrOffset = 0;
		if (!mBonusRoundStarted && aBonusTimer < 8)
			anXStrOffset = ((8 - aBonusTimer) * aStrWdth * 2) / 8;

		Color aStrColor(0, 75, 0, anAlphaVal);
		DrawStringWithOutline(g, aStr, aCenteredStrX + anXStrOffset, 260, FONT_CONTINUUMBOLD14OUTLINE, aStrColor.ToInt());
		
		float anInterpolatedVal = 280;
		if (!mBonusRoundStarted)
			anInterpolatedVal += InterpolateInt(520, 0, aBonusTimer, 16, false);

		g->SetColorizeImages(true);
		#define NUM_OF_SHELLS 5
		int aShellValues[NUM_OF_SHELLS] = { 1,2,5,10,20 };
		int aShellYOffsets[NUM_OF_SHELLS] = { 0,0,0,0,-12 };
		g->SetFont(FONT_CONTINUUMBOLD12OUTLINE);
		for (int i = 0; i < NUM_OF_SHELLS; i++)
		{
			Image* anImg = IMAGE_SHELLS;
			int aRow = i;
			if (i == 4)
			{
				anImg = IMAGE_MONEYBAG;
				aRow = 0;
			}

			g->SetColor(Color(255,255,255,anAlphaVal));
			g->DrawImageCel(anImg, i * 60 + aCenteredStrX, aShellYOffsets[i] + anInterpolatedVal, 0, aRow);

			g->SetColor(Color(255, 255, 255, anAlphaVal));
			SexyString aShellValStr = StrFormat("%d", aShellValues[i]);
			int aStrWdth = g->GetFont()->StringWidth(aShellValStr);
			int aStrY = anInterpolatedVal + 50;
			g->DrawString(aShellValStr, (28 - aStrWdth) / 2 + i * 60 + aCenteredStrX, aStrY);
		}
		g->SetColorizeImages(false);
		int anImgId = (148 - aBonusTimer) / 36;
		int ivar11 = (148 - aBonusTimer) % 36;
		if (!mBonusRoundStarted && aBonusTimer < 148 && anImgId < 3)
		{
			Image* aNumImg = GetImageById(anImgId + IMAGE_BONUS1_ID);
			float aScaleTimer = 36 - ivar11; // loc150
			int ivar9 = 31 - ivar11;
			int anAlphaCnt = ivar11; // loc158
			if (ivar11 > 20)
				anAlphaCnt = 20;
			if (ivar9 < 0)
				ivar9 = 0;
			else if (ivar9 > 10)
				ivar9 = 10;
			if (aScaleTimer > 5)
				aScaleTimer = 5;

			float aScale = (aScaleTimer * 0.8) / 5.0 + 0.2;
			int aWidth = aNumImg->mWidth;
			int aHeight = aNumImg->mHeight;
			int aScaledWidth = aWidth * aScale;
			int aScaledHeight = aHeight * aScale;
			int aHalfScaledHeight = (aScale - 1.0) * aHeight * 0.5;
			int aX = (mWidth - aScaledWidth) / 2;
			int aY = 90 - aHalfScaledHeight;

			g->SetFastStretch(!mApp->Is3DAccelerated());
			Rect aSrcRect = Rect(0, 0, aWidth, aHeight);
			Rect aDestRect = Rect(aX, aY, aScaledWidth, aScaledHeight);
			g->SetColorizeImages(true);
			g->SetColor(Color(255, 255, 255, (anAlphaCnt*255) / 20));
			g->DrawImageMirror(aNumImg, aDestRect, aSrcRect, false);
			g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
			g->SetColor(Color(255, 255, 255, (ivar9* anAlphaCnt * 255) / 400));
			g->DrawImageMirror(aNumImg, aDestRect, aSrcRect, false);
			g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
			g->SetColorizeImages(false);
		}
	}
	g->SetFont(FONT_CONTINUUMBOLD12);
	g->SetColor(aColor);
	int aTimeInSecs = m0x3bc;
	if (mBonusRoundStarted)
		aTimeInSecs -= m0x3b0;

	int aMins = aTimeInSecs / 60;
	int aSecs = aTimeInSecs % 60;
	if (aMins < 0)
		aMins = 0;
	if (aSecs < 0)
		aSecs = 0;

	SexyString aTimeRStr = StrFormat("Time Remaining: %d:%02d", aMins, aSecs);

	g->DrawString(aTimeRStr, 465, 470);
}

void Sexy::Board::DrawCoolBonusString(Graphics* g, SexyString& theString, int theX, int theY)
{
	double aWobbleBase = mGameUpdateCnt * 0.25;
	char aPrevChar = 0;
	for (int i = 0; i < theString.length(); i++)
	{
		char aChar = theString[i];

		double aAngle = (i + aWobbleBase) * 35.0 * 3.14159 / 180.0;
		float aSinVal = sinf(aAngle);

		int aWobbleY = theY + (aSinVal * 5.0f);

		SexyString aCharStr;
		aCharStr += aChar;

		g->DrawString(aCharStr, theX, aWobbleY);

		int aCharWidth = g->GetFont()->CharWidthKern(aChar, aPrevChar);
		theX += aCharWidth;

		aPrevChar = aChar;
	}
}

Sexy::BoardOverlay::BoardOverlay(Board* theBoard, int thePriority)
{
	mBoard = theBoard;
	mPriority = thePriority;
	mHasAlpha = true;
	mMouseVisible = false;
	mWidth = 640;
	mHeight = 480;
}

Sexy::BoardOverlay::~BoardOverlay()
{
}

void Sexy::BoardOverlay::Draw(Graphics* g)
{
	mBoard->DrawOverlay(g, mPriority);
}
