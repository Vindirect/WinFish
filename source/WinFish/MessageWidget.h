#pragma once

#include "SexyAppFramework/Widget.h"
#include "DataSync.h"

namespace Sexy
{
	class WinFishApp;

	class MessageWidget : public Widget
	{
	public:
		WinFishApp* mApp;
		SexyString mMessage;
		Color mColor1;
		Color mColor2;
		int m0xc8;
		Font* mFont;
		int m0xd0;
		bool m0xd4;
		int m0xd8;

	public:
		MessageWidget(WinFishApp* theApp, SexyString theMessage);
		virtual ~MessageWidget();

		virtual void			Update();
		virtual void			Draw(Graphics* g);

		void					Sync(DataSync& theSync);
	};
}

