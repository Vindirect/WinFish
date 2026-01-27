#pragma once

#include "MoneyDialog.h"
#include "WinFishApp.h"
#include "SexyAppFramework/SliderListener.h"
#include "SexyAppFramework/CheckboxListener.h"
#include "SexyAppFramework/Checkbox.h"
#include "SexyAppFramework/Slider.h"

namespace Sexy
{
	class OptionsDialog : public MoneyDialog, public ButtonListener, public SliderListener, public CheckboxListener
	{
	public:
		WinFishApp* mApp2;
		Slider* mMusicSlider;
		Slider* mSFXSlider;
		Checkbox* mFullscreenCB;
		Checkbox* mCustomCursorsCB;
		Checkbox* m3DCB;
		DialogButton* mRegisterButton;
		DialogButton* mHelpButton;
		DialogButton* mCheckUpdatesButton;
		DialogButton* mWebLinkButton;
		DialogButton* mBackButton;
		bool mFlag;

	public:
		OptionsDialog(WinFishApp* theApp, bool theFlag);
		virtual ~OptionsDialog();

		virtual void			AddedToManager(WidgetManager* theWidgetManager);
		virtual void			RemovedFromManager(WidgetManager* theWidgetManager);

		virtual void			Draw(Graphics* g);

		virtual void			Resize(int theX, int theY, int theWidth, int theHeight);

		virtual int				GetPreferredHeight(int theWidth);

		virtual void			WidgetClicked(int theId, bool theFlag);			//[77]


		virtual void			CheckboxChecked(int theId, bool checked);

		virtual void			SliderVal(int theId, double theVal);

		virtual void			ButtonPress(int theId);
		virtual void			ButtonDepress(int theId);
	};
}

