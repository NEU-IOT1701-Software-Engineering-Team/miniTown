#include"miniTown.h"
using namespace std;



bool IsMoreCloseTo(Object* A, Object* B)
{
	if (A->x + A->pic->getWidth()/2 > B->x)
	{
		if (A->x< B->x + B->pic->getWidth()/2)
		{
			if (A->y + A->pic->getHeight()/2 > B->y)
			{
				if (A->y < B->y + B->pic->getHeight()/2)
				{
					return true;
				}
			}
		}
	}
	return false;
}

//判断两个object是否接触
bool IsCloseTo(Object* A, Object* B)
{
	if (A->x + A->pic->getWidth() > B->x)
	{
		if (A->x < B->x + B->pic->getWidth())
		{
			if (A->y + A->pic->getHeight() > B->y)
			{
				if (A->y < B->y + B->pic->getHeight())
				{
					return true;
				}
			}
		}
	}
	return false;
}

//判断该物品是不是rice
bool ObjectIsRice(Object* object)
{
	if (object == NULL)
	{
		return false;
	}
	for (int i = 0; i <= NowRiceSum; i++)
	{
		if (object == &objRice[i])
			return true;
	}

	return false;

}

bool ObjectIsWood(Object* object)
{
	if (object == NULL)
	{
		return false;
	}
	for (int i = 0; i < NowWoodSum; i++)
	{
		if (object == &objWood[i])
		{
			return true;
		}
	}
	return false;
}


//计算两个Object的中心距离
float DistanceAToB(Object* A, Object* B)
{
	float distance;
	distance = sqrt((A->x+A->pic->getWidth()/2 - (B->x+B->pic->getWidth()/2)) * (A->x + A->pic->getWidth() / 2 - (B->x + B->pic->getWidth() / 2)) +
		(A->y+A->pic->getHeight()/2 - (B->y+B->pic->getHeight()/2)) * (A->y + A->pic->getHeight() / 2 - (B->y + B->pic->getHeight() / 2)));
	return distance;
}


//寻找离DrawObject最近的一个房子，找到了把它上锁
//type=0时是普通房子，1是村长房子
House* GetANearEmptyHouse(Object *man,int type)
{
	House* NearEmptyHouse = NULL;
	float MinDistance = 999999;
	for (int i = 0; i < NowHouseSum; i++)
	{
		if (house[i].isUsed == false)
		{
			if (((type == 0 && house[i].isKingHouse == false) || (type == 1 && house[i].isKingHouse == true))
				&&house[i].buildTime== HouseRequireBuildTime && IsTheHouseHasOwner(&house[i])==false)
			{
				int nowDistance = DistanceAToB(man, house[i].DrawObject);
				if (nowDistance < MinDistance)
				{
					MinDistance = nowDistance;
					NearEmptyHouse = &house[i];
				}
			}
		}
	}
	if (NearEmptyHouse != NULL)
	{
		NearEmptyHouse->isUsed = true;
	}
	return NearEmptyHouse;
}

//找到一个离DrawObject最近的田，找到了把它上锁
Field* GetANearUnUsedField(Object* man)
{
	Field* NearUnUsedField = NULL;
	float MinDistance = 999999;
	for (int i = 0; i < NowFieldSum; i++)
	{
		if (field[i].isUsed == false)
		{
			int nowDistance = DistanceAToB(man, field[i].DrawObject);
			if (nowDistance < MinDistance)
			{
				MinDistance = nowDistance;
				NearUnUsedField = &field[i];
			}
		}
	}
	if (NearUnUsedField != NULL)
	{
		NearUnUsedField->isUsed = true;
	}
	return NearUnUsedField;
}

void AddUnFinishHouse(int x, int y)
{
	house[NowHouseSum].buildTime = 0;
	house[NowHouseSum].DrawObject = &objHouse[NowHouseSum];
	house[NowHouseSum].DrawObject->pic = &picHouse1;
	house[NowHouseSum].id = NowHouseSum;
	house[NowHouseSum].DrawObject->x = x *  picHouse.getWidth();
	house[NowHouseSum].DrawObject->y = y * picHouse.getHeight();
	house[NowHouseSum].DrawObject->z = 1;
	house[NowHouseSum].FirstBuildMoney = FirstPayHousePrice;
	AddDrawObject(house[NowHouseSum].DrawObject);
	NowHouseSum++;
	
}


