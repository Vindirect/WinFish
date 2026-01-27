#pragma once

#include "SexyAppFramework/ButtonWidget.h"

namespace Sexy
{
	class ToolTipWidget;
	class MenuButtonWidget : public ButtonWidget
	{
	public:
		ToolTipWidget* mToolTipWidget;
		Image* mSlotImage;
		int mSlotImageX;
		int mSlotImageY;
		int mSlotImageCol;
		int mSlotImageRow;
		int m0x138;
		int m0x13c;
		SexyString mPriceText;
		SexyString m0x15c;
		Color mPriceTextColor;
		WidgetManager* mManager;
		int mToolTipXOffset;
		int mToolTipYOffset;

	public:
		MenuButtonWidget(WidgetManager* theWidgetManager, int theId, ButtonListener* theButtonListener, SexyString theTooltipText);
		virtual ~MenuButtonWidget();

		virtual void			RemovedFromManager(WidgetManager* theWidgetManager);

		virtual void			Update();
		virtual void			Draw(Graphics* g);

		virtual void			MouseEnter();
		virtual void			MouseLeave();

		virtual void			MouseDown(int theX, int theY, int theClickCount);

		void					Configure(Image* theImage, int theSlotImageX, int theSlotImageY, int theSlotImageRow, int theSlotImageCol);
		void					SetSlotPrice(int thePrice);
		void					SetMaxedOut();
	};
}

