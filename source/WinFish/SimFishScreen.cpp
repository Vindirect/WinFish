#include "SimFishScreen.h"
#include "WinFishApp.h"
#include "WinFishAppCommon.h"
#include "Board.h"
#include "Res.h"

#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Font.h"

#include <time.h>

const SexyString HOMETOWNS[] = {
	"Virtual Tank",
	"Abilene, Texas",
	"Adak, Alaska",
	"Canton, Ohio",
	"Alamogordo, N.M.",
	"Alamosa, Colo.",
	"Albany, N.Y.",
	"Albuquerque, N.M.",
	"Alexandria, La.",
	"Allentown, Pa.",
	"Alpena, Mich.",
	"Altus, Okla",
	"Amarillo, Texas",
	"Anchorage, Alaska",
	"Apalachicola, Fla.",
	"Asheville, N.C.",
	"Aspen, Colo.",
	"Astoria, Ore.",
	"Athens, Ga.",
	"Atlanta, Ga.",
	"Augusta, Ga.",
	"Austin, Texas",
	"Bakersfield, Calif.",
	"Baltimore, Md.",
	"Bar Harbor, Maine",
	"Barrow, Alaska",
	"Baton Rouge, La.",
	"Beckley, W.Va.",
	"Beeville, Texas",
	"Billings, Mont.",
	"Biloxi, Miss.",
	"Binghamton, N.Y.",
	"Birmingham, Ala.",
	"Bishop, Calif.",
	"Bismarck, N.D.",
	"Boston, Mass.",
	"Bridgeport, Conn.",
	"Brownsville, Texas",
	"Brunswick, Maine",
	"Buffalo, N.Y.",
	"Burlington, Vt.",
	"Cape Hatteras, N.C.",
	"Caribou, Maine",
	"Casper, Wyo.",
	"Charleston, S.C.",
	"Charleston, W.Va.",
	"Charlotte, N.C.",
	"Chattanooga, Tenn.",
	"Cheyenne, Wyo.",
	"Chicago, Ill.",
	"Cincinnati, Ohio",
	"Clayton, N.M.",
	"Cleveland, Ohio",
	"Clovis, N.M.",
	"Cocoa Beach, Fla.",
	"Colorado Springs, Colo.",
	"Columbia, Mo.",
	"Columbia, S.C.",
	"Columbus, Ga.",
	"Columbus, Miss.",
	"Columbus, Ohio",
	"Concord, N.H.",
	"Concordia, Kan.",
	"Corpus Christi, Texas",
	"Covington, Ky.",
	"Crested Butte, Colo.",
	"Daggett, Calif.",
	"Dallas, Texas",
	"Davenport, Iowa",
	"Dayton, Ohio",
	"Daytona Beach, Fla.",
	"Denver, Colo.",
	"Del Rio, Texas",
	"Des Moines, Iowa",
	"Detroit, Mich.",
	"Dodge City, Kan.",
	"Dover, Del.",
	"Dubuque, Iowa",
	"Duluth, Minn.",
	"Elkins, W.Va.",
	"Elko, Nev.",
	"El Paso, Texas",
	"Ely, Nev.",
	"Enid, Okla.",
	"Erie, Pa.",
	"Eugene, Ore.",
	"Eureka, Calif.",
	"Evansville, Ind.",
	"Fairbanks, Alaska",
	"Fallon, Nev.",
	"Fargo, N.D.",
	"Fayetteville, N.C.",
	"Flagstaff, Ariz.",
	"Flint, Mich.",
	"Fort Myers, Fla.",
	"Fort Smith, Ark.",
	"Fort Wayne, Ind.",
	"Fresno, Calif.",
	"Gainesville, Fla.",
	"Galveston, Texas",
	"Gatlinburg, Tenn.",
	"Gila Bend, Ariz.",
	"Glasgow, Mont.",
	"Glenview, Ill.",
	"Goldsboro, N.C.",
	"Goodland, Kan.",
	"Grand Forks, N.D.",
	"Grand Island, Neb.",
	"Grand Junction, Colo.",
	"Grand Rapids, Mich.",
	"Great Falls, Mont.",
	"Green Bay, Wis.",
	"Greensboro, N.C.",
	"Greenville, S.C.",
	"Gulfport, Miss.",
	"Harrisburg, Pa.",
	"Hartford, Conn.",
	"Havre, Mont.",
	"Helena, Mont.",
	"Hilo, Hawaii",
	"Hondo, Texas",
	"Honolulu, Hawaii",
	"Houghton Lake, Mich.",
	"Houston, Texas",
	"Huntington, W.Va.",
	"Huntsville, Ala.",
	"Huron, S.D.",
	"Indianapolis, Ind.",
	"Long Island, N.Y.",
	"Jackson, Ky.",
	"Jackson, Miss.",
	"Jacksonville, Fla.",
	"Jacksonville, N.C.",
	"Juneau, Alaska",
	"Kahului, Hawaii",
	"Kalispell, Mont.",
	"Kansas City, Kan.",
	"Kansas City, Mo.",
	"Key West, Fla.",
	"Killeen, Texas",
	"Kingsville, Texas",
	"Klamath Falls, Ore.",
	"Knoxville, Tenn.",
	"La Crosse, Wis.",
	"Lake Charles, La.",
	"Lake Havasu, Ariz.",
	"Lakehurst, N.J.",
	"Lake Tahoe",
	"Lander, Wyo.",
	"Lansing, Mich.",
	"Las Vegas, Nev.",
	"Lawton, Okla.",
	"Lemoore, Calif.",
	"Rehoboth Beach, Del.",
	"Lewiston, Idaho",
	"Lexington, Ky.",
	"Lexington Park, Md.",
	"Lihue, Hawaii",
	"Lincoln, Neb.",
	"Little Rock, Ark.",
	"Long Beach, Calif.",
	"Los Angeles, Calif.",
	"Louisville, Ky.",
	"Lubbock, Texas",
	"Lynchburg, Va.",
	"Macon, Ga.",
	"Madison, Wis.",
	"Mansfield, Ohio",
	"Marquette, Mich.",
	"Mascoutah, Ill.",
	"Medford, Ore.",
	"Melbourne, Fla.",
	"Memphis, Tenn.",
	"Meridian, Miss.",
	"Miami, Fla.",
	"Miles City, Mont.",
	"Milford, Utah",
	"Milwaukee, Wis.",
	"St. Paul, Minn.",
	"Minneapolis",
	"Minot, N.D.",
	"Missoula, Mont.",
	"Mobile, Ala.",
	"Moline, Ill.",
	"Monterey, Calif.",
	"Montgomery, Ala.",
	"Moses Lake, Wash.",
	"Muskegon, Mich.",
	"Myrtle Beach, S.C.",
	"Nantucket, Mass.",
	"Nashville, Tenn.",
	"New Bern, N.C.",
	"New Orleans, La.",
	"Newport, R.I.",
	"New York, N.Y.",
	"Nome, Alaska",
	"Norfolk, Neb.",
	"Norfolk, Va.",
	"North Platte, Neb.",
	"Oakland, Calif.",
	"Ocean City, Md.",
	"Oceanside, Calif.",
	"Ogden, Utah",
	"Oklahoma City, Okla.",
	"Orlando, Fla.",
	"Olympia, Wash.",
	"Omaha, Neb.",
	"Ontario, Calif.",
	"Oxnard, Calif.",
	"Paducah, Ky.",
	"Palm Springs, Calif.",
	"Panama City, Fla.",
	"Parkersburg, W.Va.",
	"Pendleton, Ore.",
	"Pensacola, Fla.",
	"Peoria, Ill.",
	"Peru, Ind.",
	"Philadelphia, Pa.",
	"Phoenix, Ariz.",
	"Pittsburgh, Pa.",
	"Pocatello, Idaho",
	"Port Angeles, Wash.",
	"Portland, Maine",
	"Portland, Ore.",
	"Portsmouth, N.H.",
	"Providence, R.I.",
	"Cape Cod, Mass.",
	"Pueblo, Colo.",
	"Quantico, Va.",
	"Quillayute, Wash.",
	"Raleigh, N.C.",
	"Rapid City, S.D.",
	"Red Bluff, Calif.",
	"Redding, Calif.",
	"Richmond, Va.",
	"Ridgecrest, Calif.",
	"Roanoke, Va.",
	"Rochester, Minn.",
	"Rochester, N.Y.",
	"Rockford, Ill.",
	"Utica, N.Y.",
	"Roswell, N.M.",
	"Sacramento, Calif.",
	"St. Cloud, Minn.",
	"St. George, Utah",
	"St. Joseph, Mo.",
	"St. Louis, Mo.",
	"St. Robert, Mo.",
	"Salt Lake City, Utah",
	"Salem, Ore.",
	"San Angelo, Texas",
	"San Antonio, Texas",
	"San Diego, Calif.",
	"San Francisco, Calif.",
	"Santa Barbara, Calif.",
	"Santa Maria, Calif.",
	"Savannah, Ga.",
	"Scottsbluff, Neb.",
	"Seattle, Wash.",
	"Tacoma, Wash.",
	"Sheridan, Wyo.",
	"Shreveport, La.",
	"Sioux City, Iowa",
	"Sioux Falls, S.D.",
	"South Bend, Ind.",
	"Spokane, Wash.",
	"Springfield, Ill.",
	"Springfield, Mass.",
	"Springfield, Mo.",
	"Sterling, Va.",
	"Stockbridge, Mass.",
	"Stockton, Calif.",
	"Sumter, S.C.",
	"Sun Valley, Idaho",
	"Syracuse, N.Y.",
	"Tallahassee, Fla.",
	"Tampa, Fla.",
	"St. Petersburg, Fla.",
	"Taos, N.M.",
	"Toledo, Ohio",
	"Topeka, Kan.",
	"Tucson, Ariz.",
	"Tupelo, Miss.",
	"Valdosta, Ga.",
	"Valentine, Neb.",
	"Vero Beach, Fla.",
	"Victoria, Texas",
	"Virginia Beach, Va.",
	"Walla Walla, Wash.",
	"Waco, Texas",
	"Washington, D.C.",
	"Waterloo, Iowa",
	"Watertown, N.Y.",
	"Wendover, Utah",
	"West Palm Beach, Fla.",
	"Whidbey Island, Wash.",
	"Wichita, Kan.",
	"Wichita Falls, Texas",
	"Williamsport, Pa.",
	"Williston, N.D.",
	"Scranton, Pa.",
	"Wilmington, Del.",
	"Wilmington, N.C.",
	"Winnemucca, Nev.",
	"Winslow, Ariz.",
	"Worcester, Mass.",
	"Yakima, Wash.",
	"Youngstown, Ohio",
	"Yuma, Ariz.",
	"Muncie, Ind",
	"McLean, Va.",
	"Issaquah, Wash.",
	"Pasadena, Calif.",
	"Walnut Creek, Calif.",
	"Moscow, Russia",
	"London, England",
	"Berlin, Germany",
	"Madrid, Spain",
	"Rome, Italy",
	"Kiev, Ukraine",
	"Paris, France",
	"Budapest, Hungary",
	"Hamburg, Germany",
	"Warsaw, Poland",
	"Wembley Park, England",
	"Kilburn, England",
	"Milan, Italy",
	"Barcelona, Spain",
	"Munich, Germany",
	"Naples, Italy",
	"Florence, Italy",
	"Riga, Latvia",
	"Leeds, England",
	"Staines, Westside",
	"Omsk, Russia",
	"Tooting, England",
	"North Pole, Alaska",
	"Why, Ariz.",
	"Lorida, Fla.",
	"Experiment, Ga.",
	"Normal, Ill.",
	"Dwarf, Ky.",
	"Hippo, Ky.",
	"Humptulips, Wash.",
	"Walla Walla, Wash.",
	"Bigfoot, Texas",
	"Happy, Texas",
	"Bird-in-Hand, Pa.",
	"Businessburg, Ohio",
	"Watford, England",
	"Loch Ness, Scotland",
	"Lake Titicaca, Peru",
	"Puddledock, England",
	"Little Snoring, U.K.",
	"Yelling, U.K.",
	"Crackpot, U.K.",
	"Great Snoring, U.K.",
	"High Ham, U.K.",
	"Giggleswick, U.K.",
	"Shiner, Texas",
	"Why, Ariz.",
	"Hasty, Colo.",
	"Protection, Kan.",
	"Moon, Ky.",
	"Trout, La.",
	"Weed, N.M.",
	"Hopton Wafers, U.K.",
	"Abu Dhabi",
	"Middle Wallop, U.K.",
	"Tokyo, Japan",
	"Easter Island",
	"Bermuda Triangle",
	"Mars",
	"The White House, D.C.",
	"Wapping, U.K.",
};

