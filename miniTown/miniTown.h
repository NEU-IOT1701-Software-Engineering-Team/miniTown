//
//
//  miniTown
//
//
//=============================================================================


#pragma once
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<iostream>
#include<time.h>
#include<tchar.h>
#include"Debug.h"
#include"Engine.h"



//----------------------------------
//调试使用的全局变量
const bool DebugShowGrowTimeFlag = false;  //输出田和树木的生长、砍伐时间

//-----------------------------------


//======================================================
// 游戏引擎部分

//用于显示窗体的全局变量
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int bits = 24;
//extern BYTE buffer[SCREEN_WIDTH * SCREEN_HEIGHT * bits / 8];
extern int BackgroundMusicVolume;
extern int SoundVolume;
const int SoundOn = true;


class coord //以60*60为单位标记坐标
{
public:
	coord(int X, int Y) :x(X), y(Y) {};
	coord() :x(0), y(0) {};
	int x, y;
};


void Draw();
const int MaxDrawObjectSum = 100000;
//extern Object* drawList[MaxDrawObjectSum];
extern int drawSum;
extern Player player;

//void AddDrawObject(Object* object);
//void RemoveDrawObecjt(Object* object);

void KeyControl();

extern float runtime;
extern int LastFPS;
extern int frame; //当前的帧率
extern float timeScale;


const int MaxHouseSum = 1000;
const int MaxFieldSum = 1000;
const int MaxFarmerSum = 1000;
const int MaxBuilderSum = 1000;
const int MaxChildSum = 1000;
const int MaxTreeSum = 1000;
const int MaxRiceSum = 1000;
const int MaxWoodSum = 1000;
const int MaxObjectSum = 1000;


extern int FieldProduceRiceSum;
extern int RicePrice;
extern int HousePrice;
extern int FirstPayHousePrice; //盖房子预先给木匠的定金
extern int MaxWantFoodLevel;
const float GrowRiceTime = 25;
const float AgeAddEveryDay = 1;
const float GrownUpAge = 4; //小孩到成年的年龄



extern float DayTimeNowRate; //今天的进度(0~1)
extern float DayTimeNow;
extern float FrameTime; //当前帧间隔时间
extern int DaySum; 

class Field
{
public:
	int id;
	bool isUsed = false;
	float growingTime;
	Object *DrawObject;
	void AddRice();
	void AI();
};

class Rice
{
public:
	int id;
	Object* DrawObject;
};

class House
{
public:
	int id;
	float buildTime=0;
	const int RequireBuildTime = 30;
	int FirstBuildMoney = 0; //盖房子预付给木匠的钱
	bool isKingHouse = false;
	bool isUsed = false;
	bool isOnBuild = false;
	Object *DrawObject;
	Object* StoneRice[MaxRiceSum];
	Object* StoneWood[MaxWoodSum];
	int StoneRiceSum;
	int StoneWoodSum;

};

class Tree
{
public:
	int id;
	Object* DrawObject;
	float cutTime = 0;
	void AddWood();
};

class FamilyTree;

class Farmer
{
public:
	int id;
	int Sex; //男的为1 女的为0
	float age;
	int money;
	Field* belongField;
	House* belongHouse;
	House* ownHouse = NULL; //给后代的房子
	FamilyTree* familyTree = NULL;
	bool isCanMarriage = false;
	bool isMarriage = false;
	Object* DrawObject;
	int wantFoodLevel;
	float wantSexLevel;
	bool isDead = false;
	bool isTryBuyHouse = false;
	Object* TakeOnThing[MaxObjectSum];
	int TakeOnThingSum = 0;
	void WalkTo(Object* object);
	void Eat();
	void Sleep();
	void GrowRice();
	void AI();
	void PutRice(House* house);
	bool GetARiceToHand();  //从房子里拿出一个水稻到手上
	bool GetAllRiceToHand(); //从房子里拿出所有水稻到手上

	bool BuyRice();
	bool BuyHouse();
	void judgeDead();

	bool SellRiceForMoney();

	int LastDaySum=0; //用来计算食欲的临时变量
	//买水稻用的临时变量
	bool isTryFeedChild = false;
	bool isTryBuyRice = false;
	bool isBuyRiceFinish = false;
	bool isTrySellRice = false;
	
};

class Builder
{
public:
	int id;
	float age;
	int money;
	int Sex; //男的为1 女的为0
	House* belongHouse;
	Tree* AimTree=NULL;
	House* AimUnFinishHouse = NULL;
	House* ownHouse=NULL; //给后代的房子
	FamilyTree* familyTree = NULL;
	bool isCanMarriage = false;
	bool isMarriage = false;

	
	int OwnHouseCount = 0;   //拥有的还没卖出去的房子数量
	int wantFoodLevel;
	float wantSexLevel;
	int buildTime = 0;
	bool isDead = false;
	Object* DrawObject;
	Object* TakeOnThing[MaxObjectSum];
	int TakeOnThingSum = 0;
	bool GetARiceToHand();
	bool GetAllRiceToHand();
	void WalkTo(Object* object);
	void BuildHouse();
	void Eat();
	void Sleep();
	void CutTree();
	Tree* FindATree();
	House* FindAUnFinishHouse();
	void AI();
	void PutWood();
	bool BuyRice();
	bool BuyHouse();
	bool PutRice(House* house);
	void judgeDead();
	

