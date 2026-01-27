#include "HatchScreen.h"
#include "WinFishAppCommon.h"
#include "Res.h"
#include "SexyAppFramework/DialogButton.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Font.h"

Sexy::HatchScreen::HatchScreen(WinFishApp* theApp, int thePetId)
{
	mApp = theApp;
	mBubbleMgr = new BubbleMgr();

	mBubbleMgr->mBubbleBounds = Rect(190, 220, 258, 200);
	mBubbleMgr->SetBubbleConfig(10, 5);
	mBubbleMgr->UpdateALot();
	m0x94 = 0;
	mPetId = thePetId;
	theApp->m0x884 = true;
	mX = 0;
	mY = 0;
	mWidth = mApp->mWidth;
	mHeight = mApp->mHeight;

	mContinueButton = MakeDialogButton2(99, this, "Please Wait...", IMAGE_MAINBUTTON);
	mContinueButton->SetFont(FONT_JUNGLEFEVER12OUTLINE);
	mContinueButton->mMouseVisible = false;
	mContinueButton->mColors[0] = Color(255, 240, 0, 255);
	mContinueButton->Resize(186, 445, 264, mContinueButton->mHeight);
	mApp->mWidgetManager->BringToFront(mContinueButton);

	mMenuButton = MakeDialogButton2(100, this, "Menu", IMAGE_MAINBUTTON);
	mMenuButton->Resize(525, 4, 80, mMenuButton->mHeight);
	if (mApp->mCurrentProfile->mTank == 5 && mApp->mCurrentProfile->mLevel == 2)
		mMenuButton->mVisible = false;

	mOverlay = new HatchScreenOverlay();
	mOverlay->mScreen = this;
	mOverlay->mMouseVisible = false;
	mOverlay->mHasAlpha = true;
	mOverlay->mWidth = 640;
	mOverlay->mHeight = 480;

	m0x9c = 0;
	m0xa0 = 0;
	m0xa4 = 480;
	mApp->StopMusic();
	mApp->PlayMusic(2, 54);
}

Sexy::HatchScreen::~HatchScreen()
{
	if (mBubbleMgr)
		delete mBubbleMgr;
	if (mContinueButton)
		delete mContinueButton;
	if (mMenuButton)
		delete mMenuButton;
	if (mOverlay)
		delete mOverlay;
}


void Sexy::HatchScreen::AddedToManager(WidgetManager* theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mContinueButton);
	theWidgetManager->AddWidget(mMenuButton);
	theWidgetManager->AddWidget(mOverlay);
}

void Sexy::HatchScreen::RemovedFromManager(WidgetManager* theWidgetManager)
{
	Widget::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mContinueButton);
	theWidgetManager->RemoveWidget(mMenuButton);
	theWidgetManager->RemoveWidget(mOverlay);
}

void Sexy::HatchScreen::Update()
{
	mBubbleMgr->Update();
	MarkDirty();
	m0x9c = mApp->mSeed->Next() % 4;
	m0xa0 = mApp->mSeed->Next() % 2;

	if (!mIsDown || m0x94 > 139)
	{
		if (m0x94 == 141)
		{
			if (mApp->mCurrentProfile->mTank == 5 && mApp->mCurrentProfile->mLevel == 1)
				mApp->PlaySample(SOUND_EVILLAFF);
			else
				mApp->PlaySample(SOUND_HATCH);
		}
	}
	else
	{
		m0x94 = 140;
	}

	if (mApp->mGameMode == WinFishApp::GAMEMODE_TIME_TRIAL)
	{
		if (m0x94 == 28 || m0x94 == 48 || m0x94 == 68)
			mApp->PlaySample(SOUND_BUY);
		if(m0x94 == 88)
			mApp->PlaySample(SOUND_GROW);
	}

	if (m0x94 >= 220 && mApp->mCurrentProfile->mTank == 5 && mApp->mCurrentProfile->mLevel == 1)
	{
		int aVal = m0x94 % 16;
		if (aVal <= 3)
			m0xa4--;
		else if (aVal > 7 && aVal < 12)
			m0xa4++;
	}
	else if(m0x94 >= 140 && mApp->mCurrentProfile->mTank == 5 && mApp->mCurrentProfile->mLevel == 1)
	{
		if (m0x94 < 161)
			m0xa4 -= 12;
		else if (m0x94 < 171)
			m0xa4 -= 8;
		else if (m0x94 < 181)
			m0xa4 -= 5;
		else if (m0x94 < 191)
			m0xa4 -= 2;
		else
			m0xa4--;
	}

	if (m0x94 == 170)
	{
		mContinueButton->mLabel = "Click Here to Continue";
		mContinueButton->mMouseVisible = true;
	}

	m0x94++;
}

