#pragma once

#include "SexyAppFramework/Widget.h"
#include "SexyAppFramework/ButtonListener.h"

namespace Sexy
{
	class WinFishApp;
	class HyperlinkWidget;

	class TitleScreen : public Widget, public ButtonListener
	{
	public:
		HyperlinkWidget* mHyperlink1;
		HyperlinkWidget* mHyperlink2;
		int m0x94;
		int m0x98;
		int m0x9c;
		bool m0xa0;
		bool m0xa1;

		SexyString m0xa4;

		int m0xbc;
		bool m0xc0;
		bool m0xc1;
		int mStinkyX;

		int m0xcc;
		int mBlendInCounter;
		WinFishApp* mApp;

	public:
		TitleScreen(WinFishApp* theApp);

		virtual ~TitleScreen();
		virtual void			AddedToManager(WidgetManager* theWidgetManager);
		virtual void			RemovedFromManager(WidgetManager* theWidgetManager);
		virtual void			Draw(Graphics* g);
		virtual void			Update();
		virtual void			Resize(int theX, int theY, int theWidth, int theHeight);

		virtual void			ButtonPress(int theId);
		virtual void			ButtonDepress(int theId);

		void					RegisterSuccessful();
		void					TrialFunction();
	};
}


