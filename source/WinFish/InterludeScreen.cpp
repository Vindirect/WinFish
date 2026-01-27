#include "InterludeScreen.h"
#include "WinFishApp.h"
#include "BubbleMgr.h"
#include "Res.h"

#include "SexyAppFramework/HyperlinkWidget.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Font.h"

bool Sexy::gInterludeBool01 = 1;
int Sexy::gInterludeInt02 = 0;
int Sexy::gInterludeInt03 = 0;
int Sexy::gInterludeArray01[15] = {0};

Sexy::InterludeScreen::InterludeScreen(WinFishApp* theApp, int unk)
{
	mApp = theApp;
	m0xa4 = 0;
	m0xa0 = unk;

	mBubbleMgr1 = new BubbleMgr();
	mBubbleMgr1->mBubbleBounds = Rect(0, -20, 640, 520);
	mBubbleMgr1->SetBubbleConfig(10, 3);
	mBubbleMgr1->UpdateALot();

	mBubbleMgr2 = new BubbleMgr();
	mBubbleMgr2->mBubbleBounds = Rect(0, -20, 640, 520);
	mBubbleMgr2->SetBubbleConfig(10, 3);
	mBubbleMgr2->SetDefaultBubbleFishVY(-1.0f);
	mBubbleMgr2->mClipBubbleFish = false;
	mBubbleMgr2->UpdateALot();

	mX = 0;
	mY = 0;
	mWidth = mApp->mWidth;
	mHeight = mApp->mHeight;

	m0xb4 = 0;
	m0xac = 540;
	m0xb0 = 35;
	m0xa8 = 0;

	mBackButton = new HyperlinkWidget(0, this);
	mBackButton->mLabel = "Back To Main Menu";
	mBackButton->SetFont(FONT_JUNGLEFEVER10OUTLINE);
	mBackButton->mColor = Color(0x808080);
	mBackButton->mOverColor = Color::White;
	mBackButton->mUnderlineSize = 0;
	mBackButton->Resize(490, 443, 150, 33);
	int aStrWdth = mBackButton->mFont->StringWidth(mBackButton->mLabel);
	int aStrHght = mBackButton->mFont->GetHeight();
	mBackButton->Resize(630 - (aStrWdth + 20), 460 - aStrHght/2, aStrWdth + 20, aStrHght);

	mOverlay = new InterludeScreenOverlay();
	mOverlay->mScreen = this;
	mOverlay->mHasAlpha = true;
	mOverlay->mMouseVisible = false;
	mOverlay->Resize(0, 0, 640, 480);

	if(gPetsDiedOnBossLevel < 24)
	{
		for (int i = gPetsDiedOnBossLevel; i < 24; i++)
			gDeadPetsIdArray[i] = -1;
	}

	gInterludeBool01 = true;
	if (gPetsDiedOnBossLevel < 4)
	{
		gDeadPetsIdArray[3] = gDeadPetsIdArray[2];
		gDeadPetsIdArray[2] = gDeadPetsIdArray[1];
		gDeadPetsIdArray[1] = gDeadPetsIdArray[0];
		gDeadPetsIdArray[0] = -1;
	}

	gInterludeInt02 = 0;
	gInterludeInt03 = m0xb0 * 6 + 7225 + m0xb0 * 6;
}

Sexy::InterludeScreen::~InterludeScreen()
{
	if (mBubbleMgr1)
		delete mBubbleMgr1;
	if (mBubbleMgr2)
		delete mBubbleMgr2;
	if (mBackButton)
		delete mBackButton;
	if (mOverlay)
		delete mOverlay;
}

void Sexy::InterludeScreen::AddedToManager(WidgetManager* theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mOverlay);
	theWidgetManager->AddWidget(mBackButton);
}

void Sexy::InterludeScreen::RemovedFromManager(WidgetManager* theWidgetManager)
{
	Widget::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mOverlay);
	theWidgetManager->RemoveWidget(mBackButton);
}