void Sexy::HatchScreen::Draw(Graphics* g)
{
	g->DrawImage(IMAGE_HATCHSCREEN, 0, 0);
	g->DrawImageBox(Rect(20, 0, 600, IMAGE_SCREENTITLE->mHeight), IMAGE_SCREENTITLE);

	if (mMenuButton->mVisible)
		g->DrawImageBox(Rect(mMenuButton->mX - 1, mMenuButton->mY - 1, mMenuButton->mWidth + 2, IMAGE_SCREENTITLEHOLE->mHeight), IMAGE_SCREENTITLEHOLE);
	
	g->SetFont(FONT_JUNGLEFEVER17OUTLINE);
	g->SetColor(Color(0xff, 200, 0, 0xff));

	g->DrawString("You have found:", 215, 25);

	int aShakeTimer = m0x94 % 32;
	int anYOffset = 0;
	if (aShakeTimer < 4)
		anYOffset = 0;
	else if (aShakeTimer < 8)
		anYOffset = -1;
	else if (aShakeTimer < 12)
		anYOffset = -2;
	else if (aShakeTimer < 16)
		anYOffset = -1;
	else if (aShakeTimer < 20)
		anYOffset = 0;
	else if (aShakeTimer < 24)
		anYOffset = 1;
	else if (aShakeTimer < 28)
		anYOffset = 2;
	else if (aShakeTimer < 32)
		anYOffset = 1;

	Graphics g2(*g);
	if (m0x94 < 81)
	{
		int aCel = 0;
		if (m0x94 > 55)
			aCel = (m0x94 - 56) / 2;
		g->DrawImageCel(IMAGE_EGGCRACK1, 258, anYOffset + 60, aCel);
	}
	else // 136 - 364
	{
		if (mApp->mCurrentProfile->mTank == 5 && mApp->mCurrentProfile->mLevel == 1)
		{
			int aCel = 0;
			if (m0x94 >= 140)
				aCel = 9;
			else if (m0x94 > 119)
				aCel = (m0x94 - 120) / 2;

			if (m0x94 > 160)
			{
				m0x9c = 0;
				m0xa0 = 0;
			}
			g->DrawImageCel(IMAGE_EGGCRACK2, m0x9c + 258, m0xa0 + 60 + anYOffset, aCel);
		}
		else if (m0x94 < 141)
		{
			int aCel = 0;
			if (m0x94 >	 119)
				aCel = (m0x94 - 120) / 2;
			g->DrawImageCel(IMAGE_EGGCRACK2, m0x9c + 258, m0xa0 + 60 + anYOffset, aCel);
		}
		else // 166
		{
			if (mPetId == GameObject::PET_AMP)
			{
				g2.ClipRect(236, 100, 160, 60);
			}
			else
			{
				int anYClip = 0;
				if (mPetId == GameObject::PET_CLYDE || mPetId == GameObject::PET_STINKY)
					anYClip = 100;
				else if (mPetId == GameObject::PET_NIKO)
					anYClip = 70;
				else
					anYClip = 90;
				g2.ClipRect(278, anYClip, 80, 80);
			}
			int aXOffset = (m0x94 % 20) / 2;
			if (mPetId == GameObject::PET_SEYMOUR || mPetId == GameObject::PET_SHRAPNEL ||
				mPetId == GameObject::PET_CLYDE || mPetId == GameObject::PET_RHUBARB || mPetId == GameObject::PET_BRINKLEY)
				aXOffset = (m0x94 % 40) / 4;
			else if (mPetId == GameObject::PET_VERT || mPetId == GameObject::PET_NIKO)
			{
				aXOffset = m0x94 % 20;
				if (aXOffset > 9)
					aXOffset = 19 - aXOffset;
			}
			Image* anImage = IMAGE_STINKY;
			int aX = 278;
			int aY = 90;
			if (mPetId != GameObject::PET_AMP)
				aXOffset *= 80;
			else
				aXOffset *= 160;

			switch (mPetId)
			{
			case GameObject::PET_STINKY:
				aY = 100;
				anImage = IMAGE_STINKY;
				break;
			case GameObject::PET_NIKO:
				aY = 70;
				anImage = IMAGE_NIKO;
				break;
			case GameObject::PET_ITCHY:
				anImage = IMAGE_ITCHY;
				break;
			case GameObject::PET_PREGO:
				anImage = IMAGE_PREGO;
				break;
			case GameObject::PET_ZORF:
				anImage = IMAGE_ZORF;
				break;
			case GameObject::PET_CLYDE:
				aY = 100;
				anImage = IMAGE_CLYDE;
				break;
			case GameObject::PET_VERT:
				anImage = IMAGE_VERT;
				break;
			case GameObject::PET_RUFUS:
				anImage = IMAGE_RUFUS;
				break;
			case GameObject::PET_MERYL:
				anImage = IMAGE_MERYL;
				break;
			case GameObject::PET_WADSWORTH:
				anImage = IMAGE_WADSWORTH;
				break;
			case GameObject::PET_SEYMOUR:
				anImage = IMAGE_SEYMOUR;
				break;
			case GameObject::PET_SHRAPNEL:
				anImage = IMAGE_SHRAPNEL;
				break;
			case GameObject::PET_GUMBO:
				anImage = IMAGE_GUMBO;
				break;
			case GameObject::PET_BLIP:
				anImage = IMAGE_BLIP;
				break;
			case GameObject::PET_RHUBARB:
				anImage = IMAGE_RHUBARB;
				break;
			case GameObject::PET_NIMBUS:
				anImage = IMAGE_NIMBUS;
				break;
			case GameObject::PET_AMP:
				aY = 100;
				aX = 236;
				anImage = IMAGE_AMP;
				break;
			case GameObject::PET_GASH:
				anImage = IMAGE_GASH;
				break;
			case GameObject::PET_ANGIE:
				anImage = IMAGE_ANGIE;
				break;
			case GameObject::PET_PRESTO:
				anImage = IMAGE_PRESTO;
				break;
			case GameObject::PET_BRINKLEY:
				anImage = IMAGE_BRINKLEY;
				break;
			case GameObject::PET_NOSTRADAMUS:
				anImage = IMAGE_NOSTRADAMUS;
				break;
			case GameObject::PET_STANLEY:
				anImage = IMAGE_STANLEY;
				break;
			case GameObject::PET_WALTER:
				anImage = IMAGE_WALTER;
				break;
			}

			g2.DrawImage(anImage, aX - aXOffset, aY);
			g2.SetColorizeImages(false);

			SexyString aPetDesc = "";
			switch (mPetId)
			{
			case GameObject::PET_STINKY:
				aPetDesc = "STINKY the Snail";
				break;
			case GameObject::PET_NIKO:
				aPetDesc = "NIKO the Oyster";
				break;
			case GameObject::PET_ITCHY:
				aPetDesc = "ITCHY the Swordfish";
				break;
			case GameObject::PET_PREGO:
				aPetDesc = "PREGO the Momma Fish";
				break;
			case GameObject::PET_ZORF:
				aPetDesc = "ZORF the Sea Horse";
				break;
			case GameObject::PET_CLYDE:
				aPetDesc = "CLYDE the Jellyfish";
				break;
			case GameObject::PET_VERT:
				aPetDesc = "VERT the Skeleton";
				break;
			case GameObject::PET_RUFUS:
				aPetDesc = "RUFUS the Fiddler Crab";
				break;
			case GameObject::PET_MERYL:
				aPetDesc = "MERYL the Mermaid";
				break;
			case GameObject::PET_WADSWORTH:
				aPetDesc = "WADSWORTH the Whale";
				break;
			case GameObject::PET_SEYMOUR:
				aPetDesc = "SEYMOUR the Turtle";
				break;
			case GameObject::PET_SHRAPNEL:
				aPetDesc = "SHRAPNEL the Robot Fish";
				break;
			case GameObject::PET_GUMBO:
				aPetDesc = "GUMBO the Angler";
				break;
			case GameObject::PET_BLIP:
				aPetDesc = "BLIP the Porpoise";
				break;
			case GameObject::PET_RHUBARB:
				aPetDesc = "RHUBARB the Hermit Crab";
				break;
			case GameObject::PET_NIMBUS:
				aPetDesc = "NIMBUS the Manta Ray";
				break;
			case GameObject::PET_AMP:
				aPetDesc = "AMP the Electric Eel";
				break;
			case GameObject::PET_GASH:
				aPetDesc = "GASH the Shark";
				break;
			case GameObject::PET_ANGIE:
				aPetDesc = "ANGIE the Angelfish";
				break;
			case GameObject::PET_PRESTO:
				aPetDesc = "PRESTO the Tadpole";
				break;
			case GameObject::PET_BRINKLEY: // 20 (0x14)
				aPetDesc = "BRINKLEY";
				break;
			case GameObject::PET_NOSTRADAMUS:
				aPetDesc = "NOSTRADAMUS the Nose";
				break;
			case GameObject::PET_STANLEY:
				aPetDesc = "STANLEY the Startlingly";
				break;
			case GameObject::PET_WALTER:
				aPetDesc = "WALTER the Penguin";
				break;
			case 999:
				aPetDesc = "Evil Alien Mastermind";
				break;
			default:
				break;
			}
			g->SetColor(Color(255, 200, 0));
			g->SetFont(FONT_JUNGLEFEVER15OUTLINE);
			int aYDesc = 260;
			if (mPetId == GameObject::PET_RHUBARB || mPetId == GameObject::PET_WADSWORTH ||
				mPetId == GameObject::PET_SHRAPNEL || mPetId == GameObject::PET_NOSTRADAMUS)
				g->SetFont(FONT_JUNGLEFEVER12OUTLINE);
			else if (mPetId == GameObject::PET_BRINKLEY || mPetId == GameObject::PET_STANLEY)
			{
				g->SetFont(FONT_JUNGLEFEVER12OUTLINE);
				SexyString aMoreDesc = "";
				if (mPetId == GameObject::PET_BRINKLEY)
					aMoreDesc = "the Scuba Diving Elephant";
				else if (mPetId == GameObject::PET_STANLEY)
					aMoreDesc = "Small Sea Serpent";

				int aStrWdth = g->GetFont()->StringWidth(aMoreDesc);
				g->DrawString(aMoreDesc, (650 - aStrWdth) / 2 - 3, 265);
				aYDesc = 245;
			}

			int aStrWdth = g->GetFont()->StringWidth(aPetDesc);
			g->DrawString(aPetDesc, (650 - aStrWdth) / 2 - 3, aYDesc);
		}
	} // 364

	if (m0x94 > 170)
	{
		g->SetColor(Color::White);
		g->SetFont(FONT_JUNGLEFEVER10OUTLINE);

		SexyString aStr1 = "";
		SexyString aStr2 = "";
		SexyString aStr3 = "";
		SexyString aStr4 = "";

		switch (mPetId)
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
			break;
		case GameObject::PET_ITCHY:
			aStr1 = "ITCHY helps you by attacking";
			aStr2 = "aliens when they appear.";
			break;
		case GameObject::PET_PREGO:
			aStr1 = "PREGO helps populate your";
			aStr2 = "tank by giving birth to a new";
			aStr3 = "baby guppy every so often.";
			break;
		case GameObject::PET_ZORF:
			aStr1 = "ZORF gives you a hand in";
			aStr2 = "keeping your fish fed.";
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
			break;
		case GameObject::PET_GASH:
			aStr1 = "GASH viciously attacks aliens,";
			aStr2 = "but will snack on one of";
			aStr3 = "your guppies from time to time.";
			break;
		case GameObject::PET_ANGIE:
			aStr1 = "ANGIE has the ability to";
			aStr2 = "resurrect dead fish.";
			break;
		case GameObject::PET_PRESTO:
			aStr1 = "PRESTO has the ability to";
			aStr2 = "metamorph into any of";
			aStr3 = "your other pets.";
			break;
		case GameObject::PET_BRINKLEY:
			aStr1 = "Likes: peach muffins, all";
			aStr2 = "things brown and sticky";
			aStr3 = "Dislikes: arugula";
			break;
		case GameObject::PET_NOSTRADAMUS:
			aStr1 = "Little known fact:  NOSTRADAMUS";
			aStr2 = "is the long lost nose of ex-president";
			aStr3 = "Rutherford B. Hayes";
			break;
		case GameObject::PET_STANLEY:
			aStr1 = "STANLEY knows no fear.. except";
			aStr2 = "that of badgers, aprons,";
			aStr3 = "and badgers wearing aprons.";
			break;
		case GameObject::PET_WALTER:
			aStr1 = "Choosing this pet donates all";
			aStr2 = "proceeds to the Falafel";
			aStr3 = "Foundation. Free the falafels!";
			break;
		case 999:
			aStr1 = "EVIL ALIEN MASTERMIND actually";
			aStr2 = "isn't very helpful at all.  Unless";
			aStr3 = "you consider devouring the entire";
			aStr4 = "contents of your fishtank helpful.";
			g->SetColor(Color::White);
			g->DrawString(aStr4, (650 - g->GetFont()->StringWidth(aStr4)) / 2 - 3, 360);
			break;
		default:
			break;
		}

		g->DrawString(aStr1, (650 - g->GetFont()->StringWidth(aStr1)) / 2 - 3, 300);
		g->DrawString(aStr2, (650 - g->GetFont()->StringWidth(aStr2)) / 2 - 3, 320);
		g->DrawString(aStr3, (650 - g->GetFont()->StringWidth(aStr3)) / 2 - 3, 340);

		g->SetColor(Color(255, 255, 100));

		if (mPetId == GameObject::PET_PRESTO)
		{
			WriteWordWrapped(g, Rect(180, 373, 284, 0), "You have been awarded 5000 shells\nfor defeating the Final Boss!", -1, 0);
		}
		else
		{
			g->DrawString("Your game has been saved.", 221, 390);
		}

		if (mPetId == 999)
		{
			g->SetColor(Color(255, 200, 0));
			SexyString aStr = "Evil Alien Mastermind";
			g->SetFont(FONT_JUNGLEFEVER15OUTLINE);
			int aStrWdth = g->GetFont()->StringWidth(aStr);
			g->DrawString(aStr, (650 - aStrWdth) / 2 - 3, 260);
		}
	}
}

