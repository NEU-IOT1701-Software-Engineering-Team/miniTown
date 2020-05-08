#include"miniTown.h"
using namespace std;

bool clkClick = false; //ÿ��һ�������һ��Ϊtrue

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
float DayTimeNow = 0; //����Ľ���(0~DayTime)
float DayTimeNowRate = 0; //����Ľ���(0~1)
int DaySum = 0; //��������������
float timeScale = 2;
int LastFPS = 0;
float FrameTime = 0; //��ǰ֡���ʱ��
int frame = 0; //֡��

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

	AddFinishHouse(7, 2,1);
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





Button b;
Button b2;
Button b3;
Button b4;
Button b5;
Button b6;
Button b7;
Button b8;
Button b9;
Button b10;
Label lb3;
Label ResourceCountLabel;
Label HintLabel;

EditBox ricePriceEditBox;
EditBox housePriceEditBox;
EditBox FirstPayHousePriceEditBox;
EditBox MoneyEditBox;



void MakeMoney()
{
	int price;
	price = atoi(MoneyEditBox.getText());
	king.MakeMoney(price);
}

void SetRicePrice()
{

	int price;
	price =atoi(ricePriceEditBox.getText());
	king.SetRicePrice(price);
}

void SetHousePrice()
{
	int price;

	price = atoi(housePriceEditBox.getText());
	king.SetHousePrice(price);
}

void DestoryMoney()
{
	int price;
	price = atoi(MoneyEditBox.getText());
	king.DestoryMoney(price);
}

void Eat()
{
	king.Eat();
}

void SetUnFinishHouseMark()
{
	king.SetUnFinishHouseMark();
}


void SetATree()
{
	king.SetATree();
}


void SetAField()
{
	king.SetAField();
}

void GetResourceCount()
{
	string str;
	ResourceCount(str);
	//char* cstr = new char(str.size()+1);
	//char* cstr = new char[str.size() + 1];
	//memset(cstr, 0, str.size()+1);
	//str.copy(cstr, str.size()+1, 0);
	
	ResourceCountLabel.setTitle(str.c_str());
	//ResourceCountLabel.setTitle(str.c_str());//�Ƽ�ʹ������д��

	//ResourceCountLabel.title=(char*)"ResourceCount\n";
}


void hint()
{


	static int hintIndex = 0;
	const int hintSum = 6;
	string hint[hintSum] = { "�峤ҲҪ�ؼҳԷ�������峤(king)��wFood����5���峤�Ͷ����ˣ���Ϸ����",
		"w/s/a/d ���ƴ峤�ƶ����ұߵİ�ť���߿�ݼ������и��߼��Ĳ���",
		"builder��Ψһ������Դ�ǸǷ��ӣ�����Ϊ�˲������Ƕ�����Ӧ�ò������Ŵ���,��h����Ҫ�ĵط��Ƿ���",
		"ʳɫ��Ҳ -�����ӡ������ϡ�\n ÿ�������������������Է��ͽ�����ӣ������ж�����𣬴����Ϊ������������ȥ����",
		"�������������˷��ӣ�����Ů���ܹ�����С����Ǯ��ʱ�����ǻ�����С��",
		"��Ϸ���ڿ������У������Ϸ��ʱû��Ŀ�꣬������һ��ɳ����Ϸ" };

	HintLabel.setTitle(hint[hintIndex].c_str());
	hintIndex++;
	if (hintIndex >= hintSum)
	{
		hintIndex = 0;
	}
}