void Sexy::InterludeScreen::Update()
{
	Widget::Update();
	if (mUpdateCnt == 1)
	{
		mApp->StopMusic();
		mApp->PlayMusic(2, 5);
	}

	if (!mApp->mHasFocus)
		return;

	mBubbleMgr1->Update();
	mBubbleMgr2->Update();
	MarkDirty();
	if (m0xa4 == 3000)
	{
		mApp->FadeOutMusic(2, 0, 0.008);
		mApp->FadeInMusic(3, 0, 0.002, false);
	}
	m0xb4 += 0.01;
	if (m0xb4 > 1.0)
		m0xb4 = -1.0f;

	if (m0xa4 < gInterludeInt03)
	{
		mBubbleMgr2->mBubbleFishBounds = Rect(0, 200, 640, 240);
		mBubbleMgr2->SetDefaultBubbleFishVY(-1.0);
		m0xa4++;
	}
	else
	{
		mBubbleMgr2->mBubbleFishBounds = Rect(0, 30, 640, 420);
		mBubbleMgr2->SetDefaultBubbleFishVY(0.0);
	}

	if (!gInterludeBool01)
	{
		for (int i = 0; i < 15; i++)
		{
			if (m0xa4 == gInterludeArray01[i])
				mApp->PlaySample(SOUND_HEAL);
		}
	}

	if (gInterludeInt02 > 0)
	{
		if (gInterludeInt02 <= m0xa4)
			mBubbleMgr2->SetBubbleFishConfig(3, 3);
		else
			mBubbleMgr2->SetBubbleFishConfig(0, 0);
	}
}

