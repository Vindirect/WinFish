#include "StoreScreen.h"
#include "WinFishApp.h"
#include "WinFishAppCommon.h"
#include "Res.h"
#include "Board.h"
#include "MyLabelWidget.h"
#include "SexyAppFramework/Font.h"
#include "SexyAppFramework/WidgetManager.h"
#include <ctime>

Sexy::StoreScreen::StoreScreen(WinFishApp* theApp)
{
	mRand = MTRand();
	mApp = theApp;
	mWidgetManager = theApp->mWidgetManager;
	mX = 0;
	mY = 0;
	mStoreButtonsX = 192;
	mStoreButtonsXGap = 111;
	mWidth = theApp->mWidth;
	mHeight = theApp->mHeight;
	m0x90 = 0;
	mOverButtonId = -1;
	mBoughtItemTimer = 0;
	mSaveData = false;

	mBackButton = MakeDialogButton(99, this, "Back", FONT_JUNGLEFEVER10OUTLINE);
	mBackButton->mColors[0] = Color(255, 240, 0);
	mBackButton->Resize(6, 440, 113, mBackButton->mHeight);

	Image* aBtnImages[8] = {IMAGE_STORELEFTBUTTON , IMAGE_STORELEFTBUTTON ,IMAGE_STORELEFTBUTTON ,
		IMAGE_STORETOPBUTTON,IMAGE_STORETOPBUTTON ,IMAGE_STORETOPBUTTON ,IMAGE_STORETOPBUTTON ,
		IMAGE_STORERIGHTBUTTON };

	Rect aBtnRects[8] = {
		Rect(1, 5, 109, 140),
		Rect(1, 143, 109, 140),
		Rect(1, 282, 109, 140),
		Rect(mStoreButtonsX + 1, 38, 98, 140),
		Rect(mStoreButtonsXGap + mStoreButtonsX, 38, 98, 140),
		Rect(mStoreButtonsXGap * 2 + mStoreButtonsX, 38, 98, 140),
		Rect(mStoreButtonsXGap * 3 + mStoreButtonsX, 38, 98, 140),
		Rect(489, 339, 121, 100)
	};

	for (int i = 0; i < 8; i++)
	{
		mStoreButtons[i] = new StoreButtonWidget(i, this, aBtnImages[i]);
		mStoreButtons[i]->Resize(aBtnRects[i]);
	}
	mStoreButtons[3]->m0x150 = 0;
	mStoreButtons[4]->m0x150 = 0;
	mStoreButtons[5]->m0x150 = 0;
	mStoreButtons[6]->m0x150 = 0;
	mStoreButtons[7]->m0x154 = 84;

	mShellsLabel = new MyLabelWidget();
	mShellsLabel->mAlignment = 2;
	mShellsLabel->mMouseVisible = false;
	mShellsLabel->mX = 238;
	mShellsLabel->mY = 439;
	mShellsLabel->mLabelFont = FONT_CONTINUUMBOLD12;
	mShellsLabel->mWidth = 80;
	mShellsLabel->mHeight = mShellsLabel->mLabelFont->GetHeight();
	mShellsLabel->mLabelColor = Color(250, 155, 150);
	mShellsString = StrFormat("%d", mApp->mCurrentProfile->mShells);
	mShellsLabel->SetLabel(mShellsString);
	mStoreScreenUpdateCnt = 0;
	mMerylBlinkTimer = mApp->mSeed->Next() % 150 + 100;
	mApp->StopMusic();
	mApp->PlayMusic(2, 0, false);
	mStoreButtonLast = nullptr;

	mOverlay = new StoreScreenOverlay();
	mOverlay->mScreen = this;
	mOverlay->mMouseVisible = false;
	mOverlay->mHasAlpha = true;
	mOverlay->mWidth = 640;
	mOverlay->mHeight = 480;
}

Sexy::StoreScreen::~StoreScreen()
{
	for (int i = 0; i < 8; i++)
		if (mStoreButtons[i])
			delete mStoreButtons[i];
	if (mBackButton)
		delete mBackButton;
	if (mShellsLabel)
		delete mShellsLabel;
	if (mOverlay)
		delete mOverlay;
}