const SexyString SPECIALHOMETOWNS[6] = { 
	"Philadelphia, Pa.",
	"Vienna, Austria",
	"Biloxi, Miss.",
	"Rensselaer, Ind.",
	"Los Angeles, Calif.",
	"North Pole"
};

const SexyString LIKES[] = {
	"Knitting",
	"Painting",
	"Woodworking",
	"Antiques",
	"Comic books",
	"Dumpster Diving",
	"Records",
	"Collecting Stamps",
	"Trading cards",
	"Smooth Jazz",
	"Board Games",
	"Card Games",
	"Historical Reenactment",
	"Rafting",
	"Photography",
	"Baseball",
	"Bowling",
	"Curling",
	"Live Action Role-playing",
	"Mountain Biking",
	"Hang Gliding",
	"Flying Ultralites",
	"Digeridooing",
	"Lottery Scratch-offs",
	"Samurai Swords",
	"Muscle Cars",
	"Mushroom Hunting",
	"Flossing",
	"Whiffleball",
	"Stargazing",
	"Tie-dying",
	"Kitchen Remodeling",
	"Mastiff Breeding",
	"Ornithology",
	"Mancala",
	"Backgammon",
	"Techno",
	"Corporate Law",
	"The Occult",
	"Videogames",
	"Gangsta Rap",
	"Poetry",
	"Wine Collecting",
	"Drag Racing",
	"Euchre",
	"Hoe-downs",
	"Jogging",
	"Breakdancing",
	"Fashion",
	"Swimming",
	"Line Dancing",
	"Mountaineering",
	"Tennis",
	"Staring",
	"Skiing",
	"Algebra",
	"Golf",
	"Power Walking",
	"High Explosives",
	"Blogging",
	"Pina Coladas",
	"WWII Memorabilia",
	"Online Chat",
	"Fundraising",
	"Yoga",
	"Coffee",
	"Fresh Fruit",
	"Salsa Dancing",
	"Improv Comedy",
	"Musicals",
	"Wine Tasting",
	"Weekend Getaways",
	"Bejeweled",
	"Zuma",
	"Bookworm",
	"Particle Physics",
	"Journalism",
	"Tearjerkers",
	"Foreign Cinema",
	"Spaceships",
	"Nanotechnology",
	"Leather Pants",
	"Country Music",
	"Grunge",
	"Styling Hair",
	"Ham Radio",
	"Fire Eating",
	"Window Tinting",
	"Bobsledding",
	"Glass Etching",
	"Poppies",
	"Glass Blowing",
	"Pottery",
	"Church Choir",
	"Spooning",
	"Gorgonzola",
	"Jam Bands",
	"Aggressive Skating",
	"Fire Walking",
	"20 Inch Rims",
	"Tricked Out Rides",
	"Poll Volunteering",
	"Indie Films",
	"Hair Bands",
	"Digital Cable",
	"HDTV",
	"Spelling Bees",
	"Combines",
	"Food Fights",
	"Beans",
	"Extreme Sports",
	"Riddles",
	"Knock-Knock Jokes",
	"Trampolines",
	"Low Carbin'",
	"Broadway Show Tunes",
	"The Lambada",
	"Car Repair",
	"Garage Sales",
	"Rubber Bands",
	"Number 2 Pencils",
	"Skim Milk",
	"Avoiding Sharks",
	"Collecting Tea Bags",
	"Tap Dancing",
	"Vaudeville",
	"Denver Omelets",
	"Basket Weaving",
	"Kung Fu Movies",
	"Debating",
	"Bargain Hunting",
	"Avoiding Hooks",
	"Train spotting",
	"Hot-tubbing",
	"Hide and Seek",
	"Local Politics",
	"General Insanity",
	"Antique Weapon Collecting",
	"Finger Painting",
	"Underwater Cartography",
	"Seismology",
	"Antique Car Restoration",
	"Blenders",
	"Skydiving",
	"Paintball",
	"Long Walks in the Park",
	"Sushi",
	"Company Picnics",
	"Filling Out Forms",
	"Meteorology",
	"Baking Cookies",
	"Billiards",
	"Soy Sauce",
	"Bubbles",
	"Trampolines",
	"Pirates",
	"Ninjas",
	"Robots",
	"Candy",
	"Camping",
	"Foreign Languages",
	"Clipping Coupons",
	"Swashbuckling",
	"Lumberjacks",
	"Entomology",
	"Weed Whacking",
	"Power Washers",
	"Working Out",
	"The Pacific Northwest",
	"Thrift Stores",
	"Monorails",
	"Recycling",
	"Beta Testing",
	"Fondue Parties",
	"Ladies Nights",
	"Infomercials",
	"Hippies",
	"Practical Jokes",
	"College Parties",
	"Soccer Hooligans",
	"Yakisoba",
	"Welding",
	"Ice Sculpting",
	"Vibraphones",
	"Anime",
	"MP3 Players",
	"Fingerless Gloves",
	"Fireside Chats",
	"Horseback Riding",
	"Working with Animals",
	"Whittling",
	"Oil Painting",
	"Macrame",
	"Sun Tanning",
	"Wakeboarding",
	"World Music",
	"Travel",
	"Horse Racing",
	"Sports Books",
	"Hockey",
	"Violin",
	"Crochet",
	"Sewing",
	"Spelunking",
	"Lizards",
	"Street Fighting",
	"Jigsaws",
	"Jujitsu",
	"Lard",
	"Pumping Iron",
	"Body Massage",
	"Stuffed Animals",
	"Stickers",
	"Glitter Glue",
	"Hot Dogs",
	"Pancakes",
	"Pork Chops",
	"Muffins",
	"Fencing",
	"Dental Visits",
	"Fireworks",
	"Skipping",
	"Glubbing",
	"Beekeeping",
	"Haggis",
	"Cheese Sculpture",
	"Shampoo",
	"Cubicles",
	"Bits of String",
	"Wood",
	"Telling Jokes",
	"Making Toast",
	"Counting to 10",
	"Pillow Fights",
	"Chili Cheese Omelets",
	"Kung Fu",
	"Karate",
	"Boxing",
	"Phone Phreaking",
	"Hacking",
	"Chemistry",
	"Karaoke",
	"Heavy Metal",
	"Ballet",
	"Casual Fridays",
	"Bejeweled 2",
	"The Stock Market",
	"Telecommuting",
	"Buddhism",
	"Vegan Cuisine",
	"TV Dinners",
	"Romantic Comedies",
	"Hearts",
	"Spades",
	"Yogic Flying",
	"Beach Workouts",
	"Collecting Shells",
	"Quantum Physics",
	"Pickling Vegetables",
	"Double Dipping",
	"Ichthyology",
	"Doomsday Cults",
	"80s Arcade Games",
	"True Crime Novels",
	"New Wave Music",
	"Bingo",
	"Breeding Pugs",
	"Dog Shows",
	"Flossing",
	"Moonlighting",
	"Asian Culture",
	"The GOP",
	"Libertarians",
	"Tai Chi",
	"Lip Balm",
	"Smoking Cigars",
	"Reality TV",
	"Haikus",
	"Lumbar Support",
	"Community Theater",
	"Organ Music",
	"Online Auctions",
	"Chili Cook-offs",
	"MMORPGs",
	"Fighting Terrorism",
	"Reggae",
	"Hog Roasts",
	"Tooth Necklaces",
	"Making Omelets",
	"Greasy Spoon Fare",
	"Port Hopping",
	"Raking Gravel",
	"Making Jerky",
	"Smoking Bacon",
	"Grilling Veggies",
	"Bar Crawling",
	"Wine Tasting",
	"Picketing",
	"Renaissance Faires",
	"Yachting",
	"Shopping for shoes",
	"Norse Mythology",
	"Strategy Gaming",
	"Glockenspiels",
	"Stock Car Racing",
	"Old Game Shows",
	"Tetherball",
	"Four Square",
	"Sleight of Hand",
	"Mandarin Cooking",
	"Hospital Corners",
	"Truffle Hunting",
	"Paintball",
	"Daytime TV",
	"Old 45s",
	"Haunted Houses",
	"Fixing Flats",
	"Public Transit",
	"GPS Games",
	"Hiking Rainier",
	"Cancer Research",
	"Woodburning Stoves",
	"Chimney Sweeping",
	"Telemarketers",
	"Moonlit Walks",
	"Value Menus",
	"Combo Meals",
	"Keeping it Real",
	"Clicking Like Crazy",
	"Bling Bling",
	"Romance Novels",
};

