#include "PetsScreen.h"
#include "Board.h"
#include "WinFishAppCommon.h"
#include "SexyAppFramework/MemoryImage.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Font.h"
#include "Res.h"

using namespace Sexy;

Sexy::PetsScreen::PetsScreen(WinFishApp* theApp)
{
	mApp = theApp;
	mX = 0;
	mY = 0;
	mWidth = theApp->mWidth;
	mHeight = theApp->mHeight;
	m0x118 = 25;
	m0x11c = 94;
	m0x124 = 0;
	m0x128 = -1;
	m0x12c = -1;
	m0x120 = 0;

	mReturnButton = MakeDialogButton2(99, this, "Click Here To Continue", IMAGE_MAINBUTTON);

	if (theApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
	{
		mReturnButton->mLabel = "Return to Tank";
		mApp->StopMusic();
		mApp->PlayMusic(2, 0);
	}
	mReturnButton->Resize(225, 250, 186, mReturnButton->mHeight);

	mMenuButton = MakeDialogButton2(100, this, "Menu", IMAGE_MAINBUTTON);

	mMenuButton->Resize(525, 4, 80, mMenuButton->mHeight);

	mBackgroundImage = new MemoryImage(mApp);
	mBackgroundImage->Create(640, 480);
	mBackgroundImage->SetImageMode(true, true);
	Graphics g(mBackgroundImage);
	g.DrawImageBox(Rect(-5, -5, 650, 490), IMAGE_SCREENBACK);
	g.DrawImageBox(Rect(20, 0, 600, IMAGE_SCREENTITLE->mHeight), IMAGE_SCREENTITLE);
	if (mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)
		g.DrawImageBox(Rect(mMenuButton->mX - 1, mMenuButton->mY - 1, mMenuButton->mWidth + 2, IMAGE_SCREENTITLEHOLE->mHeight), IMAGE_SCREENTITLEHOLE);
	
	g.DrawImage(IMAGE_FISHBOX, 216, 48);
	g.DrawImage(IMAGE_FISHBOXBUTTON, 221, 243);

	int anYOffset = -374;
	for (int i = 0; i < GameObject::PET_END; i++)
	{
		if (mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)
			mApp->mCurrentProfile->m0x5a[i] = false;

		mPetButtons[i] = new PetButtonWidget(IMAGE_SCL_STINKY_ID + i, i, this);

		if (i < 0)
		{

		}
		else if (i < 5)
		{
			mPetButtons[i]->Resize(m0x118, anYOffset + 415, 90, 83);
		}
		else if (i < 10)
		{
			mPetButtons[i]->Resize(m0x118 + m0x11c, anYOffset, 90, 83);
		}
		else if (i < 15)
		{
			mPetButtons[i]->Resize(m0x118 + m0x11c * 4 + 24, anYOffset - 415, 90, 83);
		}
		else if (i < 20)
		{
			mPetButtons[i]->Resize(m0x118 + m0x11c * 5 + 24, anYOffset - 830, 90, 83);
		}
		else if (i < 22)
		{
			mPetButtons[i]->Resize(m0x118 + m0x11c * 2 + 13, anYOffset - 996, 90, 83);
		}
		else if (i < GameObject::PET_END)
		{
			mPetButtons[i]->Resize(m0x118 + m0x11c * 3 + 16, anYOffset - 1162, 90, 83);
		}

		ButtonHoleHelper(mBackgroundImage, (MemoryImage*) IMAGE_PETBUTTONHOLE, mPetButtons[i]->mX, mPetButtons[i]->mY);
		if (!mApp->mCurrentProfile->IsPetUnlocked(i))
			mPetButtons[i]->SetDisabled(true);

		anYOffset += 83;
	}

	if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK && mApp->mBoard) // 207
	{
		for (GameObjectSet::iterator it = mApp->mBoard->mGameObjectSet.begin(); it != mApp->mBoard->mGameObjectSet.end(); ++it)
		{
			GameObject* anObj = *it;
			if (anObj->mVirtualTankId >= 1000 && anObj->mVirtualTankId - 1000 < GameObject::PET_END)
			{
				int anIdx = anObj->mVirtualTankId - 1000;
				if (!mPetButtons[anIdx]->mDisabled)
				{
					m0x124++;
					m0x12c = anIdx;
					m0x128 = anIdx;
					mPetButtons[anIdx]->Setup(true);
				}
				m0x100[anIdx] = true;
			}
		}
	}

	m0x130 = mApp->mCurrentProfile->m0xb4 < mApp->mCurrentProfile->mNumOfUnlockedPets ? mApp->mCurrentProfile->m0xb4 : mApp->mCurrentProfile->mNumOfUnlockedPets;

	if (mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK && m0x130 > 4)
		m0x130 = 4;

	mOverlay = new PetScreenOverlay();
	mOverlay->mScreen = this;
	mOverlay->mMouseVisible = false;
	mOverlay->mHasAlpha = true;
	mOverlay->mWidth = 640;
	mOverlay->mHeight = 480;

	m0x134 = 0;
}