void Sexy::StoreScreen::AddedToManager(WidgetManager* theWidgetManager)
{
	WidgetContainer::AddedToManager(theWidgetManager);
	InitializeStoreButtons(0);
	for (int i = 0; i < 8; i++)
		theWidgetManager->AddWidget(mStoreButtons[i]);
	theWidgetManager->AddWidget(mOverlay);
	theWidgetManager->AddWidget(mBackButton);
	theWidgetManager->AddWidget(mShellsLabel);
}

void Sexy::StoreScreen::RemovedFromManager(WidgetManager* theWidgetManager)
{
	WidgetContainer::RemovedFromManager(theWidgetManager);
	for (int i = 0; i < 8; i++)
		theWidgetManager->RemoveWidget(mStoreButtons[i]);
	theWidgetManager->RemoveWidget(mOverlay);
	theWidgetManager->RemoveWidget(mBackButton);
	theWidgetManager->RemoveWidget(mShellsLabel);
}

void Sexy::StoreScreen::Update()
{
	mMerylBlinkTimer--;
	if (mMerylBlinkTimer < 1)
	{
		if (mApp->mSeed->Next() % 7 == 0)
			mMerylBlinkTimer = mApp->mSeed->Next() % 15 + 15;
		else
			mMerylBlinkTimer = mApp->mSeed->Next() % 150 + 100;
	}

	MarkDirty();
	mStoreScreenUpdateCnt++;
	if (mBoughtItemTimer != 0)
	{
		if (mOverButtonId > -1)
			mBoughtItemTimer = 0;
		else
			mBoughtItemTimer--;
	}
}

void Sexy::StoreScreen::OrderInManagerChanged()
{
	for (int i = 0;i < 8;i++)
		mWidgetManager->BringToFront(mStoreButtons[i]);
	mWidgetManager->BringToFront(mOverlay);
	mWidgetManager->BringToFront(mBackButton);
	mWidgetManager->BringToFront(mShellsLabel);
}

void Sexy::StoreScreen::DrawOverlay(Graphics* g)
{
	g->DrawImage(IMAGE_STORESCREEN, 0, 0);
	for (int i = 0;i < 8;i++)
	{
		g->Translate(mStoreButtons[i]->mX, mStoreButtons[i]->mY);
		mStoreButtons[i]->DrawOverlay(g);
		g->Translate(-mStoreButtons[i]->mX, -mStoreButtons[i]->mY);
	}
	g->DrawImageAnim(IMAGE_MERYLBLINK, 255, 289, mMerylBlinkTimer);
	if ((mOverButtonId > -1 && mOverButtonId < 8 && mStoreScreenUpdateCnt > 15) || mBoughtItemTimer != 0)
	{
		SexyString aStr = "";
		if (mBoughtItemTimer == 0)
		{
			switch (mStoreButtons[mOverButtonId]->m0x14c)
			{
			case PRODUCT_SOLD:
				aStr = "Sorry, this is sold out.\nCheck back tomorrow!";
				break;
			case PRODUCT_FISH:
				aStr = mStoreButtons[mOverButtonId]->GetProduct(false)->GetStoreDescription();
				break;
			case PRODUCT_BUBBULATOR:
				aStr = "Want to keep more fish\nin your tank?  Buy\nThe Bubbulator!";
				break;
			case PRODUCT_BACKDROP:
				aStr = "Keep your fish happy\nwith this beautiful\nnew backdrop!";
				break;
			case PRODUCT_ALIEN_ATTRACTOR:
				aStr = "Need more excitement\nin your tank?  Buy\nthe alien attractor!\nTrust me.  It\'s safe!";
				break;
			case PRODUCT_UPGRADE:
				aStr = "Your fish will\ngrow faster with\nthis food upgrade!";
				break;
			}
		}
		else
			aStr = "Thanks for shopping at\nthe Fish Emporium!";

		const char* newLineChar = strchr(aStr.c_str(), '\n');
		int aNewLineCnt = 0;
		while (newLineChar)
		{
			aNewLineCnt++;
			newLineChar = strchr(newLineChar + 1, '\n');
		}
		g->DrawImage(IMAGE_SPEECHBUBBLE, 390, 220);
		g->SetColor(Color::Black);
		g->SetFont(FONT_BLAMBOTPRO8);

		Rect aStrRect(393, 235, 200, 300);
		if (aNewLineCnt <= 0)
			aStrRect.mY = 250;
		if (aNewLineCnt <= 1)
			aStrRect.mY += 10;
		else if (aNewLineCnt > 2)
			aStrRect.mY -= 5;

		WriteWordWrapped(g, aStrRect, aStr, 20, 0);
	}
}

