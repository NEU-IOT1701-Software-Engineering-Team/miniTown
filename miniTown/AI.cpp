#include"miniTown.h"
using namespace std;
int RicePrice=2;
int HousePrice=40;
int FirstPayHousePrice = 20;


void AILoop()
{
	for (int i = 0; i < NowBuilderSum; i++)
	{
		builder[i].AI();
	}
	for (int i = 0; i < NowFarmerSum; i++)
	{
		farmer[i].AI();
	}
	for (int i = 0; i < NowChildSum; i++)
	{
		child[i].AI();
	}
	king.AI();
}

void Farmer::GrowRice()
{
	if (DrawObject->x + DrawObject->pic->getWidth() > belongField->DrawObject->x)
	{
		if (DrawObject->x < belongField->DrawObject->x + belongField->DrawObject->pic->getWidth())
		{
			if (DrawObject->y + DrawObject->pic->getHeight() > belongField->DrawObject->y)
			{
				if (DrawObject->y < belongField->DrawObject->y + belongField->DrawObject->pic->getHeight())
				{
					if (LastFPS != 0)
					{
						if (clkClick)
						{
							belongField->growingTime += timeScale;
						}
						if (DebugShowGrowTimeFlag)
						{
							std::cout << "Farmer No." << this->id << " grow time " << this->belongField->growingTime << std::endl;

						}

					}
				}
			}
		}
	}
	if (belongField->growingTime < GrowRiceTime/4)
	{
		belongField->DrawObject->pic = &picField;
	}
	else if (belongField->growingTime < GrowRiceTime/2)
	{
		belongField->DrawObject->pic = &picField1;
	}
	else if (belongField->growingTime < GrowRiceTime/4*3)
	{
		belongField->DrawObject->pic = &picField2;
	}
	else if (belongField->growingTime < GrowRiceTime)
	{

		if (SoundOn)
		{
			player.playSound("sound/getRice.mp3", SoundVolume);
		}
		
		for (int i = 0; i < FieldProduceRiceSum; i++)
		{
			belongField->AddRice();		
			TakeOnThing[TakeOnThingSum] = &objRice[NowRiceSum-1];
			TakeOnThingSum++;
		}
		belongField->growingTime = 0;

	}
}

void Farmer::WalkTo(Object* object)
{
	DrawObject->WalkTo(object);

	for (int i = 0; i < TakeOnThingSum; i++)
	{
		if (TakeOnThing[i] != NULL)
		{
			TakeOnThing[i]->x = this->DrawObject->x;
			TakeOnThing[i]->y = this->DrawObject->y;
		}
	}
}
//吃一个水稻回复一个食欲水平
void Farmer::Eat()
{
	if (wantFoodLevel > 0&&belongHouse->StoneRiceSum>0)
	{
		belongHouse->StoneRiceSum--;
		wantFoodLevel--;
	}
}

void Farmer::Sleep()
{
	if (clkClick)
	{
		if (DaySum > LastDaySum)
		{
			wantFoodLevel++;
			wantSexLevel += 0.5;
			age += AgeAddEveryDay;
			//cout << "want food level update " << wantFoodLevel << endl;
			LastDaySum = DaySum;
			if (isMarriage == false)
			{
				if (Sex == 1 && NowOwnHouseSum != 0)
				{
					isCanMarriage = true;
				}
				else if (Sex == 0 && money >= RicePrice * GrownUpAge)
				{
					isCanMarriage = true;
				}
				else
				{
					isCanMarriage = false;
				}
			}
			if (isMarriage == true && Sex == 1)
			{
				if (familyTree->NowFamilyTreeChildSum < this->NowOwnHouseSum)
				{
					MakeBaby(familyTree);
				}
			}
		}
	}
}

bool Builder::GetARiceToHand()
{
	if (belongHouse->StoneRiceSum > 0)
	{
		TakeOnThing[TakeOnThingSum] = belongHouse->StoneRice[belongHouse->StoneRiceSum - 1];
		//belongHouse->StoneRiceSum--;  去村长交易的时候再减
		TakeOnThingSum++;
		return true;
	}
	return false;
}

bool Builder::GetAllRiceToHand()
{
	if (belongHouse->StoneRiceSum > 0)
	{
		for (int i = 0; i < belongHouse->StoneRiceSum; i++)
		{
			TakeOnThing[TakeOnThingSum] = belongHouse->StoneRice[i];
			TakeOnThingSum++;
		}
		//belongHouse->StoneRiceSum = 0; 在村长交易成功的时候再减
		return true;
	}
	return false;
}

void Builder::WalkTo(Object* object)
{
	DrawObject->WalkTo(object);
	for (int i = 0; i < TakeOnThingSum; i++)
	{
		if (TakeOnThing[i] != NULL)
		{
			TakeOnThing[i]->x = this->DrawObject->x;
			TakeOnThing[i]->y = this->DrawObject->y;
		}
	}
}



Tree* Builder::FindATree()
{
	int minDistance=99999999;
	Tree* ClosesetTree = NULL;
	for (int i = 0; i < NowTreeSum; i++)
	{
		int nowDistance= DistanceAToB(this->DrawObject, tree[i].DrawObject);
		if (nowDistance < minDistance)
		{
			minDistance = nowDistance;
			ClosesetTree = &tree[i];
		}
	}
	return ClosesetTree;
}

