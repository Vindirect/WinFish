#include "ToolTipWidget.h"
#include "Res.h"
#include "SexyAppFramework/Font.h"
#include "SexyAppFramework/WidgetManager.h"

Sexy::ToolTipWidget::ToolTipWidget(SexyString theToolTip)
	: MyLabelWidget()
{
	mLabelFont = FONT_TINYBOLD;
	mMouseVisible = false;

	mLabel = theToolTip;

	Resize(mX, mY, mLabelFont->StringWidth(mLabel) + 10, mLabelFont->GetHeight() + 12);
}

Sexy::ToolTipWidget::~ToolTipWidget()
{
}

void Sexy::ToolTipWidget::Update()
{
	MyLabelWidget::Update();
	mWidgetManager->BringToFront(this);
}

void Sexy::ToolTipWidget::Draw(Graphics* g)
{
	g->SetColor(Color(0xff, 0xff, 200, 0xff));
	g->FillRect(0, 0, mWidth, mHeight);
	g->SetColor(Color(Color::Black));
	g->DrawRect(0, 0, mWidth - 1, mHeight - 1);
	g->SetFont(mLabelFont);
	
	g->DrawString(mLabel, 5, mLabelFont->GetAscent() + (mHeight - mLabelFont->GetHeight()) / 2);
}