void Sexy::StoreScreen::MouseEnter()
{
	if (mStoreButtonLast != nullptr)
	{
		mStoreButtonLast->HandleMouseEvent(false);
		mStoreButtonLast = nullptr;
	}
}

void Sexy::StoreScreen::MouseMove(int theX, int theY)
{
	if (mBoughtItemTimer > 0)
	{
		mBoughtItemTimer -= 3;
		if (mBoughtItemTimer < 0)
			mBoughtItemTimer = 0;
		mOverButtonId = -1;
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			if (mStoreButtons[i]->Contains(theX, theY))
			{
				mOverButtonId = i;
				return;
			}
		}
		mOverButtonId = -1;
	}
}

void Sexy::StoreScreen::ButtonPress(int theId)
{
	mApp->PlaySample(SOUND_BUTTONCLICK);
}

void Sexy::StoreScreen::ButtonDepress(int theId)
{
	if (theId == 99)
	{
		if (mSaveData)
			mApp->SaveVirtualTankAndUserData();

		mApp->mBoard->StartMusic();
		mApp->RemoveStoreScreen();
		mApp->mBoard->PauseGame(false);
	}
	else if (theId < 8 && mStoreButtons[theId])
	{
		StoreButtonWidget* aBtn = mStoreButtons[theId];
		if (mApp->mCurrentProfile->mShells < aBtn->m0x128)
		{
			SexyString aStrExtension = "item";
			switch (aBtn->m0x14c)
			{
			case PRODUCT_FISH:
				aStrExtension = "this fish";
				break;
			case PRODUCT_BUBBULATOR:
				aStrExtension = "the Bubbulator";
				break;
			case PRODUCT_BACKDROP:
				aStrExtension = "this backdrop";
				break;
			case PRODUCT_ALIEN_ATTRACTOR:
				aStrExtension = "the Alien Attractor";
				break;
			case PRODUCT_UPGRADE:
				aStrExtension = "this upgrade";
				break;
			}

			mApp->DoDialog(WinFishApp::DIALOG_INFO, true, "Not Enough Shells", StrFormat("Sorry, but you need more Shells to purchase %s.\n\nKeep playing to earn more!", aStrExtension.c_str()), "OK", Dialog::BUTTONS_FOOTER);
		}
		else
		{
			mStoreButtonLast = aBtn;
			mStoreButtonLast->HandleMouseEvent(true);

			GameObject* aProduct = mStoreButtonLast->GetProduct(false);
			if (aProduct == nullptr || mStoreButtonLast->m0x14c != 1)
			{
				switch (mStoreButtonLast->m0x14c)
				{
				case PRODUCT_BUBBULATOR:
					mApp->DoConfirmPurchaseDialog("Would you like to\nbuy The Bubbulator?");
					break;
				case PRODUCT_BACKDROP:
					mApp->DoConfirmPurchaseDialog("Would you like to\nbuy this backdrop?");
					break;
				case PRODUCT_ALIEN_ATTRACTOR:
					mApp->DoConfirmPurchaseDialog("Would you like to\nbuy the Alien Attractor?");
					break;
				case PRODUCT_UPGRADE:
					mApp->DoConfirmPurchaseDialog("Would you like to\nupgrade your fish food?");
					break;
				default:
					break;
				}
			}
			else
			{
				int aNextId = mApp->mBoard->GetNextVirtualTankId();
				if (aNextId < 0)
				{
					if (mApp->mCurrentProfile->mBubbulatorBought == 0)
						mApp->DoDialog(WinFishApp::DIALOG_INFO, true, "Fishtank is Full", 
							"You\'ll need to free up some room in your tank before you can buy another fish.  You can either buy a bubbulator or sell a fish back to us.", 
							"OK", Dialog::BUTTONS_FOOTER);
					else
						mApp->DoDialog(WinFishApp::DIALOG_INFO, true, "Fishtank is Full", 
							"You\'ll need to free up some room in your tank before you can buy another fish.", 
							"OK", Dialog::BUTTONS_FOOTER);
				}
				else
				{
					aProduct->mVirtualTankId = aNextId;
					mApp->DoFishNamingDialog("Please choose a name for your fish.", aProduct->mName, true);
				}
			}
		}
	}
}