House* Builder::FindAUnFinishHouse()
{
	int minDistance = 99999999;
	House* ClossestHouse = NULL;
	for (int i = 0; i < NowHouseSum; i++)
	{
		if (house[i].buildTime < HouseRequireBuildTime &&house[i].isOnBuild==false)
		{
			int nowDistance = DistanceAToB(this->DrawObject, house[i].DrawObject);
			if (nowDistance < minDistance)
			{
				minDistance = nowDistance;
				ClossestHouse = &house[i];
			}
		}
		
	}
	if (ClossestHouse != NULL)
	{
		ClossestHouse->isOnBuild = true;

	}
	return ClossestHouse;
}

void Builder::CutTree()
{
	if (DrawObject->x + DrawObject->pic->getWidth() > AimTree->DrawObject->x)
	{
		if (DrawObject->x < AimTree->DrawObject->x + AimTree->DrawObject->pic->getWidth())
		{
			if (DrawObject->y + DrawObject->pic->getHeight() > AimTree->DrawObject->y)
			{
				if (DrawObject->y < AimTree->DrawObject->y + AimTree->DrawObject->pic->getHeight())
				{
					if (LastFPS != 0)
					{
						if (clkClick == true)
						{
							AimTree->cutTime += timeScale;
						}
						
					}
					if (DebugShowGrowTimeFlag)
					{
						std::cout << "Builder No." << this->id << " cut tree take time " << this->AimTree->cutTime << std::endl;

					}
					//std::cout << AimTree->cutTime << std::endl;
				}
			}
		}
	}

	if (AimTree->cutTime > 15)
	{
		AimTree->AddWood();
		AimTree->cutTime = 0;
		this->TakeOnThing[this->TakeOnThingSum] = &objWood[NowWoodSum - 1];
		this->TakeOnThingSum++;

		if (SoundOn)
		{
			player.playSound("sound/getWood.mp3", SoundVolume);
		}
		
		std::cout << "Builder No." << this->id << " take wood!" << std:: endl;
		
	}
}


void Builder::BuildHouse()
{
	
	if (IsMoreCloseTo(this->DrawObject, AimUnFinishHouse->DrawObject) == true)
	{
		if (clkClick)
		{
			if (AimUnFinishHouse->FirstBuildMoney > 0)
			{
				this->money += AimUnFinishHouse->FirstBuildMoney;
				AimUnFinishHouse->FirstBuildMoney = 0;
			}
			AimUnFinishHouse->buildTime += timeScale;
		}
		//std::cout << AimUnFinishHouse->buildTime << std::endl;
	}
	
	if (AimUnFinishHouse->buildTime > HouseRequireBuildTime)
	{
		AimUnFinishHouse->buildTime = HouseRequireBuildTime;
		AimUnFinishHouse->DrawObject->pic = &picHouse;
		AimUnFinishHouse = NULL;
		OwnHouseCount++;
		belongHouse->StoneWoodSum -= HouseRequireWood;
	}
}

void Builder::Eat()
{
	if (wantFoodLevel > 0 && belongHouse->StoneRiceSum > 0)
	{
		belongHouse->StoneRiceSum--;
		wantFoodLevel--;
	}
}

void Builder::Sleep()
{
	if (clkClick)
	{
		if (DaySum > LastDaySum)
		{
			wantFoodLevel++;
			wantSexLevel += 0.5;
			age += AgeAddEveryDay;
			//cout << "want food level update " << wantFoodLevel << endl;
			LastDaySum = DaySum;
			if (isMarriage == false)
			{
				if (Sex == 1 && NowOwnHouseSum != 0)
				{
					isCanMarriage = true;
				}
				else if (Sex == 0 && money >= RicePrice * GrownUpAge)
				{
					isCanMarriage = true;
				}
				else
				{
					isCanMarriage = false;
				}
			}
			if (isMarriage == true && Sex == 1)
			{
				if (familyTree->NowFamilyTreeChildSum < this->NowOwnHouseSum)
				{
					MakeBaby(familyTree);
				}
			}

		
		}

	}
}

