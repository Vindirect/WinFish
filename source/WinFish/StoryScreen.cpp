#include "StoryScreen.h"
#include "WinFishApp.h"
#include "WinFishAppCommon.h"
#include "BubbleMgr.h"
#include "Res.h"

#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Font.h"

const SexyString STORY_TITLES[33] = {
	"STINKY the Snail",
	"NIKO the Oyster",
	"ITCHY the Swordfish",
	"PREGO the Momma Fish",
	"ZORF the Sea Horse",
	"CLYDE the Jellyfish",
	"VERT the Skeleton",
	"RUFUS the Fiddler Crab",
	"MERYL the Mermaid",
	"WADSWORTH the Whale",
	"SEYMOUR the Turtle",
	"SHRAPNEL the Robot Fish",
	"GUMBO the Angler",
	"BLIP the Porpoise",
	"RHUBARB the Hermit Crab",
	"NIMBUS the Manta Ray",
	"AMP the Electric Eel",
	"GASH the Shark",
	"ANGIE the Angelfish",
	"PRESTO the Tadpole",
	"BRINKLEY the Scuba Diving Elephant",
	"NOSTRADAMUS the Nose",
	"STANLEY the Startlingly Small Sea Serpent",
	"WALTER the Penguin",
	"SYLVESTER the Alien",
	"BALROG the Alien",
	"GUS the Baby-Faced Man-Child",
	"DESTRUCTOR the Alien",
	"ULYSSES the Alien",
	"PSYCHOSQUID",
	"BILATERUS the Alien",
	"MINISYLVESTER the Alien",
	"CYRAX the Evil Alien Mastermind",
};

