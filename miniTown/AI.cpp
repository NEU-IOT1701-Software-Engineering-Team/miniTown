#include"miniTown.h"
using namespace std;
int RicePrice=2;
int HousePrice=40;
int FirstPayHousePrice = 20;
int FieldProduceRiceSum = 6;
int ChildGrowDayTime = 10;

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
	if (belongField->growingTime < 5)
	{
		belongField->DrawObject->pic = &picField;
	}
	else if (belongField->growingTime < 10)
	{
		belongField->DrawObject->pic = &picField1;
	}
	else if (belongField->growingTime < 20)
	{
		belongField->DrawObject->pic = &picField2;
	}
	else if (belongField->growingTime < 30)
	{

		player.playSound("sound/getRice.mp3", SoundVolume);
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
//��һ��ˮ���ظ�һ��ʳ��ˮƽ
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
			age += 0.25;
			//cout << "want food level update " << wantFoodLevel << endl;
			LastDaySum = DaySum;
			if (Sex == 1 && ownHouse != NULL)
			{
				isCanMarriage = true;
			}
			else if (Sex == 0 && money >= RicePrice * ChildGrowDayTime)
			{
				isCanMarriage = true;
			}
		}
	}
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
		if (house[i].buildTime < house[i].RequireBuildTime&&house[i].isOnBuild==false)
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

		player.playSound("sound/getWood.mp3", SoundVolume);
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
	
	if (AimUnFinishHouse->buildTime > house->RequireBuildTime)
	{
		AimUnFinishHouse->buildTime = AimUnFinishHouse->RequireBuildTime;
		AimUnFinishHouse->DrawObject->pic = &picHouse;
		AimUnFinishHouse = NULL;
		OwnHouseCount++;
		belongHouse->StoneWoodSum -= 3;
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
			age += 0.25;
			//cout << "want food level update " << wantFoodLevel << endl;
			LastDaySum = DaySum;
			if (Sex == 1 && ownHouse != NULL)
			{
				isCanMarriage = true;
			}
			else if (Sex == 0 && money >= RicePrice * ChildGrowDayTime)
			{
				isCanMarriage = true;
			}
			ResourceCount();
		}

	}
}