void AddUI() {
	int ButtonWidth = 100;
	int ButtonHeight = 50;

	ResourceCountLabel.setRect(GAME_SHOW_WIDTH, 0, ButtonWidth*7, ButtonHeight*8);
	ResourceCountLabel.setForegroundColor({ 0,255,0 });
	ResourceCountLabel.setBackgroundColor(COLOR_BLACK);
	AddLabel(&ResourceCountLabel);

	b6.title = (char*)"DestoryMoney";
	b6.setRect(GAME_SHOW_WIDTH, ButtonHeight * 8, ButtonWidth, ButtonHeight);
	b6.setForegroundColor({ 255,0,0 });
	b6.setBackgroundColor(COLOR_BLACK);
	b6.lpClickL = DestoryMoney;
	AddButton(&b6);

	b2.title = (char*)"AddMoney";
	b2.setRect(GAME_SHOW_WIDTH+ButtonWidth, ButtonHeight*8, ButtonWidth, ButtonHeight);
	b2.setForegroundColor({ 255,0,0 });
	b2.setBackgroundColor(COLOR_BLACK);
	b2.lpClickL = MakeMoney;
	AddButton(&b2);

	MoneyEditBox.setRect(GAME_SHOW_WIDTH + ButtonWidth * 2, ButtonHeight *8, ButtonWidth, ButtonHeight);
	MoneyEditBox.setText("0");
	AddEditBox(&MoneyEditBox);

	b4.title = (char*)"SetRicePrice";
	b4.setRect(GAME_SHOW_WIDTH +ButtonWidth*3, ButtonHeight * 8, ButtonWidth, ButtonHeight);
	b4.setForegroundColor({ 255,0,0 });
	b4.setBackgroundColor(COLOR_BLACK);
	b4.lpClickL = SetRicePrice;
	AddButton(&b4);

	ricePriceEditBox.setRect(GAME_SHOW_WIDTH+ButtonWidth*4, ButtonHeight * 8, ButtonWidth, ButtonHeight);
	char strRicePrice[20];
	_itoa(RicePrice, strRicePrice, 10);
	ricePriceEditBox.setText(strRicePrice);
	AddEditBox(&ricePriceEditBox);

	b5.title = (char*)"SetHousePrice";
	b5.setRect(GAME_SHOW_WIDTH+ButtonWidth*5, ButtonHeight * 8, ButtonWidth, ButtonHeight);
	b5.setForegroundColor({ 255,0,0 });
	b5.setBackgroundColor(COLOR_BLACK);
	b5.lpClickL = SetHousePrice;
	AddButton(&b5);

	housePriceEditBox.setRect(GAME_SHOW_WIDTH + ButtonWidth*6, ButtonHeight * 8, ButtonWidth, ButtonHeight);
	char strHousePrice[20];
	_itoa(HousePrice, strHousePrice, 10);
	housePriceEditBox.setText(strHousePrice);
	AddEditBox(&housePriceEditBox);




	b7.title = (char*)"SetHouse[H]";
	b7.setRect(GAME_SHOW_WIDTH, ButtonHeight * 9, ButtonWidth, ButtonHeight);
	b7.setForegroundColor({ 255,0,0 });
	b7.setBackgroundColor(COLOR_BLACK);
	b7.lpClickL = SetUnFinishHouseMark;
	AddButton(&b7);

	b8.title = (char*)"SetTree[T]";
	b8.setRect(GAME_SHOW_WIDTH+ButtonWidth*1, ButtonHeight * 9, ButtonWidth, ButtonHeight);
	b8.setForegroundColor({ 255,0,0 });
	b8.setBackgroundColor(COLOR_BLACK);
	b8.lpClickL = SetATree;
	AddButton(&b8);

	b9.title = (char*)"SetField[F]";
	b9.setRect(GAME_SHOW_WIDTH+ButtonWidth*2, ButtonHeight * 9, ButtonWidth, ButtonHeight);
	b9.setForegroundColor({ 255,0,0 });
	b9.setBackgroundColor(COLOR_BLACK);
	b9.lpClickL = SetAField;
	AddButton(&b9);
	b3.title = (char*)"Eat[E]";
	b3.setRect(GAME_SHOW_WIDTH + ButtonWidth *3, ButtonHeight *9, ButtonWidth, ButtonHeight);
	b3.setForegroundColor({ 255,0,0 });
	b3.setBackgroundColor(COLOR_BLACK);
	b3.lpClickL = Eat;
	AddButton(&b3);

	b10.title = (char*)"��ʾ";
	b10.setRect(GAME_SHOW_WIDTH + ButtonWidth * 4, ButtonHeight * 9, ButtonWidth, ButtonHeight);
	b10.setForegroundColor({ 255,0,0 });
	b10.setBackgroundColor(COLOR_BLACK);
	b10.lpClickL = hint;
	AddButton(&b10);

	HintLabel.setRect(GAME_SHOW_WIDTH, ButtonHeight * 10, ButtonWidth*7, ButtonHeight);
	HintLabel.setForegroundColor({ 255,255,255 });
	HintLabel.setBackgroundColor(COLOR_BLACK);
	AddLabel(&HintLabel);
}

Player player;
Musci MusciBackground("sound/01 Bloom.mp3", BackgroundMusicVolume);
Musci MusciGetRice1("sound/getRice.mp3", SoundVolume);
//Musci MusciGetRice2("sound/getRice.mp3", SoundVolume);
//Musci MusciGetRice3("sound/getRice.mp3", SoundVolume);
Musci MusciGetWood1("sound/getWood.mp3", SoundVolume);
//Musci MusciGetWood2("sound/getWood.mp3", SoundVolume);
//Musci MusciGetWood3("sound/getWood.mp3", SoundVolume);
void LoadMusic() {
	MusciBackground.isLoop = true;
	player.addMusci(&MusciBackground);
	player.addMusci(&MusciGetRice1);
	player.addMusci(&MusciGetWood1);
}