void Sexy::StoreScreen::KeyChar(SexyChar theChar) // TODO REMOVE WHOLE FUNC (USING THIS FOR DEBUG ONLY)
{
	if (theChar == 'r' || theChar == 'R')
	{
		InitializeStoreButtons(Rand());
	}
}

Sexy::GameObject* Sexy::StoreScreen::ConfirmPurchase()
{
	mBoughtItemTimer = 108;
	if (mStoreButtonLast == nullptr)
		return nullptr;
	mSaveData = true;
	mApp->mCurrentProfile->mStoreScreenBought[mStoreButtonLast->mId] = true;
	mApp->mCurrentProfile->AddShells(-mStoreButtonLast->m0x128);
	if (mApp->mCurrentProfile->mShells < 0)
		mApp->mCurrentProfile->mShells = 0;
	mShellsString = StrFormat("%d", mApp->mCurrentProfile->mShells);
	mShellsLabel->SetLabel(mShellsString);

	switch (mStoreButtonLast->m0x14c)
	{
	case PRODUCT_FISH:
	{
		GameObject* aProd = mStoreButtonLast->GetProduct(true);
		if (aProd)
		{
			aProd->mTimeBought = time(NULL);
			aProd->SomeBoughtFunc();
			mStoreButtonLast->Bought();
			return aProd;
		}
	}
		break;
	case PRODUCT_BUBBULATOR:
		mApp->mCurrentProfile->mBubbulatorBought = 1;
		mApp->mBoard->mBubbulatorShown = true;
		mApp->mBoard->mBubbulatorTimer = 0;
		break;
	case PRODUCT_BACKDROP:
		if (mStoreButtonLast->m0x158 < 6)
		{
			mApp->mCurrentProfile->mUnlockedBackgrounds[mStoreButtonLast->m0x158] = true;
			mApp->mBoard->ChangeBackground(mStoreButtonLast->m0x158 + 1);
		}
		break;
	case PRODUCT_ALIEN_ATTRACTOR:
		mApp->mCurrentProfile->mAlienAttractorBought = 1;
		mApp->mBoard->mAlienAttractorShown = true;
		break;
	case PRODUCT_UPGRADE:
		gFoodType = 2;
	}
	mStoreButtonLast->Bought();
	return nullptr;
}

void Sexy::StoreScreen::InitializeStoreButtons(int theSeed)
{
	long long aSeedBase = (long long)(int)gUnkInt01 + mApp->mDaysSinceLastRun + (long long)theSeed; // Should be okay
	mSeedBase = aSeedBase;
	mRand.SRand((unsigned long)(mApp->mCurrentProfile->m0x98 + aSeedBase));
	mApp->mCurrentProfile->UpdateStoreData(mSeedBase);
	
	CommonFishStoreSlots();
	ColorfulFishStoreSlots();
	SpecialFishStoreSlots();
	ItemsStoreSlots();
	for (int i = 0; i < 8;i++)
	{
		if (!mApp->mCurrentProfile->mStoreScreenBought[i] && mStoreButtons[i]->m0x14c != 0)
		{
			mStoreButtons[i]->mMouseVisible = true;
			mStoreButtons[i]->SetPrice(mStoreButtons[i]->m0x128);
		}
		else
			mStoreButtons[i]->Bought();
	}
}

void Sexy::StoreScreen::CommonFishStoreSlots()
{
	Fish* aCommonFish = new Fish(0, 0);
	Oscar* aCommonOscar = new Oscar(0, 0);

	mStoreButtons[0]->SetUpButton(aCommonFish, aCommonFish->GetShellCost());
	mStoreButtons[1]->SetUpButton(aCommonOscar, aCommonOscar->GetShellCost());
}

void Sexy::StoreScreen::ColorfulFishStoreSlots()
{
	Fish* aColorfulFish = new Fish(0, 0);
	Oscar* aColorfulOscar = new Oscar(0, 0);

	InitFishColors(aColorfulFish, true);
	InitFishColors(aColorfulOscar, true);

	mStoreButtons[3]->SetUpButton(aColorfulFish, aColorfulFish->GetShellCost());
	mStoreButtons[4]->SetUpButton(aColorfulOscar, aColorfulOscar->GetShellCost());
}