const SexyString STORY_TEXT[33] = {
	"STINKY knew from an early age that he was different from the other snails.  He had the fever.  The money fever.  He was all about collecting money.  But what does STINKY need with all that money?  STINKY answers, \"It impresses the ladies.\"",
	"One question that always comes up when talking about NIKO is:  Why doesn't NIKO open his mouth more often?  There is a reason for this.  It is because NIKO thinks that opening his mouth may allow demons to steal his soul.  Therefore, he keeps mouth opening to a minimum.",
	"ITCHY wasn't always the grizzled, alien-attacking warrior that he is today.  In fact, he used to be a watercoloring, vegan pacifist... Until one day an alien invaded ITCHY's tank.  ITCHY tried to reason with the alien, but it wouldn't listen.  The alien ate ITCHY's friends and this is when ITCHY swore a blood oath to kill all aliens!",
	"Everyone knows PREGO as the loving mother to a thousand guppies.  But the question on everybody's mind is:  Where did she get that exquisite scarf?  PREGO answers, \"Oh, this thing?\nSon 183,241 gave it to me... no wait... it was Son 183,240.  I always get those two mixed up.\"",
	"Some people wonder, Why doesn't ZORF ever upgrade to level 3 food?  The reason is that ZORF thinks that most foods are too rich in fat and artificial preservatives.  The level 2 food, on the other hand, is raised one hundred percent organically and has no artificial flavors or additives.",
	"CLYDE, much like STINKY, likes collecting money.  Unlike STINKY, though, CLYDE doesn't collect money for just any lady.  He is actually saving up to finally get his mother the operation that she needs.  So just remember that every dollar left over at the end of a level might just go to CLYDE's mother's operation fund!",
	"Some people are afraid to use VERT because they think that he is an undead product of dark occult forces and therefore may turn against them at any time.  This is simply not true.  Obviously, a bad skeleton would not give you gold coins.  He would drop rat poison.",
	"RUFUS started his career as a dance instructor, teaching people how to do his patented pop and lock arm routine.  When his dance studio burned down in a freak accident, RUFUS became a mercenary for hire, using his dance skills to amplify his already potent natural fighting ability.",
	"MERYL delights in thrilling audiences around the world with her virtuoso singing talent.  She has never been able to measure up in the eyes of the carnivores or ultravores, though.  These fish view her simply as another unoriginal corporate-sponsored pop star.  To her critics, MERYL would like to point out that she has never sold out by making a music video.",
	"WADSWORTH has made quite a comeback since his accident several years back.  Hopelessly addicted to plankton and abandoned by all his friends, WADSWORTH lost all muscle control one day and accidentally ate all of the fish in his mouth.  Since finishing rehab four years ago, WADSWORTH has re-entrenched himself in Insaniquarium players' hearts.",
	"After beating the hare a few years ago, SEYMOUR retired and returned to his home in the aquarium.  Sadly, nothing would ever top his winning that race.  SEYMOUR does not live in the present as we know it.  Look into his eyes and you can see that his mind is stuck in a hazy past... reliving his last glorious moment.",
	"When talking about SHRAPNEL, the main question is usually:  What are the market forces of supply and demand that result in SHRAPNEL's bombs being worth $150?  Alan Fishspan, chairman of the Federal Shell Reserve answers: This is a response to the current low rate on 30 year mortgages, combined with the strong performance of the Euro against the dollar.",
	"Many critics of GUMBO say that he is a wolf in sheep's clothing... that he simply leads all of your guppies to one spot to make it easier for his alien friends to kill them all.  GUMBO answers his critics by pointing out that the aliens have shown him no mercy in Tank 5.  \"I am not nor have I ever been a member of the Alien party!\" says Gumbo.",
	"BLIP wrestled for years with his psychic abilities.  He was shunned as a child by the other fish who hated him for being able to tell when they were hungry.  Then one day, BLIP got a bad feeling about a certain area of the tank.  He didn't tell anyone, though, because everyone was mean to him.  After all the other fish were killed, BLIP felt guilty and vowed to only use his powers for good.",
	"RHUBARB has often been accused of being the \"worst pet ever\".  RHUBARB doesn't really care what his detractors think, though.  He's not aiming to be a crowd pleaser.  He's not trying to prove anything to anyone.  He just wishes those darn fish would stay off the ground!",
	"Some people want to know: What's Nimbus' record for bouncing a coin up and down?  Nimbus answers: \"One thousand seven hundred and twenty-four times.\"  Way to go, Nimbus!",
	"Rumors have been circulating for years now that some companies have developed a way to make AMP turn water into diamonds.  Conspiracy theorists believe that it is simply the interests of the greedy diamond lobby that have prevented this research from becoming available to the general public.  AMP is closed-mouthed on the subject.",
	"GASH used to be one of ITCHY the Swordfish's best friends until ITCHY left him for dead in a violent alien attack.  Now, years later, GASH roams the tank trying to settle the score with the aliens and perhaps one day... with ITCHY as well!",
	"To most fans, ANGIE can do no wrong.  But to a few there will always be the question:  Why did she save this fish but not that one?  How does she make her fateful decision?  ANGIE responds that she tries to save all fish but for some, it was simply their time to go.  \"After all, if the entire tank were saved then the frame rate would go down.\"",
	"PRESTO is not actually a real pet.  He was made up for the game.",
	"BRINKLEY was a performing elephant for many years before escaping his cruel circus masters.  Unfortunately, BRINKLEY could not escape his own voracious appetite.  An appetite for... food!",
	"Asked by skeptics whether he really is the nose of President Rutherford B. Hayes, NOSTRADAMUS replies: \"Yes and no.  I was President Hayes' second nose.  His first nose was blown off in the Civil War.\"",
	"STANLEY's seeming unlimited supply of balls are the scourge of alien missile shooters everywhere.  Some players wonder why he bothers throwing balls directly at the aliens, though.  STANLEY answers: \"I like balls.\"",
	"WALTER masks his tremendous insecurities with sheer bravado and scrappiness.  On the outside, this happy-go-lucky penguin seems to have not a care in the world.  On the inside, he has a great big boxing glove which he uses to punch people.",
	"SYLVESTER used to run a popular sushi bar in downtown San Francisco, but when he discovered a mysterious wormhole that led directly to the Insaniquarium, SYLVESTER sold the restaurant to pursue his lifelong dream: EATING ALL YOUR FISH!",
	"Many people want to know: How does BALROG manage to keep his hair so thick and luxurious?  BALROG answers: First of all, to keep the shine in my hair, I try to avoid using heated styling tools.  I also like to use a wide-tooth comb while in the shower so that I can distribute my conditioner EVENLY.  Finally, when sleeping, I put my hair in a scrunchie to prevent breakage.",
	"Many people wonder: What is GUS laughing about?  Just what is so funny about killing innocent fishies?  He's actually not laughing about fish.  It turns out that GUS heard the following joke in 1984 and is still laughing about it: What did the daddy buffalo say to his kid before leaving the house?  Bison!",
	"DESTRUCTOR was created many years ago by a brilliant scientist to be a toy for his favorite nephew.  The nephew, unfortunately, was a sadistic little boy who liked to torture his pet fish.  This is where DESTRUCTOR learned his destructive behavior.  The scientist now readily admits, \"Maybe it was a bad idea giving DESTRUCTOR those missiles.\"",
	"One big \"How Come\" with ULYSSES is, How come ULYSSES doesn't just fire already-deflected missiles?  Wouldn't this kill more fish and get the job done faster?  The reason he can't do this is complicated, but it essentially boils down to a strict code of honor and fairness among cyclopses that precludes such unsportsmanlike behavior... and also, he's dumb.",
	"PSYCHOSQUID's nickname was given to him by his ex-girlfriend.  He could not comprehend when their relationship was over.  He kept calling her as if nothing had happened.  He would wait outside her door and ask what she had been up to.  This is when she started calling him PSYCHOSQUID to her friends.  ",
	"Hardcore BILATERUS fans often argue over which head is superior.  Judy K from Minnesota writes in: \"Obviously the pointy head is the brains of the operation.  The round head is just an overrated Christmas ornament.\"  Bob J from Alabama counters:  \"No way.  Roundy is the MAN!  Pointy is just along for the ride.\"  Whoa, easy there, you two.  This is one controversy that won't be settled today!",
	"MINISYLVESTER was created by CYRAX to be a sort-of \"Yes-Man\" or \"Toady\".  Originally he sought to make a MINICYRAX, but this plan was disastrous as the MINICYRAXes conspired to do away with their creator.  CYRAX found the MINISYLVESTERs to be much more malleable to his will... or at least that's what they have led him to believe...",
	"After losing the legendary Battle of Tank 5, Cyrax moved on and eventually became host of his own late night variety show.  Some people call it a cerebral high-brow interview program, while critics have deemed it a lowest-common-denominator sleaze-fest.  There is certainly no end of controversy to his show, and Cyrax wouldn't have it any other way.",
};