Sexy::PetsScreen::~PetsScreen()
{
	for (int i = 0;i < GameObject::PET_END; i++)
		if(mPetButtons[i])
			delete mPetButtons[i];

	if (mReturnButton)
		delete mReturnButton;

	if (mMenuButton)
		delete mMenuButton;

	if (mBackgroundImage)
		delete mBackgroundImage;

	if (mOverlay)
		delete mOverlay;
}

void Sexy::PetsScreen::AddedToManager(WidgetManager* theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);
	for (int i = 0; i < GameObject::PET_END; i++)
		theWidgetManager->AddWidget(mPetButtons[i]);

	theWidgetManager->AddWidget(mOverlay);
	theWidgetManager->AddWidget(mReturnButton);
	if(mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)
		theWidgetManager->AddWidget(mMenuButton);
}

void Sexy::PetsScreen::RemovedFromManager(WidgetManager* theWidgetManager)
{
	Widget::RemovedFromManager(theWidgetManager);
	for (int i = 0; i < GameObject::PET_END; i++)
		theWidgetManager->RemoveWidget(mPetButtons[i]);

	theWidgetManager->RemoveWidget(mOverlay);
	theWidgetManager->RemoveWidget(mReturnButton);
	if (mApp->mGameMode != WinFishApp::GAMEMODE_VIRTUAL_TANK)
		theWidgetManager->RemoveWidget(mMenuButton);
}

void Sexy::PetsScreen::Update()
{
	if (m0x134 != 0 && --m0x134 == 0)
		m0x128 = m0x12c;

	m0x120++;
	MarkDirty();
}

void Sexy::PetsScreen::DrawOverlay(Graphics* g)
{
	g->DrawImage(mBackgroundImage, 0, 0);
	for (int i = 0; i < GameObject::PET_END; i++)
	{
		g->DrawImage(IMAGE_PETBUTTONRING, mPetButtons[i]->mX, mPetButtons[i]->mY);
		if(!mPetButtons[i]->mDisabled)
			g->DrawImage(IMAGE_PETBUTTONREFLECT, mPetButtons[i]->mX, mPetButtons[i]->mY);
	}

	g->SetFont(FONT_JUNGLEFEVER17OUTLINE);
	g->SetColor(Color(0xff, 200, 0));
	if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
		g->DrawString("Add/Remove Pets", 215, 25);
	else
		g->DrawString("Choose Your Pets", 215, 25);

	if (m0x134 < 1 || m0x134 > 4)
	{
		DrawPetInfo(g, m0x128, 255);
	}
	else
	{
		DrawPetInfo(g, m0x128, (m0x134 * 255) / 5);
		if(m0x12c != -1)
			DrawPetInfo(g, m0x12c, 255 - (m0x134 * 255) / 5);
	}

	SexyString aStr1 = "";
	if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
		aStr1 = "to display in your tank";
	else
		aStr1 = "to take to the next level";

	int aFreeSlots = m0x130 - m0x124;
	SexyString aStr2 = "";
	if (aFreeSlots > 1)
		aStr2 = StrFormat("Choose %d more pets", aFreeSlots);
	else if (aFreeSlots == 1)
		aStr2 = "Choose 1 more pet";
	else
		aStr1 = "";

	g->SetFont(FONT_TINY);
	g->SetColor(Color::White);
	int aStrWdth = g->GetFont()->StringWidth(aStr2);
	g->DrawString(aStr2, (650 - aStrWdth) / 2 - 3, 75);
	aStrWdth = g->GetFont()->StringWidth(aStr1);
	g->DrawString(aStr1, (650 - aStrWdth) / 2 - 3, 90);
}

void Sexy::PetsScreen::ButtonPress(int theId)
{
	mApp->PlaySample(SOUND_BUTTONCLICK);
}