Sexy::SimFishScreen::SimFishScreen(WinFishApp* theApp)
{
	mApp = theApp;
	mBubbleMgr = new BubbleMgr();
	mApp->StopMusic();
	mApp->PlayMusic(2, 0);
	mX = 0;
	mY = 0;
	m0x10c = 25;
	m0x110 = 94;
	mWidth = mApp->mWidth;
	mHeight = mApp->mHeight;

	mHideAllButton = MakeDialogButton2(105, this, "Hide All", IMAGE_MAINBUTTON);
	mHideAllButton->Resize(500, 4, 100, mHideAllButton->mHeight);

	mShowAllButton = MakeDialogButton2(106, this, "Show All", IMAGE_MAINBUTTON);
	mShowAllButton->Resize(500, 4, 100, mShowAllButton->mHeight);
	mShowAllButton->mVisible = false;

	mReturnButton = MakeDialogButton(100, this, "Return to Tank", FONT_JUNGLEFEVER10OUTLINE);
	mReturnButton->Resize(218, 428, 206, mReturnButton->mHeight);

	mMenuButton = MakeDialogButton2(101, this, "Menu", IMAGE_MAINBUTTON);
	mMenuButton->Resize(525, 4, 80, mMenuButton->mHeight);

	mHideShowButton = MakeDialogButton2(102, this, "", IMAGE_LEFTBUTTON);
	mHideShowButton->Resize(225, 390, 58, mHideShowButton->mHeight);
	mHideShowButton->mTextOffsetX = 0;

	mSellButton = MakeDialogButton2(103, this, "Sell", IMAGE_RIGHTBUTTON);
	mSellButton->Layout(0x4403, mHideShowButton, 72);
	mSellButton->mTextOffsetX = 0;

	mRenameButton = MakeDialogButton2(104, this, "Rename", IMAGE_CENTERBUTTON);
	mRenameButton->Layout(0x4402, mHideShowButton, -2);
	mRenameButton->Layout(0x20000, mSellButton, 0,0,3);

	mHideShowButton->SetColor(0, Color::White);
	mSellButton->SetColor(0, Color::White);
	mRenameButton->SetColor(0, Color::White);
	mReturnButton->SetColor(0, Color(255,240,0));

	mBackgroundImage = new MemoryImage(theApp);
	mBackgroundImage->Create(640, 480);
	mBackgroundImage->SetImageMode(true, true);
	Graphics gImg(mBackgroundImage);
	gImg.DrawImageBox(Rect(-5,-5,650,490), IMAGE_SCREENBACK);
	gImg.DrawImageBox(Rect(20,0,600, IMAGE_SCREENTITLE->mHeight), IMAGE_SCREENTITLE);
	gImg.DrawImageBox(Rect(mHideAllButton->mX - 1, mHideAllButton->mY - 1, mHideAllButton->mWidth + 2, IMAGE_SCREENTITLEHOLE->mHeight), IMAGE_SCREENTITLEHOLE);
	gImg.DrawImageBox(Rect(216, 48, IMAGE_FISHBOX->mWidth, 350), IMAGE_FISHBOX);
	gImg.DrawImage(IMAGE_FISHBOXBUTTON, 221, 385);

	mBubbleMgr->mBubbleBounds = Rect(236, 48, IMAGE_FISHBOX->mWidth - 40, 335);
	mBubbleMgr->SetBubbleConfig(10, 3);
	mBubbleMgr->UpdateALot();

	memset(mObjectButtons, 0, 20);

	std::multimap<time_t, GameObject*> aFishMap;

	for (GameObjectSet::iterator it = mApp->mBoard->mGameObjectSet.begin(); it != mApp->mBoard->mGameObjectSet.end(); ++it)
	{
		GameObject* anObj = *it;
		if (anObj->mVirtualTankId > -1 && anObj->mVirtualTankId < 20)
			aFishMap.insert(std::pair<time_t, GameObject*>(anObj->mTimeBought, anObj));
	}

	int aFishBtnId = 0;
	auto it = aFishMap.begin();

	while (it != aFishMap.end() && aFishBtnId < 20)
	{
		GameObject* aFish = it->second;

		CreateFishButton(aFishBtnId, aFish);

		aFishBtnId++;
		it++;
	}

	for (int i = 0; i < 20;i++)
	{
		if (mObjectButtons[i] == nullptr)
			CreateFishButton(i, nullptr);
	}

	mOverlay = new SimFishScreenOverlay();
	mOverlay->mScreen = this;
	mOverlay->mMouseVisible = false;
	mOverlay->mHasAlpha = true;
	mOverlay->mWidth = 640;
	mOverlay->mHeight = 480;

	mSelectedFishButton = nullptr;
	m0x114 = 0;
	m0x118 = false;
	m0x119 = false;
	m0x11a = false;
	DetermineShowHideForButtons();
}

