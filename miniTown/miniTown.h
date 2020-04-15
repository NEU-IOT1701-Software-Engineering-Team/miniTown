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
//����ʹ�õ�ȫ�ֱ���
const bool DebugShowGrowTimeFlag = false;  //��������ľ������������ʱ��

//-----------------------------------


//======================================================
// ��Ϸ���沿��

//������ʾ�����ȫ�ֱ���
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int bits = 24;
//extern BYTE buffer[SCREEN_WIDTH * SCREEN_HEIGHT * bits / 8];
extern int BackgroundMusicVolume;
extern int SoundVolume;
const int SoundOn = true;


class coord //��60*60Ϊ��λ�������
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
extern int frame; //��ǰ��֡��
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
extern int FirstPayHousePrice; //�Ƿ���Ԥ�ȸ�ľ���Ķ���
extern int MaxWantFoodLevel;
const float GrowRiceTime = 25;
const float AgeAddEveryDay = 1;
const float GrownUpAge = 4; //С�������������



extern float DayTimeNowRate; //����Ľ���(0~1)
extern float DayTimeNow;
extern float FrameTime; //��ǰ֡���ʱ��
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
	int FirstBuildMoney = 0; //�Ƿ���Ԥ����ľ����Ǯ
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
	int Sex; //�е�Ϊ1 Ů��Ϊ0
	float age;
	int money;
	Field* belongField;
	House* belongHouse;
	House* ownHouse = NULL; //������ķ���
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
	bool GetARiceToHand();  //�ӷ������ó�һ��ˮ��������
	bool GetAllRiceToHand(); //�ӷ������ó�����ˮ��������

	bool BuyRice();
	bool BuyHouse();
	void judgeDead();

	bool SellRiceForMoney();

	int LastDaySum=0; //��������ʳ������ʱ����
	//��ˮ���õ���ʱ����
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
	int Sex; //�е�Ϊ1 Ů��Ϊ0
	House* belongHouse;
	Tree* AimTree=NULL;
	House* AimUnFinishHouse = NULL;
	House* ownHouse=NULL; //������ķ���
	FamilyTree* familyTree = NULL;
	bool isCanMarriage = false;
	bool isMarriage = false;

	
	int OwnHouseCount = 0;   //ӵ�еĻ�û����ȥ�ķ�������
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
	int LastDaySum = 0; //��������ʳ������ʱ����
	//��ˮ���õ���ʱ����
	bool isTryBuyRice = false;
	bool isBuyRiceFinish = false;
	//�����õ���ʱ����
	bool isTryBuyHouse = false;
	bool isTryFeedChild = false;
};

class King
{
public:
	int id;
	float age;
	int Sex; //�е�Ϊ1 Ů��Ϊ-1
	int money;
	House* ownHouse = NULL; //������ķ���
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
	void SetATree(); //�ڴ峤λ����һ����
	void SetAField(); //�ڴ峤λ��Ūһ����
	bool SetUnFinishHouseMark();  //�ڴ峤��ǰλ������һ���Ƿ��ӵļǺţ�ľ������ȡ�Ƿ��ӣ���Ҫ�峤��һ����Ǯ

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
	void Walk(); //����ڵ�ͼ��ɢ��
	void GrowUP(); //��ɳ���
	void Sleep();
	void Eat();
	void AI();
	void judgeDead();
	void WalkTo(Object* object);

	int LastDaySum = 0; //��������ʳ������ʱ����
	bool goToFlag = true; //�����ж��Ƿ񵽴�ɢ��Ŀ�ĵصı���
	int AimX = 0;
	int AimY = 0;
	Object AimObject;
};


class FamilyTree
{
public:
	int FatherType;  //���׵�ְҵ����
	int MotherType;  //ĸ�׵�ְҵ����
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
//��Ϸ����

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


void ResourceCount();//�Դ���ӵ�е���Դ����ͳ��

bool IsCloseTo(Object* A, Object* B);  //�ж�����Object�Ƿ񿿽�
bool IsMoreCloseTo(Object* A, Object* B); //�ж�����Object�Ƿ������
float DistanceAToB(Object* A, Object* B);
House* GetANearEmptyHouse(Object* man,int type=0); 
Field* GetANearUnUsedField(Object* man);
House* FindKingHouse();
coord GetCoord(Object* object); //��ȡ��ӽ�������
bool IsCoordUsed(coord point);