void Builder::AI()
{
	if (this->isDead == false)
	{

		isTryBuyHouse = false;
		//白天去砍树或种田
	//在一天的0.2到0.8部分去干活
		if (DayTimeNowRate > 0.2 && DayTimeNowRate < 0.8)
		{
			if (wantFoodLevel > 0 || wantSexLevel > 0)
			{

				if (AimTree == NULL)
				{
					AimTree = FindATree();
				}
				if (AimUnFinishHouse == NULL)
				{
					AimUnFinishHouse = FindAUnFinishHouse();
				}

				//有可以卖的房子就拿去卖
				if (this->OwnHouseCount > 0)
				{
					WalkTo(king.DrawObject);
					if (IsMoreCloseTo(this->DrawObject, king.DrawObject))
					{
						this->HouseForMoney();
					}
				}

				//如果想吃饭并且家里没饭并且今天没去买过则
				else if (wantFoodLevel > 0 && this->belongHouse->StoneRiceSum == 0
					&& isBuyRiceFinish == false)
				{

					if (isTryBuyRice == false)
					{
						//买不起一个水稻就回去
						if (this->money < RicePrice)
						{
							isTryBuyRice = true;
						}
						//去村长家买水稻
						WalkTo(king.belongHouse->DrawObject);
						if (IsMoreCloseTo(this->DrawObject, king.belongHouse->DrawObject))
						{
							BuyRice(wantFoodLevel+3);
							isTryBuyRice = true;
						}
					}
					else
					{
						//买完走回家
						WalkTo(this->belongHouse->DrawObject);
						if (IsMoreCloseTo(this->DrawObject, this->belongHouse->DrawObject))
						{
							while (TakeOnThingSum > 0 && ObjectIsRice(this->TakeOnThing[this->TakeOnThingSum - 1]) == true)
								//判断一下手上的东西是不是水稻
							{
								//是的话把手里所有水稻放进家里
								PutRice(belongHouse);


							}
							isBuyRiceFinish = true;
							isTryBuyHouse = true;
						}

					}
				}
				//如果孩子要吃饭，家里没有米,去买
				else if (isMarriage == true 
					&& this->belongHouse->StoneRiceSum == 0
					&& isBuyRiceFinish == false)
				{
					//计算需要买的米
					int NeedBuyRiceSum = 0;
					for (int i = 0; i < familyTree->NowFamilyTreeChild0Sum; i++)
					{
						NeedBuyRiceSum += (familyTree->child0List[i]->wantFoodLevel + 2) - familyTree->child0List[i]->belongHouse->StoneRiceSum;
					}
					if (NeedBuyRiceSum > 0)
					{
						if (isTryBuyRice == false)
						{
							//买不起一个水稻就回去
							if (this->money < RicePrice)
							{
								isTryBuyRice = true;
							}
							//去村长家买水稻
							WalkTo(king.belongHouse->DrawObject);
							if (IsMoreCloseTo(this->DrawObject, king.belongHouse->DrawObject))
							{
								BuyRice(NeedBuyRiceSum);
								isTryBuyRice = true;
							}
						}
						else
						{
							//买完走回家
							WalkTo(this->belongHouse->DrawObject);
							if (IsMoreCloseTo(this->DrawObject, this->belongHouse->DrawObject))
							{
								while (TakeOnThingSum > 0 && ObjectIsRice(this->TakeOnThing[this->TakeOnThingSum - 1]) == true)
									//判断一下手上的东西是不是水稻
								{
									//是的话把手里所有水稻放进家里
									PutRice(belongHouse);
								}
								isBuyRiceFinish = true;
							}

						}
					}

				}
				//如果有小孩，小孩饿的时候给他/她东西吃
				else if (isMarriage == true
					&& isTryFeedChild == false
					&& belongHouse->StoneRiceSum > 0)
				{
					bool isNeedRice = false;
					//先找下要不要喂
					for (int i = 0; i < familyTree->NowFamilyTreeChild0Sum; i++)
					{
						if (familyTree->child0List[i]->belongHouse->StoneRiceSum <= 0)
						{
							isNeedRice = true;
						}
					}
					if (isNeedRice == false)
					{
						isTryFeedChild = true; //如果全部都有米，就不用喂
					}
					for (int i = 0; i < familyTree->NowFamilyTreeChild0Sum; i++)
					{

						if (familyTree->child0List[i]->belongHouse->StoneRiceSum <= 0)
						{
							int takeRiceSum = min(belongHouse->StoneRiceSum, familyTree->child0List[i]->wantFoodLevel + 2);
							if (takeRiceSum == 0)
							{
								isTryFeedChild = true;
							}
							if (TakeOnThingSum == 0)
							{
								if (IsMoreCloseTo(DrawObject, belongHouse->DrawObject))
								{
									for (int i = 0; i < takeRiceSum; i++)
									{
										GetARiceToHand();
										AddDrawObject(TakeOnThing[TakeOnThingSum - 1]);
									}

								}
								WalkTo(belongHouse->DrawObject);

							}
							else
							{
								WalkTo(familyTree->child0List[i]->belongHouse->DrawObject);
								if (IsMoreCloseTo(this->DrawObject, familyTree->child0List[i]->belongHouse->DrawObject) == true)
								{
									while (TakeOnThingSum > 0)
									{
										PutRice(familyTree->child0List[i]->belongHouse);
									}

									if (TakeOnThingSum == 0)
									{
										isTryFeedChild = true;
									}
								}
							}
							break;
						}
						
					}
				

				}
				else if (isMarriage==false && isCanMarriage == true&&DayTimeNowRate<0.5)
				{
					//满足结婚条件去村长那里结婚
					WalkTo(king.DrawObject);
					if (IsMoreCloseTo(this->DrawObject, king.DrawObject))
					{
						//找人结婚
						for (int i = 0; i < NowFarmerSum; i++)
						{
							if (isCanMarriage == true && farmer[i].isCanMarriage == true && farmer[i].Sex != Sex && IsMoreCloseTo(king.DrawObject, farmer[i].DrawObject))
							{

								isCanMarriage = false;
								farmer[i].isCanMarriage = false;
								isMarriage = true;
								farmer[i].isMarriage = true;
								this->familyTree = new FamilyTree;
								farmer[i].familyTree = familyTree;
								if (Sex == 1)
								{
									familyTree->FatherType = 0;
									familyTree->Father0 = this;
									familyTree->MotherType = 1;
									familyTree->Mother1 = &farmer[i];
								}
								else
								{
									familyTree->FatherType = 1;
									familyTree->Father1 = &farmer[i];
									familyTree->MotherType = 0;
									familyTree->Mother0 = this;
								}
								cout << "Get Marry!" << endl;
								MakeBaby(familyTree);
								break;
							}
						}
						for (int i = 0; i < NowBuilderSum; i++)
						{
							if (isCanMarriage == true && &builder[i] != this && builder[i].isCanMarriage == true && builder[i].Sex != Sex && IsMoreCloseTo(king.DrawObject, builder[i].DrawObject))
							{
								//这里的另一对就是这个builder[i]
								isCanMarriage = false;
								builder[i].isCanMarriage = false;
								isMarriage = true;
								builder[i].isMarriage = true;
								familyTree = new FamilyTree;
								builder[i].familyTree = familyTree;
								if (Sex == 1)
								{
									familyTree->FatherType = 0;
									familyTree->Father0 = this;
									familyTree->MotherType = 0;
									familyTree->Mother0 = &builder[i];
								}
								else
								{
									familyTree->FatherType = 0;
									familyTree->Father0 = &builder[i];
									familyTree->MotherType = 0;
									familyTree->Mother0 = this;
								}
								cout << "Get Marry!" << endl;
								MakeBaby(familyTree);
								break;
							}
						}
					}
					

				}
				else if (this->Sex==1&& this->money >= HousePrice&& isTryBuyHouse == false) //有钱就去买房
				{
					WalkTo(king.DrawObject);
					if (IsMoreCloseTo(DrawObject, king.DrawObject))
					{
						if (BuyHouse() == true)
						{
							cout<<this->id << " buy house! id:"<<this->ownHouseList[this->NowOwnHouseSum-1]->id  << endl;
							
						}
						isTryBuyHouse = true;
					}
					
					
				}

				else if (belongHouse->StoneWoodSum >= HouseRequireWood && AimUnFinishHouse != NULL)
				{

					WalkTo(AimUnFinishHouse->DrawObject);
					BuildHouse();

				}
				else
				{
					if (AimTree != NULL)
					{
						WalkTo(AimTree->DrawObject);
						CutTree();
					}

				}
			}
		}
		else
		{
			//晚上去睡觉
			WalkTo(belongHouse->DrawObject);
			//到家了
			if (IsMoreCloseTo(DrawObject, belongHouse->DrawObject))
			{
				//饿了就吃饭
				Eat();
				//然后睡觉
				Sleep();
				if (ObjectIsWood(TakeOnThing[TakeOnThingSum - 1]))
				{
					PutWood();
				}
				if (ObjectIsRice(TakeOnThing[TakeOnThingSum - 1]))
				{
					PutRice(belongHouse);
				}
				judgeDead(); //判断是否饿死
				//睡觉时重置这两个状态
				isTryBuyRice = false;
				isTryFeedChild = false;
				isBuyRiceFinish = false;
			}
		}
	}

	

}