	bool HouseForMoney();
	int LastDaySum = 0; //用来计算食欲的临时变量
	//买水稻用的临时变量
	bool isTryBuyRice = false;
	bool isBuyRiceFinish = false;
	//买房子用的临时变量
	bool isTryBuyHouse = false;
	bool isTryFeedChild = false;
};

class King
{
public:
	int id;
	float age;
	int Sex; //男的为1 女的为-1
	int money;
	House* ownHouse = NULL; //给后代的房子
	House* belongHouse;
	Object* DrawObject;
	int wantFoodLevel;
	float wantSexLevel;
	Object* TakeOnThing;
	int HaveEmptyHouseSum=0;
	void WalkTo(Object* object);
	
	void AI();
	void MakeMoney(int Sum);
	void DestoryMoney(int Sum);
	void SetRicePrice(int Price);
	void SetHousePrice(int Price);
	void SetATree(); //在村长位置种一棵树
	void SetAField(); //在村长位置弄一块田
	bool SetUnFinishHouseMark();  //在村长当前位置设置一个盖房子的记号（木匠随后会取盖房子）需要村长花一部分钱

};

class Child
{
public:
	int id;
	float age;
	int Sex;
	bool isDead = false;
	House* belongHouse = NULL;
	FamilyTree* oldFamilyTree;
	Object* DrawObject;
	int wantFoodLevel;
	void Walk(); //随机在地图里散步
	void GrowUP(); //变成成人
	void Sleep();
	void Eat();
	void AI();
	void judgeDead();
	void WalkTo(Object* object);

	int LastDaySum = 0; //用来计算食欲的临时变量
	bool goToFlag = true; //用来判断是否到达散步目的地的变量
	int AimX = 0;
	int AimY = 0;
	Object AimObject;
};


class FamilyTree
{
public:
	int FatherType;  //父亲的职业类型
	int MotherType;  //母亲的职业类型
	Builder *Father0;
	Farmer *Father1;
	King *Father2;
	Builder *Mother0;
	Farmer *Mother1;
	King *Mother2;
	int ChildType;
	Child* child0;
	Builder* child1;
	Farmer* child2;
};

const int DayTime = 30;

extern Picture picLand;
extern Picture picHouse;
extern Picture picHouse1;
extern Picture picFarmer;
extern Picture picBuilder;
extern Picture picField;
extern Picture picField1;
extern Picture picField2;
extern Picture picTree;
extern Picture picKing;
extern Picture picRice;
extern Picture picWood;
extern Picture picSun;
extern Picture picBackground;
extern Picture picBackground1;
extern Picture picMoon;
extern Picture picKingHouse;
extern Picture picLove;
extern Picture picChild;


extern int NowHouseSum;
extern int NowFieldSum;
extern int NowTreeSum;
extern int NowRiceSum;
extern int NowWoodSum;
extern int NowFarmerSum;
extern int NowBuilderSum;
extern int NowChildSum;

extern Object objHouse[MaxHouseSum];
extern Object objFarmer[MaxFarmerSum];
extern Object objBuilder[MaxBuilderSum];
extern Object objChild[MaxChildSum]; 
extern Object objField[MaxFieldSum];
extern Object objRice[MaxRiceSum];
extern Object objTree[MaxTreeSum];
extern Object objWood[MaxWoodSum];
extern Object objKing;
extern Object objSun;
extern Object objMoon;
extern Object objBackground;
extern Object objLove;

extern Field field[MaxFieldSum];
extern Tree tree[MaxTreeSum];
extern House house[MaxHouseSum];
extern Farmer farmer[MaxFarmerSum];
extern Builder builder[MaxBuilderSum];
extern Child child[MaxChildSum];
extern King king;


//--------------------------------
// AI
void AILoop();



//--------------------------------
//游戏机制

extern bool clkClick;
void AddUnFinishHouse(int x, int y);
void AddFinishHouse(int x, int y,int type=0);
void AddFarmer(int x,int y,int sex);
void AddBuilder(int x,int y,int sex);
void AddChild( int sex,FamilyTree *familyTree);
void AddTree(int x,int y);
void AddField(int x, int y);
void AddKing(int x, int y,int sex=0);
bool ObjectIsRice(Object* object);
bool ObjectIsWood(Object* object);
void MakeBaby(FamilyTree* familyTree);


void ShowSky();


void ResourceCount();//对村民拥有的资源进行统计

bool IsCloseTo(Object* A, Object* B);  //判断两个Object是否靠近
bool IsMoreCloseTo(Object* A, Object* B); //判断两个Object是否更靠近
float DistanceAToB(Object* A, Object* B);
House* GetANearEmptyHouse(Object* man,int type=0); 
Field* GetANearUnUsedField(Object* man);
House* FindKingHouse();
coord GetCoord(Object* object); //获取最接近的坐标
bool IsCoordUsed(coord point);