void AddFinishHouse(int x, int y,int type)
{
	house[NowHouseSum].buildTime = HouseRequireBuildTime;
	house[NowHouseSum].DrawObject = &objHouse[NowHouseSum];
	house[NowHouseSum].DrawObject->pic = &picHouse;
	house[NowHouseSum].id = NowHouseSum;
	house[NowHouseSum].DrawObject->x = x * picHouse.getWidth();
	house[NowHouseSum].DrawObject->y = y * picHouse.getHeight();
	house[NowHouseSum].DrawObject->z = 1;
	if (type == 1)
	{
		house[NowHouseSum].isKingHouse = true;
		house[NowHouseSum].DrawObject->pic = &picKingHouse;
	}
	
	AddDrawObject(house[NowHouseSum].DrawObject);
	NowHouseSum++;
}

void AddFarmer(int x,int y,int sex)
{
	objFarmer[NowFarmerSum].pic = &picFarmer;
	objFarmer[NowFarmerSum].x = x * objFarmer[NowFarmerSum].pic->getWidth();
	objFarmer[NowFarmerSum].y = y * objFarmer[NowFarmerSum].pic->getHeight();
	objFarmer[NowFarmerSum].z = 1;

	farmer[NowFarmerSum].DrawObject = &objFarmer[NowFarmerSum];
	farmer[NowFarmerSum].belongField = GetANearUnUsedField(farmer[NowFarmerSum].DrawObject);
	farmer[NowFarmerSum].belongHouse = GetANearEmptyHouse(farmer[NowFarmerSum].DrawObject);
	farmer[NowFarmerSum].age = 18;
	farmer[NowFarmerSum].money = 10;
	farmer[NowFarmerSum].wantFoodLevel = 1;
	farmer[NowFarmerSum].wantSexLevel = 0;
	farmer[NowFarmerSum].id = NowFarmerSum;
	farmer[NowFarmerSum].Sex = sex;
	

	AddDrawObject(farmer[NowFarmerSum].DrawObject);
	NowFarmerSum++;
}

void AddBuilder(int x, int y,int sex)
{
	objBuilder[NowBuilderSum].pic = &picBuilder;
	objBuilder[NowBuilderSum].x = x*objBuilder[NowBuilderSum].pic->getWidth();
	objBuilder[NowBuilderSum].y = y*objBuilder[NowBuilderSum].pic->getHeight();
	objBuilder[NowBuilderSum].z = 1;

	builder[NowBuilderSum].DrawObject = &objBuilder[NowBuilderSum];
	builder[NowBuilderSum].belongHouse = GetANearEmptyHouse(builder[NowBuilderSum].DrawObject);
	builder[NowBuilderSum].age = 18;
	builder[NowBuilderSum].money = 10;
	builder[NowBuilderSum].wantFoodLevel = 1;
	builder[NowBuilderSum].wantSexLevel = 0;
	builder[NowBuilderSum].id = NowBuilderSum;
	builder[NowBuilderSum].Sex = sex;

	AddDrawObject(builder[NowBuilderSum].DrawObject);
	NowBuilderSum++;
}

void AddChild(int sex, FamilyTree* familyTree)
{

	
	objChild[NowChildSum].pic = &picChild;

	child[NowChildSum].DrawObject = &objChild[NowChildSum];
	if (familyTree->FatherType == 0)
	{
		child[NowChildSum].belongHouse = familyTree->Father0->ownHouseList[familyTree->NowFamilyTreeChildSum];
	}
	else if (familyTree->FatherType == 1)
	{
		child[NowChildSum].belongHouse = familyTree->Father1->ownHouseList[familyTree->NowFamilyTreeChildSum];
	}
	else if (familyTree->FatherType == 2)
	{
		child[NowChildSum].belongHouse = familyTree->Father2->ownHouse;
	}
	objChild[NowChildSum].x = child[NowChildSum].belongHouse->DrawObject->x;	
	objChild[NowChildSum].y = child[NowChildSum].belongHouse->DrawObject->y;
	objChild[NowChildSum].z = 1;
	child[NowChildSum].wantFoodLevel = 0;
	child[NowChildSum].id = NowChildSum;
	child[NowChildSum].Sex = sex;
	child[NowChildSum].oldFamilyTree = familyTree;
	
	
	familyTree->ChildTypeList[familyTree->NowFamilyTreeChildSum] = 0;
	familyTree->child0List[familyTree->NowFamilyTreeChild0Sum] = &child[NowChildSum];
	familyTree->NowFamilyTreeChildSum++;
	familyTree->NowFamilyTreeChild0Sum++;
	AddDrawObject(child[NowChildSum].DrawObject);
	NowChildSum++;
}