void Builder::PutWood()
{
	belongHouse->StoneWood[belongHouse->StoneWoodSum] = TakeOnThing[TakeOnThingSum-1];
	belongHouse->StoneWoodSum++;
	std::cout << "Wood In House No." <<belongHouse->id<<" Sum " << belongHouse->StoneWoodSum << std::endl;
	RemoveDrawObecjt(TakeOnThing[TakeOnThingSum-1]);
	TakeOnThing[TakeOnThingSum-1] = NULL;
	TakeOnThingSum--;
}

bool Builder::BuyHouse()
{
	if (this->money >= HousePrice&&king.HaveEmptyHouseSum>0)
	{
		this->money -= HousePrice;
		king.money += HousePrice;
		king.HaveEmptyHouseSum--;
		this->ownHouseList[NowOwnHouseSum] = GetANearEmptyHouse(this->DrawObject);
		NowOwnHouseSum++;
		return true;
	}
	return false;
}

bool Builder::BuyRice(int wantBuyRiceSum)
{
	
	//买不起或者买不到那么多就减一尝试
	while (this->money < wantBuyRiceSum * RicePrice||king.belongHouse->StoneRiceSum<=wantBuyRiceSum)
	{
		wantBuyRiceSum--;
	}
	//买不起就回去
	if (wantBuyRiceSum == 0)
	{
		return false;
	}
	else //买得起
	{
		for (int i = 0; i < wantBuyRiceSum; i++)
		{
			//拿起一个水稻到手上
			this->TakeOnThing[i] = king.belongHouse->StoneRice[king.belongHouse->StoneRiceSum - 1];
			king.belongHouse->StoneRiceSum--;
			king.money += RicePrice;
			this->TakeOnThingSum++;
			this->money -= RicePrice;
			AddDrawObject(this->TakeOnThing[TakeOnThingSum - 1]);//画上手里拿的水稻
		}
	}


}

bool Builder::PutRice(House* house)
{
	if (TakeOnThingSum == 0)
	{
		return false;
	}
	house->StoneRice[house->StoneRiceSum] = TakeOnThing[TakeOnThingSum - 1];

	house->StoneRiceSum++;

	if (DebugShowGrowTimeFlag)
	{
		std::cout << "Rice In House No." << house->id << " Sum " << house->StoneRiceSum << std::endl;
	}
	RemoveDrawObecjt(TakeOnThing[TakeOnThingSum - 1]);
	TakeOnThing[TakeOnThingSum - 1] = NULL;
	TakeOnThingSum--;
	return true;
}

void Builder::judgeDead()
{
	if (wantFoodLevel > MaxWantFoodLevel)
	{
		this->belongHouse->isUsed = false;
		this->AimTree = NULL;
		this->AimUnFinishHouse = NULL;
		RemoveDrawObecjt(this->DrawObject);
		this->isDead = true;
	}

}

bool Builder::HouseForMoney()
{
	if (king.money >= HousePrice-FirstPayHousePrice&&this->OwnHouseCount>0)
	{
		this->money += HousePrice- FirstPayHousePrice;
		this->OwnHouseCount--;
		king.HaveEmptyHouseSum++;
		king.money -= HousePrice- FirstPayHousePrice;
		cout << "Sell house for " << HousePrice- FirstPayHousePrice << endl;
		return true;
	}
	else
	{
		return false;
	}
}


