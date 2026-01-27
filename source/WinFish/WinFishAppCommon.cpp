#include "WinFishApp.h"
#include "WinFishAppCommon.h"
#include "SexyAppFramework/DialogButton.h"
#include "SexyAppFramework/MemoryImage.h"
#include "SexyAppFramework/Font.h"
#include "Res.h"
#include <time.h>

using namespace Sexy;

DialogButton* Sexy::MakeDialogButton(int theId, ButtonListener* theListener, SexyString theText, Font* theFont)
{
	DialogButton* aDiaBtn = new DialogButton(IMAGE_DIALOGBUTTON, theId, theListener);
	if (!theFont)
		theFont = FONT_JUNGLEFEVER10OUTLINE;
	aDiaBtn->SetFont(theFont);
	aDiaBtn->mLabel = theText;
	aDiaBtn->mHeight = aDiaBtn->mComponentImage->mHeight;
	DefaultDialogButtonSettings(aDiaBtn);
	return aDiaBtn;
}

DialogButton* Sexy::MakeDialogButton2(int theId, ButtonListener* theListener, SexyString theText, Image* theComponentImage)
{
	DialogButton* aDiaBtn = new DialogButton(theComponentImage, theId, theListener);
	aDiaBtn->SetFont(FONT_JUNGLEFEVER10OUTLINE);
	aDiaBtn->mLabel = theText;
	aDiaBtn->SetColor(0, Color(0xff, 0xf0, 0));
	aDiaBtn->SetColor(1, Color::White);
	aDiaBtn->mDoFinger = true;
	aDiaBtn->mHeight = theComponentImage->mHeight;
	DefaultDialogButtonSettings(aDiaBtn);
	return aDiaBtn;
}

void Sexy::DefaultDialogSettings(Dialog* theDialog)
{
	theDialog->SetButtonFont(FONT_JUNGLEFEVER12OUTLINE);
	theDialog->SetHeaderFont(FONT_JUNGLEFEVER15OUTLINE);
	theDialog->SetLinesFont(FONT_JUNGLEFEVER10OUTLINE);
	theDialog->mContentInsets = Insets(0x24, 0xf, 0x24, 0x24);
	theDialog->SetColor(Dialog::COLOR_HEADER, Color(0xff, 200, 0));
	theDialog->SetColor(Dialog::COLOR_LINES, Color::White);
	theDialog->SetColor(Dialog::COLOR_BUTTON_TEXT, Color(0xff, 0xff, 100));
	theDialog->mSpaceAfterHeader = 15;
	if (theDialog->mYesButton)
		DefaultDialogButtonSettings(theDialog->mYesButton);
	if (theDialog->mNoButton)
		DefaultDialogButtonSettings(theDialog->mNoButton);
}

void Sexy::DefaultDialogButtonSettings(DialogButton* theDialogButton)
{
	theDialogButton->mTranslateX = 1;
	theDialogButton->mTranslateY = 1;
	theDialogButton->mHasTransparencies = true;
	theDialogButton->mHasAlpha = true;
	int anImgWdth = theDialogButton->mComponentImage->mWidth / 3;
	int anImgHght = theDialogButton->mComponentImage->mHeight;
	theDialogButton->mOverRect.mX = anImgWdth;
	theDialogButton->mDownRect.mX = anImgWdth*2;
	theDialogButton->mNormalRect.mWidth = anImgWdth;
	theDialogButton->mOverRect.mWidth = anImgWdth;
	theDialogButton->mDownRect.mWidth = anImgWdth;
	theDialogButton->mNormalRect.mHeight = anImgHght;
	theDialogButton->mOverRect.mHeight = anImgHght;
	theDialogButton->mDownRect.mHeight = anImgHght;	
	theDialogButton->mTextOffsetX = -2;
}

EditWidget* Sexy::MakeEditWidget(int theId, EditListener* theEditListener)
{
	EditWidget* anEditWidget = new EditWidget(theId, theEditListener);
	anEditWidget->SetFont(FONT_LIDDIE12, 0);
	int anEditWidgetColors[][4] =
	{ {0, 0, 0, 0},
	{0, 0, 0, 0},
	{240, 240, 255, 255},
	{255, 255, 255, 255},
	{0, 0, 0, 255} };
	anEditWidget->SetColors(anEditWidgetColors, EditWidget::NUM_COLORS);
	anEditWidget->mBlinkDelay = 14;
	return anEditWidget;
}

