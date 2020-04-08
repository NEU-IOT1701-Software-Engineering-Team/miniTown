#include"miniTown.h"
using namespace std;

bool clkClick = false; //每过一整秒就设一下为true

Picture picLand;
Picture picHouse;
Picture picHouse1;
Picture picFarmer;
Picture picBuilder;
Picture picField;
Picture picField1;
Picture picField2;
Picture picTree;
Picture picKing;
Picture picRice;
Picture picWood;
Picture picSun;
Picture picBackground;
Picture picBackground1;
Picture picMoon;
Picture picKingHouse;
Picture picLove;
Picture picChild;


int NowHouseSum = 0;
int NowFieldSum = 0;
int NowTreeSum = 0;
int NowRiceSum = 0;
int NowWoodSum = 0;
int NowFarmerSum = 0;
int NowBuilderSum = 0;
int NowChildSum = 0;
Object objHouse[MaxHouseSum];
Object objFarmer[MaxFarmerSum];
Object objBuilder[MaxBuilderSum];
Object objChild[MaxChildSum];
Object objField[MaxFieldSum];
Object objRice[MaxRiceSum];
Object objTree[MaxTreeSum];
Object objWood[MaxWoodSum];
Object objBackground;

Object objKing;

Object objSun;
Object objMoon;
Object objLove;

Farmer farmer[MaxFarmerSum];
Builder builder[MaxBuilderSum];
Child child[MaxChildSum];
King king;
Field field[MaxFieldSum];
Tree tree[MaxTreeSum];
House house[MaxHouseSum];



float runtime = 0;
float DayTimeNow = 0; //今天的进度(0~DayTime)
float DayTimeNowRate = 0; //今天的进度(0~1)
int DaySum = 0; //经过的日子数量
float timeScale = 1;
int LastFPS = 0;
float FrameTime = 0; //当前帧间隔时间
int frame = 0; //帧率

int BackgroundMusicVolume=300;
int SoundVolume=600;

void initLoadPic()
{
	picLand.loadImage("pic/land.bmp");
	picHouse.loadImage("pic/house.bmp");
	picHouse1.loadImage("pic/house1.bmp");
	picFarmer.loadImage("pic/farmer.bmp");
	picBuilder.loadImage("pic/builder.bmp");
	picField.loadImage("pic/field.bmp");
	picTree.loadImage("pic/tree.bmp");
	picKing.loadImage("pic/king.bmp");
	picField1.loadImage("pic/field1.bmp");
	picField2.loadImage("pic/field2.bmp");
	picRice.loadImage("pic/rice.bmp");
	picWood.loadImage("pic/wood.bmp");
	picSun.loadImage("pic/sun.bmp");
	picBackground.loadImage("pic/background.bmp");
	picBackground1.loadImage("pic/background1.bmp");
	picMoon.loadImage("pic/moon.bmp");
	picKingHouse.loadImage("pic/kinghouse.bmp");
	picLove.loadImage("pic/love.bmp");
	picChild.loadImage("pic/child.bmp");

	cout << "Load Pic OK!" << endl;
}

void initObject()
{
	srand(time(NULL));
	for (int i = 1; i < 4; i++)
	{
		AddField(1, i);
		AddFinishHouse(2, i);
		AddFarmer(2, i,1);
		
	}

	for (int i = 1; i < 3; i++)
	{
		AddTree(3, i);
		AddFinishHouse(4, i);
		AddBuilder(4, i,0);
		AddUnFinishHouse(5, i);
	}

	AddTree(-1, 5);

	AddFinishHouse(8, 2,1);
	AddKing(8, 2);
	king.money = 100;


	objBackground.pic = &picBackground;
	objBackground.point.x = 0;
	objBackground.point.y = 0;
	AddDrawObject(&objBackground);

	objSun.pic = &picSun;
	objSun.x = 0;
	objSun.y = 0;
	AddDrawObject(&objSun);

	objMoon.pic = &picMoon;
	objMoon.x = -objMoon.pic->getWidth();
	objMoon.y = 0;
	AddDrawObject(&objMoon);


	objLove.pic = &picLove;
	objLove.x = 0;
	objLove.y = 0;

}