Sexy::SimFishScreen::~SimFishScreen()
{
	if (mBubbleMgr)
		delete mBubbleMgr;
	for (int i = 0;i < 20;i++)
		delete mObjectButtons[i];
	if (mHideAllButton)
		delete mHideAllButton;
	if (mShowAllButton)
		delete mShowAllButton;
	if (mReturnButton)
		delete mReturnButton;
	if (mMenuButton)
		delete mMenuButton;
	if (mBackgroundImage)
		delete mBackgroundImage;
	if (mOverlay)
		delete mOverlay;
	if (mHideShowButton)
		delete mHideShowButton;
	if (mSellButton)
		delete mSellButton;
	if (mRenameButton)
		delete mRenameButton;
}

void Sexy::SimFishScreen::AddedToManager(WidgetManager* theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);
	for (int i = 0; i < 20;i++)
		theWidgetManager->AddWidget(mObjectButtons[i]);
	theWidgetManager->AddWidget(mOverlay);
	theWidgetManager->AddWidget(mReturnButton);
	theWidgetManager->AddWidget(mHideAllButton);
	theWidgetManager->AddWidget(mShowAllButton);
	theWidgetManager->AddWidget(mSellButton);
	theWidgetManager->AddWidget(mHideShowButton);
	theWidgetManager->AddWidget(mRenameButton);
}

