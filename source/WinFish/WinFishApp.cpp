#include "WinFishApp.h"

#include "WinFishAppCommon.h"
#include "InternetManager.h"
#include "SexyAppFramework/SEHCatcher.h"

#include "SexyAppFramework/SexyAppBase.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/ResourceManager.h"
#include "SexyAppFramework/ImageFont.h"
#include "SexyAppFramework/DialogButton.h"
#include "SexyAppFramework/SWTri.h"
#include "SexyAppFramework/Checkbox.h"
#include "SexyAppFramework/BassMusicInterface.h"
#include "SexyAppFramework/SoundManager.h"
#include "SexyAppFramework/Buffer.h"
#include "SexyAppFramework/Color.h"

#include "TitleScreen.h"
#include "HelpScreen.h"
#include "StoreScreen.h"
#include "HatchScreen.h"
#include "GameSelector.h"
#include "BonusScreen.h"
#include "HighScoreScreen.h"
#include "InterludeScreen.h"
#include "StoryScreen.h"
#include "SimFishScreen.h"
#include "TankScreen.h"
#include "PetsScreen.h"
#include "SimSetupScreen.h"
#include "Res.h"
#include "Board.h"
#include "Bilaterus.h"

#include "WorkerThread.h"
#include "HighScoreMgr.h"

#include <chrono>
#include <ctime>
#include "MoneyDialog.h"
#include "ContinueDialog.h"
#include "NewUserDialog.h"
#include "OptionsDialog.h"
#include "UserDialog.h"
#include "UpdateCheckDialog.h"
#include "VirtualDialog.h"
#include "PetDialog.h"
#include "ScreenSaverDialog.h"
#include "FishNamingDialog.h"
#include "FoodDialog.h"
#include "RegisterDialog.h"

#include "FishSongMgr.h"

namespace Sexy
{
	int gWadsworthTimer = 0;
	int gWadsworthX = 0, gWadsworthY = 0;
	bool gMerylActive = false;
	bool gUnkBool01 = false;
	bool gDoHundredUpdates = false;
	bool gUnkBool02 = false;
	bool gUnkBool03 = false;
	bool gUnkBool04 = false;
	bool gUnkBool05 = false;
	bool gUnkBool06 = false;
	bool gUnkBool07 = false;
	bool gUnkBool08 = false;
	bool gUnkBool09 = false;
	bool gCanRemapMusic = true;
	bool gFishSongsLoaded = false;
	int gUsersDatVersion = 14;
	bool gScreenSaverStarted;
	bool gZombieMode = false;
	int gUpdateFramesCounter = 0;
	bool gFirstVirtualTankEnter = false;
	int gFoodType = 0;
	int gFoodLimit = 1;
	int gUnkInt01 = 0;
	FishSongData* gKilgoreSongDataPtr = nullptr;
	FishSongData* gTestSongDataPtr = nullptr;
	int gUnkInt02 = 0;
	int gUnkInt05 = 0;
	int gUnkInt06 = 0;
	int gUnkInt03 = 0;
	int gUnkInt07 = 5000;
	int gPetsDiedOnBossLevel = 0;
	int gUnkInt09 = -1;
	int gUnkInt10 = -1;
	int gUnkInt11 = 2160;
	int gLastStoryId = 0;
	int gDeadPetsIdArray[24] = {0};
	int gUnkIntArray02[100] = {0};
	SexyString gFishSongParseError;
	Color gUnkColor01 = Color(0,0,0,0);
	std::vector<FishSongData*> gSongsVector1;
	std::vector<FishSongData*> gSongsVector2;
	std::vector<FishSongData*> gLudwigSongs;
	std::vector<FishSongData*> gSantaSongs;
	std::map<SexyString, FishSongData*, Sexy::StringLessNoCase> gLongSongsMap;
	int gUnkInt04 = 0;
	int gLudwigSongId = 0;
	int gSantaSongId = 0;
	int gSongs2Id = 0;
}

using namespace Sexy;