void Sexy::StoreScreen::SpecialFishStoreSlots()
{
	GameObject* aFish1 = GetRandomSpecialFish(false, -1, 9);
	int aVal = Unk01(true);
	GameObject* aFish2 = nullptr;
	if (aVal == 8)
		aFish2 = GetSpecial8Fish();

	if (aFish2 == nullptr)
	{
		aFish2 = GetRandomSpecialFish(true, aFish1->mType, aVal);
		SetUpSpecialFish(aFish2, true);
	}

	mStoreButtons[5]->SetUpButton(aFish1, aFish1->GetShellCost());
	mStoreButtons[6]->SetUpButton(aFish2, aFish2->GetShellCost());
}

void Sexy::StoreScreen::ItemsStoreSlots()
{
	UserProfile* aProf = mApp->mCurrentProfile;
	if (aProf->mBubbulatorBought == 0)
		mStoreButtons[2]->SetProductType(PRODUCT_BUBBULATOR, 0, 20000);
	else if(aProf->mAlienAttractorBought == 0)
		mStoreButtons[2]->SetProductType(PRODUCT_ALIEN_ATTRACTOR, 0, 50000);
	else if(gFoodType == 0)
		mStoreButtons[2]->SetProductType(PRODUCT_UPGRADE, 0, 20000);
	else
	{
		bool inTank = IsSpecialFishInTank(GameObject::COOKIE);
		GameObject* aProd = nullptr;
		if (!inTank)
			aProd = MakeSpecialFish(GameObject::COOKIE);
		else
		{
			aProd = GetRandomSpecialFish(false, -1, 9);
			if ((mRand.Next() & 1) == 0)
				SetUpSpecialFish(aProd, false);
		}
		mStoreButtons[2]->SetUpButton(aProd, aProd->GetShellCost());
	}

	std::vector<int> aBGCandidates;
	for (int i = 0; i < 6; i++)
	{
		if (!aProf->mUnlockedBackgrounds[i])
		{
			if (i == 5)
			{
				if(mRand.Next() % 5 == 0)
					aBGCandidates.push_back(i);
			}
			else
				aBGCandidates.push_back(i);
		}
	}

	if (aBGCandidates.empty())
	{
		GameObject* aProd = GetRandomSpecialFish(true, -1, 9);
		if ((mRand.Next() & 1) == 0)
			SetUpSpecialFish(aProd, false);
		mStoreButtons[7]->SetUpButton(aProd, aProd->GetShellCost());
	}
	else
	{
		long long aVal = mSeedBase % aBGCandidates.size();
		mStoreButtons[7]->SetProductType(3, aBGCandidates[aVal], 15000);
	}
}

bool Sexy::StoreScreen::IsSpecialFishInTank(int theSpecialFishId)
{
	for (GameObjectSet::iterator it = mApp->mBoard->mGameObjectSet.begin(); it != mApp->mBoard->mGameObjectSet.end(); ++it)
	{
		GameObject* anObj = *it;
		if (anObj->mPreNamedTypeId == theSpecialFishId)
			return true;
	}
}

void Sexy::StoreScreen::InitFishColors(Fish* theFish, bool canBeRainbow)
{
	bool isRainbow = false;
	if (canBeRainbow)
	{
		if (mRand.Next() % 30 == 0)
			isRainbow = true;
	}

	theFish->SetFishColors(mRand.Next() % 1000, isRainbow);
}