Checkbox* Sexy::MakeCheckbox(int theId, CheckboxListener* theCheckboxListener, bool theChecked)
{
	Checkbox* aCB = new Checkbox(IMAGE_UNCHECKED, IMAGE_CHECKED, theId, theCheckboxListener);
	aCB->mChecked = theChecked;
	aCB->mHasTransparencies = true;
	aCB->mHasAlpha = true;
	return aCB;
}

HyperlinkWidget* Sexy::MakeHyperlinkWidget(int theId, ButtonListener* theButtonListener, SexyString theLabel)
{
    HyperlinkWidget* aWidget = new HyperlinkWidget(theId, theButtonListener);
    aWidget->mLabel = theLabel;
    aWidget->SetFont(FONT_JUNGLEFEVER10OUTLINE);
    aWidget->mColor = Color(0x808080);
    aWidget->mOverColor = Color(0xffffff);
    aWidget->mUnderlineSize = 0;
    int aStrWdth = aWidget->mFont->StringWidth(aWidget->mLabel);
    aWidget->mWidth = aStrWdth + 20;
    int aStrHght = aWidget->mFont->GetHeight();
    aWidget->mHeight = aStrHght;
    return aWidget;
}

void Sexy::DrawStringWithOutline(Graphics* g, SexyString theLine, int theX, int theY, Font* theFont, ulong theOutlineColor)
{
	Color aPrevColor = g->GetColor();
	Font* aPrevFont = g->GetFont();
	g->SetFont(theFont);
	g->SetColor(Color(theOutlineColor));
	g->DrawString(theLine, theX, theY);
	g->SetFont(aPrevFont);
	g->SetColor(aPrevColor);
	g->DrawString(theLine, theX, theY);
}

void Sexy::DrawWaves(Graphics* g, int theY, int theUpdateCnt)
{
    g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
    int aCel = IMAGE_WAVE->GetAnimCel(theUpdateCnt);
    g->DrawImageCel(IMAGE_WAVE, 0, theY, aCel);
    g->DrawImageCel(IMAGE_WAVE, 160, theY, aCel);
    g->DrawImageCel(IMAGE_WAVE, 320, theY, aCel);
    g->DrawImageCel(IMAGE_WAVE, 480, theY, aCel);
    g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
}

void Sexy::DrawHorzCrease(Graphics* g, Image* theImage, int theX, int theY, int theWidth)
{
    if (theWidth <= 0)
        return;

    for (int i = 0; i < theWidth; i += theImage->mWidth)
    {
        int aSrcWdth = theWidth - i;
        if (theImage->mWidth <= aSrcWdth)
            aSrcWdth = theImage->mWidth;
        g->DrawImage(theImage, theX + i, theY, Rect(0, 0, aSrcWdth, theImage->mHeight));
    }
}

void Sexy::DrawVertCrease(Graphics* g, Image* theImage, int theX, int theY, int theHeight)
{
    if (theHeight <= 0)
        return;

    for (int i = 0; i < theHeight; i += theImage->mHeight)
    {
        int aSrcHeight = theHeight - i;
        if (theImage->mHeight <= aSrcHeight)
            aSrcHeight = theImage->mHeight;
        g->DrawImage(theImage, theX, theY + i, Rect(0, 0, theImage->mWidth, aSrcHeight));
    }
}

void Sexy::DrawCheckboxString(Graphics* g, const SexyString& theLine, Checkbox* theCB, const char* theExtensionString)
{
    if (theExtensionString == nullptr)
    {
        g->DrawString(theLine, theCB->mX - g->mTransX + 43, theCB->mY - g->mTransY + 24);
    }
    else
    {
        g->DrawString(theLine, theCB->mX - g->mTransX + 43, theCB->mY - g->mTransY + 15);

        SexyString aStr; 
        aStr.assign(theExtensionString);
        g->DrawString(aStr, theCB->mX - g->mTransX + 43, theCB->mY - g->mTransY + 30);
    }
}

int Sexy::InterpolateInt(int startValue, int endValue, int currentProgress, int totalDuration, bool swap)
{
    int start;
    int end;

    if (swap)
    {
        start = endValue;
        end = startValue;
    }
    else
    {
        start = startValue;
        end = endValue;
    }

    if (currentProgress <= 0)
        return start;

    if (currentProgress >= totalDuration)
        return end;

    long long term1 = (long long)start * (totalDuration - currentProgress);
    long long term2 = (long long)end * currentProgress;

    return (int)((term1 + term2) / totalDuration);
}