int main()
{
	player.playSoundLoop("sound/01 Bloom.mp3", BackgroundMusicVolume);

	if (_CreateWindow(title, nScreenWidth, nScreenHeight))
	{
		return -1;
	}
	
	cout << "miniTown" << endl;

	initLoadPic();
	initObject();

	clock_t start, stop;
	clock_t OneSecondTime = clock();

	start = clock();
	
	//cout << "Time scale?";

	//cin >> timeScale;
	timeScale = 2;
	
	while (1)
	{

		KeyControl();
		//MsgGet();加入到了Draw()
		Draw();
		

		AILoop();
		ShowSky();
		
		frame++;
		stop = clock();
		FrameTime = (stop - start) * 1.0 / CLOCKS_PER_SEC * timeScale;
		if (FrameTime > 0.1) //防止暂停
		{
			FrameTime = 0;
		}
		runtime += FrameTime;
		DayTimeNow+= FrameTime;

		if (DayTimeNow > DayTime)
		{
			DayTimeNow = 0;
			cout << "Day Out!" << endl;
		}
		DayTimeNowRate = DayTimeNow / (float)DayTime;
		start = stop;
		if (stop - OneSecondTime >= CLOCKS_PER_SEC)
		{
			OneSecondTime = stop;
			clkClick = true;

			DaySum = (int)runtime / DayTime;
			//cout << DaySum << endl;
			char title[200];
			sprintf_s(title, "FPS %d ", frame);
			SetWindowText(hWnd, title);
			LastFPS = frame;
			frame = 0;
			//cout << "run time:" << runtime << endl;
		}
		else
		{
			clkClick = false;
		}

	}

	return 0;
}




void KeyControl()
{
	static int pressL = false;
	static int pressR = false;
	static int pressH = false;
	static int pressM = false;
	static int pressN = false;
	static int pressB = false;
	int speed = 40; //村长的移动速度
	if (screen_keys[VK_ESCAPE])
	{
		//在这里调用其他模块的释放函数
		freeSomethingForEngine();
		exit(1);		//正常结束
	}
	 if (screen_keys['W'])
	{
		king.DrawObject->y-= (float)timeScale * speed * FrameTime;
	}
	if (screen_keys['S'])
	{
		king.DrawObject->y+= (float)timeScale * speed * FrameTime;
	}
	if (screen_keys['A'])
	{
		king.DrawObject->x-= (float)timeScale * speed * FrameTime;
	}
	if (screen_keys['D'])
	{
		king.DrawObject->x+= (float)timeScale * speed * FrameTime;
	}
	if (screen_keys['Q']) {
		int angle = king.DrawObject->getAngle();
		king.DrawObject->setAngle(angle + 5);

	}
	if (screen_keys['E']) {
		int angle = king.DrawObject->getAngle();
		king.DrawObject->setAngle(angle - 5);

	}
	if (screen_keys['L'])  //list 显示当前村民资源
	{
		if (pressL == false)
		{
			ResourceCount();
		}
		pressL = true;
	}
	else
	{
		pressL = false;
	}
	if (screen_keys['R'])
	{
		if (pressR == false)
		{
			cout << "Enter Rice Price:";
			int price;
			cin >> price;
			king.SetRicePrice(price);
		}
		pressR = true;
	}
	else
	{
		pressR = false;
	}
	if (screen_keys['H'])
	{
		if (pressH == false)
		{
			cout << "Enter House Price:";
			int price;
			cin >> price;
			king.SetHousePrice(price);
		}
		pressH = true;
	}
	else
	{
		pressH = false;
	}
	if (screen_keys['M'])
	{
		if (pressM == false)
		{
			cout << "Enter Sum of the Money Add:";
			int sum;
			cin >> sum;
			king.MakeMoney(sum);
		}
		pressM = true;
	}
	else
	{
		pressM = false;
	}
	if (screen_keys['N'])
	{
		if (pressN == false)
		{
			cout << "Enter Sum of the Money Destory:";
			int sum;
			cin >> sum;
			king.DestoryMoney(sum);
		}
		pressN = true;
	}
	else
	{
		pressN = false;
	}
	if (screen_keys['B'])
	{
		if (pressB == false)
		{
			king.SetUnFinishHouseMark();
		}
		pressB = true;
	}
	else
	{
		pressB = false;
	}

}