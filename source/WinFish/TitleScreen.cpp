#include "TitleScreen.h"
#include "Res.h"
#include "WinFishApp.h"

#include "SexyAppFramework/Font.h"
#include "SexyAppFramework/Graphics.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Rect.h"
#include "SexyAppFramework/HyperlinkWidget.h"

using namespace Sexy;

TitleScreen::TitleScreen(WinFishApp* theApp)
{
	mApp = theApp;
	m0x94 = 0;
	m0x98 = IMAGE_LOADERBAR->mWidth;
	m0x9c = 0;
	m0xc0 = false;
	m0xa0 = false;
	m0xc1 = false;
	m0xa1 = false;

	mHyperlink1 = new HyperlinkWidget(0, this);
	mHyperlink1->SetFont(FONT_JUNGLEFEVER17OUTLINE);
	mHyperlink1->mColor = Color(0xffec91);
	mHyperlink1->mOverColor = Color::White;
	mHyperlink1->mUnderlineSize = 2;
	mHyperlink1->SetVisible(false);

	mHyperlink2 = new HyperlinkWidget(0, this);
	mHyperlink2->mLabel = "Click Here To Register!";
	mHyperlink2->SetFont(FONT_JUNGLEFEVER17OUTLINE);
	mHyperlink2->mColor = Color(0xffec91);
	mHyperlink2->mOverColor = Color::White;
	mHyperlink2->mUnderlineSize = 2;
	mHyperlink2->SetVisible(true);

	mHyperlink1->Resize(180, 415, IMAGE_LOADERBAR->mWidth, IMAGE_LOADERBAR->mHeight);
	int aStrWdth = FONT_JUNGLEFEVER17OUTLINE->StringWidth(mHyperlink2->mLabel);
	int aStrHeight = FONT_JUNGLEFEVER17OUTLINE->GetHeight();
	mHyperlink2->Resize(0, 0, aStrWdth + 20, aStrHeight + 20);

	if (!mApp->mIsRegistered)
	{
		if (!mApp->IsScreenSaver())
		{
			if (!mApp->mBuildUnlocked)
				TrialFunction();
		}
		else
			mHyperlink2->SetVisible(false);
	}
	else
		mHyperlink2->SetVisible(false);

	mStinkyX = 0xa0;
	m0xcc = 0;
	mBlendInCounter = 0;
}

TitleScreen::~TitleScreen()
{
	if (mHyperlink1)
		delete mHyperlink1;
	if (mHyperlink2)
		delete mHyperlink2;
}

void TitleScreen::AddedToManager(WidgetManager* theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mHyperlink2);
	theWidgetManager->AddWidget(mHyperlink1);
}

void TitleScreen::RemovedFromManager(WidgetManager* theWidgetManager)
{
	Widget::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mHyperlink2);
	theWidgetManager->RemoveWidget(mHyperlink1);
}

void TitleScreen::Draw(Graphics* g)
{
	int anImgId = mHyperlink1->mVisible ? mHyperlink1->mIsOver + IMAGE_LOADERBAR_ID : IMAGE_LOADERBAR_ID;
	int anXBtnDraw = 180;
	int anYBtnDraw = 415;
	if (mHyperlink1->mVisible && mHyperlink1->mIsOver && mHyperlink1->mIsDown)
	{
		anXBtnDraw = 181;
		anYBtnDraw = 416;
	}
	Image* aLoaderImg = GetImageById(anImgId);
	g->DrawImage(IMAGE_TITLEPAGE, 0, 0);

	Graphics g2(*g);

	g2.ClipRect(Rect(anXBtnDraw, anYBtnDraw, m0x94, aLoaderImg->mHeight));
	g2.DrawImage(aLoaderImg, anXBtnDraw, anYBtnDraw);

	if (!mHyperlink1->mVisible)
		g2.DrawImage(IMAGE_LOADERBARLOADING, 230, 415);
	else
	{
		int anAlpha = (mBlendInCounter * 255) / 8;

		if (anAlpha > 0)
		{
			g->SetColorizeImages(true);
			g->SetColor(Color(255, 255, 255, anAlpha));
			g->DrawImage(IMAGE_LOADERBARLOADING, 230, 415);
			g->SetColor(Color(255, 255, 255, 255- anAlpha));
			g->DrawImage(IMAGE_LOADERPLAY, anXBtnDraw, anYBtnDraw);
			g->SetColorizeImages(false);
		}
		else
		{
			g->DrawImage(IMAGE_LOADERPLAY, anXBtnDraw, anYBtnDraw);
		}
	}

	if (mStinkyX < 510)
		g->DrawImageMirror(IMAGE_STINKY, mStinkyX, anYBtnDraw - 30, Rect(((mUpdateCnt / 4) % 10) * 80, 0, 80, 80), true);
	g->DrawImage(IMAGE_TITLEPAGEMASK, 384, 388);
	if (!mApp->IsScreenSaver())
	{
		if (mApp->mIsRegistered && !mApp->mBuildUnlocked)
		{
			SexyString aStr = "THANKS FOR REGISTERING!";
			g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
			g->SetColor(Color::White);
			int aStrWdth = g->GetFont()->StringWidth(aStr);
			g->WriteString(aStr, mWidth - aStrWdth / 2 - 13, 470);
		}
		if (!mApp->mIsRegistered)
		{
			g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
			g->SetColor(Color::White);
			int aStrWdth = g->GetFont()->StringWidth(m0xa4);
			g->WriteString(m0xa4, mWidth - aStrWdth / 2 - 13, 470);
		}
	}
	else
	{
		SexyString aStr = "Starting Screensaver";
		g->SetFont(FONT_JUNGLEFEVER17OUTLINE);
		g->SetColor(Color(0xffec91));
		int aStrWdth = g->GetFont()->StringWidth(aStr);
		g->WriteString(m0xa4, mWidth - aStrWdth / 2 - 13, 395);
	}

	g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
	g->SetColor(Color::White);
	SexyString aVersionStr = "Version " + mApp->mProductVersion;
	int aStrWdth = g->GetFont()->StringWidth(aVersionStr);
	g->WriteString(aVersionStr, mWidth - aStrWdth / 2 - 16, 211);
}