void Sexy::SetVaryingBlueColorGraphics(Graphics* g, int theUpdateCnt,int theY)
{
    int aVal = (theUpdateCnt + theY) % 200;
    if (aVal > 100)
        aVal = 200 - aVal;
    g->SetColor(Color(gSexyAppBase->HSLToRGB(159, 119, (aVal * 70) / 100 + 180)));
}

SexyString Sexy::GetCyraxEndGameString(int theCyraxCnt)
{
    if (theCyraxCnt < 2)
        return SexyString("FINAL BOSS");

    int aCyraxDefeatNum = theCyraxCnt;
    SexyString aNumOrderStr = "th";
    if (aCyraxDefeatNum / 10 % 10 != 1)
    {
        if (aCyraxDefeatNum % 10 == 1)
            aNumOrderStr = "st";
        else if (aCyraxDefeatNum % 10 == 2)
            aNumOrderStr = "nd";
        else if (aCyraxDefeatNum % 10 == 3)
            aNumOrderStr = "rd";
    }

    return SexyString(StrFormat("%d%s FINAL BOSS", aCyraxDefeatNum, aNumOrderStr.c_str()));
}

SexyString Sexy::GetPlayTimeString(int theTimeInSec)
{
    int anHours = theTimeInSec / 3600;
    int aMins = (theTimeInSec / 60) % 60;
    int aSec = theTimeInSec % 60;
    if (anHours < 1)
        return StrFormat("%d:%02d", aMins, aSec);
    return StrFormat("%02d:%02d:%02d", anHours, aMins, aSec);
}

SexyString Sexy::GetMentalStateString(GameObject* theObject)
{
    switch (theObject->mMentalState)
    {
    case 0:
        return "Horribly Depressed";
    case 1:
        return "Feeling Neglected";
    case 2:
        return "Quite Hungry";
    case 3:
        return "Contented";
    case 4:
        return "Happy";
    case 5:
        return "Chipper";
    case 6:
    {
        SexyString aPossibleStrs[] = {
        "Super Pumped" ,
        "Lovin\' It!",
        "High on Life" ,
        "Totally Stoked",
        "Fish-tastic!" 
        };
        return aPossibleStrs[theObject->m0x150 % 5];
    }
    default:
        return "Unknown";
    }
}

void Sexy::KnuthShuffleFishSongs(FishSongData** first, FishSongData** last)
{
    if (first == last)
        return;

    for (int i = 1; first + i < last; ++i)
    {
        int j = Rand() % (i + 1);

        std::swap(first[i], first[j]);
    }
}

FishSongData* Sexy::GetSongData(int theSpecId)
{
    if (gUnkBool05)
        return nullptr;

    if (theSpecId == 4)
        return gKilgoreSongDataPtr;

    if (gTestSongDataPtr != nullptr)
        return gTestSongDataPtr;

    std::vector<FishSongData*>* aSongList = nullptr;
    int* aListId = nullptr;

    switch (theSpecId)
    {
    case 1:
        aSongList = &gLudwigSongs;
        aListId = &gLudwigSongId;
        break;
    case 5:
        aSongList = &gSantaSongs;
        aListId = &gSantaSongId;
        break;
    default:
        aSongList = &gSongsVector2;
        aListId = &gSongs2Id;
        break;
    }

    if (aSongList == nullptr || aSongList->empty())
        return nullptr;

    if (*aListId >= aSongList->size())
        *aListId = 0;

    if (*aListId == 0 && !aSongList->empty())
        KnuthShuffleFishSongs(aSongList->data(), aSongList->data() + aSongList->size());

    FishSongData* aChosenSong = (*aSongList)[*aListId];

    (*aListId)++;

    if (gUnkInt04 < 5)
        gUnkInt04++;

    SexyString aPropVal = "";
    if (aChosenSong->GetProperty("short", &aPropVal))
    {
        bool unkbool = true;
        if (!aChosenSong->GetProperty("beethoven", nullptr) || theSpecId == 1)
            unkbool = false;

        if (!unkbool && gUnkInt04 == 5)
        {
            std::map<SexyString, FishSongData*, Sexy::StringLessNoCase>::iterator it = gLongSongsMap.find(aPropVal);
            if (it != gLongSongsMap.end())
            {
                gUnkInt04 = 0;
                return it->second;
            }
        }
    }

    return aChosenSong;
}