//为了保证不同帧率移动速度一样
//根据当前一帧的时间计算移动速度
//如移速为10pixel每秒 
//则200帧下，一次移动10/200
//50帧下，一次移动10/50
void Object::WalkTo(Object* object)
{
	int speed = 60;
	int direction = -1;
	float walkNowStep = speed * FrameTime*timeScale;
	if ((int)object->x > x)
	{
		if ((int)object->y > y)
		{
			direction = 1;
			this->x += min(sqrt(2)/2*walkNowStep,abs(object->x-x));
			this->y += min(sqrt(2)/2*walkNowStep,abs(object->y-y));
		}
		else if((int)object->y<y)
		{
			direction = 3;
			this->x +=min( sqrt(2)/2*walkNowStep,abs(object->x-x));
			this->y -=min( sqrt(2)/2*walkNowStep,abs(object->y-y));
		}
		else
		{
			direction = 2;
			this->x += min(walkNowStep,abs(object->x-x));
		}

	}
	else if ((int)object->x < x)
	{
		if ((int)object->y > y)
		{
			direction = 7;
			this->x -= sqrt(2) / 2 * min( walkNowStep,abs(object->x-x));
			this->y += sqrt(2) / 2 *min( walkNowStep,abs(object->y-y));
		}
		else if ((int)object->y < y)
		{
			direction = 5;
			this->x -= sqrt(2) / 2 *min( walkNowStep,abs(object->x-x));
			this->y -= sqrt(2) / 2 *min( walkNowStep,abs(object->y-y));
		}
		else if((int)object->y==y)
		{
			direction = 6;
			this->x -=min( walkNowStep,abs(object->x-x));
		}
	}
	else if ((int)object->x == x)
	{
		if ((int)object->y > y)
		{
			direction = 0;
			this->y +=min( walkNowStep,abs(object->y-y));
		}
		if ((int)object->y < y)
		{
			direction = 4;
			this->y -=min( walkNowStep,abs(object->y-y));
		}
	}
	/*

	if ((int)object->x > x)
	{
		//moveStep是为了解决移动到最后一点距离的时候物体来回晃动而用的
		float moveStep = min( object->x-x, (float)timeScale * speed * FrameTime);
		x += moveStep;
	}
	else if ((int)object->x < x)
	{
		float moveStep = min(x-object->x, (float)timeScale * speed * FrameTime);
		x -= moveStep;
	}
	if ((int)object->y > y)
	{
		float moveStep = min(object->y-y, (float)timeScale * speed * FrameTime);
		y += moveStep;
	}
	else if ((int)object->y < y)
	{
		float moveStep = min(y - object->y, (float)timeScale * speed * FrameTime);
		y -= moveStep;
	}
	*/

}