void TitleScreen::Update()
{
	Widget::Update();
	if (mBlendInCounter > 0)
		mBlendInCounter--;
	if (m0x94 < (mApp->GetLoadingThreadProgress() * m0x98))
	{
		if (m0x9c < 3)
			m0x9c++;
		m0x94 += m0x9c;
		m0xcc++;
		mStinkyX = 0xa0 + m0x94;
		if (m0x98 < m0x94)
		{
			m0x94 = m0x98;
			m0x9c--;
		}

		MarkDirty();
	}
	else if (!m0xa1)
	{
		if (gSexyApp->mLoadingThreadCompleted)
		{
			m0xa1 = true;

			if (!mApp->IsScreenSaver())
			{
				mBlendInCounter = 8;
				mHyperlink1->SetVisible(true);
				MarkDirty();
			}
		}
	}
	else
	{
		if (mStinkyX < mWidth)
		{
			if (m0x9c < 3)
				m0x9c++;
			m0xcc++;
			mStinkyX += m0x9c;
			MarkDirty();
		}
		if (mApp->IsScreenSaver() && mStinkyX >= 511)
			mApp->TitleScreenIsFinished();
	}

	if (m0xa0)
	{
		if (mApp->mCompletedLoadingThreadTasks > 29)
		{
			m0xa0 = false;
			mApp->DoRegisterDialog();
		}
	}
	if (m0xc1)
	{
		if (mApp->mCompletedLoadingThreadTasks > 29)
		{
			m0xc1 = false;
			mApp->DoTrialVersionExpiredDialog();
		}
	}
	if (mUpdateCnt % 4 == 0)
		MarkDirty();
}

void Sexy::TitleScreen::Resize(int theX, int theY, int theWidth, int theHeight)
{
	Widget::Resize(theX, theY, theWidth, theHeight);
	mHyperlink2->Resize(mWidth - mHyperlink2->mWidth / 2 + theX, 350, mHyperlink2->mWidth, mHyperlink2->mHeight);
}

void Sexy::TitleScreen::ButtonPress(int theId)
{
	mApp->PlaySample(SOUND_BUTTONCLICK);
}

void Sexy::TitleScreen::ButtonDepress(int theId)
{
	if (theId == 0)
		mApp->TitleScreenIsFinished();
	else if (theId == 1)
		mUpdateIteratorModified = true;
}

void Sexy::TitleScreen::RegisterSuccessful()
{
	m0xc0 = false;
	mHyperlink2->SetVisible(false);
	if (!mApp->IsScreenSaver())
		mHyperlink1->SetVisible(m0x94 == m0x98);
	MarkDirty();
}

void Sexy::TitleScreen::TrialFunction()
{
	if (mApp->mMaxTime > 0)
	{
		int aTimeRemain = mApp->mMaxTime - mApp->mTimesPlayed;
		if (aTimeRemain < 1)
			m0xc0 = true;
		else if (aTimeRemain == 1)
			m0xa4 = "YOU HAVE 1 MINUTE LEFT IN THIS TRIAL!";
		else
			m0xa4 = StrFormat("YOU HAVE %d MINUTES LEFT IN THIS TRIAL!", aTimeRemain);
	}
	else if(mApp->mMaxTime < 1)
	{
		if (mApp->mMaxExecutions > 0)
		{
			int aExecs = mApp->mMaxExecutions - mApp->mTimesExecuted;
			if (aExecs < 1)
				m0xc0 = true;
			else if (aExecs == 1)
				m0xa4 = "YOU HAVE 1 FREE PLAY REMAINING!";
			else
				m0xa4 = StrFormat("YOU HAVE %d FREE PLAYS REMAINING!", aExecs);
		}
	}
	else
	{
		int aPlays = mApp->mMaxPlays - mApp->mTimesPlayed;
		if (aPlays < 1)
			m0xc0 = true;
		else if(aPlays == 1)
			m0xa4 = "YOU HAVE 1 FREE PLAY REMAINING!";
		else
			m0xa4 = StrFormat("YOU HAVE %d FREE PLAYS REMAINING!", aPlays);
	}
	if (m0xc0)
	{
		m0xc1 = true;
		mHyperlink2->SetVisible(false);
		mHyperlink1->SetVisible(false);
	}
}