void Sexy::SimFishScreen::RemovedFromManager(WidgetManager* theWidgetManager)
{
	Widget::RemovedFromManager(theWidgetManager);
	for (int i = 0; i < 20;i++)
		theWidgetManager->RemoveWidget(mObjectButtons[i]);
	theWidgetManager->RemoveWidget(mOverlay);
	theWidgetManager->RemoveWidget(mReturnButton);
	theWidgetManager->RemoveWidget(mHideAllButton);
	theWidgetManager->RemoveWidget(mShowAllButton);
	theWidgetManager->RemoveWidget(mSellButton);
	theWidgetManager->RemoveWidget(mHideShowButton);
	theWidgetManager->RemoveWidget(mRenameButton);
}

void Sexy::SimFishScreen::Update()
{
	MarkDirty();
	mBubbleMgr->Update();
	if (m0x114 > 0)
	{
		if (m0x118)
			m0x114--;
		else
		{
			m0x114++;
			if (m0x114 > 7)
				m0x114 = 0;
		}
	}
}

void Sexy::SimFishScreen::OrderInManagerChanged()
{
	for (int i = 0; i < 20;i++)
		mWidgetManager->BringToFront(mObjectButtons[i]);
	mWidgetManager->BringToFront(mOverlay);
	mWidgetManager->BringToFront(mReturnButton);
	mWidgetManager->BringToFront(mHideAllButton);
	mWidgetManager->BringToFront(mShowAllButton);
	mWidgetManager->BringToFront(mSellButton);
	mWidgetManager->BringToFront(mHideShowButton);
	mWidgetManager->BringToFront(mRenameButton);
}