void Farmer::AI()
{
	if (isDead == false)
	{

		House* kingHouse = FindKingHouse();
		if (belongField == NULL)
		{
			belongField = GetANearUnUsedField(this->DrawObject);
		}
		//白天去种田
		//在一天的0.2到0.8部分去干活
		if (DayTimeNowRate > 0.2 && DayTimeNowRate < 0.8)
		{
			if (wantFoodLevel > 0 || wantSexLevel > 0)
			{
				//如果有一个水稻就拿去卖
				if (belongHouse->StoneRiceSum > 0&&isTrySellRice==false)
				{
					//cout << "take On thing sum " << TakeOnThingSum << endl;
					//从房子里拿一个水稻来卖

					if (TakeOnThingSum == 0)
					{
						if (IsMoreCloseTo(DrawObject, belongHouse->DrawObject))
						{
							if (GetAllRiceToHand() == false)
							{
								isTrySellRice = true;
							}
						}
						
						WalkTo(belongHouse->DrawObject);

					}

					//如果拿到水稻就拿去卖
					if (TakeOnThingSum > 0)
					{
						AddDrawObject(TakeOnThing[TakeOnThingSum - 1]);
						WalkTo(kingHouse->DrawObject);
						if (IsMoreCloseTo(DrawObject, kingHouse->DrawObject))
						{
							SellRiceForMoney();
							if (TakeOnThingSum == 0)
							{
								isTrySellRice = true;
							}
							
						}
					}

				}
				//如果孩子要吃饭，家里没有米,去买
				else if (isMarriage==true
					&& this->belongHouse->StoneRiceSum == 0
					&& isBuyRiceFinish == false)
				{
					//计算需要买的米
					int NeedBuyRiceSum = 0;
					for (int i = 0; i < familyTree->NowFamilyTreeChild0Sum; i++)
					{
						NeedBuyRiceSum += (familyTree->child0List[i]->wantFoodLevel +2)- familyTree->child0List[i]->belongHouse->StoneRiceSum;
					}
					if (NeedBuyRiceSum > 0)
					{
						if (isTryBuyRice == false)
						{
							//买不起一个水稻就回去
							if (this->money < RicePrice)
							{
								isTryBuyRice = true;
							}
							//去村长家买水稻
							WalkTo(king.belongHouse->DrawObject);
							if (IsMoreCloseTo(this->DrawObject, king.belongHouse->DrawObject))
							{
								BuyRice(NeedBuyRiceSum);
								isTryBuyRice = true;
							}
						}
						else
						{
							//买完走回家
							WalkTo(this->belongHouse->DrawObject);
							if (IsMoreCloseTo(this->DrawObject, this->belongHouse->DrawObject))
							{
								while (TakeOnThingSum > 0 && ObjectIsRice(this->TakeOnThing[this->TakeOnThingSum - 1]) == true)
									//判断一下手上的东西是不是水稻
								{
									//是的话把手里所有水稻放进家里
									PutRice(belongHouse);
								}
								isBuyRiceFinish = true;
							}

						}
					}
				
				}
				//如果有小孩，小孩饿的时候给他/她东西吃
				else if (isMarriage == true
					&& isTryFeedChild == false
					&& belongHouse->StoneRiceSum > 0)
				{
					int Child0Now = -1;
					for (int i = 0; i < familyTree->NowFamilyTreeChildSum; i++)
					{
						if (familyTree->ChildTypeList[i] == 0)
						{
							Child0Now++;
							if (familyTree->child0List[Child0Now]->belongHouse->StoneRiceSum <= 0)
							{
								int takeRiceSum = min(belongHouse->StoneRiceSum, familyTree->child0List[Child0Now]->wantFoodLevel + 2);
								if (takeRiceSum == 0)
								{
									isTryFeedChild = true;
								}
								if (TakeOnThingSum == 0)
								{
									if (IsMoreCloseTo(DrawObject, belongHouse->DrawObject))
									{
										for (int i = 0; i < takeRiceSum; i++)
										{
											GetARiceToHand();
											AddDrawObject(TakeOnThing[TakeOnThingSum - 1]);
										}

									}
									WalkTo(belongHouse->DrawObject);

								}
								else
								{
									WalkTo(familyTree->child0List[Child0Now]->belongHouse->DrawObject);
									if (IsMoreCloseTo(this->DrawObject, familyTree->child0List[Child0Now]->belongHouse->DrawObject) == true)
									{
										while (TakeOnThingSum > 0)
										{
											PutRice(familyTree->child0List[Child0Now]->belongHouse);
										}

										if (TakeOnThingSum == 0)
										{
											isTryFeedChild = true;
										}
									}
								}
								break;
							}


						}

					}


				}
				else if (isMarriage == false && isCanMarriage == true && DayTimeNowRate < 0.5)
				{
					//满足结婚条件去村长那里结婚
					WalkTo(king.DrawObject);
					if (IsMoreCloseTo(this->DrawObject, king.DrawObject))
					{
						//找人结婚
						for (int i = 0; i < NowFarmerSum; i++)
						{
							if (isCanMarriage == true && &farmer[i] != this && farmer[i].isCanMarriage == true && farmer[i].Sex != Sex && IsMoreCloseTo(king.DrawObject, farmer[i].DrawObject))
							{
								isCanMarriage = false;
								farmer[i].isCanMarriage = false;
								isMarriage = true;
								farmer[i].isMarriage = true;
								familyTree = new FamilyTree;
								farmer[i].familyTree = familyTree;
								if (Sex == 1)
								{
									familyTree->FatherType = 1;
									familyTree->Father1 = this;
									familyTree->MotherType = 1;
									familyTree->Mother1 = &farmer[i];
								}
								else
								{
									familyTree->FatherType = 1;
									familyTree->Father1 = &farmer[i];
									familyTree->MotherType = 1;
									familyTree->Mother1 = this;
								}
								cout << "Get Marry!" << endl;
								MakeBaby(familyTree);
								break;

							}
						}
						for (int i = 0; i < NowBuilderSum; i++)
						{
							if (isCanMarriage == true && builder[i].isCanMarriage == true && builder[i].Sex != Sex && IsMoreCloseTo(king.DrawObject, builder[i].DrawObject))
							{
								isCanMarriage = false;
								builder[i].isCanMarriage = false;
								isMarriage = true;
								builder[i].isMarriage = true;
								familyTree = new FamilyTree;
								builder[i].familyTree = familyTree;
								if (Sex == 1)
								{
									familyTree->FatherType = 1;
									familyTree->Father1 = this;
									familyTree->MotherType = 0;
									familyTree->Mother0 = &builder[i];
								}
								else
								{
									familyTree->FatherType = 0;
									familyTree->Father0 = &builder[i];
									familyTree->MotherType = 1;
									familyTree->Mother1 = this;
								}
								cout << "Get Marry!" << endl;
								MakeBaby(familyTree);
								break;
							}
						}
					}
					

				}
				else if (this->Sex==1&& this->money >= HousePrice&&isTryBuyHouse==false) //有钱就去买房
				{
					WalkTo(king.DrawObject);
					if (IsMoreCloseTo(DrawObject, king.DrawObject))
					{
						if (BuyHouse() == true)
						{
							cout << this->id << " buy house! id:" << this->ownHouseList[this->NowOwnHouseSum - 1]->id << endl;
						}
						
						isTryBuyHouse = true;
					}
					
					
				}
				else
				{
					if (belongField != NULL)
					{
						WalkTo(belongField->DrawObject);

						GrowRice();
					}

				}

			}
		}
		else
		{
			//晚上去睡觉
			WalkTo(belongHouse->DrawObject);
			//到家了
			if (IsMoreCloseTo(DrawObject, belongHouse->DrawObject))
			{

				while (TakeOnThingSum > 0)
				{
					if (ObjectIsRice(TakeOnThing[TakeOnThingSum - 1]))
					{
						PutRice(belongHouse);
					}
				}

				Eat();
				Sleep();
				isTryBuyHouse = false; //尝试买房子状态清零
				isTryFeedChild = false;
				isTrySellRice = false;

			}
		}
	}
	
}

