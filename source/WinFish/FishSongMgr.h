#pragma once
#include "SexyAppFramework/Common.h"
namespace Sexy
{
	struct NoteData 
	{
		double				mPitch;
		double				mVolume;
		int					mDuration;

		void 				Parse(const SexyChar* theNoteInfo);
	};

	class FishSongData 
	{
	public:
		std::vector<NoteData>				mNoteVectors[3];
		float 								mLineVolumes[3];
		float 								mSpeed;
		float 								mGlobalPitchShift;
		float 								mLinePitchShifts[3];
		bool								mSkip;
		int									mCurrentLine;
		std::map<SexyString, SexyString, Sexy::StringLessNoCase>	mProperties;

	public:
		FishSongData();
		~FishSongData();

		void 				Init();

		bool 				Parse(SexyString& theFilePath);
		void 				ParseCommand(const SexyChar* theCommand);

		bool 				GetProperty(const SexyString& key, SexyString* outValue);
		int 				GetLineDuration(int theLine);
	};

	class FishSong
	{
	public:
		std::vector<NoteData> mNoteDataVector;
		int					mCurrentNoteIndex;
		DWORD				mStartTime;
		int 				mTotalDurationInSamples;
		int 				mTone;
		int 				mToneHi;
		int 				mToneSuperHi;
		int 				mToneLo;
		int 				mApplause;
		int 				mSongId;

	public:
		FishSong();
		~FishSong();

		void				Init(float theSpeed, int thePitchShift);
		bool				Update(DWORD theCurrentMusicTick, bool theAllowSound);
	};

	typedef std::list<FishSong*> FishSongList;

	class FishSongMgr
	{
	public:
		FishSongList		mSongList;
		int					mTone;
		int					mToneHi;
		int					mToneSuperHi;
		int					mToneLo;
		int					mCounter;
		DWORD				mLastUpdateTime;

	public:
		FishSongMgr();
		~FishSongMgr();

		void				Update();
		void				AddSong(FishSong* theSong);
		void				StopFishSong(int theSongId);
		bool				IsSongInList(int theSongId);
		void				PausedGameDelaySong();
		FishSong*			PlayFishSong(FishSongData* theSongData, int theSongId);
		int					Unk01(int theSongId);
		void				ClearSongs();
	};
}