Sexy::GameObject* Sexy::StoreScreen::GetRandomSpecialFish(bool possibleClassicFish, int theSkipType, int unk2)
{
	if (possibleClassicFish)
	{
		if (mRand.Next() % 100 <= 75)
		{
			Fish* aFish = RandomGuppyOrOscar();
			if (mRand.Next() % 100 <= 75)
				InitFishColors(aFish, true);
			return aFish;
		}
	}

	struct WeightedEntry
	{
		int mWeight;
		int mType;
	};

	WeightedEntry aTable[8];
	aTable[0].mWeight = 26; aTable[0].mType = GameObject::TYPE_PENTA;
	aTable[1].mWeight = 25; aTable[1].mType = GameObject::TYPE_GRUBBER;
	aTable[2].mWeight = 25; aTable[2].mType = GameObject::TYPE_GEKKO;
	aTable[3].mWeight = 10; aTable[3].mType = GameObject::TYPE_BREEDER;
	aTable[4].mWeight = 5;  aTable[4].mType = GameObject::TYPE_ULTRA;
	aTable[5].mWeight = 3;  aTable[5].mType = GameObject::TYPE_BI_FISH;
	aTable[6].mWeight = 3;  aTable[6].mType = GameObject::TYPE_BALL_FISH;
	aTable[7].mWeight = 3;  aTable[7].mType = GameObject::TYPE_SYLVESTER_FISH;

	int aLoopCnt = 0;
	while (aLoopCnt < 100)
	{
		int aRand = mRand.Next() % 100;
		int aCurWeightSum = 0;
		int aSelectedObj = 0;
		for (; aSelectedObj < 8; aSelectedObj++)
		{
			aCurWeightSum += aTable[aSelectedObj].mWeight;
			if (aRand < aCurWeightSum)
				break;
		}
		if (aSelectedObj == 8)
			aSelectedObj = 0;

		if (aTable[aSelectedObj].mType != theSkipType && 
			IsSpecialFishAllowed(unk2, aTable[aSelectedObj].mType))
		{
			switch (aTable[aSelectedObj].mType)
			{
			case GameObject::TYPE_ULTRA:
				return new Ultra(0, 0);
			case GameObject::TYPE_GEKKO:
				return new Gekko(0, 0);
			case GameObject::TYPE_PENTA:
				return new Penta(0, 0);
			case GameObject::TYPE_GRUBBER:
				return new Grubber(0, 0);
			case GameObject::TYPE_BREEDER:
				return new Breeder(0, 0);
			case GameObject::TYPE_SYLVESTER_FISH:
				return new SylvesterFish(0, 0, false);
			case GameObject::TYPE_BALL_FISH:
				return new BallFish(0, 0, false);
			case GameObject::TYPE_BI_FISH:
			{
				BiFish* aFish = new BiFish(0, 0, false);
				aFish->m0x230 = mRand.Next() & 1;
				return aFish;
			}
			default:
				return new Penta(0, 0);
			}
		}

		aLoopCnt++;
	}
	return GetRandomSpecialFish(possibleClassicFish, -1, 9);
}

Sexy::Fish* Sexy::StoreScreen::RandomGuppyOrOscar()
{
	if (mRand.Next() % 2 == 0)
		return new Fish(0, 0);
	else
		return new Oscar(0,0);
}

bool Sexy::StoreScreen::IsSpecialFishAllowed(int unk, int theType)
{
	if (unk > 6) return true;

	switch (unk)
	{
	case 1:
		if (theType == GameObject::TYPE_BALL_FISH) return false;
		if (theType == GameObject::TYPE_PENTA) return false;
		return true;
	case 4:
		if (theType == GameObject::TYPE_PENTA) return false;
		if (theType == GameObject::TYPE_GRUBBER) return false;
		if (theType == GameObject::TYPE_BALL_FISH) return false;
		return true;
	case 5:
		return (theType != GameObject::TYPE_BALL_FISH);
	case 6:
		if (theType == GameObject::TYPE_PENTA) return false;
		if (theType == GameObject::TYPE_GRUBBER) return false;
		return true;
	case 2:
	case 3:
	default:
		return true;
	}
	return false;
}

int Sexy::StoreScreen::Unk01(bool deterministic)
{
	if(!gUnkBool09)
	{
		gUnkBool09 = true;
		for (int i = 0; i < 100; i++)
		{
			if ((i & 1) == 0)
			{
				gUnkIntArray02[i] = 5;
			}
			else
			{
				int mod10 = i % 10;

				if (mod10 == 1 || mod10 == 5) gUnkIntArray02[i] = 3;
				else if (mod10 == 3) gUnkIntArray02[i] = 1;
				else
				{
					int mod20 = i % 20;
					if (mod20 == 7) gUnkIntArray02[i] = 2;
					else if (mod20 == 9) gUnkIntArray02[i] = 0;
					else if (mod20 == 17) gUnkIntArray02[i] = 6;
					else if (mod20 == 19) gUnkIntArray02[i] = 4;
				}
			}
			if(i % 10 == 0)
				gUnkIntArray02[i] = ((i/10) & 1) + 7;
		}
	}

	if (deterministic)
	{
		int idx = mSeedBase % 100;
		return gUnkIntArray02[idx];
	}

	struct WeightedEntry
	{
		int mWeight;
		int mType;
	};

	WeightedEntry aTable[8] = {
		{ 48, 5 },
		{ 20, 1 },
		{ 10, 3 },
		{ 5,  2 },
		{ 5,  0 },
		{ 5,  6 },
		{ 5,  4 },
		{ 2,  7 }
	};

	int aRand = mRand.Next() % 100;
	int aCurWeightSum = 0;
	int i;
	for (i = 0; i < 8; i++)
	{
		aCurWeightSum += aTable[i].mWeight;
		if (aRand < aCurWeightSum)
			break;
	}
	if (i == 8)
		i = 0;

	return aTable[i].mType;
}