void Farmer::PutRice(House *house)
{
	house->StoneRice[house->StoneRiceSum] = TakeOnThing[TakeOnThingSum-1];
	
	house->StoneRiceSum++;
	
	if (DebugShowGrowTimeFlag)
	{
		std::cout << "Rice In House No." << house->id << " Sum " << house->StoneRiceSum << std::endl;
	}
	RemoveDrawObecjt(TakeOnThing[TakeOnThingSum-1]);
	TakeOnThing[TakeOnThingSum-1] = NULL;
	TakeOnThingSum--;
	
}

bool Farmer::GetARiceToHand()
{
	if (belongHouse->StoneRiceSum > 0)
	{
		TakeOnThing[TakeOnThingSum] = belongHouse->StoneRice[belongHouse->StoneRiceSum - 1];
		//房子里的水稻是去村长交易后消失的
		TakeOnThingSum++;
		return true;
	}
	return false;

}

bool Farmer::GetAllRiceToHand()
{
	if (belongHouse->StoneRiceSum > 0)
	{
		for (int i = 0; i < belongHouse->StoneRiceSum; i++)
		{
			TakeOnThing[TakeOnThingSum] = belongHouse->StoneRice[i];
			TakeOnThingSum++;
		}
		return true;
	}
	return false;

}

bool Farmer::BuyRice(int wantBuyRiceSum)
{

	//买不起或者买不到那么多就减一尝试
	while (this->money < wantBuyRiceSum * RicePrice || king.belongHouse->StoneRiceSum <= wantBuyRiceSum)
	{
		wantBuyRiceSum--;
	}
	//买不起就回去
	if (wantBuyRiceSum == 0)
	{
		return false;
	}
	else //买得起
	{
		for (int i = 0; i < wantBuyRiceSum; i++)
		{
			//拿起一个水稻到手上
			this->TakeOnThing[i] = king.belongHouse->StoneRice[king.belongHouse->StoneRiceSum - 1];
			king.belongHouse->StoneRiceSum--;
			king.money += RicePrice;
			this->TakeOnThingSum++;
			this->money -= RicePrice;
			AddDrawObject(this->TakeOnThing[TakeOnThingSum - 1]);//画上手里拿的水稻
		}
	}
}

bool Farmer::BuyHouse()
{
	if (this->money >= HousePrice && king.HaveEmptyHouseSum > 0)
	{
		this->ownHouseList[NowOwnHouseSum] = GetANearEmptyHouse(this->DrawObject);
		if (this->ownHouseList[NowOwnHouseSum] != NULL)
		{
			this->money -= HousePrice;
			king.money += HousePrice;
			king.HaveEmptyHouseSum--;

			NowOwnHouseSum++;
			return true;
		}
		return false;
	}
	return false;
}

void Farmer::judgeDead()
{
	if (this->wantFoodLevel > MaxWantFoodLevel)
	{
		this->belongHouse->isUsed = false;
		this->belongField->isUsed = false; 
		RemoveDrawObecjt(this->DrawObject);
		this->isDead = true;
	}

}

//卖米给村长，换钱
bool Farmer::SellRiceForMoney()
{
	if (king.money >= RicePrice&&TakeOnThingSum>0)
	{
		this->money += RicePrice;
		RemoveDrawObecjt(TakeOnThing[TakeOnThingSum-1]);
		TakeOnThing[TakeOnThingSum-1] = NULL;
		TakeOnThingSum--;

		king.money -= RicePrice;
		king.belongHouse->StoneRice[king.belongHouse->StoneRiceSum] = this->belongHouse->StoneRice[this->belongHouse->StoneRiceSum-1];
		king.belongHouse->StoneRiceSum++;
		this->belongHouse->StoneRiceSum--;
		cout << "Sell rice for " << RicePrice << endl;

		return true;
	}
	else
	{
		return false;
	}
}

void Field::AI()
{

}

void Field::AddRice()
{
	Object* newObjRice = &objRice[NowRiceSum];
	newObjRice->x = this->DrawObject->x;
	newObjRice->y = this->DrawObject->y;
	newObjRice->z = 2;
	newObjRice->pic = &picRice;
	NowRiceSum++;
	//std::cout << "Add Rice Called!" << std::endl;
	AddDrawObject(newObjRice);
}

void Tree::AddWood()
{
	Object* newObjWood = &objWood[NowWoodSum];
	newObjWood->x = this->DrawObject->x;
	newObjWood->y = this->DrawObject->y;
	newObjWood->z = 2;
	newObjWood->pic = &picWood;
	NowWoodSum++;
	AddDrawObject(newObjWood);
}

void King::AI()
{
	if (this->isDead == false)
	{
		Sleep();
	}
}

void King::Eat()
{
	if (IsMoreCloseTo(this->DrawObject, this->belongHouse->DrawObject) == true)
	{
		if (this->belongHouse->StoneRiceSum > 0&&this->wantFoodLevel>0)
		{
			this->wantFoodLevel--;
			this->belongHouse->StoneRiceSum--;
		}
	}
}

void King::judgeDead()
{
	if (wantFoodLevel > MaxWantFoodLevel)
	{
		this->belongHouse->isUsed = false;
		RemoveDrawObecjt(this->DrawObject);
		this->isDead = true;
	}
}

void King::Sleep()
{
	if (clkClick)
	{
		if (DaySum > LastDaySum)
		{
			wantFoodLevel++;
			wantSexLevel += 0.5;
			age += AgeAddEveryDay;
			//cout << "want food level update " << wantFoodLevel << endl;
			LastDaySum = DaySum;
			judgeDead();
			
		}

	}
}

void King::MakeMoney(int Sum)
{
	this->money += Sum;
}