void Sexy::PetsScreen::ButtonDepress(int theId)
{
	if (theId < 24)
	{
		if (mPetButtons[theId]->m0x130)
			m0x12c = theId;
		else if(m0x12c == theId)
		{
			m0x12c = -1;
			for (int i = 0; i < 24; i++)
			{
				if (mPetButtons[i]->m0x130)
					m0x12c = i;
			}
		}
	}
	else
	{
		if (theId == 99)
		{
			bool aFlag = false;
			if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
			{
				for (int i = 0; i < 24; i++)
				{
					if (m0x100[i] != mPetButtons[i]->m0x130)
					{
						aFlag = true;
						if (!mPetButtons[i]->m0x130)
						{
							for (auto it = mApp->mBoard->mGameObjectSet.begin(); it != mApp->mBoard->mGameObjectSet.end(); ++it)
							{
								GameObject* aCurObj = *it;
								if (aCurObj->mVirtualTankId == i + 1000)
								{
									aCurObj->RemoveHelper02(false);
									mApp->SafeDeleteWidget(aCurObj);
									break;
								}
							}
						}
						else
							mApp->mBoard->SpawnPet(i, -1, -1, false, false);
					}
				}
			}
			else
			{
				for (int i = 0; i < 24; i++)
					mApp->mCurrentProfile->m0x5a[i] = mPetButtons[i]->m0x130;
			}

			if (m0x124 > 2 || mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK)
			{
				mApp->RemovePetsScreen();
				if (aFlag)
					mApp->SaveVirtualTankAndUserData();

				if (mApp->mBoard == nullptr)
					mApp->StartGame();
				else
				{
					mApp->mBoard->StartMusic();
					mApp->mBoard->PauseGame(false);
				}
			}
			else if(m0x130 < 4)
			{
				mApp->DoDialog(20, true, "Select More Pets?", 
					"Are you sure you want to continue to the next level without selecting 3 pets?", 
					"", Dialog::BUTTONS_YES_NO);
			}
			else
			{
				mApp->DoDialog(20, true, "Select More Pets?", 
					"Are you sure you want to continue to the next level without selecting at least 3 pets?", 
					"", Dialog::BUTTONS_YES_NO);
			}
		}
		else if (theId == 100)
		{
			mApp->RemovePetsScreen();
			mApp->SwitchToGameSelector();
		}
	}
}

void Sexy::PetsScreen::ButtonMouseEnter(int theId)
{
	if (theId < 24)
	{
		m0x128 = theId;
		m0x134 = 0;
	}
}

void Sexy::PetsScreen::ButtonMouseLeave(int theId)
{
	if (theId < 24 && !mPetButtons[theId]->m0x130)
		m0x134 = 7;
}