Sexy::GameObject* Sexy::StoreScreen::GetSpecial8Fish()
{
	std::set<int> aSet;
	aSet.insert(5);
	aSet.insert(2);

	for (GameObjectSet::iterator it = mApp->mBoard->mGameObjectSet.begin(); 
		it != mApp->mBoard->mGameObjectSet.end(); ++it)
	{
		GameObject* anObj = *it;
		if (anObj->mPreNamedTypeId != -1)
			aSet.insert(anObj->mPreNamedTypeId);
	}

	std::vector<int> aCandidatesVector;

	for (int i = 0; i < 6; i++)
	{
		if (aSet.find(i) == aSet.end())
			aCandidatesVector.push_back(i);
	}

	GameObject* aRes = nullptr;
	if (!aCandidatesVector.empty())
	{
		int anIdx = mRand.Next() % aCandidatesVector.size();
		aRes = MakeSpecialFish(aCandidatesVector[anIdx]);
	}
	return aRes;
}

Sexy::Fish* Sexy::StoreScreen::MakeSpecialFish(int theId)
{
	Fish* aFish = nullptr;
	switch (theId)
	{
	case GameObject::ROCKY:
	{
		aFish = new Fish(0, 0);
		aFish->mHasSpecialColors = true;
		aFish->mColors[0] = Color(0x2288ff);
		aFish->mColors[1] = Color(0xffffff);
		aFish->mVoracious = true;
		aFish->mExoticDietFoodType = Food::EXO_FOOD_ULTRA;
		aFish->mName = "Rocky";
		break;
	}
	case GameObject::LUDWIG:
	{
		aFish = new Oscar(0, 0);
		aFish->mSinging = true;
		aFish->mName = "Ludwig";
		aFish->mHasSpecialColors = true;
		aFish->mColors[0] = Color(0);
		aFish->mColors[1] = Color(0x8df5be);
		aFish->mColors[2] = Color(0x9ffa);
		break;
	}
	case GameObject::COOKIE:
	{
		aFish = new Gekko(0, 0);
		aFish->mName = "Cookie";
		aFish->mExoticDietFoodType = 6;
		break;
	}
	case GameObject::JOHNNYV:
	{
		aFish = new Oscar(0, 0);
		aFish->SetFishColors(mRand.Next() % 1000, true);
		aFish->mName = "Johnny V.";
		aFish->mExoticDietFoodType = Food::EXO_FOOD_PIZZA;
		break;
	}
	case GameObject::KILGORE:
	{
		aFish = new Ultra(0, 0);
		aFish->mName = "Kilgore";
		aFish->mVoracious = true;
		aFish->mSinging = true;
		break;
	}
	}
	if (aFish)
		aFish->mPreNamedTypeId = theId;
	return aFish;
}

void Sexy::StoreScreen::SetUpSpecialFish(GameObject* theObject, bool deterministic)
{
	for (int i = 0; i < 100; i++)
	{
		int aVal = Unk01(deterministic);
		switch (aVal)
		{
		case 0:
			theObject->mInvisible = true;
			break;
		case 1:
			theObject->mVoracious = true;
			break;
		case 2:
			theObject->mSpeedy = true;
			break;
		case 3:
			theObject->mSinging = true;
			break;
		case 4:
			theObject->mForwardlyChallenged = true;
			break;
		case 5:
			SpecialId5SetUp(theObject);
			break;
		case 6:
			SpecialId6SetUp(theObject);
			break;
		case 7:
		case 8:
			SpecialId78SetUp(theObject);
			break;
		}

		if (IsSpecialFishValid(theObject))
			return;

		theObject->ResetSpecialProperties();

		if (i >= 50)
			deterministic = false;
	}
}