void LoadFishSongs(bool flag)
{
	if (!flag && gFishSongsLoaded)
		return;
	gFishSongsLoaded = true;

	Sexy::gSongsVector1.clear();
	Sexy::gSongsVector2.clear();
	Sexy::gLudwigSongs.clear();
	Sexy::gLongSongsMap.clear();
	// Santa songs clear?

	gKilgoreSongDataPtr = nullptr;
	gTestSongDataPtr = nullptr;

	WIN32_FIND_DATAA aFindFileData;
	HANDLE hFind = FindFirstFileA("fishsongs\\*.txt", &aFindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
		return;

	bool hasOpenedErrorFile = false;
	FILE* anErrorFile = nullptr;

	do 
	{
		FishSongData* aNewSong = new FishSongData();
		gSongsVector1.push_back(aNewSong);

		SexyString aFilePath = "fishsongs\\";
		aFilePath.append(aFindFileData.cFileName);

		aNewSong->mProperties.clear();

		if (!aNewSong->Parse(aFilePath)) // 128
		{
			if (!hasOpenedErrorFile)
			{
				hasOpenedErrorFile = true;
				anErrorFile = fopen("fishsongerror.txt", "w");
			}
			if (anErrorFile != nullptr)
				fprintf(anErrorFile, "%s - %s\n", aFindFileData.cFileName, gFishSongParseError.c_str());

			gSongsVector1.pop_back();
			delete aNewSong;
		}
		else 
		{
			char* aProperty = strchr(aFindFileData.cFileName, '_');
			if (aProperty != 0)
			{
				*aProperty = '\0';
				if (stricmp(aProperty + 1, "long.txt") == 0)
					aNewSong->mProperties["long"] = aFindFileData.cFileName;
				else if(stricmp(aProperty + 1, "short.txt") == 0)
					aNewSong->mProperties["short"] = aFindFileData.cFileName;
			}
		}

	} while (FindNextFileA(hFind, &aFindFileData) != 0);

	FindClose(hFind);
	if(anErrorFile != nullptr)
		fclose(anErrorFile);

	for (int i = 0; i < gSongsVector1.size(); i++)
	{
		FishSongData* aSong = gSongsVector1[i];

		bool addToSongsVec2 = true;

		if (!aSong->GetProperty("test", nullptr))
		{
			SexyString aValue = "";
			if (aSong->GetProperty("long", &aValue))
			{ // 329
				gLongSongsMap[aValue] = aSong;
				addToSongsVec2 = false;
			}
			else if (aSong->GetProperty("beethovenrare", nullptr))
			{ // 321
				gLudwigSongs.push_back(aSong);
				addToSongsVec2 = false;
			}
			else if (aSong->GetProperty("beethoven", nullptr))
			{ // 312
				gLudwigSongs.push_back(aSong);
			}
			else if (aSong->GetProperty("santarare", nullptr))
			{ // 304
				gSantaSongs.push_back(aSong);
				addToSongsVec2 = false;
			}
			else if (aSong->GetProperty("santa", nullptr))
			{ // 299
				gSantaSongs.push_back(aSong);
			}
		}
		else // Is test
			gTestSongDataPtr = aSong;

		if (aSong->GetProperty("kilgore", nullptr))
			gKilgoreSongDataPtr = aSong;

		if (addToSongsVec2)
			gSongsVector2.push_back(aSong);
	}
}

void LoadFishSongsTaskWrapper(void* userData)
{
	LoadFishSongs(0);
	gUnkBool05 = false;
}

WinFishApp::WinFishApp()
{
	mBoard = NULL;
	mPrestoMenuData = NULL;
	mGameSelector = NULL;
	mTitleScreen = NULL;
	mPetsScreen = NULL;
	mStoreScreen = NULL;
	mHatchScreen = NULL;
	mBonusScreen = NULL;
	mInterludeScreen = NULL;
	mStoryScreen = NULL;
	mSimSetupScreen = NULL;
	mSimFishScreen = NULL;
	mTankScreen = NULL;
	mHighScoreScreen = NULL;
	mHelpScreen = NULL;

	// Project started in november 2022 and was called simply Test Project
	mProdName = "Insaniquarium";
	
	mTitle = StringToSexyStringFast("Insaniquarium Deluxe " + mProductVersion);
	
	mRegKey = "PopCap\\Insaniquarium";
	mScreenSaverRegKey = "ScreenSaver\\";
	mScreenSaverRegPath = mScreenSaverRegKey;
	mYieldMainThread = false;
	mCurrentProfile = NULL;

	mSeed = new MTRand(Rand());

	mMusicVolume = 1.5;
	m0x7a4 = -1;
	m0x7a8 = -1;
	m0x7ac = -1;
	m0x8a8 = false;
	m0x8a9 = false;
	m0x8aa = false;
	m0x8ab = false;
	m0x883 = true;
	mFrameTime = 28;
	mGameMode = 0;
	mAutoEnable3D = true;
	m0x881 = true;
	m0x882 = true;
	mRelaxMode = false;
	m0x884 = false;
	mPlaytimeCounter2 = 0; 
	mPlaytimeCounter = 0; 
	mMaxExecutions = 0; 
	mMaxPlays = 0; 
	mMaxTime = 0; 
	mCompletedLoadingThreadTasks = 0;

	mProfileMgr = new ProfileMgr();

	mHighScoreMgr = new HighScoreMgr();

	mPrestoPtr = NULL;

	mScreenSaverSound = true;            // 0x839
	mScreenSaverRotateBackdrops = true;  // 0x83a
	mScreenSaverPeriodicDim = true;      // 0x83b
	mScreenSaverShowMoney = false;       // 0x83c
	mScreenSaverPowerSave = false;       // 0x83e
	mScreenSaverEnabled = false;         // 0x838
	mScreenSaverUnk01 = false;           // 0x83d

	mTankGameModeChoose = 1;

	m0x878 = false;

	time_t aTodayTimeSec = GetTodayStartSeconds();
	mDaysSinceFirstRun = aTodayTimeSec;
	mDaysSinceLastRun = aTodayTimeSec;
	mLDAccum = 0;

	mWorkerThread = new WorkerThread();

	mCurrentSongOffset = -1;
	mCurrentSongId = -1;
}


Sexy::WinFishApp::~WinFishApp()
{
	if (mBoard)
		SaveCurrentUserData();

	StopMusic();

	for (int i = 0; i < DIALOG_END_ID; i++)
		KillDialog(i);
	ProcessSafeDeleteList(); // This line missed in original game TODO

	if (mBoard != NULL)
	{
		mBoard->SaveCurrentGame();
		mWidgetManager->RemoveWidget(mBoard);
		delete mBoard;
		mBoard = NULL;
	}

	if (mTitleScreen != NULL)
		mWidgetManager->RemoveWidget(mTitleScreen);
	delete mTitleScreen;

	if (mStoreScreen != NULL)
		mWidgetManager->RemoveWidget(mStoreScreen);
	delete mStoreScreen;

	if (mHatchScreen != NULL)
		mWidgetManager->RemoveWidget(mHatchScreen);
	delete mHatchScreen;

	if (mBonusScreen != NULL)
		mWidgetManager->RemoveWidget(mBonusScreen);
	delete mBonusScreen;

	if (mHighScoreScreen != NULL)
		mWidgetManager->RemoveWidget(mHighScoreScreen);
	delete mHighScoreScreen;

	if (mHelpScreen != NULL)
		mWidgetManager->RemoveWidget(mHelpScreen);
	delete mHelpScreen;

	if (mInterludeScreen != NULL)
		mWidgetManager->RemoveWidget(mInterludeScreen);
	delete mInterludeScreen;

	if (mStoryScreen != NULL)
		mWidgetManager->RemoveWidget(mStoryScreen);
	delete mStoryScreen;

	if (mSimFishScreen != NULL)
		mWidgetManager->RemoveWidget(mSimFishScreen);
	delete mSimFishScreen;

	if (mTankScreen != NULL)
		mWidgetManager->RemoveWidget(mTankScreen);
	delete mTankScreen;

	if (mPetsScreen != NULL)
		mWidgetManager->RemoveWidget(mPetsScreen);
	delete mPetsScreen;

	if (mSimSetupScreen != NULL)
		mWidgetManager->RemoveWidget(mSimSetupScreen);
	delete mSimSetupScreen;

	if (mGameSelector != NULL)
		mWidgetManager->RemoveWidget(mGameSelector);
	delete mGameSelector;

	if(mProfileMgr)
		delete mProfileMgr;
	if(mHighScoreMgr)
		delete mHighScoreMgr;
}

void Sexy::WinFishApp::Init()
{
	DoParseCmdLine();
	bool anIsScreenSaver = IsScreenSaver();
	if (anIsScreenSaver)
		SEHCatcher::mShowUI = false;

	SexyApp::Init();

	if (CheckForVista())
	{
		MkDir(GetAppDataFolder());
		AllowAllAccess(GetAppDataFolder());
		InitUserDirectories("userdata");
		//	TODO WinFishAppMoveOldUserdataFolder 
		//	Migrate userdata from game folder to program data
	}

	if (!anIsScreenSaver)
	{
		char aModuleFileName[260];
		GetModuleFileNameA(NULL, aModuleFileName, sizeof(aModuleFileName));
		SexyString aModulePath = aModuleFileName;

		if (CheckForVista())
		{
			std::string aFileName = GetFileName(aModuleFileName);
			std::string aPath = RemoveTrailingSlash(mChangeDirTo) + "\\" + aFileName;
			aModulePath = aPath;
		}
		SexyString aExeDir = GetFileDir(aModulePath);

		SexyString aRegKeyDir = mScreenSaverRegKey + "Directory";

		RegistryWriteString(aRegKeyDir, aExeDir);

		bool aShouldCopy = GetBoolean("ScrCopy", false);

		if (!aShouldCopy)
		{
			SexyString aScrExeName = GetString("ScrExe", "");

			if (aScrExeName.length() == 0)
			{
				SexyString aRegKeyExe = mScreenSaverRegKey + "Executable";
				RegistryWriteString(aRegKeyExe, aModulePath);
			}
			else // If WinFish_Scr.exe exists in string map
			{
				std::string aDir = GetFileDir(aModulePath);

				std::string aDestPath = aDir + "\\" + aScrExeName;

				//if (!FileExists(aDestPath))
					//CopyFileA(aModulePath.c_str(), aDestPath.c_str(), FALSE);

				SexyString aRegKeyExe = mScreenSaverRegKey + "Executable";
				RegistryWriteString(aRegKeyExe, aDestPath);
			}
		}
	}

	bool aSuccessfulResLoad = mResourceManager->ParseResourcesFile("properties\\resources.xml");


	if (aSuccessfulResLoad)
	{
		aSuccessfulResLoad = mResourceManager->LoadResources("Init") && 
			ExtractInitResources(mResourceManager) && 
			LoadImageById(mResourceManager, IMAGE_STINKY_ID);

		if (aSuccessfulResLoad)
		{
			mProfileMgr->ReadUsersDat();
			mHighScoreMgr->ReadHighScoresData();

			SexyString aPath = mScreenSaverRegPath + "User";
			SexyString aUserName;

			if (RegistryReadString(aPath, &mScreenSaverUserName) && anIsScreenSaver && mScreenSaverUserName.size() != 0)
				mCurrentProfile = mProfileMgr->GetUserProfile(mScreenSaverUserName);

			bool aSuccessCurUser = true;

			if (mCurrentProfile == NULL)
			{
				if (!RegistryReadString("CurUser", &aUserName))
					aSuccessCurUser = false;
			}
			else
				aSuccessCurUser = false;

			if (aSuccessCurUser)
				mCurrentProfile = mProfileMgr->GetUserProfile(aUserName);

			if (mCurrentProfile == NULL)
				mCurrentProfile = mProfileMgr->GetFirstUserProfile();

			if (!anIsScreenSaver || mCurrentProfile != NULL)
			{
				mMaxExecutions = GetInteger("MaxExecutions", 0);
				mMaxPlays = GetInteger("MaxPlays", 0);
				mMaxTime = GetInteger("MaxTime", 60);

				Image* aImg = LoadMaskImage(IMAGE_TITLEPAGE, IMAGE_TITLEPAGEMASK, 384, 388);
				ReplaceImageById(mResourceManager, IMAGE_TITLEPAGEMASK_ID, aImg);

				mPrestoMenuData = new PrestoMenuData();

				mTitleScreen = new TitleScreen(this);
				mTitleScreen->Resize(0, 0, mWidth, mHeight);
				mWidgetManager->AddWidget(mTitleScreen);


				if (!anIsScreenSaver)
				{
					mMusicInterface->LoadMusic(4, "music/Insaniq2.mo3");
					mMusicInterface->LoadMusic(1, "music/Alien.mo3");
					mMusicInterface->LoadMusic(3, "music/Lullaby.mo3");

					// FUN_0054b1a0
					PlayMusic(2, 45);
				}
				SetCursorImage(CURSOR_POINTER, IMAGE_CURSOR_POINTER);
				SetCursorImage(CURSOR_HAND, IMAGE_CURSOR_HAND);
				SetCursorImage(CURSOR_DRAGGING, IMAGE_CURSOR_DRAGGING);
				SetCursorImage(CURSOR_TEXT, IMAGE_CURSOR_TEXT);
			}
			else
			{
				LogScreenSaverError("No User Profile");
				mLoadingFailed = true;
			}
		}
	}

	if (!aSuccessfulResLoad)
		WFAShowResourceError();
}

void Sexy::WinFishApp::LoadingThreadProc()
{
	SexyString aResNames[] = {"Register", "LoadingThread"};

	int arraySize = sizeof(aResNames) / sizeof(aResNames[0]);
	for (int i = 0; i < arraySize; i++)
		mNumLoadingThreadTasks += mResourceManager->GetNumResources(aResNames[i]);

	mNumLoadingThreadTasks += 33;


	for (int i = 0; i < arraySize; i++)
	{
		mResourceManager->StartLoadResources(aResNames[i]);

		while (mResourceManager->LoadNextResource())
		{
			if (mShutdown)
				return;
			mCompletedLoadingThreadTasks++;
		}

		if (mResourceManager->HadError() || !ExtractResourcesByName(mResourceManager, aResNames[i].c_str()))
		{
			WFAShowResourceError(false);
			mLoadingFailed = true;
		}

		m0x878 = true;
	}

	MemoryImage* aVertCreaseMImage = new MemoryImage(this);
	MemoryImage* aHorzCreaseMImage = new MemoryImage(this);
	aVertCreaseMImage->Create(IMAGE_VERTCREASE->mWidth, 60);
	aHorzCreaseMImage->Create(60, IMAGE_HORZCREASE->mHeight);
	Graphics aGVert(aVertCreaseMImage);
	Graphics aGHorz(aHorzCreaseMImage);

	int aHghtOrWdth = 0;
	while (aHghtOrWdth < 60)
	{
		aGVert.DrawImage(IMAGE_VERTCREASE, 0, aHghtOrWdth);
		aHghtOrWdth += IMAGE_VERTCREASE->mHeight;
	}
	aHghtOrWdth = 0;
	while (aHghtOrWdth < 60)
	{
		aGHorz.DrawImage(IMAGE_HORZCREASE, aHghtOrWdth, 0);
		aHghtOrWdth += IMAGE_HORZCREASE->mWidth;
	}

	ReplaceImageById(mResourceManager, IMAGE_VERTCREASE_ID, aVertCreaseMImage);
	ReplaceImageById(mResourceManager, IMAGE_HORZCREASE_ID, aHorzCreaseMImage);

	int aImgId = IMAGE_TANKMASK1_ID;
	int aImgYOffset = 365;
	int aImgCounter = 0;

	while (aImgCounter < 6)
	{
		Image* aMaskAquaImg = GetImageById(aImgId);
		Image* aAquaImg = GetImageById(aImgId-6);
		Image* aReplacementImg = LoadMaskImage(aAquaImg, aMaskAquaImg, 0, aImgYOffset);
		ReplaceImageById(mResourceManager, aImgId, aReplacementImg);
		if (mShutdown)
			return;

		mCompletedLoadingThreadTasks++;
		aImgId++;
		aImgCounter = aImgId - IMAGE_TANKMASK1_ID;
	}

	aImgId = IMAGE_SCL_STINKY_ID;
	aImgCounter = 0;
	while (aImgCounter < 24)
	{
		Image* aSclImg = GetImageById(aImgId + aImgCounter);
		MemoryImage* aSclMImage = new MemoryImage(this);
		aSclMImage->Create(40, 40);
		Graphics aGScl(aSclMImage);
		aGScl.SetFastStretch(false);

		Rect aDestRect;
		Rect aSrcRect;
		if (aImgId + aImgCounter == IMAGE_SCL_AMP_ID)
		{
			int aHght = aSclImg->mHeight;
			if (aHght > 23)
				aHght = 24;
			int aWdth = aSclImg->mWidth;
			if (aWdth > 63)
				aWdth = 64;
			aDestRect = Rect(0, 10, aSclMImage->mWidth, aSclMImage->mHeight-20);
			aSrcRect = Rect(0,0, aWdth, aHght);
		}
		else
		{
			int aHght = aSclImg->mHeight;
			if (aHght > 60)
				aHght = 60;
			int aWdth = aSclImg->mWidth;
			if (aWdth > 60)
				aWdth = 60;
			aDestRect = Rect(0, 0, aSclMImage->mWidth, aSclMImage->mHeight);
			aSrcRect = Rect(0, 0, aWdth, aHght);
		}

		aGScl.DrawImage(aSclImg, aDestRect, aSrcRect);

		mPrestoMenuData->mPetImages[aImgCounter] = aSclMImage;
		if (mShutdown)
			return;
		mCompletedLoadingThreadTasks++;
		aImgCounter++;
	}
}

void Sexy::WinFishApp::LoadingThreadCompleted()
{
	if (IsScreenSaver())
		return;

	if (ShouldCheckForUpdate())
		DoUpdateDialog();
}

MusicInterface* Sexy::WinFishApp::CreateMusicInterface(HWND theHWnd)
{
	if (!IsScreenSaver())
		return SexyApp::CreateMusicInterface(theHWnd);

	return new MusicInterface;
}

void Sexy::WinFishApp::URLOpenFailed(const std::string& theURL)
{
	SexyApp::URLOpenFailed(theURL);
	KillDialog(DIALOG_URL_INFO);
	CopyToClipboard(theURL);

	DoDialog(DIALOG_URL_INFO, true, "Open Browser", "Please open the following URL in your browser\n\n\n\nFor your convenience, this URL has already been copied to your clipboard.", "OK", Dialog::BUTTONS_FOOTER);
}

void Sexy::WinFishApp::URLOpenSucceeded(const std::string& theURL)
{
	SexyApp::URLOpenSucceeded(theURL);
	KillDialog(DIALOG_URL_INFO);
}

bool Sexy::WinFishApp::OpenURL(const std::string& theURL, bool shutdownOnOpen)
{
	DoDialog(DIALOG_URL_INFO, true, "Opening Browser", "Opening Browser", "", Dialog::BUTTONS_NONE);
	DrawDirtyStuff();
	return SexyApp::OpenURL(theURL, shutdownOnOpen);
}

Dialog* Sexy::WinFishApp::DoDialog(int theDialogId, bool isModal, const SexyString& theDialogHeader, const SexyString& theDialogLines, const SexyString& theDialogFooter, int theButtonMode)
{
	return SexyApp::DoDialog(theDialogId, isModal, theDialogHeader, theDialogLines, theDialogFooter, theButtonMode);
}

bool Sexy::WinFishApp::KillDialog(int theDialogId)
{
	if (SexyApp::KillDialog(theDialogId))
	{
		Widget* aTheScr = NULL;
		if (mDialogMap.size() == 0 && ((aTheScr = mStoreScreen) != NULL || 
			(aTheScr = mBoard) != NULL || (aTheScr = mGameSelector) != NULL))
			mWidgetManager->SetFocus(aTheScr);

		if (mBoard != NULL)
			if (!ShouldKillDialog())
				mBoard->PauseGame(false);

		return true;
	}
	return false;
}

Dialog* Sexy::WinFishApp::NewDialog(int theDialogId, bool isModal, const SexyString& theDialogHeader, const SexyString& theDialogLines, const SexyString& theDialogFooter, int theButtonMode)
{
	MoneyDialog* aDialog = new MoneyDialog(this, IMAGE_DIALOG, IMAGE_DIALOGBUTTON, 
		theDialogId, isModal, theDialogHeader, theDialogLines, theDialogFooter, theButtonMode);
	int aStrWdth = aDialog->mHeaderFont->StringWidth(theDialogHeader);
	aStrWdth += 150;
	if (aStrWdth < 348)
		aStrWdth = 348;
	int aPrefHeight = aDialog->GetPreferredHeight(aStrWdth);
	int aMinHeight = aDialog->mComponentImage->mHeight * 2 / 3 + 10;
	if (aPrefHeight < aMinHeight)
		aPrefHeight = aMinHeight;
	aDialog->Resize(143, 142, aStrWdth, aPrefHeight);
	return aDialog;
}

void Sexy::WinFishApp::SetMusicVolume(double theVolume)
{
	SexyApp::SetMusicVolume(theVolume);
}

void Sexy::WinFishApp::ModalOpen()
{
	if (mBoard != NULL)
		if (ShouldKillDialog())
			mBoard->PauseGame(true); 
}

void WinFishApp::LostFocus()
{
	if (mBoard != NULL && mGameMode != GAMEMODE_VIRTUAL_TANK)
		DoLostFocusDialog();
}

bool Sexy::WinFishApp::DebugKeyDown(int theKey)
{
	return SexyApp::DebugKeyDown(theKey);
}

void Sexy::WinFishApp::ButtonDepress(int theId)
{
	int anIdVar1 = theId % 10000;
	if (anIdVar1 < 2000)
		return;
	if (anIdVar1 >= 3000)
	{
		if (anIdVar1 > 3999) return;
		anIdVar1 = theId - 3000;
		if (anIdVar1 < 10007)
		{
			if (anIdVar1 != 10006)
			{
				switch(theId)
				{
				case 3003:
					KillDialog(3);
					Shutdown();
					break;
				case 3004:
					KillDialog(4);
					break;
				case 3000 + DIALOG_USER_DIALOG:
					UserDialogOkPressed(false);
					break;
				case 3000 + DIALOG_NEW_USER:
					MakeNewUser(false);
					break;
				case 3000 + DIALOG_ARE_YOU_SURE_DELETE:
					DeleteUser(false);
					break;
				case 3000 + DIALOG_RENAME:
					RenameUser(false);
					break;
				case 3000 + DIALOG_PET_DIALOG:
					ApplyPrestoMorph(-1);
					break;
				case 3000 + DIALOG_CONFIRM_PURCHASE:
					ConfirmFishPurchase(false);
					break;
				case 3000 + DIALOG_SELL_DIALOG:
					ApplySellGameObject(false);
					break;
				case 3000 + DIALOG_TIMES_UP:
					KillDialog(DIALOG_TIMES_UP);
					SwitchToBonusScreen();
					break;
				case 3000 + DIALOG_GIVE_SHELLS:
					ApplyGiveShells(false);
					break;
				default:
					KillDialog(anIdVar1);
				}
				return;
			}

			KillDialog(10006);
			anIdVar1 = 6;
		}

		KillDialog(anIdVar1);
	}
	anIdVar1 = theId - 2000;
	if (anIdVar1 > 10006)
	{
		if (anIdVar1 == 20006)
		{
			KillDialog(20006);
			KillDialog(DIALOG_UPDATE_CHECK);
			return;
		}
		KillDialog(anIdVar1);
		return;
	}
	if (anIdVar1 == 10006)
	{
		KillDialog(10006);
		KillDialog(DIALOG_UPDATE_CHECK);
		OpenURL(mInternetManager->GetUpdateURL(), true);
		return;
	}

	switch (anIdVar1)
	{
	case 0:
		KillDialog(0);
		SwitchToGameSelector();
		break;
	case DIALOG_OPTIONS:
		ApplyOptionsSettings();
		break;
	case DIALOG_REGISTER:
	{
		RegisterDialog* aDia = (RegisterDialog*)GetDialog(DIALOG_REGISTER);
		if (aDia)
		{
			SexyString aName = aDia->mNameEditWidget->mString;
			SexyString aCode = aDia->mCodeEditWidget->mString;
			if (aCode.length() == 0)
				OpenRegisterPage();
			else
			{
				if (Validate(aName, aCode))
				{
					KillDialog(DIALOG_REGISTER);
					KillDialog(3);
					mUserName = aName;
					mRegCode = aCode;
					mIsRegistered = true;
					mWidgetManager->MarkAllDirty();
					if (mTitleScreen == nullptr)
						DoDialog(DIALOG_THANKS_FOR_REGISTER, true, "Thanks!", "Thank you for registering Insaniquarium!", "Ok", Dialog::BUTTONS_FOOTER);
					else
						mTitleScreen->RegisterSuccessful();
				}
				else
				{
					DoInvalidCodeDialog();
				}
			}
		}
		break;
	}
	case 4:
		KillDialog(4);
		StartGame();
		break;
	case DIALOG_UPDATE_ASK:
		KillDialog(DIALOG_UPDATE_ASK);
		DoUpdateCheckDialog();
		break;
	case DIALOG_QUIT_GAME:
		KillDialog(DIALOG_QUIT_GAME);
		Shutdown();
		break;
	case 13:
		KillDialog(13);
	case 3:
		DoRegisterDialog();
		break;
	case DIALOG_INFO:
		KillDialog(DIALOG_INFO);
		break;
	case DIALOG_FISH_NAMING:
		ApplyFishNameDialog();
		break;
	case DIALOG_FIRST_LVL_GAME_OVER:
		KillDialog(DIALOG_FIRST_LVL_GAME_OVER);
		mBoard->SpawnGuppyBought();
		break;
	case DIALOG_GAME_OVER:
		KillDialog(DIALOG_GAME_OVER);
		RemoveBoard();
		SwitchToGameSelector();
		break;
	case DIALOG_LOST_FOCUS:
		KillDialog(DIALOG_LOST_FOCUS);
		break;
	case DIALOG_OUT_OF_MONEY_LOAN:
		KillDialog(DIALOG_OUT_OF_MONEY_LOAN);
		mBoard->Unk07(100);
		break;
	case 20:
		KillDialog(20);
		RemovePetsScreen();
		StartGame();
		break;
	case 21:
		KillDialog(21);
		if (mBoard)
			mBoard->BonusRoundDropShell();
		break;
	case DIALOG_LEAVE_GAME:
		KillDialog(DIALOG_LEAVE_GAME);
		LeaveGameBoard();
		break;
	case DIALOG_USER_DIALOG:
		UserDialogOkPressed(true);
		break;
	case DIALOG_NEW_USER:
		MakeNewUser(true);
		break;
	case DIALOG_ARE_YOU_SURE_DELETE:
		DeleteUser(true);
		break;
	case DIALOG_RENAME:
		RenameUser(true);
		break;
	case DIALOG_INFO_NEW_USER:
	{
		KillDialog(DIALOG_INFO_NEW_USER);
		NewUserDialog* aDia = (NewUserDialog*)GetDialog(DIALOG_NEW_USER);
		if (aDia)
			mWidgetManager->SetFocus(aDia->mEditWidget);
		break;
	}
	case DIALOG_NAME_CONFLICT:
	{
		KillDialog(DIALOG_NAME_CONFLICT);
		NewUserDialog* aDia = (NewUserDialog*)GetDialog(DIALOG_RENAME);
		if (aDia)
			mWidgetManager->SetFocus(aDia->mEditWidget);
	}
		break;
	case DIALOG_RESTART_GAME:
		RestartLevel();
		break;
	case DIALOG_CONFIRM_PURCHASE:
		ConfirmFishPurchase(true);
		break;
	case DIALOG_SELL_DIALOG:
		ApplySellGameObject(true);
		break;
	case DIALOG_SCREENSAVER:
		ApplyScreenSaverSettings();
		break;
	case DIALOG_TIMES_UP:
		KillDialog(DIALOG_TIMES_UP);
		SwitchToBonusScreen();
		break;
	case DIALOG_GIVE_SHELLS:
		ApplyGiveShells(true);
		break;
	default:
		KillDialog(anIdVar1);
		break;
	}
}

// AI Generated
void Sexy::WinFishApp::InitUserDirectories(const std::string& theSubfolderName)
{
	if (!CheckForVista())
		return;

	std::string aCurrentDir = GetCurDir();
	aCurrentDir = RemoveTrailingSlash(aCurrentDir);
	std::string aRelativePath = aCurrentDir + "\\" + theSubfolderName;

	std::string aAppDataPath = GetAppDataFolder();
	std::string aProperPath = aAppDataPath + "\\" + theSubfolderName;

	if (FileExists(aRelativePath) && FileExists(aProperPath))
		return;

	MkDir(aRelativePath);
	MkDir(aProperPath);
}

void Sexy::WinFishApp::PlayMusic(int theSongId, int theSongOffset, bool noLoop)
{
	mCurrentSongId = theSongId;
	mCurrentSongOffset = theSongOffset;

	RemapMusicTrack(&theSongId, &theSongOffset);

	mMusicInterface->PlayMusic(theSongId, theSongOffset, noLoop);
}

void Sexy::WinFishApp::SomeMusicFunc(bool flag)
{
	if (mGameMode == GAMEMODE_VIRTUAL_TANK)
	{
		flag = false;
		StopMusic();
	}
	else if (flag)
	{
		int aTank = GetCurrentTank();
		if (aTank == 1 || aTank == 2)
			FadeOutMusic(0, false, 0.008);
		else if (aTank == 3 || aTank == 4)
			FadeOutMusic(2, false, 0.008);
	}
	else
		StopMusic();

	PlayMusic(1, 0);
	if (flag)
	{
		mMusicInterface->StopMusic(1);
		FadeInMusic(1, -1, 0.002, false);
	}
}

void Sexy::WinFishApp::FadeOutMusic(int theSongId, bool stopSong, double theSpeed)
{
	if (mCurrentSongId == theSongId)
		mCurrentSongId = -1;

	if (gCanRemapMusic && (theSongId == 0 || theSongId == 2))
		theSongId = 4;

	mMusicInterface->FadeOut(theSongId, stopSong, theSpeed);
}

void Sexy::WinFishApp::FadeInMusic(int theSongId, int theSongOffset, double theSpeed, bool noLoop)
{
	mCurrentSongOffset = theSongOffset;
	mCurrentSongId = theSongId;
	RemapMusicTrack(&theSongId, &theSongOffset);
	mMusicInterface->FadeIn(theSongId, theSongOffset, theSpeed, noLoop);
}

void Sexy::WinFishApp::RemapMusicTrack(int* theSongId, int* theSongOffset) // FUN_0054b090
{
	if (!gCanRemapMusic)
		return;

	int aOriginalSongId = *theSongId;
	int aOriginalOffset = *theSongOffset;

	if (aOriginalSongId == 0)
	{
		if (aOriginalOffset == 22) // 0x16
		{
			*theSongId = 4;
			*theSongOffset = 0; // asembler postavlja ovo na originalni song id, što je 0
			return;
		}
		if (aOriginalOffset == 0)
		{
			*theSongId = 4;
			*theSongOffset = 12; // 0xc
			return;
		}
	}

	if (aOriginalSongId == 2)
	{
		if (aOriginalOffset == 32) { // 0x20
			*theSongId = 4;
			*theSongOffset = 25; // 0x19
			return;
		}
		if (aOriginalOffset == 45) { // 0x2d
			*theSongId = 4;
			*theSongOffset = 37; // 0x25
			return;
		}
		if (aOriginalOffset == 0) {
			*theSongId = 4;
			*theSongOffset = 45; // 0x2d
			return;
		}
		if (aOriginalOffset == 54) { // 0x36
			*theSongId = 4;
			*theSongOffset = 49; // 0x31
			return;
		}
		if (aOriginalOffset == 5) {
			*theSongId = 4;
			*theSongOffset = 50; // 0x32
			return;
		}
	}

	if ((aOriginalSongId == 0) && (aOriginalOffset == 13)) // 0xd
	{
		*theSongId = 4;
		*theSongOffset = 58; // 0x3a
		return;
	}
}

Image* Sexy::WinFishApp::LoadMaskImage(Image* theImage, Image* theImageMask, int theX, int theY)
{
	int aImageWidth = theImage->GetWidth();
	int aImageHeight = theImage->GetHeight();

	int aMaskWidth = theImageMask->GetWidth();
	int aMaskHeight = theImageMask->GetHeight();

	Rect aImageRect(0, 0, aImageWidth, aImageHeight);

	Rect aMaskRect(theX, theY, aMaskWidth, aMaskHeight);

	Rect aFinalClipRect = aMaskRect.Intersection(aImageRect);

	if (aFinalClipRect.mWidth <= 0 || aFinalClipRect.mHeight <= 0)
		return NULL;

	MemoryImage* aNewImage = new MemoryImage(gSexyApp);
	aNewImage->Create(aFinalClipRect.mWidth, aFinalClipRect.mHeight);

	Graphics g(aNewImage);
	g.DrawImage(theImage, -aFinalClipRect.mX, -aFinalClipRect.mY);


	DWORD* aNewBits = aNewImage->GetBits();
	MemoryImage* aMemoryMask = dynamic_cast<MemoryImage*>(theImageMask);
	DWORD* aMaskBits = aMemoryMask->GetBits();

	if (aNewBits == NULL || aMaskBits == NULL)
	{
		delete aNewImage;
		return NULL;
	}

	for (int y = 0; y < aFinalClipRect.mHeight; ++y)
	{
		for (int x = 0; x < aFinalClipRect.mWidth; ++x)
		{
			int aNewImageIndex = y * aFinalClipRect.mWidth + x;

			int aMaskX = x + (aFinalClipRect.mX - theX);
			int aMaskY = y + (aFinalClipRect.mY - theY);
			int aMaskIndex = aMaskY * aMaskWidth + aMaskX;

			DWORD aColor = aNewBits[aNewImageIndex] & 0x00FFFFFF;

			DWORD aAlpha = aMaskBits[aMaskIndex] & 0xFF000000;

			aNewBits[aNewImageIndex] = aColor | aAlpha;
		}
	}

	aNewImage->BitsChanged();
	return aNewImage;
}

void Sexy::WinFishApp::Start()
{
	if (!mLoadingFailed)
	{
		SexyApp::Start();
		return;
	}

	if (IsScreenSaver() && !gScreenSaverStarted)
	{
		StartScreenSaver();
		return;
	}
}

void Sexy::WinFishApp::Shutdown()
{
	if (mShutdown)
		return;

	if (mBoard != NULL)
	{
		SaveCurrentUserData();
		RemoveBoard();
	}
	UpdatePlayData();

	time_t aTodaysDate = GetTodayStartSeconds();
	int aSecondsPlayed = mUpdateCount / 36;
	mLDAccum += aSecondsPlayed;

	if ((mLDAccum >= 3600) || (aTodaysDate > mDaysSinceFirstRun)) // modified crack code i guess
	{
		mDaysSinceFirstRun = aTodaysDate;
		mLDAccum = 0;
	}
	else if (aTodaysDate < mDaysSinceFirstRun)
	{
	}

	SexyApp::Shutdown();
}

void Sexy::WinFishApp::StartScreenSaver()
{
	mScreenSaverEnabled = false;
	gScreenSaverStarted = true;
	SetScreenSaver(mScreenSaverOldPath.c_str());
}

void Sexy::WinFishApp::SetScreenSaver(const char* thePath)
{
	OSVERSIONINFOA aVersionInfo;
	ZeroMemory(&aVersionInfo, sizeof(OSVERSIONINFOA));
	aVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
	GetVersionExA(&aVersionInfo);

	if (aVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	{
		const char* aValue = thePath;
		if (thePath != NULL && strlen(thePath) == 0)
			aValue = NULL;

		WritePrivateProfileStringA("boot", "SCRNSAVE.EXE", aValue, "SYSTEM.INI");
	}
	else
	{
		HKEY aKey;
		if (RegOpenKeyExA(HKEY_CURRENT_USER, "Control Panel\\Desktop", 0, KEY_WRITE, &aKey) == ERROR_SUCCESS)
		{
			if (thePath != NULL && strlen(thePath) > 0)
			{
				DWORD aLen = (DWORD)strlen(thePath) + 1;
				RegSetValueExA(aKey, "SCRNSAVE.EXE", 0, REG_SZ, (const BYTE*)thePath, aLen);
			}
			else
			{
				//RegDeleteValueA(aKey, "SCRNSAVE.EXE");
			}

			RegCloseKey(aKey);
		}
	}
}

void Sexy::WinFishApp::ReadFromRegistry()
{
	SexyApp::ReadFromRegistry();

	bool aScrSound = true;
	GetBoolean("ScrSound", &aScrSound);
	mScreenSaverSound = aScrSound;

	bool aScrPowersave = false;
	GetBoolean("ScrPowersave", &aScrPowersave);	
	mScreenSaverPowerSave = aScrPowersave;

	char aUserNameBuffer[1024];
	DWORD aUserNameSize = sizeof(aUserNameBuffer);
	if (GetUserNameA(aUserNameBuffer, &aUserNameSize))
	{
		mScreenSaverRegPath.append(aUserNameBuffer, strlen(aUserNameBuffer));
		mScreenSaverRegPath += '\\';
	}

	DemoSyncString(&mScreenSaverRegPath);

	RegistryReadString(mScreenSaverRegPath + "OldPath", &mScreenSaverOldPath);
	RegistryReadBoolean(mScreenSaverRegPath + "Enabled", &mScreenSaverEnabled);
	ReadSSFromRegistry();

	if (IsScreenSaver())
	{
		if (!mScreenSaverPowerSave)
		{
			mIsWindowed = false;
			mAllowMonitorPowersave = false;
		}
		else
		{
			mIsWindowed = true;
			mFullScreenWindow = true;
		}
		if (mScreenSaverSound)
			mNoSoundNeeded = true;
		return;
	}

	RegistryReadInteger("ldaccum", &mLDAccum);

	int aTempLDInfo;
	bool aSuccessRead = RegistryReadInteger("ldinfo", &aTempLDInfo);
	time_t aDaysLDInfo;
	if (aSuccessRead)
		aDaysLDInfo = (unsigned int)aTempLDInfo;

	bool aInvalidData = !aSuccessRead;

	if (aSuccessRead && aDaysLDInfo > mDaysSinceLastRun)
		aInvalidData = false;

	if (!aInvalidData)
		mLDAccum = 0;
	else
		mDaysSinceLastRun = mDaysSinceFirstRun;
}

void Sexy::WinFishApp::WriteToRegistry()
{
	if (mCurrentProfile)
	{
		if (!IsScreenSaver())
			RegistryWriteString("CurUser", mCurrentProfile->mUserName);
		mCurrentProfile->Save();
	}

	if (!IsScreenSaver())
	{
		SexyApp::WriteToRegistry();
		RegistryWriteInteger("ldinfo", mDaysSinceFirstRun);
		RegistryWriteInteger("ldaccum", mLDAccum);
	}
}

void Sexy::WinFishApp::ReadSSFromRegistry()
{
	RegistryReadBoolean(mScreenSaverRegPath + "Sound", &mScreenSaverSound);
	RegistryReadBoolean(mScreenSaverRegPath + "RotateBackDrops", &mScreenSaverRotateBackdrops);
	RegistryReadBoolean(mScreenSaverRegPath + "PeriodicDim", &mScreenSaverPeriodicDim);
	RegistryReadBoolean(mScreenSaverRegPath + "ShowMoney", &mScreenSaverShowMoney);
	RegistryReadBoolean(mScreenSaverRegPath + "Powersave", &mScreenSaverPowerSave);

	SexyString aPath2 = GetScreenSaverFilePath();// FUN_0054d230(local_30);
	SexyString aPathToSYSSS;
	GetSystemScreenSaverPath(aPathToSYSSS);//FUN_0054d0c0(); 0054e46b

	bool aPathsAreTheSame = (_stricmp(aPathToSYSSS.c_str(), aPath2.c_str()) == 0);

	if (!mScreenSaverEnabled)
	{
		if (aPathsAreTheSame)
			mScreenSaverEnabled = true;
	}
	else
	{
		if (!aPathsAreTheSame)
			mScreenSaverEnabled = false;
	}
}

SexyString Sexy::WinFishApp::GetScreenSaverFilePath()
{
	char aModulePathBuffer[260];
	GetModuleFileNameA(NULL, aModulePathBuffer, sizeof(aModulePathBuffer));
	SexyString aModulePath = aModulePathBuffer;
	SexyString aParam1;

	if (CheckForVista())
	{
		SexyString aFileName = GetFileName(aModulePath, false);
		SexyString aDir = RemoveTrailingSlash(mChangeDirTo);
		aModulePath = aDir + "\\" + aFileName;
	}

	if (CheckForVista())
		aParam1 = RemoveTrailingSlash(mChangeDirTo);
	else
		aParam1 = GetFileDir(aModulePath, false);

	SexyString aDestPath = aParam1 + "\\Insaniquarium.scr";

	char aShortPathBuffer[260];
	if (GetShortPathNameA(aDestPath.c_str(), aShortPathBuffer, sizeof(aShortPathBuffer)) > 0)
		aDestPath = aShortPathBuffer;

	return aDestPath;
}

void Sexy::WinFishApp::GetSystemScreenSaverPath(SexyString& theDest)
{
	OSVERSIONINFOA aVersionInfo;
	ZeroMemory(&aVersionInfo, sizeof(OSVERSIONINFOA));
	aVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
	GetVersionExA(&aVersionInfo);

	char aPathBuffer[261]; 
	aPathBuffer[0] = '\0';

	if (aVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	{
		GetPrivateProfileStringA(
			"boot",
			"SCRNSAVE.EXE",
			"",
			aPathBuffer,
			sizeof(aPathBuffer),
			"SYSTEM.INI"
		);
	}
	else
	{
		HKEY aKey;
		if (RegOpenKeyExA(HKEY_CURRENT_USER, "Control Panel\\Desktop", 0, KEY_READ, &aKey) == ERROR_SUCCESS)
		{
			DWORD aLen = sizeof(aPathBuffer);
			DWORD aType = REG_SZ;

			RegQueryValueExA(
				aKey,
				"SCRNSAVE.EXE",
				NULL,
				&aType,
				(LPBYTE)aPathBuffer,
				&aLen
			);

			RegCloseKey(aKey);

			if (aLen > 0 && aLen < sizeof(aPathBuffer))
				aPathBuffer[aLen] = '\0';
			else
				aPathBuffer[0] = '\0';
		}
	}

	//Sexy::CopyStringTo(aOutPath, aPathBuffer, strlen(aPathBuffer));
	theDest.assign(aPathBuffer, strlen(aPathBuffer));
}

bool Sexy::WinFishApp::ShouldKillDialog()
{
	if ((mStoreScreen == nullptr) && (mPetsScreen == nullptr) &&
		(mSimSetupScreen == nullptr) && (mSimFishScreen == nullptr))
	{
		if (mDialogList.size() != 0)
		{
			Dialog* aTopDialog = mDialogList.back();

			if (aTopDialog->mId != 31 && aTopDialog->mId != 32)
				return true;
		}
		return false;
	}
	return true;
}

void Sexy::WinFishApp::DoLeaveGameDialog()
{
	Dialog* aDia = DoDialog(DIALOG_LEAVE_GAME, true, "Leave Game?", "Do you want to return to the\nmain menu?\n\nYour game will be saved.", "", Dialog::BUTTONS_YES_NO);
	aDia->mYesButton->mLabel = "LEAVE";
	aDia->mNoButton->mLabel = "CANCEL";
}

void Sexy::WinFishApp::DoLostFocusDialog()
{
	if (mBoard != NULL)
	{
		if (!IsScreenSaver() && !mBoard->mPause)
		{
			mBoard->PauseGame(true);
			CleanDialogs();
			DoDialog(DIALOG_LOST_FOCUS, true, "GAME PAUSED", "Click to resume game", "Resume game", Dialog::BUTTONS_FOOTER);
		}
	}
}

void Sexy::WinFishApp::DoVirtualDialog()
{
	KillDialog(DIALOG_VIRTUAL);

	VirtualDialog* aDia = new VirtualDialog(this);
	aDia->Resize(320 - aDia->mWidth / 2, 70, aDia->mWidth, aDia->mHeight);

	AddDialog(DIALOG_VIRTUAL, aDia);
}

void Sexy::WinFishApp::DoScreenSaverDialog()
{
	ReadSSFromRegistry();
	ScreenSaverDialog* aDia = new ScreenSaverDialog(this);
	int aPrefHght = aDia->GetPreferredHeight(420);
	aDia->Resize(110, 40, 420, aPrefHght);
	AddDialog(DIALOG_SCREENSAVER, aDia);
}

void Sexy::WinFishApp::DoAreYouSureSellDialog(const SexyString& theLine)
{
	Dialog* aDia = DoDialog(DIALOG_SELL_DIALOG, true, "ARE YOU SURE?", theLine, "", Dialog::BUTTONS_OK_CANCEL);
	aDia->mYesButton->mLabel = "Sell";
	int aDiaY = 250;
	if (mSimFishScreen == nullptr)
		aDiaY = 60;
	aDia->Resize(aDia->mX, aDiaY, aDia->mWidth, aDia->mHeight);
}

void Sexy::WinFishApp::OpenPrestoDialog(GameObject* thePet)
{
	CleanDialogs();
	mPrestoPtr = (OtherTypePet*) thePet;
	Dialog* aDia = new PetDialog(this);
	AddDialog(DIALOG_PET_DIALOG, aDia);
}

void Sexy::WinFishApp::DoConfirmPurchaseDialog(const SexyString& theLine)
{
	Dialog* aDia = DoDialog(DIALOG_CONFIRM_PURCHASE, true, "Confirm Purchase", theLine, "", Dialog::BUTTONS_OK_CANCEL);

	aDia->mYesButton->mLabel = "BUY";
	int aPrefHght = aDia->GetPreferredHeight(350);
	aDia->Resize(aDia->mX, aDia->mY, 350, aPrefHght);
}

void Sexy::WinFishApp::DoFishNamingDialog(const SexyString& theLine, SexyString& theName, bool flag)
{
	FishNamingDialog* aDia = new FishNamingDialog(this, theLine);
	aDia->m0x168 = flag;
	aDia->mEditWidget->SetText(theName, true);
	aDia->mEditWidget->EnforceMaxPixels();
	aDia->mEditWidget->mCursorPos = aDia->mEditWidget->mString.length();
	aDia->mEditWidget->mHilitePos = 0;
	if (mStoreScreen == nullptr)
		aDia->Resize(120, 150, 400, aDia->GetPreferredHeight(400));
	else
		aDia->Resize(218, 200, 400, aDia->GetPreferredHeight(400));
	AddDialog(DIALOG_FISH_NAMING, aDia);
}

void Sexy::WinFishApp::DoFoodDialog()
{
	CleanDialogs();
	FoodDialog* aDia = new FoodDialog(this);
	AddDialog(DIALOG_FOOD, aDia);
}

void Sexy::WinFishApp::DoTimesUpDialog()
{
	CleanDialogs();
	DoDialogUnkF(DIALOG_TIMES_UP, true, "TIME\'S UP!", StrFormat("Your %d minutes are up!", mBoard->m0x3bc / 60), "Click for Results", Dialog::BUTTONS_FOOTER);
}

void Sexy::WinFishApp::DoRegisterDialog()
{
	if (mCompletedLoadingThreadTasks <= 29)
		return;

	if (!mIsRegistered)
	{
		RegisterDialog* aDia = new RegisterDialog(this);
		int aPrefHght = aDia->GetPreferredHeight(420);
		aDia->Resize((mWidth - 420) / 2, 32, 420, aPrefHght);
		AddDialog(DIALOG_REGISTER ,aDia);
	}
	else
	{
		DoDialog(DIALOG_INFO, true, "Already Registered", "You have already registered Insaniquarium.", "OK", Dialog::BUTTONS_FOOTER);
	}
}

void Sexy::WinFishApp::DoInvalidCodeDialog()
{
	DoDialog(DIALOG_INVALID_CODE, true, "Invalid Code", 
		"The license code you entered is not valid for that name.\n\nMake sure the name and registration number are entered correctly.", 
		"OK", Dialog::BUTTONS_FOOTER);
}

void Sexy::WinFishApp::DoDialogUnkF(int theId, bool isModal, const SexyString& theDiaHeader, const SexyString& theDiaLines, const SexyString& theDiaFooter, int theBtnMode)
{
	MoneyDialog* aDia = (MoneyDialog*) DoDialog(theId, isModal, theDiaHeader, theDiaLines, theDiaFooter, theBtnMode);
	aDia->DisableButtons(30);
}

void Sexy::WinFishApp::CleanDialogs()
{
	ApplyPrestoMorph(-1);
	KillDialog(32);
}

void Sexy::WinFishApp::ApplyPrestoMorph(int thePetId)
{
	KillDialog(DIALOG_PET_DIALOG);
	if (thePetId > -1 && mPrestoPtr && mBoard)
	{
		GameObject* aPet = mBoard->GetPrestoPet();
		if (aPet)
			aPet->PrestoMorph(thePetId);
	}
	mPrestoPtr = nullptr;
}

void Sexy::WinFishApp::SaveCurrentUserData()
{
	if (mCurrentProfile)
		mCurrentProfile->Save();
}

void Sexy::WinFishApp::SaveVirtualTankAndUserData()
{
	if (mGameMode != GAMEMODE_VIRTUAL_TANK)
		return;
	SaveCurrentUserData();
	if (mBoard)
		mBoard->SaveCurrentGame();
}

void Sexy::WinFishApp::ApplyGiveShells(bool doApply)
{
	UserDialog* aDia = (UserDialog*)GetDialog(DIALOG_GIVE_SHELLS);
	if (!aDia)
		return;

	if (doApply && mCurrentProfile)
	{
		int aShells = aDia->GetShellsValue();

		if (mCurrentProfile->mShells < aShells)
		{
			DoDialog(DIALOG_INFO, true, "Not Enough Shells", "You don\'t have that many shells to transfer.", "OK", Dialog::BUTTONS_FOOTER);
			return;
		}

		SexyString aSelUserName = aDia->GetSelectedUserName();
		UserProfile* aProf = mProfileMgr->GetUserProfile(aSelUserName);
		if (!aProf)
		{
			DoDialog(DIALOG_INFO, true, "Choose User", "Please choose a user from the list.", "OK", Dialog::BUTTONS_FOOTER);
			return;
		}
		if (aProf != mCurrentProfile)
		{
			aProf->LoadFromMemory();
			aProf->AddShells(aShells);
			mCurrentProfile->AddShells(-aShells);
			aProf->Save();
			mCurrentProfile->Save();
		}
	}

	KillDialog(DIALOG_GIVE_SHELLS);
}

void Sexy::WinFishApp::ApplyScreenSaverSettings()
{
	ScreenSaverDialog* aDia = (ScreenSaverDialog*)GetDialog(DIALOG_SCREENSAVER);
	if (aDia == nullptr)
		return;

	SexyString aSSFilePath = GetScreenSaverFilePath();

	if (!aDia->mSSEnabledCB->mChecked)
		StartScreenSaver();
	else
	{
		SexyString aFilePath = "";
		GetSystemScreenSaverPath(aFilePath);
		if (strcmp(aSSFilePath.c_str(), aFilePath.c_str()) != 0)
			mScreenSaverOldPath = aFilePath;

		if (FileExists(aSSFilePath))
		{
			bool copyFailed = GetBoolean("ScrCopy");
			if (copyFailed)
			{
				if (DoScrCopy(aSSFilePath))
					copyFailed = false;
			}

			if (copyFailed)
			{
				Popup("Failed to make screensaver.dat");
				mScreenSaverEnabled = false;
			}
			else
			{
				PVOID aParam = 0;
				SystemParametersInfoA(SPI_GETSCREENSAVEACTIVE, 0, &aParam, 0);
				if(aParam == 0)
					SystemParametersInfoA(SPI_SETSCREENSAVEACTIVE, 1, &aParam, 1);
				SetScreenSaver(aSSFilePath.c_str());
				mScreenSaverEnabled = true;
			}
		}
		else
			mScreenSaverEnabled = false;
	}

	mScreenSaverSound = aDia->mSSSoundCB->mChecked;
	mScreenSaverRotateBackdrops = aDia->mSSRotateBackdropsCB->mChecked;
	mScreenSaverPeriodicDim = aDia->mSSPeriodicDimCB->mChecked;
	mScreenSaverShowMoney = aDia->mSSShowMoneyCB->mChecked;
	mScreenSaverPowerSave = aDia->mSSPowerSaveCB->mChecked;

	if (!aDia->mSSEnabledCB->mChecked || mCurrentProfile == nullptr)
		mScreenSaverUserName = "";
	else
		mScreenSaverUserName = mCurrentProfile->mUserName;

	RegistryWriteString(mScreenSaverRegPath + "User", mScreenSaverUserName);
	RegistryWriteString(mScreenSaverRegPath + "OldPath", mScreenSaverOldPath);
	RegistryWriteBoolean(mScreenSaverRegPath + "Enabled", mScreenSaverEnabled);
	RegistryWriteBoolean(mScreenSaverRegPath + "Sound", mScreenSaverSound);
	RegistryWriteBoolean(mScreenSaverRegPath + "RotateBackDrops", mScreenSaverRotateBackdrops);
	RegistryWriteBoolean(mScreenSaverRegPath + "PeriodicDim", mScreenSaverPeriodicDim);
	RegistryWriteBoolean(mScreenSaverRegPath + "ShowMoney", mScreenSaverShowMoney);
	RegistryWriteBoolean(mScreenSaverRegPath + "Powersave", mScreenSaverPowerSave);
	KillDialog(DIALOG_SCREENSAVER);
}

void Sexy::WinFishApp::ApplyFishNameDialog()
{
	FishNamingDialog* aDia = (FishNamingDialog*)GetDialog(DIALOG_FISH_NAMING);
	SexyString aFishName = aDia->mEditWidget->mString;
	SexyString aWhitespaceChars = " \t\r\n";
	inlineTrim(aFishName);
	if (aFishName.length() == 0)
	{
		DoDialog(DIALOG_INFO, true, "Invalid Name", "Names must be one or more letters in length", "OK", Dialog::BUTTONS_FOOTER);
	}
	else
	{
		KillDialog(DIALOG_FISH_NAMING);
		if (mStoreScreen == nullptr)
		{
			if (mSimFishScreen)
			{
				mSimFishScreen->RenameSelectedFish(aFishName);
				mWidgetManager->BringToFront(mSimFishScreen);
			}
		}
		else
		{
			GameObject* aProduct = mStoreScreen->ConfirmPurchase();
			if (aProduct)
			{
				aProduct->mName = aFishName;
				InitializeSpecialNamedFish(aProduct);
				mBoard->SpawnGameObject(aProduct, true);
				mWidgetManager->BringToFront(mStoreScreen);
			}
		}
	}
}

void Sexy::WinFishApp::ApplySellGameObject(bool sell)
{
	if (mSimFishScreen)
		mSimFishScreen->SellSelectedObject(sell);
	else if (mSimSetupScreen)
		mSimSetupScreen->SellBackground(sell);
	KillDialog(DIALOG_SELL_DIALOG);
}

void Sexy::WinFishApp::ConfirmFishPurchase(bool confirmed)
{
	KillDialog(DIALOG_CONFIRM_PURCHASE);
	if (confirmed)
	{
		if (mStoreScreen)
		{
			mStoreScreen->ConfirmPurchase();
		}
		else if (mBonusScreen)
		{
			mBonusScreen->ConfirmPurchase();
		}
	}
}

void Sexy::WinFishApp::InitializeSpecialNamedFish(GameObject* theObject)
{
	if (stricmp(theObject->mName.c_str(), "1SingingFish") == 0)
	{
		theObject->mSinging = true;
	}
	else if (stricmp(theObject->mName.c_str(), "santa") == 0 && theObject->mType == GameObject::TYPE_GUPPY)
	{
		for (GameObjectSet::iterator it = mBoard->mGameObjectSet.begin(); it != mBoard->mGameObjectSet.end(); ++it)
		{
			GameObject* anObj = *it;
			if (anObj->mPreNamedTypeId == GameObject::SANTA)
				return;
		}
		Fish* aFish = (Fish*)theObject;
		if (!aFish->mHasSpecialColors && !aFish->mRainbowFish && aFish->mPreNamedTypeId == -1)
		{
			aFish->mPreNamedTypeId = GameObject::SANTA;
			aFish->mHasSpecialColors = true;
			aFish->mColors[0] = Color(0xffffff);
			aFish->mColors[1] = Color(0xff0000);
			aFish->mColors[2] = Color(0xffffff);
			aFish->mSinging = true;
			aFish->mFoodNeededToGrow = 1;
		}
	}
}

bool Sexy::WinFishApp::CanAlienChaseAnyFish()
{
	if (mRelaxMode)
	{
		if (mUpdateCount - gUnkInt03 >= 360)
		{
			int aFishCnt = mBoard->mFishList->size();
			int anOscarCnt = mBoard->mOscarList->size();
			int anUltraCnt = mBoard->mUltraList->size();
			int aBreederCnt = mBoard->mBreederList->size();
			int aPentaCnt = mBoard->mPentaList->size();
			int aGrubberCnt = mBoard->mGrubberList->size();
			int aGekkoCnt = mBoard->mGekkoList->size();
			
			int allCnt = aFishCnt + anOscarCnt + anUltraCnt + aBreederCnt + aPentaCnt + aGrubberCnt + aGekkoCnt;
			if (allCnt > 1)
				return true;

			gUnkInt03 = mUpdateCount - 500;
		}
		return false;
	}
	return true;
}

bool GetFileLastWriteTime(const char* path, FILETIME* outTime)
{
	HANDLE hFile = CreateFileA(
		path,
		GENERIC_READ,
		0,
		nullptr,
		OPEN_EXISTING,
		0,
		nullptr
	);

	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	BOOL ok = GetFileTime(hFile, nullptr, nullptr, outTime);
	CloseHandle(hFile);

	return ok != 0;
}

bool Sexy::WinFishApp::DoScrCopy(SexyString& theScrSvrPath)
{
	SexyString aDatPath = "";
	if (CheckForVista())
	{
		aDatPath = GetAppDataFolder() + "\\screensaver.dat";
	}
	else
	{
		aDatPath = GetFileDir(theScrSvrPath) + "\\screensaver.dat";
	}

	FILETIME ft1;
	FILETIME ft2;

	bool isOk1 = GetFileLastWriteTime(theScrSvrPath.c_str(), &ft1);
	bool isOk2 = GetFileLastWriteTime(aDatPath.c_str(), &ft2);

	// TODO
	return false;
}

void Sexy::WinFishApp::DoOptionsDialog(bool theFlag)
{
	CleanDialogs();
	OptionsDialog* aDia = new OptionsDialog(this, theFlag);
	int aPrefHght = aDia->GetPreferredHeight(348);
	aDia->Resize(143, 26, 348, aPrefHght);
	AddDialog(DIALOG_OPTIONS, aDia);
}

void Sexy::WinFishApp::SwitchToBoard(bool theFlag1, bool theFlag2)
{
	if (CheckTrialEnded())
	{
		SwitchToGameSelector();
		return;
	}
	RemoveBoard();
	if (mGameMode == GAMEMODE_VIRTUAL_TANK && !gFirstVirtualTankEnter)
	{
		gUnkBool05 = true;
		gFirstVirtualTankEnter = true;
		mWorkerThread->DoTask(LoadFishSongsTaskWrapper, 0);
	}
	if (theFlag1 && LoadBoardGame())
		return;

	for (int i = 0; i < 24; i++)
		mCurrentProfile->m0x5a[i] = false;

	if (theFlag2 && mGameMode == GAMEMODE_ADVENTURE && !mCurrentProfile->mFinishedGame && 
		mCurrentProfile->mTank == 1 && mCurrentProfile->mLevel < 4)
	{
		SwitchToHelpScreen(true);
		return;
	}
	if (!IsBonusLevel() && mGameMode != GAMEMODE_VIRTUAL_TANK && 
		(mCurrentProfile->mNumOfUnlockedPets > 3 && (mGameMode != GAMEMODE_ADVENTURE || mCurrentProfile->mTank != 5)))
	{
		SwitchToPetsScreen();
		return;
	}
	StartGame();
}

void Sexy::WinFishApp::DoQuitDialog()
{
	Dialog* aDia = DoDialog(DIALOG_QUIT_GAME, true, "Quit", "Stop the insanity?", "", Dialog::BUTTONS_OK_CANCEL);
	aDia->mYesButton->mLabel = "Quit";
}

void Sexy::WinFishApp::SwitchToGameSelector()
{
	RemoveBoard();
	UpdatePlayData();
	if (mGameSelector)
	{
		mWidgetManager->RemoveWidget(mGameSelector);
		SafeDeleteWidget(mGameSelector);
	}
	if (mCurrentProfile)
		mCurrentProfile->Unk01();

	mGameSelector = new GameSelector(this);
	mGameSelector->Resize(0, 0, mWidth, mHeight);
	mWidgetManager->AddWidget(mGameSelector);
	mWidgetManager->BringToBack(mGameSelector);
	mWidgetManager->SetFocus(mGameSelector);

	if (!mCurrentProfile)
	{
		DoNewUserDialog();
	}
	if (CheckTrialEnded())
		DoTrialVersionExpiredDialog();
}

void Sexy::WinFishApp::SwitchToHelpScreen(bool instructions)
{
	RemoveGameSelector();
	RemoveHelpScreen();
	mHelpScreen = new HelpScreen(this, instructions);
	mHelpScreen->Resize(0, 0, mWidth, mHeight);
	mWidgetManager->AddWidget(mHelpScreen);
}

void Sexy::WinFishApp::RemoveBoard()
{
	CleanDialogs();
	if (mBoard)
	{
		mBoard->SaveCurrentGame();
		if (mBoard->Unk02() > 120)
			m0x884 = true;

		mWidgetManager->RemoveWidget(mBoard);
		SafeDeleteWidget(mBoard);
		mBoard = 0;
	}
}

void Sexy::WinFishApp::RestartLevel()
{
	KillDialog(DIALOG_CONTINUE_GAME);
	KillDialog(DIALOG_RESTART_GAME);
	RemoveBoard();
	SexyString aSavePath = mCurrentProfile->GetSaveGameFilePath(mGameMode, mCurrentProfile->mId);
	EraseFile(aSavePath);
	StopMusic();
	PlayMusic(2, 0);
	if (mGameMode == GAMEMODE_ADVENTURE)
		SwitchToBoard(false, true);
	else
		SwitchToTankScreen();
}

void Sexy::WinFishApp::RemoveGameSelector()
{
	if (mGameSelector)
	{
		mWidgetManager->RemoveWidget(mGameSelector);
		SafeDeleteWidget(mGameSelector);
		mGameSelector = 0;
	}
}

void Sexy::WinFishApp::UpdatePlayData()
{
	if (m0x884)
	{
		mPlaytimeCounter2++;
		m0x884 = false;
	}
	if (mMaxTime > 0)
	{
		int anUnk01 = mPlaytimeCounter * mFrameTime;
		mTimesPlayed = mTimesPlayed + anUnk01 / 60000;
		mPlaytimeCounter2 = 0;
		mPlaytimeCounter = 0;
		return;
	}
	if (mMaxPlays > 0)
	{
		mTimesPlayed += mPlaytimeCounter2;
	}
	mPlaytimeCounter2 = 0;
	mPlaytimeCounter = 0;
}

bool Sexy::WinFishApp::CheckTrialEnded()
{
	if (mIsRegistered || IsScreenSaver())
		return false;
	UpdatePlayData();

	if (mMaxTime > 0)
		if(mMaxTime <= mTimesPlayed)
			return true;

	if (mMaxTime <= 0 && mMaxPlays > 0)
	{
		int aPlaysLeft = mMaxPlays - mTimesPlayed;

		if (m0x884)
			aPlaysLeft--;

		if (aPlaysLeft <= 0)
			return true; 
	}

	if (mMaxTime <= 0 && mMaxPlays <= 0 && mMaxExecutions > 0)
		if (mTimesExecuted > mMaxExecutions)
			return true;

	return false;
}

void Sexy::WinFishApp::ApplyOptionsSettings()
{
	OptionsDialog* aDia = (OptionsDialog*)GetDialog(DIALOG_OPTIONS);
	if (!aDia)
		return;
	SwitchScreenMode(!aDia->mFullscreenCB->IsChecked(), aDia->m3DCB->IsChecked());
	EnableCustomCursors(aDia->mCustomCursorsCB->IsChecked());
	KillDialog(DIALOG_OPTIONS);
	ClearUpdateBacklog(true);
	if (mBoard)
		mBoard->ApplyShadowsIf3D();
}

void Sexy::WinFishApp::DoTrialVersionExpiredDialog()
{
	Dialog* aDia = DoDialog(3, true, "PLEASE REGISTER!", "Your trial version of Insaniquarium has expired!\n\nYou must register your copy\nto continue playing.", "", Dialog::BUTTONS_OK_CANCEL);
	aDia->mYesButton->mLabel = "Register";
	aDia->mNoButton->mLabel = "Quit";
}

void Sexy::WinFishApp::DoUpdateDialog()
{
	Dialog* aDia = DoDialog(DIALOG_UPDATE_ASK, true, "Updates", "Do you want to check for updates to Insaniquarium? New versions may offer new features and bug fixes.  This requires an active Internet connection.", "", Dialog::BUTTONS_YES_NO);
	int aPrefHeight = aDia->GetPreferredHeight(348);
	aDia->Resize(146, 50, 348, aPrefHeight);
}

void Sexy::WinFishApp::DoContinueDialog()
{
	ContinueDialog* aDia = new ContinueDialog(this);
	int aPrefHght = aDia->GetPreferredHeight(380);
	aDia->Resize((mWidth - 380) / 2, 70, 380, aPrefHght);
	AddDialog(DIALOG_CONTINUE_GAME, aDia);
}

void Sexy::WinFishApp::DoNewUserDialog()
{
	KillDialog(DIALOG_NEW_USER);
	NewUserDialog* aDia = new NewUserDialog(this, false);
	int aPrefHght = aDia->GetPreferredHeight(400);
	aDia->Resize((mWidth - 400) / 2, (mHeight - aPrefHght) / 2, 400, aPrefHght);
	AddDialog(DIALOG_NEW_USER, aDia);
}

void Sexy::WinFishApp::StartGame()
{
	m0x883 = false;
	if (mGameMode == GAMEMODE_VIRTUAL_TANK)
	{
		if (LoadBoardGame())
			return;
	}
	CreateBoard();
	mBoard->InitLevel();
	if (IsBonusLevel())
	{
		mBoard->InitBonusLevel();
		StartGameMusic();
		return;
	}
	if (mGameMode == GAMEMODE_VIRTUAL_TANK)
	{
		mBoard->StartVirtualTank();
		StartGameMusic();
		return;
	}
	mBoard->StartGame();
	StartGameMusic();
}

bool Sexy::WinFishApp::LoadBoardGame()
{
	CreateBoard();
	bool aSuccess = mBoard->LoadGame(mCurrentProfile->GetSaveGameFilePath(mGameMode, mCurrentProfile->mId));
	if (!aSuccess)
		RemoveBoard();
	else
	{
		m0x883 = false;
		if (mGameMode != GAMEMODE_VIRTUAL_TANK)
			DoContinueDialog();
	}
	return aSuccess;
}

void Sexy::WinFishApp::MakeNewUser(bool makeUser)
{
	NewUserDialog* aDia =(NewUserDialog*) GetDialog(DIALOG_NEW_USER);
	if (!aDia)
		return;

	SexyString aNewUserName = aDia->CleanString();

	if (makeUser && aNewUserName.size() == 0)
	{
		DoDialog(DIALOG_INFO_NEW_USER, true, "Enter Your Name", "Please enter your name to create a new user profile for storing high score data and game progress.", "OK", Dialog::BUTTONS_FOOTER);
		return;
	}
	else
	{
		if (mCurrentProfile || (makeUser && aNewUserName.size() != 0))
		{
			if (!makeUser)
			{
				KillDialog(DIALOG_NEW_USER);
				return;
			}
			else
			{
				UserProfile* aProf = mProfileMgr->MakeNewUser(&aNewUserName);
				if (!aProf)
				{
					DoDialog(DIALOG_INFO_NEW_USER, true, "Name Conflict",
						"The name you entered is already being used.  Please enter a unique player name.", 
						"OK", Dialog::BUTTONS_FOOTER);
					return;
				}
				else
				{
					mCurrentProfile = aProf;
					mProfileMgr->SaveUsersDat();
					KillDialog(DIALOG_USER_DIALOG);
					KillDialog(DIALOG_NEW_USER);
					mWidgetManager->MarkAllDirty();
					// BlankFunction();
					return;
				}
			}
		}

		DoDialog(DIALOG_INFO_NEW_USER, true, "Enter Your Name",
			"Please enter your name to create a new user profile for storing high score data and game progress.", 
			"OK", Dialog::BUTTONS_FOOTER);
		return;
	}
}

void Sexy::WinFishApp::CreateBoard()
{
	RemoveBoard();
	m0x884 = false;
	mBoard = new Board(this);
	mBoard->Resize(0, 0, mWidth, mHeight);
	mWidgetManager->AddWidget(mBoard);
	mWidgetManager->BringToBack(mBoard);
	mWidgetManager->SetFocus(mBoard);
	mCurrentProfile->m0x4c++;
}

bool Sexy::WinFishApp::IsBonusLevel()
{
	if (mGameMode == GAMEMODE_ADVENTURE && mCurrentProfile->mLevel == 6)
		return true;
	if (mBoard)
		return mBoard->mIsBonusRound;
	return false;
}

bool Sexy::WinFishApp::IsSongPlaying(int theSongId, int theSongOffset)
{
	return mCurrentSongId == theSongId && mCurrentSongOffset == theSongOffset;
}

void Sexy::WinFishApp::StopMusic()
{
	if (!mMusicInterface)
		return;

	mMusicInterface->StopMusic(0);
	mMusicInterface->StopMusic(1);
	mMusicInterface->StopMusic(2);
	mMusicInterface->StopMusic(3);
	mMusicInterface->StopMusic(4);
	mCurrentSongId = -1;
	mCurrentSongOffset = -1;
}

void Sexy::WinFishApp::StartGameMusic()
{
	StopMusic();
	if (mGameMode == GAMEMODE_VIRTUAL_TANK)
		return;

	if (IsBonusLevel())
		PlayMusic(0, 13);
	else if(GetCurrentTank() == 1)
		PlayMusic(0, 22);
	else if(GetCurrentTank() == 2)
		PlayMusic(0, 0);
	else if(GetCurrentTank() == 3)
		PlayMusic(2, 32);
	else if(GetCurrentTank() == 4)
		PlayMusic(2, 45);
}

void Sexy::WinFishApp::SomeMusicPlayFunc(bool flag)
{
	if (mGameMode == GAMEMODE_VIRTUAL_TANK)
	{
		StopMusic();
		return;
	}

	if (!flag)
		StopMusic();
	else
		FadeOutMusic(true, true, 0.008);

	double aSpeed = flag ? 0.002 : 1.0;
	int aTank = GetCurrentTank();
	if (aTank == 1)
		FadeInMusic(0, 22, aSpeed, false);
	else if(aTank == 2)
		FadeInMusic(0, 0, aSpeed, false);
	else if(aTank == 3)
		FadeInMusic(2, 32, aSpeed, false);
	else if(aTank == 4)
		FadeInMusic(2, 45, aSpeed, false);
}

bool Sexy::WinFishApp::ChangeDirHook(const char* theIntendedPath)
{
	if (!IsScreenSaver())
		return false;

	SexyString aRegPath = mScreenSaverRegKey + "Directory";
	SexyString aDirectoryPath;

	if (RegistryReadString(aRegPath, &aDirectoryPath))
		if (SetCurrentDirectoryA(aDirectoryPath.c_str()) != 0)
			return true;

	return false;
}

void Sexy::WinFishApp::LogScreenSaverError(const std::string& theError)
{
	if (!gScreenSaverStarted)
		StartScreenSaver();
	SexyApp::LogScreenSaverError(theError);
}

void Sexy::WinFishApp::UpdateFrames()
{
	if (!gUnkBool01)
	{
		if (gDoHundredUpdates)
		{
			for(int i = 0; i < 100; i++)
				SexyApp::UpdateFrames();
			return;
		}
	}
	else
	{
		if (gUpdateFramesCounter < 3)
		{
			gUpdateFramesCounter++;
			return;
		}
		gUpdateFramesCounter = 0;
	}

	SexyApp::UpdateFrames();
}

void Sexy::WinFishApp::TitleScreenIsFinished()
{
	mWidgetManager->RemoveWidget(mTitleScreen);
	SafeDeleteWidget(mTitleScreen);
	mTitleScreen = NULL;

	mResourceManager->ReplaceImage("IMAGE_TITLEPAGE", 0);
	mResourceManager->ReplaceImage("IMAGE_LOADERBAR", 0);
	mResourceManager->ReplaceImage("IMAGE_LOADERBAROVER", 0);
	mResourceManager->ReplaceImage("IMAGE_LOADERBAROVER2", 0);
	mResourceManager->ReplaceImage("IMAGE_LOADERPLAY", 0);
	mResourceManager->ReplaceImage("IMAGE_TITLEPAGEMASK", 0);
	mResourceManager->ReplaceImage("IMAGE_TITLEPAGEMASK", 0);

	if (IsScreenSaver())
	{
		mGameMode = GAMEMODE_VIRTUAL_TANK;
		SwitchToBoard(true, true);
		ClearUpdateBacklog();
		return;
	}

	SwitchToGameSelector();
}

void Sexy::WinFishApp::WFAShowResourceError(bool doExit)
{
	if (!IsScreenSaver())
	{
		ShowResourceError(doExit);
		return;
	}

	SexyString aErrText = mResourceManager->GetErrorText();
	LogScreenSaverError(aErrText);
	mLoadingFailed = true;
}

void Sexy::WinFishApp::SwitchToHighScoreScreen()
{
	RemoveGameSelector();
	RemoveHighScoreScreen();
	mHighScoreScreen = new HighScoreScreen(this);
	mHighScoreScreen->Resize(0, 0, mWidth, mHeight);
	mWidgetManager->AddWidget(mHighScoreScreen);
}

void Sexy::WinFishApp::SwitchToPetsScreen()
{
	CleanDialogs();
	RemovePetsScreen();
	if (mBoard)
		mBoard->PauseGame(true);
	mPetsScreen = new PetsScreen(this);
	mPetsScreen->Resize(0, 0, mWidth, mHeight);
	mWidgetManager->AddWidget(mPetsScreen);
}

void Sexy::WinFishApp::SwitchToHatchScreen(int thePetId)
{
	CleanDialogs();
	m0x884 = true;
	if (mBoard)
		mBoard->mShouldSave = false;
	RemoveHatchScreen();
	mHatchScreen = new HatchScreen(this, thePetId);
	mHatchScreen->Resize(0, 0, mWidth, mHeight);
	mWidgetManager->AddWidget(mHatchScreen);
	SaveCurrentUserData();
	RemoveBoard();
}

void Sexy::WinFishApp::SwitchToInterludeScreen()
{
	CleanDialogs();
	RemoveInterludeScreen();
	mInterludeScreen = new InterludeScreen(this, 0);
	mInterludeScreen->Resize(0, 0, mWidth, mHeight);
	mWidgetManager->AddWidget(mInterludeScreen);
	mWidgetManager->SetFocus(mInterludeScreen);
	if (mBoard == nullptr)
		mWidgetManager->BringToBack(mInterludeScreen);
	else
		mWidgetManager->PutInfront(mInterludeScreen, mBoard);
}
void Sexy::WinFishApp::SwitchToSimSetupScreen()
{
	ReadSSFromRegistry();
	CleanDialogs();
	RemoveSimSetupScreen();
	if (mBoard)
		mBoard->PauseGame(true);

	mSimSetupScreen = new SimSetupScreen(this);
	mSimSetupScreen->Resize(0, 0, mWidth, mHeight);
	mWidgetManager->AddWidget(mSimSetupScreen);
}
void Sexy::WinFishApp::SwitchToTankScreen()
{
	CleanDialogs();
	RemoveTankScreen();

	mTankScreen = new TankScreen(this);
	mTankScreen->Resize(0, 0, mWidth, mHeight);
	mWidgetManager->AddWidget(mTankScreen);
	if (mBoard == nullptr)
		mWidgetManager->BringToBack(mTankScreen);
	else
		mWidgetManager->PutInfront(mTankScreen, mBoard);
}

void Sexy::WinFishApp::SwitchToStoryScreen(int unk)
{
	CleanDialogs();
	RemoveStoryScreen();

	int aStoryId = -1;
	int aGroupIdx = unk - 1;
	int isAdventure = false;
	if (aGroupIdx < 0)
		aGroupIdx = 0;
	else if (aGroupIdx < 4)
		isAdventure = true;
	else
		aGroupIdx = 3;

	static int aStoryTable[32] = {
		0, 1, 2, 3, 4, 20, 24, 25,
		5, 6, 7, 8, 9, 21, 26, 27,
		10, 11, 12, 13, 14, 22, 28, 29,
		15, 16, 17, 18, 19, 23, 30, 31
	};

	aStoryId = -2;
	bool storyFound = false;
	if (isAdventure)
	{
		if (mCurrentProfile->m0x7c == -1)
		{
			if (mCurrentProfile->m0x80 == 0 && mCurrentProfile->mBonusItemId > 5)
			{
				aStoryId = 32;
				SaveCurrentUserData();
				storyFound = true;
			}
			else
			{
				int aLimit = 33;
				if (mCurrentProfile->m0x80 != 1) aLimit = 32;
				aStoryId = Rand() % aLimit;
				if (aStoryId != -1)
					storyFound = true;
			}
		}

		int* aGroupStart = &aStoryTable[aGroupIdx * 8];
		if (!storyFound)
		{
			for (int i = 0; i < 8; i++)
			{
				int anId = aGroupStart[i];
				if ((mCurrentProfile->m0x7c & (1 << (anId & 0x1F))) == 0 &&
					(anId > 23 || mCurrentProfile->IsPetUnlocked(anId)))
				{
					aStoryId = anId;
					mCurrentProfile->m0x7c |= (1 << (anId & 0x1F));
					SaveCurrentUserData();
					if(aStoryId != -1)
						storyFound = true;
					break;
				}
			}
		}

		if (!storyFound)
		{
			std::vector<int> aSeenStories;
			for (int i = 0; i < 8; i++)
			{
				int anId = aGroupStart[i];
				bool seen = (mCurrentProfile->m0x7c & (1 << (anId & 0x1F))) != 0;
				if (seen)
				{
					aSeenStories.push_back(anId);
				}
			}

			if (aSeenStories.empty())
				aSeenStories.push_back(0);

			int aRandIdx = Rand() % aSeenStories.size();
			aStoryId = aSeenStories[aRandIdx];
		}
	}

	mStoryScreen = new StoryScreen(this, aStoryId);
	mStoryScreen->Resize(0, 0, mWidth, mHeight);
	mWidgetManager->AddWidget(mStoryScreen);
	mWidgetManager->SetFocus(mStoryScreen);
}

void Sexy::WinFishApp::SwitchToStoreScreen()
{
	CleanDialogs();
	RemoveStoreScreen();
	if (mBoard)
		mBoard->PauseGame(true);

	if (mCurrentProfile)
		mCurrentProfile->Unk01();

	mStoreScreen = new StoreScreen(this);
	mStoreScreen->Resize(0, 0, mWidth, mHeight);
	mWidgetManager->AddWidget(mStoreScreen);
	mWidgetManager->SetFocus(mStoreScreen);
}

void Sexy::WinFishApp::SwitchToSimFishScreen()
{
	CleanDialogs();
	RemoveSimFishScreen();
	if (mBoard)
		mBoard->PauseGame(true);

	mSimFishScreen = new SimFishScreen(this);
	mSimFishScreen->Resize(0, 0, mWidth, mHeight);
	mWidgetManager->AddWidget(mSimFishScreen);
	mWidgetManager->SetFocus(mSimFishScreen);
}

void Sexy::WinFishApp::SwitchToBonusScreen()
{
	CleanDialogs();
	m0x884 = true;
	if (mBoard)
		mBoard->mShouldSave = false;
	RemoveBonusScreen();
	if (mCurrentProfile)
		mCurrentProfile->Unk01();

	mBonusScreen = new BonusScreen(this);
	mBonusScreen->Resize(0, 0, mWidth, mHeight);
	mWidgetManager->AddWidget(mBonusScreen);
	SaveCurrentUserData();
	RemoveBoard();
	StopMusic();
	PlayMusic(2, 54);
}

void Sexy::WinFishApp::RemoveHighScoreScreen()
{
	if (mHighScoreScreen)
	{
		mWidgetManager->RemoveWidget(mHighScoreScreen);
		SafeDeleteWidget(mHighScoreScreen);
		mHighScoreScreen = nullptr;
		SwitchToGameSelector();
	}
}

void Sexy::WinFishApp::RemovePetsScreen()
{
	if (mPetsScreen)
	{
		mWidgetManager->RemoveWidget(mPetsScreen);
		SafeDeleteWidget(mPetsScreen);
		mPetsScreen = nullptr;
	}
}

void Sexy::WinFishApp::RemoveHatchScreen()
{
	if (mHatchScreen)
	{
		mWidgetManager->RemoveWidget(mHatchScreen);
		SafeDeleteWidget(mHatchScreen);
		mHatchScreen = nullptr;
	}
}

void Sexy::WinFishApp::RemoveSimSetupScreen()
{
	if (mSimSetupScreen)
	{
		mWidgetManager->RemoveWidget(mSimSetupScreen);
		SafeDeleteWidget(mSimSetupScreen);
		mSimSetupScreen = nullptr;
	}
}

void Sexy::WinFishApp::RemoveTankScreen()
{
	if (mTankScreen)
	{
		mWidgetManager->RemoveWidget(mTankScreen);
		SafeDeleteWidget(mTankScreen);
		mTankScreen = nullptr;
	}
}

void Sexy::WinFishApp::RemoveStoryScreen()
{
	if (mStoryScreen)
	{
		mWidgetManager->RemoveWidget(mStoryScreen);
		SafeDeleteWidget(mStoryScreen);
		mStoryScreen = nullptr;
	}
}

void Sexy::WinFishApp::RemoveStoreScreen()
{
	if (mStoreScreen)
	{
		mWidgetManager->RemoveWidget(mStoreScreen);
		SafeDeleteWidget(mStoreScreen);
		mStoreScreen = nullptr;
		if (mBoard != nullptr)
			mWidgetManager->SetFocus(mBoard);
	}
}

void Sexy::WinFishApp::RemoveSimFishScreen()
{
	if (mSimFishScreen)
	{
		mWidgetManager->RemoveWidget(mSimFishScreen);
		SafeDeleteWidget(mSimFishScreen);
		mSimFishScreen = nullptr;
		if (mBoard != nullptr)
			mWidgetManager->SetFocus(mBoard);
	}
}

void Sexy::WinFishApp::RemoveBonusScreen()
{
	if (mBonusScreen)
	{
		mWidgetManager->RemoveWidget(mBonusScreen);
		SafeDeleteWidget(mBonusScreen);
		mBonusScreen = nullptr;
	}
}

void Sexy::WinFishApp::RemoveInterludeScreen()
{
	if (mInterludeScreen)
	{
		mWidgetManager->RemoveWidget(mInterludeScreen);
		SafeDeleteWidget(mInterludeScreen);
		mInterludeScreen = nullptr;
	}
}

void Sexy::WinFishApp::LeaveGameBoard()
{
	SaveCurrentUserData();
	ApplyOptionsSettings();
	RemoveBoard();
	SwitchToGameSelector();
}

void Sexy::WinFishApp::RemoveHelpScreen()
{
	if (mHelpScreen)
	{
		mWidgetManager->RemoveWidget(mHelpScreen);
		SafeDeleteWidget(mHelpScreen);
		mHelpScreen = 0;
	}
}

void Sexy::WinFishApp::DoUpdateCheckDialog()
{
	UpdateCheckDialog* aDialog = new UpdateCheckDialog(IMAGE_DIALOG, IMAGE_DIALOGBUTTON, IMAGE_WAIT_BAR, DIALOG_UPDATE_CHECK);
	DefaultDialogSettings(aDialog);
	int aPrefHght = aDialog->GetPreferredHeight(348);
	aDialog->Resize(146, 50, 348, aPrefHght);
	AddDialog(DIALOG_UPDATE_CHECK, aDialog);
	SexyString anUrl = "http://www.popcap.com/win32updatecheck.php?prod=" + mProdName + 
		"&ver=" + mProductVersion
		+ "&referid=" + std::to_string(mWinFishReferId);
	mInternetManager->StartUpdateCheck(anUrl);
}

void Sexy::WinFishApp::DoWhoAreYouDialog()
{
	KillDialog(DIALOG_USER_DIALOG);
	UserDialog* aDia = new UserDialog(this, false);
	int aPrefHght = aDia->GetPreferredHeight(400);
	aDia->Resize(0, 17, 400, aPrefHght);
	AddDialog(DIALOG_USER_DIALOG, aDia);
}

void Sexy::WinFishApp::DoGiveDialog()
{
	KillDialog(DIALOG_GIVE_SHELLS);
	if (mCurrentProfile && (mCurrentProfile->mFinishedGame || mCurrentProfile->mTank != 1))
	{
		UserDialog* aDia = new UserDialog(this, true);
		int aPrefHght = aDia->GetPreferredHeight(400);
		aDia->Resize(0, 100, 400, aPrefHght);
		AddDialog(DIALOG_GIVE_SHELLS, aDia);
		return;
	}

	DoDialog(DIALOG_INFO, true, "Not Allowed", "You need to beat the first tank before you can transfer shells.", "OK", Dialog::BUTTONS_FOOTER);
}

void Sexy::WinFishApp::DoDeleteWarningDialog(SexyString& theName)
{
	KillDialog(DIALOG_ARE_YOU_SURE_DELETE);
	DoDialog(DIALOG_ARE_YOU_SURE_DELETE, true, "Are You Sure?", StrFormat("This will permanently remove \'%s\' from the player roster!", theName.c_str()), "", Dialog::BUTTONS_YES_NO);
}

void Sexy::WinFishApp::RenameUser(bool makeUser)
{
	if (!makeUser)
	{
		KillDialog(DIALOG_RENAME);
		return;
	}

	UserDialog* anUserDialog = (UserDialog*)GetDialog(DIALOG_USER_DIALOG);
	NewUserDialog* aRenameDialog = (NewUserDialog*)GetDialog(DIALOG_RENAME);

	if (!anUserDialog || !aRenameDialog)
		return;

	SexyString anOldName = anUserDialog->GetSelectedUserName();
	SexyString aNewName = aRenameDialog->CleanString();

	if (aNewName.size() > 0)
	{
		UserProfile* aProf = mProfileMgr->GetUserProfile(anOldName);
		UserProfile* aCurProf = mCurrentProfile;
		bool aSuccess = mProfileMgr->RenameUser(anOldName, aNewName);
		if (!aSuccess)
		{
			DoDialog(DIALOG_NAME_CONFLICT, true, "Name Conflict", "The name you entered is already being used.  Please enter a unique player name.", "OK", Dialog::BUTTONS_FOOTER);
			return;
		}

		mProfileMgr->SaveUsersDat();
		if (aProf == aCurProf)
			mCurrentProfile = mProfileMgr->GetUserProfile(aNewName);

		int aSelIdx = anUserDialog->mListWidget->mSelectIdx;
		if (aSelIdx > 0)
			anUserDialog->mListWidget->SetLine(aSelIdx, aNewName);

		mWidgetManager->MarkAllDirty();
		KillDialog(DIALOG_RENAME);
	}
}

void Sexy::WinFishApp::DeleteUser(bool deleteUser)
{
	KillDialog(DIALOG_ARE_YOU_SURE_DELETE);
	UserDialog* aDia = (UserDialog*)GetDialog(DIALOG_USER_DIALOG);
	if (!deleteUser || !aDia)
		return;

	SexyString aUserName = "";
	if (mCurrentProfile)
		aUserName = mCurrentProfile->mUserName;

	SexyString aSelUserName = aDia->GetSelectedUserName();

	if (_stricmp(aUserName.c_str(), aSelUserName.c_str()) == 0)
		mCurrentProfile = nullptr;
	mProfileMgr->DeleteUser(aSelUserName);
	int aSelIdx = aDia->mListWidget->mSelectIdx;
	aDia->mListWidget->RemoveLine(aSelIdx);
	aSelIdx--;
	if (aSelIdx < 1)
		aSelIdx = 1;
	if (aDia->mListWidget->GetLineCount() > 1)
		aDia->mListWidget->SetSelect(aSelIdx);
	if (!mCurrentProfile)
	{
		SexyString aNextUserName = aDia->GetSelectedUserName();
		mCurrentProfile = mProfileMgr->GetUserProfile(aNextUserName);
		if (!mCurrentProfile)
			mCurrentProfile = mProfileMgr->GetFirstUserProfile();
	}
	mProfileMgr->SaveUsersDat();
	if (!mCurrentProfile)
		DoNewUserDialog();
	mWidgetManager->MarkAllDirty();
}

void Sexy::WinFishApp::DoRenameDialog(SexyString theUserName)
{
	KillDialog(DIALOG_RENAME);
	NewUserDialog* aRenameDialog = new NewUserDialog(this, true);
	int aPrefHght = aRenameDialog->GetPreferredHeight(400);
	aRenameDialog->Resize((mWidth - 400) / 2, (mHeight - aPrefHght) / 2, 400, aPrefHght);
	aRenameDialog->mEditWidget->SetText(theUserName, true);
	aRenameDialog->mEditWidget->mCursorPos = theUserName.size();
	aRenameDialog->mEditWidget->mHilitePos = 0;
	AddDialog(DIALOG_RENAME, aRenameDialog);
}

void Sexy::WinFishApp::UserDialogOkPressed(bool applyChanges)
{
	UserDialog* aDia =(UserDialog*) GetDialog(DIALOG_USER_DIALOG);
	if (!aDia)
		return;

	if (applyChanges)
	{
		SexyString aUserName = aDia->GetSelectedUserName();
		UserProfile* aProf = mProfileMgr->GetUserProfile(aUserName);
		if (aProf)
		{
			mCurrentProfile = aProf;
			if (mCurrentProfile->mTank == 5 && mCurrentProfile->mLevel > 1)
			{
				mCurrentProfile->mTank = 1;
				mCurrentProfile->mLevel = 1;
				mCurrentProfile->mFinishedGameCount = 0;
				mCurrentProfile->m0x58 = false;
			}
		}
	}

	KillDialog(DIALOG_USER_DIALOG);
}

int Sexy::WinFishApp::GetCurrentTank()
{
	if (mBoard)
		return mBoard->mTank;
	return mCurrentProfile->mTank;
}
