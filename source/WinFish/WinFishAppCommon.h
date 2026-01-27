#pragma once

#include "SexyAppFramework/Dialog.h"
#include "SexyAppFramework/EditWidget.h"
#include "SexyAppFramework/Checkbox.h"
#include "SexyAppFramework/HyperlinkWidget.h"
#include "SexyAppFramework/MemoryImage.h"
#include "FishSongMgr.h"
#include "GameObject.h"

namespace Sexy
{
	DialogButton*		MakeDialogButton(int theId, ButtonListener* theListener, SexyString theText, Font* theFont);
	DialogButton*		MakeDialogButton2(int theId, ButtonListener* theListener, SexyString theText, Image* theComponentImage);
	void				DefaultDialogSettings(Dialog* theDialog);
	void				DefaultDialogButtonSettings(DialogButton* theDialogButton);
	EditWidget*			MakeEditWidget(int theId, EditListener* theEditListener);
	Checkbox*			MakeCheckbox(int theId, CheckboxListener* theCheckboxListener, bool theChecked);
	HyperlinkWidget*	MakeHyperlinkWidget(int theId, ButtonListener* theButtonListener, SexyString theLabel);

	void				DrawStringWithOutline(Graphics* g, SexyString theLine, int theX, int theY, Font* theFont, ulong theOutlineColor);
	void				DrawWaves(Graphics* g, int theY, int theUpdateCnt);
	void				DrawHorzCrease(Graphics* g, Image* theImage,int theX, int theY, int theWidth);
	void				DrawVertCrease(Graphics* g, Image* theImage,int theX, int theY, int theHeight);
	void				DrawCheckboxString(Graphics* g, const SexyString& theLine, Checkbox* theCB, const char* theExtensionString);

	int					InterpolateInt(int startValue, int endValue, int currentProgress, int totalDuration, bool swap);

	void				SetVaryingBlueColorGraphics(Graphics* g, int theUpdateCnt, int theY);

	SexyString			GetCyraxEndGameString(int theUnk01);
	SexyString			GetPlayTimeString(int theTimeInSec);
	SexyString			GetMentalStateString(GameObject* theObject);

	void				KnuthShuffleFishSongs(FishSongData** first, FishSongData** last);
	FishSongData*		GetSongData(int theSpecId);

	long long			GetTodayStartSeconds();

	void				ButtonHoleHelper(MemoryImage* theImage, MemoryImage* theHoleImage, int theX, int theY);
	SexyString			GetPetName(int thePetId);

	void				NormalizeVector3(float* theVector);

	int					GetLevelIndex(int theTank, int theLevel);
}