void Sexy::StoreScreen::SpecialId5SetUp(GameObject* theObject)
{
	int aRand = mRand.Next() % 3;
	if (aRand == 0)
		theObject->mExoticDietFoodType = Food::EXO_FOOD_PIZZA;
	else if (aRand == 1)
		theObject->mExoticDietFoodType = Food::EXO_FOOD_ICE_CREAM;
	else if (aRand == 2)
		theObject->mExoticDietFoodType = Food::EXO_FOOD_CHICKEN;
}

void Sexy::StoreScreen::SpecialId6SetUp(GameObject* theObject)
{
	while (true)
	{
		int aRand = mRand.Next() % 3;
		if (aRand == 0)
			theObject->mExoticDietFoodType = Food::EXO_FOOD_GUPPY;
		else if (aRand == 1)
			theObject->mExoticDietFoodType = Food::EXO_FOOD_OSCAR;
		else
			theObject->mExoticDietFoodType = Food::EXO_FOOD_ULTRA;

		if (theObject->mType == GameObject::TYPE_ULTRA)
		{
			if (theObject->mExoticDietFoodType == Food::EXO_FOOD_OSCAR)
				continue;
			break;
		}

		if (theObject->mType == GameObject::TYPE_OSCAR || 
			theObject->mType == GameObject::TYPE_GRUBBER || 
			theObject->mType == GameObject::TYPE_BI_FISH)
		{
			if (theObject->mExoticDietFoodType == Food::EXO_FOOD_GUPPY)
				continue;
			break;
		}

		if (theObject->mType == GameObject::TYPE_SYLVESTER_FISH)
		{
			theObject->mExoticDietFoodType = Food::EXO_FOOD_GUPPY;
			break;
		}

		break;
	}
}

void Sexy::StoreScreen::SpecialId78SetUp(GameObject* theObject)
{
	for (int i = 0;i < 100;i++)
	{
		int aRandVal = mRand.Next() % 7;
		switch (aRandVal)
		{
		case 0:
			theObject->mInvisible = true;
			theObject->mVoracious = true;
			break;
		case 1:
			theObject->mInvisible = true;
			theObject->mSinging = true;
			break;
		case 2:
			theObject->mVoracious = true;
			theObject->mSinging = true;
			break;
		case 3:
			theObject->mForwardlyChallenged = true;
			theObject->mSpeedy = true;
			break;
		case 4:
			theObject->mVoracious = true;
			theObject->mSpeedy = true;
			break;
		case 5:
			theObject->mInvisible = true;
			theObject->mVoracious = true;
			theObject->mSinging = true;
			break;
		case 6:
		{
			int subTrait = mRand.Next() % 5;
			switch (subTrait)
			{
			case 0: theObject->mInvisible			= true; break;
			case 1: theObject->mVoracious			= true; break;
			case 2: theObject->mSpeedy				= true;	break;
			case 3: theObject->mSinging				= true;	break;
			case 4: theObject->mForwardlyChallenged = true; break;
			}

			if ((mRand.Next() & 1) == 0)
				SpecialId6SetUp(theObject);
			else
				SpecialId5SetUp(theObject);
		}
			break;
		}
		if (IsSpecialFishValid(theObject))
			return;

		theObject->ResetSpecialProperties();
	}
}

bool Sexy::StoreScreen::IsSpecialFishValid(GameObject* theObject)
{
	int fishType = theObject->mType;
	int dietType = theObject->mExoticDietFoodType;

	bool isValid = true;

	if (theObject->mVoracious)
	{
		if (!IsSpecialFishAllowed(1, fishType))
			isValid = false;
	}

	if (isValid && theObject->mForwardlyChallenged)
	{
		if (!IsSpecialFishAllowed(4, fishType))
			isValid = false;
	}

	if (isValid && (dietType == Food::EXO_FOOD_PIZZA || dietType == Food::EXO_FOOD_ICE_CREAM || dietType == Food::EXO_FOOD_CHICKEN))
	{
		if (!IsSpecialFishAllowed(5, fishType))
			isValid = false;
	}

	if (isValid && (dietType == Food::EXO_FOOD_ULTRA || dietType == Food::EXO_FOOD_OSCAR || dietType == Food::EXO_FOOD_GUPPY))
	{
		if (!IsSpecialFishAllowed(6, fishType))
			isValid = false;
	}

	return isValid;
}

void Sexy::StoreScreenOverlay::Draw(Graphics* g)
{
	mScreen->DrawOverlay(g);
}