void AddKing(int x, int y,int sex)
{
	objKing.pic = &picKing;
	objKing.x = x * objKing.pic->getWidth();
	objKing.y = y * objKing.pic->getHeight();
	objKing.z = 1;
	king.Sex = sex;

	king.DrawObject = &objKing;
	king.belongHouse = GetANearEmptyHouse(king.DrawObject,1);
	king.belongHouse->isUsed = true;
	king.id = 0;
	king.age = 18;
	king.money = 0;
	king.wantFoodLevel = 1;
	king.wantSexLevel = 1;
	AddDrawObject(king.DrawObject);
}

void AddTree(int x, int y)
{
	objTree[NowTreeSum].pic = &picTree;
	objTree[NowTreeSum].x = x * picTree.getWidth();
	objTree[NowTreeSum].y = y * picTree.getHeight();
	objTree[NowTreeSum].z = 1;
	tree[NowTreeSum].cutTime = 0;
	tree[NowTreeSum].DrawObject = &objTree[NowTreeSum];
	tree[NowTreeSum].id = NowTreeSum ;
	

	AddDrawObject(&objTree[NowTreeSum]);
	NowTreeSum++;

}

void AddField(int x, int y)
{
	objField[NowFieldSum].pic = &picField;
	objField[NowFieldSum].x = x * picField.getWidth();
	objField[NowFieldSum].y = y * picField.getHeight();
	objField[NowFieldSum].z = -1;

	field[NowFieldSum].DrawObject = &objField[NowFieldSum];
	field[NowFieldSum].id = NowFieldSum ;
	field[NowFieldSum].growingTime = 0;
	field[NowFieldSum].isUsed = false;

	AddDrawObject(&objField[NowFieldSum]);
	NowFieldSum++;
}

void ResourceCount(string& Str)
{
	int moneyCount = 0;
	int riceCount = 0;
	int woodCount = 0;
	int FinishHouseCount = 0;
	int UnFinishHouseCount = 0;

	for (int i = 0; i < NowHouseSum; i++)
	{
		if (house[i].isUsed == true)
		{
			riceCount += house[i].StoneRiceSum;
			woodCount += house[i].StoneWoodSum;
		}
	}

	for (int i = 0; i < NowFarmerSum; i++)
	{
		moneyCount += farmer[i].money;
	}
	for (int i = 0; i < NowBuilderSum; i++)
	{
		moneyCount += builder[i].money;
	}
	moneyCount += king.money;

	for (int i = 0; i < NowHouseSum; i++)
	{
		if (house[i].buildTime == HouseRequireBuildTime)
		{
			FinishHouseCount++;
		}
		else
		{
			UnFinishHouseCount++;
		}
	}

	char  tempStr[200] = { 0 };

	Str += "ResourceCount\n"; 
	sprintf(tempStr, "money %d rice %d house %d Unfinish house %d\n", moneyCount,riceCount,woodCount,FinishHouseCount,UnFinishHouseCount);
	Str += tempStr;



	Str +="Farmer:\nid\tsex\tmoney\tage\trice\twFood\twSex\tisMarry\toHouseSum\n";
	for (int i = 0; i < NowFarmerSum; i++)
	{
		if (farmer[i].isDead == false)
		{
			sprintf(tempStr, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", farmer[i].id, farmer[i].Sex, farmer[i].money, farmer[i].age, farmer[i].belongHouse->StoneRiceSum,
				farmer[i].wantFoodLevel, farmer[i].wantSexLevel, farmer[i].isMarriage, farmer[i].NowOwnHouseSum);
			Str += tempStr;
			std::cout << 233 << std::endl;
		}
	}
	Str+= "Builder:\nid\tsex\tmoney\tage\trice\tHouse\tWood\twFood\twSex\tisMarry\toHouseSum\n";
	for (int i = 0; i < NowBuilderSum; i++)
	{
		if (builder[i].isDead == false)
		{

			sprintf(tempStr, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", builder[i].id, builder[i].Sex, builder[i].money, builder[i].age, builder[i].belongHouse->StoneRiceSum
				, builder[i].OwnHouseCount, builder[i].belongHouse->StoneWoodSum, builder[i].wantFoodLevel, builder[i].wantSexLevel, builder[i].isMarriage, builder[i].NowOwnHouseSum);
			Str += tempStr;
		}
	}
	Str += "Child:\nid\tsex\tage\trice\twFood\tHouseId\n";
	for (int i = 0; i < NowChildSum; i++)
	{
		if (child[i].isDead == false)
		{
			
			sprintf(tempStr, "%d\t%d\t%d\t%d\t%d\t%d\n", child[i].id, child[i].Sex, child[i].age, child[i].belongHouse->StoneRiceSum, child[i].wantFoodLevel, child[i].belongHouse->id);
			Str += tempStr;
		}
	}
	Str += "King:\nsex\tmoney\tage\tHouse\tRice\twFood\n";
	sprintf(tempStr, "%d\t%d\t%d\t%d\t%d\t%d\n", king.Sex, king.money, king.age, king.HaveEmptyHouseSum, king.belongHouse->StoneRiceSum, king.wantFoodLevel);
	Str += tempStr;
}