long long Sexy::GetTodayStartSeconds() // Returns the start of todays day in sec from 1970 to today
{
    __time64_t currentTime_t = _time64(nullptr);

    if (currentTime_t < 0)
        currentTime_t = 0;

    tm* localTimeInfo = _localtime64(&currentTime_t);

    __time64_t secondsIntoDay = localTimeInfo->tm_hour * 3600 +
        localTimeInfo->tm_min * 60 +
        localTimeInfo->tm_sec;

    __time64_t midnightTime_t = currentTime_t - secondsIntoDay;

    long long total_days = midnightTime_t / 86400;

    return total_days + gUnkInt01;
}

void Sexy::ButtonHoleHelper(MemoryImage* theImage, MemoryImage* theHoleImage, int theX, int theY)
{
    Rect anIntersectRect = Rect(0, 0, theImage->mWidth, theImage->mHeight)
        .Intersection(Rect(theX, theY, theHoleImage->mWidth, theHoleImage->mHeight));

    if (anIntersectRect.mWidth <= 0 || anIntersectRect.mHeight <= 0)
        return;

    ulong* aHoleImgBits = theHoleImage->GetBits();
    ulong* aHolePos = aHoleImgBits + (anIntersectRect.mY - theY) * theHoleImage->mWidth - theX + anIntersectRect.mX;

    ulong* anImgBits = theImage->GetBits();
    ulong* anImgPos = anImgBits + theImage->mWidth * anIntersectRect.mY + anIntersectRect.mX;

    for (int y = 0; y < anIntersectRect.mHeight; y++)
    {
        ulong* aTemp1 = anImgPos;
        ulong* aTemp2 = aHolePos;

        for (int x = 0; x < anIntersectRect.mWidth; x++)
        {
            ulong aBitVal = *aTemp2;
            aTemp2++;
            if ((aBitVal & 0xff000000) != 0)
                *aTemp1 = 0;
            aTemp1++;
        }

        anImgPos += theImage->mWidth;
        aHolePos += theHoleImage->mWidth;
    }
}

SexyString Sexy::GetPetName(int thePetId)
{
    switch (thePetId)
    {
    case GameObject::PET_STINKY:
        return "Stinky";
    case GameObject::PET_NIKO:
        return "Niko";
    case GameObject::PET_ITCHY:
        return "Itchy";
    case GameObject::PET_PREGO:
        return "Prego";
    case GameObject::PET_ZORF:
        return "Zorf";
    case GameObject::PET_CLYDE:
        return "Clyde";
    case GameObject::PET_VERT:
        return "Vert";
    case GameObject::PET_RUFUS:
        return "Rufus";
    case GameObject::PET_MERYL:
        return "Meryl";
    case GameObject::PET_WADSWORTH:
        return "Wadsworth";
    case GameObject::PET_SEYMOUR:
        return "Seymour";
    case GameObject::PET_SHRAPNEL:
        return "Shrapnel";
    case GameObject::PET_GUMBO:
        return "Gumbo";
    case GameObject::PET_BLIP:
        return "Blip";
    case GameObject::PET_RHUBARB:
        return "Rhubarb";
    case GameObject::PET_NIMBUS:
        return "Nimbus";
    case GameObject::PET_AMP:
        return "Amp";
    case GameObject::PET_GASH:
        return "Gash";
    case GameObject::PET_ANGIE:
        return "Angie";
    case GameObject::PET_PRESTO:
        return "Presto";
    case GameObject::PET_BRINKLEY:
        return "Brinkley";
    case GameObject::PET_NOSTRADAMUS:
        return "Nostradamus";
    case GameObject::PET_STANLEY:
        return "Stanley";
    case GameObject::PET_WALTER:
        return "Walter";
    default:
        return "";
    }
}

void Sexy::NormalizeVector3(float* theVector)
{
    float aLenSq = theVector[0] * theVector[0] + theVector[1] * theVector[1] + theVector[2] * theVector[2];
    float aLen = sqrt(aLenSq);
    if (aLen > 0.00001f)
    {
        theVector[0] = theVector[0] / aLen;
        theVector[1] = theVector[1] / aLen;
        theVector[2] = theVector[2] / aLen;
    }
}

int Sexy::GetLevelIndex(int theTank, int theLevel)
{
    if (theTank < 6 && theLevel < 6 && (theTank != 5 || theLevel == 1))
        return theTank * 5 - 6 + theLevel;
    return -1;
}