void Sexy::PetsScreen::DrawPetInfo(Graphics* g, int thePetId, int theAlpha)
{
	SexyString& aPetName = GetPetName(thePetId);
	SexyString aStr1, aStr2, aStr3;
	int aXDrawPet = 280;
	int aYDrawPet = 90;
	switch (thePetId)
	{
	case GameObject::PET_STINKY:
		aStr1 = "STINKY roams around the";
		aStr2 = "bottom of your tank, catching";
		aStr3 = "any coins you may have missed.";
		break;
	case GameObject::PET_NIKO:
		aStr1 = "NIKO produces pearls that";
		aStr2 = "you can click on for a";
		aStr3 = "hefty sum of money.";
		aYDrawPet = 85;
		break;
	case GameObject::PET_ITCHY:
		aStr1 = "ITCHY helps you by attacking";
		aStr2 = "aliens when they appear.";
		aStr3 = "";
		break;
	case GameObject::PET_PREGO:
		aStr1 = "PREGO helps populate your";
		aStr2 = "tank by giving birth to a new";
		aStr3 = "baby guppy every so often.";
		break;
	case GameObject::PET_ZORF:
		aStr1 = "ZORF gives you a hand in";
		aStr2 = "keeping your fish fed.";
		aStr3 = "";
		break;
	case GameObject::PET_CLYDE:
		aStr1 = "CLYDE drifts slowly through";
		aStr2 = "your tank, collecting any";
		aStr3 = "coins it passes by.";
		break;
	case GameObject::PET_VERT:
		aStr1 = "VERT drops gold coins just like";
		aStr2 = "a large guppy, but doesn't need";
		aStr3 = "fish food to survive.";
		break;
	case GameObject::PET_RUFUS:
		aStr1 = "RUFUS does heavy damage to";
		aStr2 = "enemies you've lured to the";
		aStr3 = "bottom of the tank.";
		break;
	case GameObject::PET_MERYL:
		aStr1 = "MERYL's song cheers up all the";
		aStr2 = "guppies in the tank, making";
		aStr3 = "them drop coins faster.";
		break;
	case GameObject::PET_WADSWORTH:
		aStr1 = "WADSWORTH helps by sheltering";
		aStr2 = "your baby and medium guppies";
		aStr3 = "from hungry aliens.";
		break;
	case GameObject::PET_SEYMOUR:
		aStr1 = "SEYMOUR's presence makes all";
		aStr2 = "coins and diamonds drift";
		aStr3 = "at a slower rate.";
		break;
	case GameObject::PET_SHRAPNEL:
		aStr1 = "SHRAPNEL drops bombs that";
		aStr2 = "blow up fish on contact but";
		aStr3 = "give lots of cash when clicked.";
		break;
	case GameObject::PET_GUMBO:
		aStr1 = "GUMBO attracts guppies using";
		aStr2 = "the lantern on his head,";
		aStr3 = "luring them away from aliens.";
		break;
	case GameObject::PET_BLIP:
		aStr1 = "BLIP provides you with info";
		aStr2 = "that helps you better combat";
		aStr3 = "aliens and keep your fish fed.";
		break;
	case GameObject::PET_RHUBARB:
		aStr1 = "RHUBARB snaps his claws at";
		aStr2 = "fish, keeping them off the";
		aStr3 = "bottom of your tank.";
		break;
	case GameObject::PET_NIMBUS:
		aStr1 = "NIMBUS tosses any coins or";
		aStr2 = "food he catches back up";
		aStr3 = "toward the top of the tank.";
		break;
	case GameObject::PET_AMP:
		aStr1 = "AMP can electrocute your";
		aStr2 = "entire tank, killing your fish";
		aStr3 = "and turning them into diamonds.";
		aXDrawPet = 240;
		break;
	case GameObject::PET_GASH:
		aStr1 = "GASH viciously attacks aliens,";
		aStr2 = "but will snack on one of";
		aStr3 = "your guppies from time to time.";
		break;
	case GameObject::PET_ANGIE:
		aStr1 = "ANGIE has the ability to";
		aStr2 = "resurrect dead fish.";
		aStr3 = "";
		break;
	case GameObject::PET_PRESTO:
		aStr1 = "Change PRESTO into";
		aStr2 = "any of your other pets";
		aStr3 = "by right-clicking on him.";
		break;
	case GameObject::PET_WALTER:
		aStr1 = "Likes: peach muffins, all";
		aStr2 = "things brown and sticky";
		aStr3 = "Dislikes: arugula";
		break;
	case GameObject::PET_NOSTRADAMUS:
		aStr1 = "Little known fact:  NOSTRADAMUS";
		aStr2 = "is the nose of ex-president";
		aStr3 = "Rutherford B. Hayes";
		break;
	case GameObject::PET_STANLEY:
		aStr1 = "STANLEY knows no fear.. except";
		aStr2 = "that of badgers, aprons,";
		aStr3 = "and badgers wearing aprons.";
		break;
	case GameObject::PET_BRINKLEY:
		aStr1 = "Choosing this pet donates all";
		aStr2 = "proceeds to the Falafel";
		aStr3 = "Foundation. Free the falafels!";
		break;
	default:
		aStr1 = "";
		aStr2 = "";
		aStr3 = "";
		break;
	}

	if (thePetId >= GameObject::PET_STINKY)
	{
		if (theAlpha != 255)
		{
			g->SetColorizeImages(true);
			g->SetColor(Color(255,255,255,theAlpha));
		}
		int aCol = (m0x120 % 20) / 2;
		if (thePetId == GameObject::PET_NIKO || thePetId == GameObject::PET_VERT)
		{
			aCol = m0x120 % 18;
			if (aCol > 9)
				aCol = 18 - aCol;
		}
		else if (thePetId == GameObject::PET_SEYMOUR || thePetId == GameObject::PET_CLYDE || thePetId == GameObject::PET_SHRAPNEL || thePetId == GameObject::PET_RHUBARB)
		{
			aCol = (m0x120 % 40) / 4;
		}

		Image* anImg = GetImageById(thePetId + IMAGE_STINKY_ID);
		g->DrawImageCel(anImg, aXDrawPet, aYDrawPet, aCol, 0);
		g->SetColorizeImages(false);
		g->SetFont(FONT_JUNGLEFEVER15OUTLINE);
		g->SetColor(Color(0xff, 200, 0, theAlpha));
		
		int aStrWdth = g->GetFont()->StringWidth(aPetName);
		g->DrawString(aPetName, (650 - aStrWdth) / 2 - 3, 180);
	}

	g->SetFont(FONT_TINY);
	g->SetColor(Color(255, 255, 255, theAlpha));

	int aStrWdth = g->GetFont()->StringWidth(aStr1);
	g->DrawString(aStr1, (650 - aStrWdth) / 2 - 3, 200);

	aStrWdth = g->GetFont()->StringWidth(aStr2);
	g->DrawString(aStr2, (650 - aStrWdth) / 2 - 3, 215);

	aStrWdth = g->GetFont()->StringWidth(aStr3);
	g->DrawString(aStr3, (650 - aStrWdth) / 2 - 3, 230);
}

void Sexy::PetScreenOverlay::Draw(Graphics* g)
{
	mScreen->DrawOverlay(g);
}