Sexy::StoryScreen::StoryScreen(WinFishApp* theApp, int theStoryId)
{
	mApp = theApp;
	theApp->PlayMusic(2, 5);
	mStoryId = theStoryId;
	mBubbleMgr = new BubbleMgr();
	mBubbleMgr->mBubbleBounds = Rect(0, -20, 640, 520);
	mBubbleMgr->SetBubbleConfig(10, 3);
	mBubbleMgr->UpdateALot();
	mX = 0;
	mY = 0;
	mWidth = mApp->mWidth;
	mHeight = mApp->mHeight;

	mBackToMainMenuButton = MakeHyperlinkWidget(0, this, "Back To Main Menu");
	mBackToMainMenuButton->mX = 630 - mBackToMainMenuButton->mWidth;
	mBackToMainMenuButton->mY = 460 - mBackToMainMenuButton->mHeight / 2;

	mBackButton = MakeHyperlinkWidget(1, this, "Back");
	mNextButton = MakeHyperlinkWidget(2, this, "Next");

	mBackButton->mX = 220 - mBackButton->mWidth;
	mBackButton->mY = 15;
	mNextButton->mX = 420;
	mNextButton->mY = 15;

	int aStories = mApp->mCurrentProfile->m0x7c;
	int aSeenStoryCnt = 0;
	if (aStories != 0)
	{
		do
		{
			if (aSeenStoryCnt >= 2)
				break;
			if ((aStories & 1) != 0)
				aSeenStoryCnt++;
			aStories >>= 1;
		} while (aStories != 0);
	}
	if (aSeenStoryCnt <= 1)
	{
		mNextButton->mVisible = false;
		mBackButton->mVisible = false;
	}
	mTalkShowImg = nullptr;
	if (mStoryId < 0 || mStoryId > 32)
		mStoryId = gLastStoryId;
	ChangeStory(0);
	LoadTalkShow();
}

Sexy::StoryScreen::~StoryScreen()
{
	if (mBubbleMgr)
		delete mBubbleMgr;
	if (mBackToMainMenuButton)
		delete mBackToMainMenuButton;
	if (mNextButton)
		delete mNextButton;
	if (mBackButton)
		delete mBackButton;
	if (mTalkShowImg)
		delete mTalkShowImg;
	gLastStoryId = mStoryId;
}

void Sexy::StoryScreen::AddedToManager(WidgetManager* theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mBackToMainMenuButton);
	theWidgetManager->AddWidget(mNextButton);
	theWidgetManager->AddWidget(mBackButton);
}

void Sexy::StoryScreen::RemovedFromManager(WidgetManager* theWidgetManager)
{
	Widget::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mBackToMainMenuButton);
	theWidgetManager->RemoveWidget(mNextButton);
	theWidgetManager->RemoveWidget(mBackButton);
}

void Sexy::StoryScreen::Update()
{
	Widget::Update();
	if(mApp->mHasFocus)
	{
		mBubbleMgr->Update();
		MarkDirty();
	}
}