void Sexy::InterludeScreen::Draw(Graphics* g)
{
	g->SetColor(Color(25, 105, 195, 255));
	g->FillRect(0, 0, mWidth, mHeight);
	mBubbleMgr2->Draw(g);
	g->Translate(0, -m0xa4);
	int local10 = m0xa4 % 20;
	int ivar3 = local10 / 2;
	int ivar4 = m0xa4 % 40;
	int local8 = ivar4 / 4;
	int localc = local10;
	if (local10 > 9)
		localc = 19 - local10;

	m0xac = 540;
	m0xa8 = 0;
	DrawStringHelper(g, "Congratulations!", 0, 0, 0);
	DrawStringHelper(g, "You\'ve successfully vanquished the alien hordes!", 2, 1, 0);
	DrawStringHelper(g, "The epic conflict between fish and evil has", 2, 1, 0);
	DrawStringHelper(g, "finally come to a close.", 1, 1, 0);
	DrawStringHelper(g, "Thanks to you, aquarium owners can", 2, 1, 0);
	DrawStringHelper(g, "sleep easy tonight!", 1, 1, 0);
	DrawStringHelper(g, "Many a pet gave their lives in the final battle,", 3, 1, 0);
	DrawStringHelper(g, "but do not grieve, little one!", 1, 1, 0);
	DrawStringHelper(g, "Angie purposefully sat out of the fight, and", 2, 1, 0);
	DrawStringHelper(g, "word is she\'s got a few tricks up her sleeves...", 1, 1, 0);

	m0xac += 150;

	int aRef1 = m0xac - 535; // local2c
	int aRef2 = m0xac - 280; // local28
	int aRef3 = m0xac - 25; // local2c

	int aTimeDelta1 = m0xa4 - aRef1;
	int anAngieX1 = 640.0 - aTimeDelta1 * 3.0;

	int aTimeDelta2 = m0xa4 - aRef2; 
	int anAngieX2 = aTimeDelta2 * 3.0 - 80.0; // local1c

	int aTimeDelta3 = m0xa4 - aRef3;
	int anAngieX3 = 640.0 - aTimeDelta3 * 3.0; // local 20

	static int sInterludeArray01[15] = {
		480, 360, 240, 120, 0,
		10, 130, 250, 370, 490,
		480, 360, 240, 120, 0
	};

	if (gInterludeBool01)
	{
		double aBaseY = aRef1;
		for (int i = 0; i < 15; i++)
		{
			int aStaticVal = sInterludeArray01[i];
			int aCalculatedY = ((640.0 - (double)aStaticVal) / 3.0) + aBaseY;
			gInterludeArray01[i] = aCalculatedY;
		}

		double aBaseY2 = aRef2;
		for (int i = 0; i < 5; i++)
		{
			int aStaticVal = sInterludeArray01[i+5] + 80;
			int aResultY = (int)(((double)aStaticVal / 3.0) + aBaseY2);
			gInterludeArray01[i + 5] = aResultY;
		}

		double aBaseY3 = aRef3;
		for (int i = 0; i < 5; i++)
		{
			int aStaticVal = sInterludeArray01[i + 10];
			int aCalculatedY = ((640.0 - (double)aStaticVal) / 3.0) + aBaseY3;
			gInterludeArray01[i+10] = aCalculatedY;
		}

		for (int i = 0; i < 15; i++)
		{
			if (gDeadPetsIdArray[i] == -1)
				gInterludeArray01[i] = -100;
		}
		gInterludeBool01 = false;
	}

	static int gInterludeArray02[15] = {
		90, 70, 80, 90, 70,
		70, 90, 80, 70, 90,
		90, 70, 80, 90, 70
	};

	if (gPetsDiedOnBossLevel > 0)
	{
		DrawAngieHalo(g, anAngieX1, m0xac - 50, m0xb4, ivar3, false);
		for (int i = 0; i < 5; i++)
		{
			DrawPetAngieSpawns(g, gInterludeArray01[i], gDeadPetsIdArray[i], sInterludeArray01[i], 
				m0xac - gInterludeArray02[i], ivar3, local8, localc);
		}
	}

	if (gPetsDiedOnBossLevel > 5)
	{
		DrawAngieHalo(g, anAngieX2, m0xac + 100, m0xb4, ivar3, true);
		for (int i = 0; i < 5; i++)
		{
			DrawPetAngieSpawns(g, gInterludeArray01[i+5], gDeadPetsIdArray[i+5], sInterludeArray01[i+5],
				m0xac - gInterludeArray02[i+5] + 150, ivar3, local8, localc);
		}
	}
	
	if (gPetsDiedOnBossLevel > 10)
	{
		DrawAngieHalo(g, anAngieX3, m0xac + 250, m0xb4, ivar3, false);
		for (int i = 0; i < 5; i++)
		{
			DrawPetAngieSpawns(g, gInterludeArray01[i+ 10], gDeadPetsIdArray[i+ 10], sInterludeArray01[i+ 10],
				m0xac - gInterludeArray02[i+10] + 300, ivar3, local8, localc);
		}
	}
	int local20 = m0xac + 150;
	if (gPetsDiedOnBossLevel == 0)
	{
		DrawStringHelper(g, "Wait a minute!", 0, 1, 0);
		DrawStringHelper(g, "You didn\'t lose any pets!!", 1, 1, 0);
		DrawStringHelper(g, "How did you do that?", 1, 1, 0);
		DrawStringHelper(g, "That\'s really amazing!", 1, 1, 0);
		DrawStringHelper(g, "Seriously, though...", 3, 1, 0);
		DrawStringHelper(g, "How did you do that??", 1, 1, 0);
		DrawStringHelper(g, "Fine, keep it to yourself.", 3, 1, 0);
	}
	else if(gPetsDiedOnBossLevel == 1)
	{
		DrawStringHelper(g, "My goodness!  That\'s it?", 5, 1, 0);	
		DrawStringHelper(g, "You only lost one pet?!", 1, 1, 0);
		DrawStringHelper(g, "That\'s unbelievable!!", 1, 1, 0);
		DrawStringHelper(g, "How the heck did that happen?!", 2, 1, 0);
		DrawStringHelper(g, "You rule this game!!", 1, 1, 0);
	}
	else if (gPetsDiedOnBossLevel < 4)
	{
		DrawStringHelper(g, "Woah, hold on...", 5, 1, 0);
		DrawStringHelper(g, StrFormat("only %d pets died?!", gPetsDiedOnBossLevel), 1, 1, 0);
		DrawStringHelper(g, "That\'s incredible!!", 2, 1, 0);
		DrawStringHelper(g, "Good shooting, there!", 1, 1, 0);
	}
	else if (gPetsDiedOnBossLevel < 6)
	{
		DrawStringHelper(g, "Hmmm... actually it looks like", 5, 1, 0);
		DrawStringHelper(g, "not that many pets died.", 1, 1, 0);
		DrawStringHelper(g, "Great work!!", 2, 1, 0);
	}
	else if (gPetsDiedOnBossLevel < 11)
	{
		DrawStringHelper(g, "Also, you saved a fair number of pets yourself.", 7, 1, 0);
		DrawStringHelper(g, "Good job!", 1, 1, 0);
	}

	m0xac = local20;

	DrawStringHelper(g, "Thanks for playing!", 8, 1, 0);
	DrawStringHelper(g, "We\'ll see you next game!", 1, 1, 0);
	DrawStringHelper(g, "I N S A N I Q U A R I U M", 4, 1, 0);
	DrawStringHelper(g, "Cast", 4, 0, 0);
	DrawStringHelper(g, "Guppy", 4, 1, 1);
	g->DrawImageCel(IMAGE_SMALLSWIM, 160, m0xac - 90, ivar3, 0);
	DrawStringHelper(g, "Carnivore", 0, 1, 2);
	g->DrawImageCel(IMAGE_SMALLSWIM, 400, m0xac - 90, ivar3, 4);
	DrawStringHelper(g, "Starcatcher", 4, 1, 1);
	g->DrawImageCel(IMAGE_STARCATCHER, 160, m0xac - 90, ivar3, 0);
	DrawStringHelper(g, "Guppycruncher", 0, 1, 2);
	g->DrawImageCel(IMAGE_GRUBBER, 400, m0xac - 100, ivar3, 0);
	DrawStringHelper(g, "Beetlemuncher", 4, 1, 1);
	g->DrawImageCel(IMAGE_GEKKO, 160, m0xac - 90, ivar3, 0);
	DrawStringHelper(g, "Breeder", 0, 1, 2);
	g->DrawImageCel(IMAGE_BREEDER, 400, m0xac - 90, ivar3, 6);
	DrawStringHelper(g, "Ultravore", 5, 1, 0);
	g->DrawImageCel(IMAGE_ULTRA, 240, m0xac - 170, ivar3, 0);
	DrawStringHelper(g, "Sylvester", 8, 1, 1);
	g->DrawImageCel(IMAGE_SYLV, 120, m0xac - 180, ivar3, 0);
	DrawStringHelper(g, "Balrog", 0, 1, 2);
	g->DrawImageCel(IMAGE_BALROG, 360, m0xac - 180, ivar3, 0);
	DrawStringHelper(g, "Gus", 6, 1, 1);
	g->DrawImageCel(IMAGE_GUS, 120, m0xac - 180, ivar3, 0);
	DrawStringHelper(g, "Destructor", 0, 1, 2);
	g->DrawImageCel(IMAGE_DESTRUCTOR, 360, m0xac - 180, ivar3, 0);
	DrawStringHelper(g, "Ulysses", 6, 1, 1);
	g->DrawImageCel(IMAGE_ULYSSES, 120, m0xac - 180, local8, 0);
	DrawStringHelper(g, "Psychosquid", 0, 1, 2);
	g->DrawImageCel(IMAGE_PSYCHOSQUID, 360, m0xac - 165, ivar3, 0);

	DrawStringHelper(g, "Bilaterus", 6, 1, 1);
	g->DrawImageCel(IMAGE_BILATERUS, 255, m0xac - 90, 9, 4);
	g->DrawImageCel(IMAGE_BILATERUS, 225, m0xac - 95, 2, 7);
	g->DrawImageCel(IMAGE_BILATERUS, 195, m0xac - 100, 2, 6);
	g->DrawImageCel(IMAGE_BILATERUS, 165, m0xac - 95, 2, 6);
	g->DrawImageCel(IMAGE_BILATERUS, 135, m0xac - 85, 2, 7);
	g->DrawImageCel(IMAGE_BILATERUS, 105, m0xac - 80, 2, 6);
	g->DrawImageCel(IMAGE_BILATERUS, 75, m0xac - 85, 2, 7);
	g->DrawImageCel(IMAGE_BILATERUS, 45, m0xac - 90, localc, 2);

	DrawStringHelper(g, "Cyrax", 0, 1, 2);
	g->DrawImageCel(IMAGE_BOSS, 360, m0xac - 180, ivar3, 0);
	DrawStringHelper(g, "Stinky", 7, 1, 1);
	g->DrawImageCel(IMAGE_STINKY, 160, m0xac - 90, ivar3, 0);
	DrawStringHelper(g, "Niko", 0, 1, 2);
	g->DrawImageCel(IMAGE_NIKO, 400, m0xac - 105, localc, 0);
	DrawStringHelper(g, "Itchy", 4, 1, 1);
	g->DrawImageCel(IMAGE_ITCHY, 160, m0xac - 90, ivar3, 0);
	DrawStringHelper(g, "Prego", 0, 1, 2);
	g->DrawImageCel(IMAGE_PREGO, 400, m0xac - 105, ivar3, 0);
	DrawStringHelper(g, "Zorf", 4, 1, 1);
	g->DrawImageCel(IMAGE_ZORF, 160, m0xac - 105, ivar3, 0);
	DrawStringHelper(g, "Clyde", 0, 1, 2);
	g->DrawImageCel(IMAGE_CLYDE, 400, m0xac - 95, local8, 0);
	DrawStringHelper(g, "Vert", 4, 1, 1);
	g->DrawImageCel(IMAGE_VERT, 160, m0xac - 90, localc, 0);
	DrawStringHelper(g, "Rufus", 0, 1, 2);
	g->DrawImageCel(IMAGE_RUFUS, 400, m0xac - 90, ivar3, 0);
	DrawStringHelper(g, "Meryl", 4, 1, 1);
	g->DrawImageCel(IMAGE_MERYL, 160, m0xac - 105, ivar3, 0);
	DrawStringHelper(g, "Wadsworth", 0, 1, 2);
	g->DrawImageCel(IMAGE_WADSWORTH, 400, m0xac - 90, ivar3, 0);
	DrawStringHelper(g, "SEYMOUR", 4, 1, 1);
	g->DrawImageCel(IMAGE_SEYMOUR, 160, m0xac - 90, local8, 0);
	DrawStringHelper(g, "SHRAPNEL", 0, 1, 2);
	g->DrawImageCel(IMAGE_SHRAPNEL, 400, m0xac - 100, local8, 0);
	DrawStringHelper(g, "GUMBO", 4, 1, 1);
	g->DrawImageCel(IMAGE_GUMBO, 160, m0xac - 90, ivar3, 0);
	DrawStringHelper(g, "BLIP", 0, 1, 2);
	g->DrawImageCel(IMAGE_BLIP, 400, m0xac - 90, ivar3, 0);
	DrawStringHelper(g, "RHUBARB", 4, 1, 1);
	g->DrawImageCel(IMAGE_RHUBARB, 160, m0xac - 100, local8, 0);
	DrawStringHelper(g, "NIMBUS", 0, 1, 2);
	g->DrawImageCel(IMAGE_NIMBUS, 400, m0xac - 90, ivar3, 0);
	DrawStringHelper(g, "AMP", 4, 1, 1);
	g->DrawImageCel(IMAGE_AMP, 120, m0xac - 75, ivar3, 0);

	g->SetColorizeImages(true);
	g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
	double aVal = abs(m0xb4);
	int anAlpha = aVal * 255.0;
	g->SetColor(Color(255, 255, 200, anAlpha));
	g->DrawImageCel(IMAGE_AMPCHARGE, 120, m0xac - 75, ivar3, 0);
	g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
	g->SetColorizeImages(false);

	DrawStringHelper(g, "GASH", 0, 1, 2);
	g->DrawImageCel(IMAGE_GASH, 400, m0xac - 100, ivar3, 0);
	DrawStringHelper(g, "ANGIE", 4, 1, 1);
	DrawAngieHalo(g, 160, m0xac - 100, m0xb4, ivar3, false);
	DrawStringHelper(g, "PRESTO", 0, 1, 2);
	g->DrawImageCel(IMAGE_PRESTO, 400, m0xac - 90, ivar3, 0);

	m0xac += m0xb0 * 6;
	if (!mApp->mCurrentProfile->IsPetUnlocked(GameObject::PET_BRINKLEY))
	{
		DrawStringHelper(g, "?", 0, 0, 1);
		g->DrawImage(IMAGE_EGGBRINKLEY, 150, m0xac - 150);
	}
	else
	{
		DrawStringHelper(g, "BRINKLEY", 0, 1, 1);
		g->DrawImageCel(IMAGE_BRINKLEY, 160, m0xac - 100, ivar3, 0);
	}

	if (!mApp->mCurrentProfile->IsPetUnlocked(GameObject::PET_NOSTRADAMUS))
	{
		DrawStringHelper(g, "?", 0, 0, 2);
		g->DrawImage(IMAGE_EGGNOSTRADAMUS, 390, m0xac - 150);
	}
	else
	{
		DrawStringHelper(g, "NOSTRADAMUS", 0, 1, 2);
		g->DrawImageCel(IMAGE_NOSTRADAMUS, 400, m0xac - 100, ivar3, 0);
	}

	m0xac += m0xb0 * 6;
	if (!mApp->mCurrentProfile->IsPetUnlocked(GameObject::PET_BRINKLEY))
	{
		DrawStringHelper(g, "?", 0, 0, 1);
		g->DrawImage(IMAGE_EGGSTANLEY, 150, m0xac - 150);
	}
	else
	{
		DrawStringHelper(g, "STANLEY", 0, 1, 1);
		g->DrawImageCel(IMAGE_STANLEY, 160, m0xac - 100, ivar3, 0);
	}

	if (!mApp->mCurrentProfile->IsPetUnlocked(GameObject::PET_WALTER))
	{
		DrawStringHelper(g, "?", 0, 0, 2);
		g->DrawImage(IMAGE_EGGWALTER, 390, m0xac - 150);
	}
	else
	{
		DrawStringHelper(g, "WALTER", 0, 1, 2);
		g->DrawImageCel(IMAGE_WALTER, 400, m0xac - 100, ivar3, 0);
	}

	if (gInterludeInt02 == 0)
		gInterludeInt02 = m0xac;

	DrawStringHelper(g, "Credits", 7, 0, 0);

	DrawStringHelper(g, "Game Design", 3, 0, 0);
	DrawStringHelper(g, "George Fan", 1, 1, 0);

	DrawStringHelper(g, "Producer", 3, 0, 0);
	DrawStringHelper(g, "Jason Kapalka", 1, 1, 0);
	DrawStringHelper(g, "Sukhbir Sidhu", 1, 1, 0);

	DrawStringHelper(g, "Programming", 3, 0, 0);
	DrawStringHelper(g, "George Fan", 1, 1, 0);
	DrawStringHelper(g, "Thien Tran", 1, 1, 0);
	DrawStringHelper(g, "Brian Rothstein", 1, 1, 0);

	DrawStringHelper(g, "Art", 3, 0, 0);
	DrawStringHelper(g, "Josh Langley", 1, 1, 0);
	DrawStringHelper(g, "Walter Wilson", 1, 1, 0);

	DrawStringHelper(g, "Character Design", 3, 0, 0);
	DrawStringHelper(g, "George Fan", 1, 1, 0);

	DrawStringHelper(g, "Music", 3, 0, 0);
	DrawStringHelper(g, "Jonne Valtonen", 1, 1, 0);
	DrawStringHelper(g, "George Fan", 1, 1, 0);

	DrawStringHelper(g, "PopCap Framework", 3, 0, 0);
	DrawStringHelper(g, "Brian Fiete", 1, 1, 0);

	DrawStringHelper(g, "Biz Dev", 3, 0, 0);
	DrawStringHelper(g, "Don Walters", 1, 1, 0);

	DrawStringHelper(g, "QA", 3, 0, 0);
	DrawStringHelper(g, "Eric Harman", 1, 1, 0);
	DrawStringHelper(g, "Shawn Conard", 1, 1, 0);
	DrawStringHelper(g, "Brenna Flood", 1, 1, 0);
	DrawStringHelper(g, "Chad Zoellner", 1, 1, 0);

	DrawStringHelper(g, "Marmot Salesman", 3, 0, 0);
	DrawStringHelper(g, "David P. Wycliff", 1, 1, 0);

	DrawStringHelper(g, "Special Thanks", 3, 0, 0);
	DrawStringHelper(g, "PopCap Beta Testers", 1, 1, 0);
	DrawStringHelper(g, "Tysen Henderson", 1, 1, 0);
	DrawStringHelper(g, "Hai-Pao Fan", 1, 1, 0);
	DrawStringHelper(g, "Jan Campbell", 1, 1, 0);

	DrawStringHelper(g, "brought to you by:", 7, 1, 0);
	DrawStringHelper(g, "Flying Bear Entertainment", 1, 0, 0);
	DrawStringHelper(g, "and", 1, 1, 0);
	DrawStringHelper(g, "PopCap Games", 1, 0, 0);
	g->Translate(0, m0xa4);
}