void King::DestoryMoney(int Sum)
{
	this->money -= Sum;
	if (this->money < 0)
	{
		this->money = 0;
	}
}

void King::SetRicePrice(int Price)
{
	RicePrice = Price;
}

void King::SetHousePrice(int Price)
{
	HousePrice = Price;
}

void King::SetATree()
{

	//获得当前位置的坐标
	coord CoordHere = GetCoord(this->DrawObject);
	//如果现在这个坐标被用了，则退出
	if (IsCoordUsed(CoordHere))
	{
		return;
	}

	AddTree(CoordHere.x, CoordHere.y);
}

void King::SetAField()
{
	//获得当前位置的坐标
	coord CoordHere = GetCoord(this->DrawObject);
	//如果现在这个坐标被用了，则退出
	if (IsCoordUsed(CoordHere))
	{
		return;
	}

	AddField(CoordHere.x, CoordHere.y);
}


//在村长当前位置设置一个盖房子的标记，木匠随后会去盖房子
bool King::SetUnFinishHouseMark()
{
	if (money >= FirstPayHousePrice)
	{
		//获得当前位置的坐标
		coord CoordHere = GetCoord(this->DrawObject);
		//如果现在这个坐标被用了，则退出
		if (IsCoordUsed(CoordHere))
		{
			return false;
		}

		AddUnFinishHouse(CoordHere.x, CoordHere.y);
		money -= FirstPayHousePrice;
		return true;
	}
	else
	{
		return false;
	}

}


void Child::Walk()
{
	if (goToFlag == true)
	{
		AimX = (rand() %GAME_SHOW_WIDTH/60)*60;
		AimY = ((rand() % (GAME_SHOW_HEIGHT / 60 - 1) + 1) * 60);
		goToFlag = false;
	}

	AimObject.pic = &picChild;
	AimObject.x = AimX;
	AimObject.y = AimY;
	WalkTo(&AimObject);
	if (IsMoreCloseTo(this->DrawObject,& AimObject))
	{
		goToFlag = true;
	}
}

void Child::GrowUP()
{
	int JobType = rand() % 2;
	if (JobType == 0)
	{
		coord nowCoord;
		nowCoord = GetCoord(this->DrawObject);
		AddBuilder(nowCoord.x, nowCoord.y, this->Sex);
		oldFamilyTree->ChildTypeList[oldFamilyTree->NowFamilyTreeChildSum-1] = 1;
		oldFamilyTree->child1List[oldFamilyTree->NowFamilyTreeChild1Sum] = &builder[NowBuilderSum - 1];
		builder[NowBuilderSum - 1].age = age;
		builder[NowBuilderSum - 1].belongHouse = belongHouse;
		builder[NowBuilderSum - 1].wantFoodLevel = wantFoodLevel;
		builder[NowBuilderSum - 1].wantSexLevel = 0;
		builder[NowBuilderSum - 1].oleFamilyTree = oldFamilyTree;

		oldFamilyTree->NowFamilyTreeChild1Sum++;
		oldFamilyTree->NowFamilyTreeChild0Sum--;
	}
	else if (JobType == 1)
	{
		coord nowCoord;
		nowCoord = GetCoord(this->DrawObject);
		AddFarmer(nowCoord.x, nowCoord.y, this->Sex);
		oldFamilyTree->ChildTypeList[oldFamilyTree->NowFamilyTreeChildSum-1] = 2;
		oldFamilyTree->child2List[oldFamilyTree->NowFamilyTreeChild2Sum] = &farmer[NowFarmerSum - 1];
		farmer[NowFarmerSum - 1].age = age;
		farmer[NowFarmerSum - 1].belongHouse = belongHouse;
		farmer[NowFarmerSum - 1].wantFoodLevel = wantFoodLevel;
		farmer[NowFarmerSum - 1].wantSexLevel = 0;
		farmer[NowFarmerSum - 1].oleFamilyTree = oldFamilyTree;
		oldFamilyTree->NowFamilyTreeChild2Sum++;
		oldFamilyTree->NowFamilyTreeChild0Sum--;
	}
	this->belongHouse->isUsed = false;
	RemoveDrawObecjt(this->DrawObject);
	this->isDead = true;
}

void Child::Sleep()
{
	if (clkClick)
	{
		if (DaySum > LastDaySum)
		{
			wantFoodLevel++;
			age += AgeAddEveryDay;
			//cout << "want food level update " << wantFoodLevel << endl;
			LastDaySum = DaySum;
		}

	}
}

void Child::Eat()
{
	if (wantFoodLevel > 0 && belongHouse->StoneRiceSum > 0)
	{
		belongHouse->StoneRiceSum--;
		wantFoodLevel--;
	}
}

void Child::AI()
{
	if (this->isDead == false)
	{
		if (DayTimeNowRate > 0.1 && DayTimeNowRate < 0.8)
		{
			Walk();
		}
		else
		{
			WalkTo(belongHouse->DrawObject);
			if (IsMoreCloseTo(this->DrawObject, belongHouse->DrawObject))
			{
				if (DayTimeNow < 0.9)
				{
					Eat();
				}
				Sleep();
				if (age >= GrownUpAge)
				{
					GrowUP();
				}
				judgeDead();
			}
		}
	}
}

void Child::judgeDead()
{
	if (this->wantFoodLevel > MaxWantFoodLevel)
	{
		this->belongHouse->isUsed = false;
		RemoveDrawObecjt(this->DrawObject);
		this->isDead = true;
	}
}

void Child::WalkTo(Object* object)
{
	DrawObject->WalkTo(object);

}