void Sexy::StoryScreen::Draw(Graphics* g)
{
	if (mStoryId == 32 && mTalkShowImg)
	{
		g->DrawImage(mTalkShowImg, 0, 0);
		g->SetFont(FONT_JUNGLEFEVER17OUTLINE);
		g->SetColor(Color(0xffffff));
		WriteCenteredLine(g, 80, "The Story Of");
		g->SetFont(FONT_JUNGLEFEVER15OUTLINE);
		g->SetColor(Color(0xffffff));
		WriteCenteredLine(g, 110, STORY_TITLES[mStoryId]);
	}
	else
	{
		g->SetColor(Color(25, 105, 195, 255));
		g->FillRect(0, 0, mWidth, mHeight);
		mBubbleMgr->Draw(g);

		g->SetFont(FONT_LIDDIE18);
		g->SetColor(Color(215, 255, 255, 255));
		WriteCenteredLine(g, 80, "The Story Of");

		g->SetFont(FONT_LIDDIE15);
		g->SetColor(Color(150, 250, 150, 255));
		WriteCenteredLine(g, 110, STORY_TITLES[mStoryId]);
	}

	int anImgId;
	if (mStoryId < 24)
		anImgId = mStoryId + IMAGE_STINKY_ID;
	else
		anImgId = (mStoryId - 24) + IMAGE_SYLV_ID;

	Image* anImg = GetImageById(anImgId);
	int aCelWdth = anImg->GetCelWidth();
	int aCelHght = anImg->GetCelHeight();
	int anAnimCel = anImg->GetAnimCel(mUpdateCnt);
	int anImgX = 320 - aCelWdth / 2;
	int anImgY = 190 - aCelHght / 2;

	Rect aStrWrapRect(50, 280, 540, 0);
	g->SetFont(FONT_CONTINUUMBOLD14);
	if (anImg == IMAGE_BILATERUS)
	{
		int aTimer = mUpdateCnt % 20;
		int aSlowTime = aTimer / 2;
		if (aTimer > 9)
			aTimer = 19 - aTimer;

		Rect aRect = IMAGE_BILATERUS->GetCelRect(aSlowTime, 5);
		g->DrawImageMirror(IMAGE_BILATERUS, 395, 135, aRect, true);
		g->DrawImageCel(IMAGE_BILATERUS, 360, 145, 2, 7);
		g->DrawImageCel(IMAGE_BILATERUS, 330, 140, 2, 6);
		g->DrawImageCel(IMAGE_BILATERUS, 300, 145, 2, 6);
		g->DrawImageCel(IMAGE_BILATERUS, 270, 155, 2, 7);
		g->DrawImageCel(IMAGE_BILATERUS, 240, 160, 2, 6);
		g->DrawImageCel(IMAGE_BILATERUS, 210, 155, 2, 7);
		g->DrawImageCel(IMAGE_BILATERUS, 180, 150, aTimer, 2);
	}
	else
	{
		if (anImg == IMAGE_BOSS && mTalkShowImg != nullptr)
		{
			g->SetFont(FONT_CONTINUUMBOLD12OUTLINE);
			aStrWrapRect.mY = 330;
		}
		else
			g->DrawImageCel(anImg, anImgX, anImgY, anAnimCel, mStoryId == 7);
	}

	g->SetColor(Color::White);
	WriteWordWrapped(g, aStrWrapRect, STORY_TEXT[mStoryId], -1, -1);

	g->SetColor(Color::Black);
	g->FillRect(0, 0, mWidth, 40);
	g->FillRect(0, mHeight - 40, mWidth, 40);
	g->SetColor(Color::White);
	g->SetFont(FONT_CONTINUUMBOLD12);
	SexyString aStoryPageStr = StrFormat("%d of 33", mStoryId);
	int aStrWdth = g->GetFont()->StringWidth(aStoryPageStr);
	g->DrawString(aStoryPageStr, 320 - aStrWdth / 2, 28);
}

void Sexy::StoryScreen::ButtonPress(int theId)
{
	mApp->PlaySample(SOUND_BUTTONCLICK);
}

void Sexy::StoryScreen::ButtonDepress(int theId)
{
	if (theId == 0)
	{
		mApp->StopMusic();
		mApp->RemoveStoryScreen();
		mApp->SwitchToGameSelector();
	}
	else if (theId == 1 || theId == 2)
		ChangeStory(theId == 1 ? -1 : 1);
}

void Sexy::StoryScreen::ChangeStory(int theDirection)
{
	int aDir = theDirection;
	if (theDirection == 0)
	{
		if ((mApp->mCurrentProfile->m0x7c & (1 << mStoryId)) != 0)
			return;
		aDir = 1;
	}

	int aSearchId = mStoryId;
	int i = 0;
	while (i < 33)
	{
		aSearchId = (aSearchId + 33 + aDir) % 33;
		if (aSearchId < 32)
		{
			if ((mApp->mCurrentProfile->m0x7c & (1 << aSearchId)) != 0) break;
		}
		else
		{
			if (mApp->mCurrentProfile->m0x80 == 1) break;
		}
		i++;
	}

	if (i != 33)
		mStoryId = aSearchId;

	LoadTalkShow();
}

void Sexy::StoryScreen::LoadTalkShow()
{
	if (mStoryId == 32 && mTalkShowImg == nullptr)
		mTalkShowImg = (Image*) mApp->GetImage("images/talkshow");
}