void Sexy::InterludeScreen::DrawOverlay(Graphics* g)
{
	mBubbleMgr1->Draw(g);
	g->SetColor(Color::Black);
	g->FillRect(0, 0, mWidth, 40);
	g->FillRect(0, mHeight - 40, mWidth, 40);
}

void Sexy::InterludeScreen::MouseDown(int x, int y, int theClickCount)
{
	Widget::MouseDown(x, y, theClickCount);
	if (gInterludeInt03 <= m0xa4)
		ButtonDepress(0);
}

void Sexy::InterludeScreen::ButtonPress(int theId)
{
	mApp->PlaySample(SOUND_BUTTONCLICK);
}

void Sexy::InterludeScreen::ButtonDepress(int theId)
{
	mApp->StopMusic();
	mApp->RemoveInterludeScreen();
	mApp->SwitchToGameSelector();
}

void Sexy::InterludeScreen::DrawStringHelper(Graphics* g, const SexyString& theString, int unk1, int unk2, int unk3)
{
	m0xac += m0xb0 * unk1;
	if (unk2 == 0)
	{
		g->SetFont(FONT_LIDDIE18);
		g->SetColor(Color(215, 255, 255, 255));
	}
	else
	{
		g->SetFont(FONT_LIDDIE15);
		g->SetColor(Color(150, 250, 250, 255));
	}

	if (unk3 == 0)
		m0xa8 = (650 - g->GetFont()->StringWidth(theString)) / 2;
	else if(unk3 == 1)
		m0xa8 = (395 - g->GetFont()->StringWidth(theString)) / 2;
	else if(unk3 == 2)
		m0xa8 = (885 - g->GetFont()->StringWidth(theString)) / 2;

	g->DrawString(theString, m0xa8, m0xac);
}