void ResourceCount()//对村民拥有的资源进行统计
{
	int moneyCount = 0;
	int riceCount = 0;
	int woodCount = 0;
	int FinishHouseCount = 0;
	int UnFinishHouseCount = 0;

	for (int i = 0; i < NowHouseSum; i++)
	{
		if (house[i].isUsed == true)
		{
			riceCount += house[i].StoneRiceSum;
			woodCount += house[i].StoneWoodSum;
		}
	}

	for (int i = 0; i < NowFarmerSum; i++)
	{
		moneyCount += farmer[i].money;
	}
	for (int i = 0; i < NowBuilderSum; i++)
	{
		moneyCount += builder[i].money;
	}
	moneyCount += king.money;

	for (int i = 0; i < NowHouseSum; i++)
	{
		if (house[i].buildTime == HouseRequireBuildTime)
		{
			FinishHouseCount++;
		}
		else
		{
			UnFinishHouseCount++;
		}
	}

	cout << "==================" << endl;
	cout << "ResourceCount" << endl;
	//cout << "DrawObjectSum:" << drawSum << endl;
	cout << "money:" << moneyCount << endl;
	cout << "rice:" << riceCount << endl;
	cout << "wood:" << woodCount << endl;
	cout << "house:" << FinishHouseCount << endl;
	cout << "Unfinish house:" << UnFinishHouseCount << endl;
	
	cout << endl;


	cout << "Farmer:" << endl;
	cout << "id\tsex\tmoney\tage\trice\twFood\twSex\tisMarry\tHouseId\toHouseSum" << endl;
	for (int i = 0; i < NowFarmerSum; i++)
	{
		if (farmer[i].isDead == false)
		{
			cout << farmer[i].id << "\t" << ((farmer[i].Sex == 1) ? "man" : "woman") << "\t" << farmer[i].money << "\t" << farmer[i].age << "\t" << farmer[i].belongHouse->StoneRiceSum 
				<< "\t" << farmer[i].wantFoodLevel << "\t" << farmer[i].wantSexLevel<<"\t"<<farmer[i].isMarriage<<"\t"<<farmer[i].belongHouse->id <<"\t"<<farmer[i].NowOwnHouseSum<< endl;

		}
	}
	cout << "Builder:" << endl;
	cout << "id\tsex\tmoney\tage\trice\tHouse\tWood\twFood\twSex\tisMarry\tHouseId\toHouseSum" << endl;
	for (int i = 0; i < NowBuilderSum; i++)
	{
		if (builder[i].isDead == false)
		{
			cout << builder[i].id << "\t" << ((builder[i].Sex == 1) ? "man" : "woman") << "\t" << builder[i].money << "\t" << builder[i].age << "\t" << builder[i].belongHouse->StoneRiceSum 
				<< "\t" << builder[i].OwnHouseCount << "\t" << builder[i].belongHouse->StoneWoodSum << "\t" << builder[i].wantFoodLevel << "\t" << builder[i].wantSexLevel<<"\t"<<builder[i].isMarriage <<"\t"<<builder[i].belongHouse->id<< "\t" << builder[i].NowOwnHouseSum<< endl;

		}
	}
	cout << "Child:" << endl;
	cout << "id\tsex\tage\trice\twFood\tHouseId" << endl;
	for (int i = 0; i < NowChildSum; i++)
	{
		if (child[i].isDead == false)
		{
			cout << child[i].id << "\t" << ((child[i].Sex == 1) ? "man" : "woman") << "\t" << child[i].age << "\t" << child[i].belongHouse->StoneRiceSum << "\t" << child[i].wantFoodLevel<<"\t"<<child[i].belongHouse->id << endl;

		}
	}
	cout << "King:" << endl;
	cout << "sex\tmoney\tage\tHouse\tRice\twFood" << endl;
	cout << king.Sex<<"\t"<< king.money << "\t" << king.age << "\t" << king.HaveEmptyHouseSum<<"\t"<<king.belongHouse->StoneRiceSum<<"\t"<<king.wantFoodLevel << endl;
	cout << "==================" << endl;
	
}