void Sexy::HatchScreen::DrawOverlay(Graphics* g)
{
	g->DrawImage(IMAGE_HATCHREFLECTION, 240, 60);
	if (mApp->mCurrentProfile->mTank == 5 && mApp->mCurrentProfile->mLevel == 1)
	{
		if (m0x94 > 139)
		{
			Graphics g2(*g);
			g2.ClipRect(238, m0xa4, 160, 160);
			int aFrame = (m0x94 % 20) / 2;
			g2.DrawImage(IMAGE_BOSS, 238 - (aFrame * 160), m0xa4);
		}
	}
	else if(m0x94 > 139 && m0x94 < 200)
	{
		int aShardValues[16] = { -10, -15, 3, -13, 10, 5, 14, 12, 1, -11, -14, 12, -15, 11, 13, -1 };

		for (int i = 0; i < 8; i++)
		{
			int anOffset = m0x94 - 140;
			g->DrawImageCel(IMAGE_EGGSHARDS, aShardValues[i + 8] * anOffset + 268, aShardValues[i] * anOffset + 70, i);
		}
	}
}

void Sexy::HatchScreen::ButtonPress(int theId)
{
	mApp->PlaySample(SOUND_BUTTONCLICK);
}

void Sexy::HatchScreen::ButtonDepress(int theId)
{
	if (theId == 99)
	{
		mApp->RemoveHatchScreen();
		if (mApp->mGameMode == WinFishApp::GAMEMODE_TIME_TRIAL || mApp->mGameMode == WinFishApp::GAMEMODE_CHALLENGE)
			mApp->LeaveGameBoard();
		else if (mApp->mCurrentProfile->mTank != 5 || mApp->mCurrentProfile->mLevel != 2)
			mApp->SwitchToBoard(false, false);
		else
			mApp->SwitchToInterludeScreen();
	}
	else if (theId == 100)
	{
		mApp->RemoveHatchScreen();
		if (mApp->mCurrentProfile->mTank == 5 && mApp->mCurrentProfile->mLevel == 2)
			mApp->SwitchToBoard(false, false);
		else
			mApp->SwitchToGameSelector();
	}
}

void Sexy::HatchScreenOverlay::Draw(Graphics* g)
{
	mScreen->DrawOverlay(g);
}