void Sexy::InterludeScreen::DrawAngieHalo(Graphics* g, int theX, int theY, float theAlpha, int theFrame, bool mirror)
{
	Rect aSrc(theFrame * 80, 0, 80, 80);
	g->DrawImageMirror(IMAGE_ANGIE, theX, theY, aSrc, mirror);
	g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
	g->SetColorizeImages(true);
	g->SetColor(Color(255, 255, 255, abs(theAlpha) * 255.0));
	g->DrawImageMirror(IMAGE_HALO, theX, theY, aSrc, mirror);
	g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
	g->SetColorizeImages(false);
}

void Sexy::InterludeScreen::DrawPetAngieSpawns(Graphics* g, int unk1, int thePetId, int theX, int theY, 
	int unk5, int unk6, int unk7)
{
	int aRow = unk1;
	if (unk1 > 0 && thePetId < 24 && unk1 <= m0xa4 && m0xa4 < 1700)
	{
		switch (thePetId)
		{
		case GameObject::PET_NIKO:
		case GameObject::PET_VERT:
			aRow = unk7;
			break;
		default:
			aRow = unk5;
			break;
		case GameObject::PET_CLYDE:
		case GameObject::PET_SEYMOUR:
		case GameObject::PET_SHRAPNEL:
		case GameObject::PET_RHUBARB:
			aRow = unk6;
		}
		if (thePetId == GameObject::PET_NIKO)
			theY -= 20;
		else if (thePetId == GameObject::PET_AMP)
		{
			theX -= 40;
			theY += 40;
		}
		Image* anImg = GetImageById(IMAGE_STINKY_ID + thePetId);
		int anAlpha = m0xa4 - unk1;
		if (anAlpha > 255)
			anAlpha = 255;
		g->SetColorizeImages(true);
		g->SetColor(Color(255, 255, 255, anAlpha));
		g->DrawImageCel(anImg, theX, theY, aRow, 0);
		g->SetColorizeImages(false);
	}
}

void Sexy::InterludeScreenOverlay::Draw(Graphics* g)
{
	mScreen->DrawOverlay(g);
}