void Sexy::SimFishScreen::DrawOverlay(Graphics* g)
{
	g->DrawImage(mBackgroundImage, 0, 0);
	mBubbleMgr->Draw(g);
	g->SetFont(FONT_JUNGLEFEVER17OUTLINE);
	g->SetColor(Color(255, 200, 0));
	WriteCenteredLine(g, 25, "Fish Setup");

	for (int i = 0;i < 20;i++)
	{
		g->DrawImage(IMAGE_PETBUTTONRING, mObjectButtons[i]->mX, mObjectButtons[i]->mY);
		if (!mObjectButtons[i]->mDisabled)
		{
			g->DrawImage(IMAGE_PETBUTTONREFLECT, mObjectButtons[i]->mX, mObjectButtons[i]->mY);
			if (mObjectButtons[i]->mObject && !mObjectButtons[i]->mObject->mShown)
			{
				g->SetFont(FONT_JUNGLEFEVER12OUTLINE);
				g->SetColor(Color(0xff6060));
				g->DrawString("HIDDEN", mObjectButtons[i]->mX + 13, mObjectButtons[i]->mY + 50);
			}
		}
	}
	GameObject* anObj = mSelectedFishButton ? mSelectedFishButton->mObject : nullptr;
	if (mSelectedFishButton == nullptr || anObj == nullptr)
	{
		g->SetFont(FONT_JUNGLEFEVER15OUTLINE);
		g->SetColor(Color(255, 200, 0));
		WriteWordWrapped(g, Rect(230, 180, 180, 100), "Select\na\nFish", -1, 0);
	}
	else
	{
		g->Translate(320, 90);
		anObj->DrawStoreAnimation(g, 4);
		g->Translate(-320, -90);
		g->SetFont(FONT_JUNGLEFEVER15OUTLINE);
		g->SetColor(Color(255, 200, 0));
		int aNameWidth = g->GetFont()->StringWidth(anObj->mName);
		if (aNameWidth < 171 || anObj->mName.length() < 5)
		{
			WriteCenteredLine(g, 80, anObj->mName);
		}
		else
		{
			bool hasJr = (anObj->mName.length() >= 4) &&
				(anObj->mName.substr(anObj->mName.length() - 4) == " JR.");
			if (!hasJr)
			{
				WriteCenteredLine(g, 80, anObj->mName);
			}
			else
			{
				SexyString aBaseName = anObj->mName.substr(0, anObj->mName.length() - 4);
				WriteCenteredLine(g, 80, anObj->mName);
				WriteCenteredLine(g, 80, "JR.");
			}
		}

		if (!anObj->mShown)
		{
			g->SetColor(Color(0xff6060));
		}

		g->SetFont(FONT_JUNGLEFEVER10OUTLINE);
		Color aCol1 = Color(255, 200, 0, 255);
		Color aCol3 = Color::White;
		int aFntHght = g->GetFont()->GetHeight();
		int aHeightSeparator = aFntHght * 1.75;
		if (!m0x118 || m0x114 > 0)
		{
			int aCurHght = 180;
			if (m0x114 > 0)
			{
				aCol1.mAlpha = m0x114 * 255 / 8;
				aCol3.mAlpha = aCol1.mAlpha;
			}
			g->SetColor(aCol1);
			SexyString aPurchaseDateStr = "Purchase Date";
			if (anObj->mType == GameObject::TYPE_GUPPY && ((Fish*)anObj)->mVirtualFish)
				aPurchaseDateStr = "Date of Birth";
			WriteCenteredLine(g, aCurHght, aPurchaseDateStr);

			time_t aTime = anObj->mTimeBought;
			if (anObj->mTimeBought < 0)
				aTime = 0;
			tm* aTM = localtime(&aTime);
			char aTimeString[1000];
			if (aTM == nullptr)
				strcpy(aTimeString, "Unknown");
			else
				strftime(aTimeString, 1000, "%b %d %Y", aTM);

			g->SetColor(aCol3);
			aCurHght += aFntHght;
			WriteCenteredLine(g, aCurHght, aTimeString);

			g->SetColor(aCol1);
			aCurHght += aHeightSeparator;
			WriteCenteredLine(g, aCurHght, "Hometown");

			g->SetColor(aCol3);
			SexyString aHometownStr = "";
			if (gUnkInt09 >= 0 && gUnkInt09 <= 374)
				aHometownStr = StrFormat("%s", HOMETOWNS[gUnkInt09].c_str());
			else if (anObj->mPreNamedTypeId >= GameObject::ROCKY && anObj->mPreNamedTypeId <= GameObject::SANTA)
				aHometownStr = StrFormat("%s", SPECIALHOMETOWNS[anObj->mPreNamedTypeId].c_str());
			else if(anObj->mHometownIdx >= 0 && anObj->mHometownIdx <= 374)
				aHometownStr = StrFormat("%s", HOMETOWNS[anObj->mHometownIdx].c_str());
			else
				aHometownStr = "Unknown";

			aCurHght += aFntHght;
			WriteCenteredLine(g, aCurHght, aHometownStr);

			g->SetColor(aCol1);
			aCurHght += aHeightSeparator;
			WriteCenteredLine(g, aCurHght, "Mental State");

			g->SetColor(aCol3);
			aCurHght += aFntHght;
			WriteCenteredLine(g, aCurHght, GetMentalStateString(anObj));

			g->SetColor(aCol1);
			aCurHght += aHeightSeparator;
			WriteCenteredLine(g, aCurHght, "Additional Notes");

			g->SetColor(aCol3);
			SexyString aLikesStr = "";
			if (gUnkInt10 >= 0 && gUnkInt10 <= 330)
				aLikesStr = StrFormat("Likes %s, %s, and %s.", LIKES[gUnkInt10].c_str(),LIKES[gUnkInt10].c_str(),LIKES[gUnkInt10].c_str());
			else
				aLikesStr = GetAdditionalNotes(anObj);

			aCurHght += 5;
			WriteWordWrapped(g, Rect(234, aCurHght, 172, 100), aLikesStr, -1, 0);
		}

		if (m0x118 || m0x114 >= 1)
		{
			Color aCol2 = Color(255, 200, 0, 255);
			if (m0x114 > 0)
			{
				aCol2.mAlpha = 255 - ((m0x114 * 255) / 8);
				aCol3.mAlpha = aCol2.mAlpha;
			}
			g->SetColor(aCol2);
			WriteCenteredLine(g, 190, "Purchase Price");

			g->SetColor(aCol3);
			SexyString aPriceStr = StrFormat("%d Shells", anObj->mShellPrice);
			if (anObj->mType == GameObject::TYPE_GUPPY && ((Fish*)anObj)->mVirtualFish)
				aPriceStr = "N/A";
			WriteCenteredLine(g, 190 + aFntHght, aPriceStr);

			int aRefundValue = anObj->GetShellPrice();
			SexyString aRefundStr = "Full Refund";
			if (aRefundValue != -1)
				aRefundStr = StrFormat("%d Shells", aRefundValue);
			g->SetColor(aCol2);
			WriteCenteredLine(g, 190 + aFntHght*2, "Resale Value");
			g->SetColor(aCol3);
			WriteCenteredLine(g, 190 + aFntHght * 3, aRefundStr);
		}
	}
}