void Builder::AI()
{
	if (this->isDead == false)
	{
		isTryBuyHouse = false;
		//����ȥ����������
	//��һ���0.2��0.8����ȥ�ɻ�
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

				//�п������ķ��Ӿ���ȥ��
				if (this->OwnHouseCount > 0)
				{
					WalkTo(king.DrawObject);
					if (IsMoreCloseTo(this->DrawObject, king.DrawObject))
					{
						this->HouseForMoney();
					}
				}

				//�����Է����Ҽ���û�����ҽ���ûȥ�����
				else if (wantFoodLevel > 0 && this->belongHouse->StoneRiceSum == 0
					&& isBuyRiceFinish == false)
				{

					if (isTryBuyRice == false)
					{
						//����һ��ˮ���ͻ�ȥ
						if (this->money < RicePrice)
						{
							isTryBuyRice = true;
						}
						//ȥ�峤����ˮ��
						WalkTo(king.belongHouse->DrawObject);
						if (IsMoreCloseTo(this->DrawObject, king.belongHouse->DrawObject))
						{
							BuyRice();
							isTryBuyRice = true;
						}
					}
					else
					{
						//�����߻ؼ�
						WalkTo(this->belongHouse->DrawObject);
						if (IsMoreCloseTo(this->DrawObject, this->belongHouse->DrawObject))
						{
							while (TakeOnThingSum > 0 && ObjectIsRice(this->TakeOnThing[this->TakeOnThingSum - 1]) == true)
								//�ж�һ�����ϵĶ����ǲ���ˮ��
							{
								//�ǵĻ�����������ˮ���Ž�����
								PutRice();


							}
							isBuyRiceFinish = true;
							isTryBuyHouse = true;
						}

					}


				}
				else if (isMarriage==false && isCanMarriage == true&&DayTimeNowRate<0.5)
				{
					//����������ȥ�峤������
					WalkTo(king.DrawObject);
					//���˽��
					for (int i = 0; i < NowFarmerSum; i++)
					{
						if (isCanMarriage==true&& farmer[i].isCanMarriage == true&&farmer[i].Sex!=Sex && IsMoreCloseTo(king.DrawObject, farmer[i].DrawObject))
						{
							
							isCanMarriage = false;
							farmer[i].isCanMarriage = false;
							isMarriage = true;
							farmer[i].isMarriage = true;
							this->familyTree = new FamilyTree;
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
						if (isCanMarriage==true&& &builder[i] != this && builder[i].isCanMarriage == true && builder[i].Sex != Sex && IsMoreCloseTo(king.DrawObject, builder[i].DrawObject))
						{
							isCanMarriage = false;
							builder[i].isCanMarriage = false;
							isMarriage = true;
							builder[i].isMarriage = true;
							familyTree = new FamilyTree;
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
				else if (this->Sex==1&& this->money >= HousePrice && this->ownHouse == NULL && isTryBuyHouse == false) //��Ǯ��ȥ��
				{
					WalkTo(king.DrawObject);
					if (IsMoreCloseTo(DrawObject, king.DrawObject))
					{
						if (BuyHouse() == true)
						{
							cout << "Buy house!" << endl;
						}
						isTryBuyHouse = true;
					}
					
					
				}

				else if (belongHouse->StoneWoodSum >= 3 && AimUnFinishHouse != NULL)
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
			//����ȥ˯��
			WalkTo(belongHouse->DrawObject);
			//������
			if (IsMoreCloseTo(DrawObject, belongHouse->DrawObject))
			{
				//���˾ͳԷ�
				Eat();
				//Ȼ��˯��
				Sleep();
				if (ObjectIsWood(TakeOnThing[TakeOnThingSum - 1]))
				{
					PutWood();
				}
				if (ObjectIsRice(TakeOnThing[TakeOnThingSum - 1]))
				{
					PutRice();
				}
				judgeDead(); //�ж��Ƿ����
				//˯��ʱ����������״̬
				isTryBuyRice = false;
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
		this->ownHouse = GetANearEmptyHouse(this->DrawObject);
		return true;
	}
	return false;
}

bool Builder::BuyRice()
{
	int wantBuyRiceSum = wantFoodLevel + 3; //����Ҫ��ˮ��������
	
	//��������򲻵���ô��ͼ�һ����
	while (this->money < wantBuyRiceSum * RicePrice||king.belongHouse->StoneRiceSum<=wantBuyRiceSum)
	{
		wantBuyRiceSum--;
	}
	//����ͻ�ȥ
	if (wantBuyRiceSum == 0)
	{
		return false;
	}
	else //�����
	{
		for (int i = 0; i < wantBuyRiceSum; i++)
		{
			//����һ��ˮ��������
			this->TakeOnThing[i] = king.belongHouse->StoneRice[king.belongHouse->StoneRiceSum - 1];
			king.belongHouse->StoneRiceSum--;
			king.money += RicePrice;
			this->TakeOnThingSum++;
			this->money -= RicePrice;
			AddDrawObject(this->TakeOnThing[TakeOnThingSum - 1]);//���������õ�ˮ��
		}
	}


}

bool Builder::PutRice()
{
	if (TakeOnThingSum == 0)
	{
		return false;
	}
	belongHouse->StoneRice[belongHouse->StoneRiceSum] = TakeOnThing[TakeOnThingSum - 1];

	belongHouse->StoneRiceSum++;

	if (DebugShowGrowTimeFlag)
	{
		std::cout << "Rice In House No." << belongHouse->id << " Sum " << belongHouse->StoneRiceSum << std::endl;
	}
	RemoveDrawObecjt(TakeOnThing[TakeOnThingSum - 1]);
	TakeOnThing[TakeOnThingSum - 1] = NULL;
	TakeOnThingSum--;
	return true;
}

void Builder::judgeDead()
{
	if (wantFoodLevel > 5)
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


//Ϊ�˱�֤��ͬ֡���ƶ��ٶ�һ��
//���ݵ�ǰһ֡��ʱ������ƶ��ٶ�
//������Ϊ10pixelÿ�� 
//��200֡�£�һ���ƶ�10/200
//50֡�£�һ���ƶ�10/50
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
		//moveStep��Ϊ�˽���ƶ������һ������ʱ���������ػζ����õ�
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
		//����ȥ����
		//��һ���0.2��0.8����ȥ�ɻ�
		if (DayTimeNowRate > 0.2 && DayTimeNowRate < 0.8)
		{
			if (wantFoodLevel > 0 || wantSexLevel > 0)
			{
				//�����һ��ˮ������ȥ��
				if (belongHouse->StoneRiceSum > 0)
				{
					//cout << "take On thing sum " << TakeOnThingSum << endl;
					//�ӷ�������һ��ˮ������

					if (TakeOnThingSum == 0)
					{
						if (IsMoreCloseTo(DrawObject, belongHouse->DrawObject))
						{
							GetAllRiceToHand();
						}
						WalkTo(belongHouse->DrawObject);

					}

					//����õ�ˮ������ȥ��
					if (TakeOnThingSum > 0)
					{
						AddDrawObject(TakeOnThing[TakeOnThingSum - 1]);
						WalkTo(kingHouse->DrawObject);
						if (IsMoreCloseTo(DrawObject, kingHouse->DrawObject))
						{
							SellRiceForMoney();
						}
					}

				}
				else if (isMarriage == false && isCanMarriage == true && DayTimeNowRate < 0.5)
				{
					//����������ȥ�峤������
					WalkTo(king.DrawObject);
					//���˽��
					for (int i = 0; i < NowFarmerSum; i++)
					{
						if (isCanMarriage == true&& &farmer[i]!=this && farmer[i].isCanMarriage == true && farmer[i].Sex != Sex && IsMoreCloseTo(king.DrawObject, farmer[i].DrawObject))
						{
							isCanMarriage = false;
							farmer[i].isCanMarriage = false;
							isMarriage = true;
							farmer[i].isMarriage = true;
							familyTree = new FamilyTree;
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
				else if (this->Sex==1&& this->money >= HousePrice&&this->ownHouse==NULL&& isTryBuyHouse==false) //��Ǯ��ȥ��
				{
					WalkTo(king.DrawObject);
					if (IsMoreCloseTo(DrawObject, king.DrawObject))
					{
						if (BuyHouse() == true)
						{
							cout << "Buy house!" << endl;
						}
						
						isTryBuyHouse = true;
					}
					
					
				}
				else
				{

					WalkTo(belongField->DrawObject);

					GrowRice();
				}

			}
		}
		else
		{
			//����ȥ˯��
			WalkTo(belongHouse->DrawObject);
			//������
			if (IsMoreCloseTo(DrawObject, belongHouse->DrawObject))
			{

				while (TakeOnThingSum > 0)
				{
					if (ObjectIsRice(TakeOnThing[TakeOnThingSum - 1]))
					{
						PutRice();
					}
				}

				Eat();
				Sleep();
				isTryBuyHouse = false; //��������״̬����

			}
		}
	}
	
}

void Farmer::PutRice()
{
	belongHouse->StoneRice[belongHouse->StoneRiceSum] = TakeOnThing[TakeOnThingSum-1];
	
	belongHouse->StoneRiceSum++;
	
	if (DebugShowGrowTimeFlag)
	{
		std::cout << "Rice In House No." << belongHouse->id << " Sum " << belongHouse->StoneRiceSum << std::endl;
	}
	RemoveDrawObecjt(TakeOnThing[TakeOnThingSum-1]);
	TakeOnThing[TakeOnThingSum-1] = NULL;
	TakeOnThingSum--;
	
}

void Farmer::GetARiceToHand()
{
	TakeOnThing[TakeOnThingSum] = belongHouse->StoneRice[belongHouse->StoneRiceSum - 1];
	TakeOnThingSum++;
}

void Farmer::GetAllRiceToHand()
{
	for (int i = 0; i < belongHouse->StoneRiceSum; i++)
	{
		TakeOnThing[TakeOnThingSum] = belongHouse->StoneRice[i];
		TakeOnThingSum++;
	}
	//belongHouse->StoneRiceSum = 0;
}

bool Farmer::BuyHouse()
{
	if (this->money >= HousePrice && king.HaveEmptyHouseSum > 0)
	{
		this->money -= HousePrice;
		king.money += HousePrice;
		king.HaveEmptyHouseSum--;
		this->ownHouse = GetANearEmptyHouse(this->DrawObject);
		return true;
	}
	return false;
}

void Farmer::judgeDead()
{
	if (this->wantFoodLevel > 5)
	{
		this->belongHouse->isUsed = false;
		this->belongField->isUsed = false; 
		RemoveDrawObecjt(this->DrawObject);
		this->isDead = true;
	}

}

//���׸��峤����Ǯ
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
	newObjRice->z = 0;
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
	newObjWood->z = 0;
	newObjWood->pic = &picWood;
	NowWoodSum++;
	AddDrawObject(newObjWood);
}

void King::AI()
{

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


//�ڴ峤��ǰλ������һ���Ƿ��ӵı�ǣ�ľ������ȥ�Ƿ���
bool King::SetUnFinishHouseMark()
{
	if (money >= FirstPayHousePrice)
	{
		//��õ�ǰλ�õ�����
		Point CoordHere = GetPoint(this->DrawObject);
		//�������������걻���ˣ����˳�
		if (IsPointUsed(CoordHere))
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
		AimX = (rand() %SCREEN_WIDTH/60)*60;
		AimY = ((rand() % (SCREEN_HEIGHT / 60 - 1) + 1) * 60);
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

void Child::Sleep()
{
	if (clkClick)
	{
		if (DaySum > LastDaySum)
		{
			wantFoodLevel++;
			age += 0.25;
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
			}
		}
	}
}

void Child::WalkTo(Object* object)
{
	DrawObject->WalkTo(object);

}