//���� �뱣��------��------
/*
EditBox editBox;
Label lb2;
Picture picTest;
void EBChange() {
	
}
*/
//���� �뱣��------��------
int main()
{
	//���� �뱣��------��------
	/*
	lb2.title = (char*)"id\tsex\n0\tman";
	lb2.setRect(400, 400, 80, 40);
	lb2.setForegroundColor({ 0,255,0 });
	lb2.setBackgroundColor(COLOR_BLACK);
	//b2.lpClickL = func;
	AddLabel(&lb2);


	editBox.setRect(300, 300, 100, 25);
	editBox.setText((char*)"0");
	editBox.lpValueChange = EBChange;
	AddEditBox(&editBox);

	
	picTest.loadImage("pic\\test3.bmp");
	*/

	//����  �뱣��------��------

	LoadMusic();

	if (SoundOn)
	{
		//player.playSoundLoop("sound/01 Bloom.mp3", BackgroundMusicVolume);
		MusciBackground.play();
	}

	AddUI();

	if (_CreateWindow(WindowTitle, nScreenWidth, nScreenHeight))
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

	hint();
	while (1)
	{

		KeyControl();
		//MsgGet();���뵽��Draw()
		Draw();
		

		AILoop();
		ShowSky();
		
		frame++;
		string str;

		stop = clock();
		FrameTime = (stop - start) * 1.0 / CLOCKS_PER_SEC * timeScale;
		if (FrameTime > 0.1) //��ֹ��ͣ
		{
			FrameTime = 0;
		}
		runtime += FrameTime;
		DayTimeNow+= FrameTime;

		if (DayTimeNow > DayTime)
		{
			DayTimeNow = 0;
			cout << "Day Out!" << endl;
			string str;
			ResourceCount(str);
		}
		if (frame % 10 == 0)
		{
			ResourceCount(str);
			ResourceCountLabel.title = str;

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

		if (king.isDead == true)
		{
			cout << "King Dead! Game Over!" << endl;
			return 0;
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
	static int pressT = false;
	static int pressF = false;
	static int pressE = false;
	int speed = 40; //�峤���ƶ��ٶ�
	if (king.isDead == false)
	{
		if (screen_keys[VK_ESCAPE])
		{
			//�������������ģ����ͷź���
			freeSomethingForEngine();
			exit(1);		//��������
		}
		if (screen_keys['W'])
		{
			if (king.DrawObject->y + king.DrawObject->pic->getHeight() - 10 > 60)
			{
				king.DrawObject->y -= (float)timeScale * speed * FrameTime;
				king.isMove = true;
			}

		}
		if (screen_keys['S'])
		{
			if (king.DrawObject->y + king.DrawObject->pic->getHeight()+10 < GAME_SHOW_HEIGHT)
			{
				king.DrawObject->y += (float)timeScale * speed * FrameTime;
				king.isMove = true;
			}
		}
		if (screen_keys['A'])
		{
			if (king.DrawObject->x > 0)
			{
				king.DrawObject->x -= (float)timeScale * speed * FrameTime;
				king.isMove = true;
			}

		}
		if (screen_keys['D'])
		{
			if (king.DrawObject->x + king.DrawObject->pic->getWidth() < GAME_SHOW_WIDTH)
			{
				king.DrawObject->x += (float)timeScale * speed * FrameTime;
				king.isMove = true;
			}
		}
		if (screen_keys['Q']) {
			int angle = king.DrawObject->getAngle();
			king.DrawObject->setAngle(angle + 5);

		}
		if (screen_keys['E']) {
			if (pressE == false)
			{
				king.Eat();
			}
			pressE = true;

		}
		else
		{
			pressE = false;
		}

		if (screen_keys['H'])
		{
			if (pressH == false)
			{
				king.SetUnFinishHouseMark();
			}
			pressH = true;
		}
		else
		{
			pressH = false;
		}

		if (screen_keys['T'])
		{
			if (pressT == false)
			{
				king.SetATree();
			}
			pressT = true;
		}
		else
		{
			pressT = false;
		}
		if (screen_keys['F'])
		{
			if (pressF == false)
			{
				king.SetAField();
			}
			pressF = true;
		}
		else
		{
			pressF = false;
		}
	}
	
}