void Sexy::SimFishScreen::ButtonPress(int theId, int theClickCount)
{
	mApp->PlaySample(SOUND_BUTTONCLICK);
	if (theClickCount == 2 && theId < 20)
		ButtonDepress(102);
}

void Sexy::SimFishScreen::ButtonDepress(int theId)
{
	if (theId < 20)
	{
		FishButtonWidget* aBtn = mObjectButtons[theId];
		if (aBtn != nullptr)
		{
			if (mSelectedFishButton != nullptr)
			{
				if (mSelectedFishButton->mId == theId)
					return;

				mSelectedFishButton->SwitchDrawRects(false);
			}
			mSelectedFishButton = aBtn;
			aBtn->SwitchDrawRects(true);
			DetermineShowHideForButtons();
		}
	}
	else if (theId == 100)
	{
		mApp->StopMusic();
		mApp->RemoveSimFishScreen();
		if (m0x11a)
			mApp->SaveVirtualTankAndUserData();
		if (mApp->mBoard)
		{
			mApp->mBoard->StartMusic();
			mApp->mBoard->PauseGame(false);
		}
	}
	else if (theId == 103)
	{
		if (mSelectedFishButton != nullptr && mSelectedFishButton->mObject != nullptr)
		{
			m0x119 = true;
			m0x118 = true;
			m0x114 = 0;
			SexyString aStr = "Are you sure you want to sell your fish?";
			if (mSelectedFishButton->mObject->mType == GameObject::TYPE_BREEDER)
			{
				GameObject* anObj = mApp->mBoard->GetGameObjectByVirtualId(mSelectedFishButton->mObject->mVirtualTankId + 100);
				if (anObj)
					aStr += "\n\nNote that selling the mama fish will not sell her baby fish.";
			}

			mApp->DoAreYouSureSellDialog(aStr);
		}
	}
	else if (theId == 102)
	{
		if (mSelectedFishButton != nullptr && mSelectedFishButton->mObject != nullptr)
		{
			m0x11a = true;
			mApp->mBoard->HideObject(mSelectedFishButton->mObject, !mSelectedFishButton->mObject->mShown);
			DetermineShowHideForButtons();
			mWidgetManager->BringToFront(this);
		}
	}
	else if (theId == 104)
	{
		if (mSelectedFishButton != nullptr && mSelectedFishButton->mObject != nullptr)
		{
			m0x11a = true;
			mApp->DoFishNamingDialog("Please choose a name for your fish", 
				mSelectedFishButton->mObject->mName, 
				mSelectedFishButton->mObject->mType == GameObject::TYPE_BREEDER);
		}
	}
	else if (theId == 105 || theId == 106)
	{
		m0x11a = true;
		for (int i = 0; i < 20; i++)
		{
			if (mObjectButtons[i] != nullptr && mObjectButtons[i]->mObject != nullptr)
				mApp->mBoard->HideObject(mObjectButtons[i]->mObject, theId == 106);
			mWidgetManager->BringToFront(this);
			DetermineShowHideForButtons();
		}
	}
}

void Sexy::SimFishScreen::ButtonMouseEnter(int theId)
{
	if (theId == 103)
	{
		m0x118 = true;
		m0x114 = 8;
	}
}

void Sexy::SimFishScreen::ButtonMouseLeave(int theId)
{
	if (theId == 103 && !m0x119)
	{
		m0x118 = false;
		m0x114 = 1;
	}
}

