#pragma once

#include "MoneyDialog.h"
#include "WinFishApp.h"
#include "SexyAppFramework/CheckboxListener.h"
#include "SexyAppFramework/Checkbox.h"

namespace Sexy
{
	class ScreenSaverDialog : public MoneyDialog, public ButtonListener, public CheckboxListener
	{
	public:
		Checkbox* mSSEnabledCB;
		Checkbox* mSSSoundCB;
		Checkbox* mSSRotateBackdropsCB;
		Checkbox* mSSPeriodicDimCB;
		Checkbox* mSSShowMoneyCB;
		Checkbox* mSSPowerSaveCB;

	public:
		ScreenSaverDialog(WinFishApp* theApp);
		virtual ~ScreenSaverDialog();

		virtual void			AddedToManager(WidgetManager* theWidgetManager);
		virtual void			RemovedFromManager(WidgetManager* theWidgetManager);

		virtual void			Draw(Graphics* g);
		virtual void			Resize(int theX, int theY, int theWidth, int theHeight);

		virtual int				GetPreferredHeight(int theWidth);

		virtual void			WidgetClicked(int theId, bool theFlag);			//[77]

		virtual void			CheckboxChecked(int theId, bool checked);
	};
}