void ShowSky()
{
	//白天显示太阳和background
	//白天DayTimeNowRate(0~0.5) 
	//白天是早上6点到晚上6点，即6/24=0.25,18/24=0.75
	
	
	if (DayTimeNowRate > 0.75)
	{
		objMoon.x=(DayTimeNowRate-0.75) *2 * GAME_SHOW_WIDTH;
		objBackground.pic = &picBackground1;
		objSun.x = -objSun.pic->getWidth();
	}
	else if (DayTimeNowRate < 0.25)
	{
		objMoon.x = (DayTimeNowRate*2 + 0.5) * GAME_SHOW_WIDTH;
		objBackground.pic = &picBackground1;
		objSun.x = -objSun.pic->getWidth();
	}
	else
	{
		objSun.x = (DayTimeNowRate - 0.25)* 2 * GAME_SHOW_WIDTH;
		objBackground.pic = &picBackground;
		objMoon.x = -objMoon.pic->getWidth();
	}

}

House* FindKingHouse()
{
	return king.belongHouse;
}

coord GetCoord(Object* object)
{
	int x = (object->point.x+object->pic->getWidth()/2) / 60;
	int y = (object->point.y+object->pic->getHeight()/2) / 60;
	return coord(x,y);
}

bool IsCoordUsed(coord Coord)
{

	for (int i = 0; i < NowHouseSum; i++)
	{
		if (house[i].DrawObject->point.x == Coord.x * house[i].DrawObject->pic->getWidth() && house[i].DrawObject->point.y == Coord.y * house[i].DrawObject->pic->getHeight())
		{
			return true;
		}
	}
	for (int i = 0; i < NowFieldSum; i++)
	{
		if (field[i].DrawObject->point.x == Coord.x * field[i].DrawObject->pic->getWidth() && field[i].DrawObject->point.y == Coord.y * field[i].DrawObject->pic->getHeight())
		{
			return true;
		}
	}
	for (int i = 0; i < NowTreeSum; i++)
	{
		if (tree[i].DrawObject->point.x == Coord.x * tree[i].DrawObject->pic->getWidth() && tree[i].DrawObject->point.y == Coord.y * tree[i].DrawObject->pic->getHeight())
		{
			return true;
		}
	}
	return false;
}

void WalkRotation(Object* object)
{
	//1s旋转1次60°
	float rotationSize = 45; //摆动的幅度
	float rotationSpeed = 30;

	float angle = object->getAngle(233);

	if (object->RotationDirection == 0)
	{
		if (angle < rotationSize / 2 * PI / 180)
		{
			object->setAngle(float(angle + FrameTime * rotationSpeed * PI / 180.0));
		}
		else
		{
			object->RotationDirection = 1;
		}
	}
	else
	{
		if (angle > -rotationSize / 2 * PI / 180)
		{
			object->setAngle(float(angle - FrameTime * rotationSpeed * PI / 180.0));
		}
		else
		{
			object->RotationDirection = 0;
		}
	}

}

void WalkRotationBack(Object* object)
{
	//1s旋转1次60°
	float rotationSize = 45; //摆动的幅度
	float rotationSpeed = 30;

	float angle = object->getAngle(233);

	if (angle < 0)
	{
		if (abs(angle) <  FrameTime * rotationSpeed * PI / 180.0)
		{
			object->setAngle(float(angle + FrameTime * rotationSpeed * PI / 180.0));
		}
		else
		{
			object->setAngle(0);
		}
	}
	if (angle > 0)
	{
		if (abs(angle) < FrameTime * rotationSpeed * PI / 180.0)
		{
			object->setAngle(float(angle - FrameTime * rotationSpeed * PI / 180.0));
		}
		else
		{
			object->setAngle(0);
		}
	}

}


void MakeBaby(FamilyTree* familyTree)
{
	int sex = rand() % 2;
	AddChild(sex, familyTree);
}

bool IsTheHouseHasOwner(House* house)
{
	for (int i = 0; i < NowFarmerSum; i++)
	{
		Farmer* NowFarmer = &farmer[i];
		if (house == NowFarmer->belongHouse)
		{
			return true;
		}
		for (int j = 0; j < NowFarmer->NowOwnHouseSum; j++)
		{
			if (house == NowFarmer->ownHouseList[j])
			{
				return true;
			}
		}
	}
	for (int i = 0; i < NowBuilderSum; i++)
	{
		Builder* NowBuilder = &builder[i];
		if (house == NowBuilder->belongHouse)
		{
			return true;
		}
		for (int j = 0; j < NowBuilder->NowOwnHouseSum; j++)
		{
			if (house == NowBuilder->ownHouseList[j])
			{
				return true;
			}
		}
	}
	if (house == king.belongHouse)
	{
		return true;
	}
	return false;
}