SexyString Sexy::SimFishScreen::GetAdditionalNotes(GameObject* theObject)
{
	switch (theObject->mPreNamedTypeId)
	{
	case GameObject::ROCKY:
		return "By doctor\'s orders, is on a special low-carb high-Ultravore diet.";
	case GameObject::LUDWIG:
		return "Likes monster truck rallies, Thai kick-boxing, and Beethoven.";
	case GameObject::COOKIE:
		return "A fishy philanthropist who likes to feed food to famished fish? That\'s Fish-tastic!";
	case GameObject::JOHNNYV:
		return "Has been trying to stop eating fast food and pizza.";
	case GameObject::KILGORE:
		return "Likes to intimidate other fish by playing the music of Wagner.";
	case GameObject::SANTA:
		return "Likes giving lots of toys to all the good girls and boys.";
	default:
	{
		SexyString aLastLike = GetLastSpecialLike(theObject);
		return StrFormat("Likes %s, %s, and %s.", 
			LIKES[theObject->mLikes[0]].c_str(),
			LIKES[theObject->mLikes[1]].c_str(),
			aLastLike.empty() ? LIKES[theObject->mLikes[2]].c_str() : aLastLike.c_str()
		);
	}
	}
	return "";
}

SexyString Sexy::SimFishScreen::GetLastSpecialLike(GameObject* theObject)
{
	int anAttrib = theObject->GetAttribute();
	if(anAttrib != 9)
		switch (anAttrib)
		{
		case 0:
			return "stealth";
		case 1:
			return "eating";
		case 2:
			return "quickness";
		case 3:
			return "singing";
		case 4:
			return "swimming backwards";
		case 5:
			if (theObject->mExoticDietFoodType == Food::EXO_FOOD_PIZZA)
				return "pizza";
			else if (theObject->mExoticDietFoodType == Food::EXO_FOOD_ICE_CREAM)
				return "ice cream";
			else if (theObject->mExoticDietFoodType == Food::EXO_FOOD_CHICKEN)
				return "chicken";
			break;
		case 6:
			if (theObject->mExoticDietFoodType == Food::EXO_FOOD_GUPPY)
				return "eating guppies";
			else if (theObject->mExoticDietFoodType == Food::EXO_FOOD_OSCAR)
				return "eating carnivores";
			else if (theObject->mExoticDietFoodType == Food::EXO_FOOD_ULTRA)
				return "eating ultravores";
			break;
		default:
			return "being different";
		}
	return "";
}

void Sexy::SimFishScreen::CreateFishButton(int theButtonId, GameObject* theObject)
{
	if (theButtonId > 19)
		return;

	mObjectButtons[theButtonId] = new FishButtonWidget(theObject, theButtonId, this);

	if (theButtonId < 5)
		mObjectButtons[theButtonId]->Resize(m0x10c, 41 + theButtonId*83, 90, 83);
	else if(theButtonId < 10)
		mObjectButtons[theButtonId]->Resize(m0x110 + m0x10c, theButtonId * 83 - 374, 90, 83);
	else if(theButtonId < 15)
		mObjectButtons[theButtonId]->Resize(m0x110*4 + m0x10c + 24, theButtonId * 83 - 789, 90, 83);
	else if(theButtonId < 20)
		mObjectButtons[theButtonId]->Resize(m0x110*5 + m0x10c + 24, theButtonId * 83 -1204, 90, 83);

	ButtonHoleHelper(mBackgroundImage, (MemoryImage*)IMAGE_PETBUTTONHOLE, mObjectButtons[theButtonId]->mX, mObjectButtons[theButtonId]->mY);
}

void Sexy::SimFishScreen::DetermineShowHideForButtons()
{
	bool anyHidden = false;
	for (int i = 0; i < 20; i++)
	{
		FishButtonWidget* aBtn = mObjectButtons[i];
		if (aBtn != nullptr && aBtn->mObject != nullptr && aBtn->mObject->mShown)
		{
			anyHidden = true;
			break;
		}
	}

	if (anyHidden)
	{
		mHideAllButton->mLabel = "Hide All";
		mHideAllButton->mId = 105;
	}
	else
	{
		mHideAllButton->mLabel = "Show All";
		mHideAllButton->mId = 106;
	}

	mHideShowButton->mLabel = "Hide";
	if (mSelectedFishButton != nullptr && mSelectedFishButton->mObject != nullptr && !mSelectedFishButton->mObject->mShown)
		mHideShowButton->mLabel = "Show";
}

void Sexy::SimFishScreen::RenameSelectedFish(SexyString theName)
{
	if (mSelectedFishButton != nullptr && mSelectedFishButton->mObject != nullptr)
		mSelectedFishButton->mObject->mName = theName;
}

void Sexy::SimFishScreen::SellSelectedObject(bool sell)
{
	m0x119 = false;
	m0x118 = false;
	m0x114 = 1;
	if (sell && mSelectedFishButton != nullptr && mSelectedFishButton->mObject != nullptr)
	{
		m0x11a = true;
		int aVal = mSelectedFishButton->mObject->GetShellPrice();
		if (aVal <= 0)
			aVal = mSelectedFishButton->mObject->mShellPrice;
		mApp->mCurrentProfile->AddShells(aVal);
		mSelectedFishButton->mObject->RemoveHelper02(false);
		GameObject* anObj = nullptr;
		if (mSelectedFishButton->mObject->mType == GameObject::TYPE_BREEDER)
		{
			anObj = mApp->mBoard->GetGameObjectByVirtualId(100 + mSelectedFishButton->mObject->mVirtualTankId);
			if (anObj)
				anObj->mVirtualTankId = mSelectedFishButton->mObject->mVirtualTankId;
		}

		mApp->SafeDeleteWidget(mSelectedFishButton->mObject);
		mSelectedFishButton->mObject = anObj;
		mSelectedFishButton->SetDisabled(anObj == nullptr);
		DetermineShowHideForButtons();
	}
}

void Sexy::SimFishScreenOverlay::Draw(Graphics* g)
{
	mScreen->DrawOverlay(g);
}
