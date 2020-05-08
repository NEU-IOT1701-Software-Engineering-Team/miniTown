#include "miniTown.h"

const char* WindowTitle = "miniTown";
int nScreenWidth = SCREEN_WIDTH;
int nScreenHeight = SCREEN_HEIGHT;
HWND hWnd = NULL;
HDC hMemDC = NULL;
HBITMAP hBitmapBuffer = NULL;
HBITMAP hBitmapDevice = NULL;
BYTE* buffer = NULL;
BYTE screen_keys[512] = { 0 };
//Player player;

//Object* drawList[MaxDrawObjectSum];
//绘制队列
std::vector<ObjectPointer> drawList;
//std::vector<BaseUI*> listBaseUI;
std::vector<Button*> listButton;
std::vector<Label*> listLabel;
std::vector<EditBox*> listEditBox;



//Description:
//	向绘制队列中加入一个新对象,如果该对象已被加入，则不会重复加入.
//Paramter: 
//	Object* object 将要加入对象的地址
//Return Value:
//	NONE
void AddDrawObject(Object* object)
{
	ObjectPointer op(object);
	bool isExist = false;//该对象是否已存在
	for (std::vector<ObjectPointer>::iterator it = drawList.begin();
		it != drawList.end(); ++it) {
		if ((*it) == op) {
			isExist = true;
			break;
		}
	}
	if (!isExist) {
		//不存在需要添加
		drawList.push_back(op);
	}
}

//Description:
//	从绘制队列中删掉一个对象。
//Paramter: 
//	Object* object 将要删除的对象地址
//Return Value:
//	NONE
void RemoveDrawObecjt(Object* object)
{
	ObjectPointer op(object);
	for (std::vector<ObjectPointer>::iterator it = drawList.begin();
		it != drawList.end(); ++it) {
		if ((*it) == op) {
			drawList.erase(it);
			break;
		}
	}
}


//base
inline void DrawPoint(int nPos, BYTE r, BYTE g, BYTE b, BYTE a = 0) {


	buffer[nPos] = ((buffer[nPos] * a) >> 8) + ((b * (255 - a)) >> 8);;
	buffer[nPos + 1] = ((buffer[nPos + 1] * a) >> 8) + ((g * (255 - a)) >> 8);
	buffer[nPos + 2] =((buffer[nPos+2] * a) >> 8) + ((r * (255 - a)) >> 8);
}

inline void DrawPoint(int nPos, Color c) {
	DrawPoint(nPos, c.r, c.g, c.b, c.a);
}

inline void DrawPoint(int x, int y, BYTE r, BYTE g, BYTE b, BYTE a = 0) {
	if (x > nScreenWidth || x<0 || y>nScreenHeight || y < 0) {
		return;
	}
	int nPos = (y * nScreenWidth + x) * 4;
	DrawPoint(nPos, r, g, b, a);
}

inline void DrawPoint(int x, int y, Color c) {
	DrawPoint(x,y, c.r, c.g, c.b, c.a);
}

//base
inline void DrawLine(int x1, int y1, int x2, int y2, BYTE r, BYTE g, BYTE b, BYTE a=0 ) {
	int x, y, dx, dy;
	float k, e;
	if (x1 > x2) {
		x = x1; x1 = x2; x2 = x;
		y = y1; y1 = y2; y2 = y;
	}
	if (x1 == x2) {
		if (y1 > y2) {
			y = y1; y1 = y2; y2 = y;
		}
		x = x1;
		for (y = y1; y < y2; ++y) {
			DrawPoint(x, y, r, g, b, a);
		}
		return ;
	}
	dx = x2 - x1;
	dy = y2 - y1;
	k = dy / dx;
	
	x = x1;
	y = y1;
	int driect = signbit(k) ? -1 : 1;
	e = -driect*0.5;
	for (int i = 0; i < dx; ++i) {
		DrawPoint(x, y, r, g, b);
		x += 1;
		e += driect * k;
		
		if (e >= 0) {
			y += driect * 1;
			e -= 1;
		}
		
	}
}

inline void DrawLine(Point p1, Point p2, BYTE r, BYTE g, BYTE b, BYTE a = 0) {
	DrawLine(p1.x, p1.y, p2.x, p2.y, r, g, b,a);
}

inline void DrawLine(Point p1, Point p2, Color color) {
	DrawLine(p1.x, p1.y, p2.x, p2.y, color.r, color.g, color.b, color.a);
}

//base
inline void DrawRect(int x, int y, int width, int height, BYTE r, BYTE g, BYTE b, BYTE a = 0) {
	for (int i = 0; i < height; ++i) {
		int posBuffer = (nScreenWidth * (y + i) + x) * 4;//计算每行的起点
		for (int j = 0; j < width; ++j) {
			DrawPoint(posBuffer, r, g, b);
			posBuffer += 4;
		}
	}
}

inline void DrawRect(RECT rect, Color color) {
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	DrawRect(rect.left, rect.top, width, height, color.r, color.g, color.b, color.a);
}



//uncheck
inline void DrawPic(int x, int y, Picture* pic, RECT rect) {
	int xBuffer = 0, yBuffer = 0;
	int xPic, yPic;

	for (yPic = rect.top; yPic < rect.bottom; ++yPic) {
		int posBuffer = (nScreenWidth * (y + yBuffer++) + x) * 4;
		for (xPic = rect.left; xPic < rect.right; ++xPic) {
			int posPic = pic->getWidth() * yPic + xPic;

			DrawPoint(posBuffer, pic->pChannelR[posPic], pic->pChannelG[posPic], pic->pChannelB[posPic]);

			posBuffer += 4;
			++posPic;
		}
	}
}

//uncheck
inline void DrawPicA(int x, int y, Picture* pic, RECT rect) {
	int xBuffer = 0, yBuffer = 0;
	int xPic, yPic;

	for (yPic = rect.top; yPic < rect.bottom; ++yPic) {
		int posBuffer = (nScreenWidth * (y + yBuffer++) + x) * 4;
		for (xPic = rect.left; xPic < rect.right; ++xPic) {
			int posPic = pic->getWidth() * yPic + xPic;

			DrawPoint(posBuffer, pic->pChannelR[posPic], pic->pChannelG[posPic], pic->pChannelB[posPic], pic->pChannelA[posPic]);
			
			posBuffer += 4;
			++posPic;
		}
	}
}
inline void DrawPic(int x, int y, Picture* pic) {
	RECT rect;
	int _x, _y;
	//纵向检查
	if ((y + pic->getHeight()) < 0) {
		//完全超出上边界
		//不绘制
		return;
	}
	else if (y < 0) {
		//部分超出上边界
		rect.top = -y;
		rect.bottom = pic->getHeight();
		//_x = x;
		_y = 0;
	}
	else if ((y + pic->getHeight()) < nScreenHeight) {
		//完全在窗口内
		rect.top = 0;
		rect.bottom = pic->getHeight();
		//_x = x;
		_y = y;
	}
	else if (y < nScreenHeight) {
		//部分超出下边界
		rect.top = 0;
		rect.bottom = nScreenHeight - y;
		//_x = x;
		_y = y;
	}
	else {
		//完全超出下边界
		//不绘制
		return;
	}

	//横向检查
	if ((x + pic->getWidth()) < 0) {
		//完全超出左边界
		//不绘制
		return;
	}
	else if (x < 0) {
		//部分超出左边界
		rect.left = -x;
		rect.right = pic->getWidth();
		_x = 0;
		//_y = y;
	}
	else if ((x + pic->getWidth()) < nScreenWidth) {
		//完全在窗口内
		rect.left = 0;
		rect.right = pic->getWidth();
		_x = x;
		//_y = y;
	}
	else if (x < nScreenWidth) {
		//部分超出右边界
		rect.left = 0;
		rect.right = nScreenWidth - x;
		_x = x;
		//_y = y;
	}
	else {
		//完全超出下边界
		//不绘制
		return;
	}
	if (pic->isIncludeAlpha()) {
		DrawPicA(_x, _y, pic, rect);
	}
	else {
		DrawPic(_x, _y, pic, rect);
	}
	
}

//Description:
//	P点绕P0顺时针旋转angle
//Paramter: 
//	POINT p		被旋转点
//	POINT p0	中心点
//	float angle	旋转角度
//	POINT& p_	旋转后点
//Return:
//	NONE
inline void spin(Point p, Point p0, float angle, Point& p_) {
	float sinAngle = sin(angle);
	float cosAngle = cos(angle);

	float tempX = p.x * cosAngle + p.y * sinAngle + p0.x * (1 - cosAngle) - p0.y * sinAngle;
	p_.x = tempX + 1;

	float tempY = -p.x * sinAngle + p.y * cosAngle + p0.y * (1 - cosAngle) + p0.x * sinAngle;
	p_.y = tempY + 1;
}

inline void DrawObject(Object* obj) {
	//后期考虑 作为object的成员函数
	float angle = obj->getAngle(1);

	if (angle == 0)//应该可以
	{
		//不需要旋转
		DrawPic(obj->point.x, obj->point.y, obj->pic);
	}
	else {
		

		int nWidth = obj->pic->getWidth();
		int nHeight = obj->pic->getHeight();


		int count = 0;
		int size = nWidth * nHeight;

		float co = cos(angle);
		float si = sin(angle);
		int rotateW, rotateH; //旋转后的宽高

		int xMin, xMax, yMin, yMax;
		float xSrc = 0;
		float ySrc = 0;  //变换后图像的坐标在原图中的坐标

		int xOff = nWidth / 2; //x中心的偏移量
		int yOff = nHeight / 2; //y中心的偏移量

		int nchannel = 3;  //颜色通道


		float a1, a2, a3, a4;


		//旋转后的坐标范围
		rotateH = nWidth * fabs(si) + nHeight * fabs(co);
		rotateW = nWidth * fabs(co) + nHeight * fabs(si);

		yMin = (nHeight - rotateH) / 2;
		yMax = yMin + rotateH + 1;
		xMin = (nWidth - rotateW) / 2;
		xMax = xMin + rotateW + 1;

		int x, y;
		for (y = yMin; y <= yMax; y++)
		{
			for (x = xMin; x < xMax; x++)
			{
				//求在原图中的坐标
				ySrc = si * float(x - xOff) + co * float(y - yOff) + float(int(nHeight / 2));
				xSrc = co * float(x - xOff) - si * float(y - yOff) + float(int(nWidth / 2));

				//如果在原图范围内
				if (ySrc >= 0. && ySrc < nHeight - 0.5 && xSrc >= 0. && xSrc < nWidth - 0.5)
				{
					//开始插值
					int xSmall = floor(xSrc);
					int xBig = ceil(xSrc);
					int ySmall = floor(ySrc);
					int yBig = ceil(ySrc);

					float ux = xSrc - xSmall;
					float uy = ySrc - ySmall;
					float valueTempR;
					float valueTempG;
					float valueTempB;
					float valueTempA;

					Point DrawNowPoint = Point(x, y);
					DrawNowPoint += obj->point;
					int DrawNowPointMemLocation = (DrawNowPoint.y * nScreenWidth + DrawNowPoint.x) * 4;

					a1 = (xSmall >= 0 && ySmall >= 0 ? obj->pic->pChannelR[ySmall * nWidth + xSmall] : 0);
					a2 = (xBig < nWidth&& ySmall >= 0 ? obj->pic->pChannelR[ySmall * nWidth + xBig] : 0);
					a3 = (xSmall >= 0 && yBig < nHeight ? obj->pic->pChannelR[yBig * nWidth + xSmall] : 0);
					a4 = (xBig < nWidth&& yBig < nHeight ? obj->pic->pChannelR[yBig * nWidth + xBig] : 0);

					valueTempR = (1 - ux) * (1 - uy) * a1 + (1 - ux) * uy * a3 + (1 - uy) * ux * a2 + ux * uy * a4;

					a1 = (xSmall >= 0 && ySmall >= 0 ? obj->pic->pChannelG[ySmall * nWidth + xSmall] : 0);
					a2 = (xBig < nWidth&& ySmall >= 0 ? obj->pic->pChannelG[ySmall * nWidth + xBig] : 0);
					a3 = (xSmall >= 0 && yBig < nHeight ? obj->pic->pChannelG[yBig * nWidth + xSmall] : 0);
					a4 = (xBig < nWidth&& yBig < nHeight ? obj->pic->pChannelG[yBig * nWidth + xBig] : 0);

					valueTempG = (1 - ux) * (1 - uy) * a1 + (1 - ux) * uy * a3 + (1 - uy) * ux * a2 + ux * uy * a4+1; //四舍五入

					a1 = (xSmall >= 0 && ySmall >= 0 ? obj->pic->pChannelB[ySmall * nWidth + xSmall] : 0);
					a2 = (xBig < nWidth&& ySmall >= 0 ? obj->pic->pChannelB[ySmall * nWidth + xBig] : 0);
					a3 = (xSmall >= 0 && yBig < nHeight ? obj->pic->pChannelB[yBig * nWidth + xSmall] : 0);
					a4 = (xBig < nWidth&& yBig < nHeight ? obj->pic->pChannelB[yBig * nWidth + xBig] : 0);

					valueTempB = (1 - ux) * (1 - uy) * a1 + (1 - ux) * uy * a3 + (1 - uy) * ux * a2 + ux * uy * a4+1; //四舍五入

					a1 = (xSmall >= 0 && ySmall >= 0 ? obj->pic->pChannelA[ySmall * nWidth + xSmall] : 0);
					a2 = (xBig < nWidth && ySmall >= 0 ? obj->pic->pChannelA[ySmall * nWidth + xBig] : 0);
					a3 = (xSmall >= 0 && yBig < nHeight ? obj->pic->pChannelA[yBig * nWidth + xSmall] : 0);
					a4 = (xBig < nWidth && yBig < nHeight ? obj->pic->pChannelA[yBig * nWidth + xBig] : 0);

					if (a1 == 255 || a2 == 255 || a3 == 255 || a4 == 255)
					{
						valueTempA = 255;
					}
					else
					{
						valueTempA = (1 - ux) * (1 - uy) * a1 + (1 - ux) * uy * a3 + (1 - uy) * ux * a2 + ux * uy * a4; //四舍五入

					}

					Color c = Color(valueTempR, valueTempG, valueTempB,valueTempA);
					//std::cout << valueTempA << std::endl;

					//c = Color(50, 50, 50);
					//std::cout << "DrawPos x" << DrawNowPoint.x << " y " << DrawNowPoint.y << std::endl;
					DrawPoint(DrawNowPointMemLocation, c);
				}
			}
		}
		
		count = size;

	}

}

//Description:
//	更新重画窗口
//	接收消息
//Paramter: 
//	
//Return:
//	
void Draw() {
	void getMessage();

	DrawRect(0, 0, nScreenWidth, nScreenHeight, 192, 224, 0);

	//DrawLine(300, 300, 0, 0, 255, 0, 0);
	//DrawLine(300, 300, 600, 0, 255, 0, 0);
	//DrawLine(300,300, 600, 600, 255, 0, 0);
	//DrawLine(300, 300, 0, 600, 255, 0, 0);

	/*
	Object obj;
	obj.x = 50;
	obj.y = 50;
	obj.z = 1;
	//obj.setAngle(20);
	obj.pic = &picTest;
	drawList.clear();
	AddDrawObject(&obj);
	*/
	/*
	Object obj;
	obj.point.x = 10;
	obj.point.y = 10;
	obj.z = 1;
	//obj.setAngle(20);
	obj.pic = &picField;

	Object obj2;
	obj2.point.x = 10;
	obj2.point.y = 20;
	obj2.z = 2;
	obj2.setAngle(120);
	obj2.pic = &picLove;

	Object obj3;
	obj3.point.x = 10;
	obj3.point.y = 20;
	//obj3.setAngle(-60);
	obj3.z = 1;
	obj3.pic = &picField2;

	drawList.clear();
	AddDrawObject(&obj);
	AddDrawObject(&obj2);
	AddDrawObject(&obj3);
	std::sort(drawList.begin(), drawList.end());
	for (int i = 0; i < drawList.size(); ++i) {
		DrawObject(drawList[i].pObject);
	}*/
	std::sort(drawList.begin(), drawList.end());
	for (int i = 0; i < drawList.size(); ++i) {
		drawList[i].pObject->updatePoint();
		DrawObject(drawList[i].pObject);
	}

	//绘制UI
	/*
	for (int i = 0; i < listBaseUI.size(); ++i) {
		listBaseUI[i]->Draw();
	}*/  //暂不成熟
	for (int i = 0; i < listButton.size(); ++i) {
		listButton[i]->Draw(hMemDC);
	}
	for (int i = 0; i < listLabel.size(); ++i) {
		listLabel[i]->Draw(hMemDC);
	}
	for (int i = 0; i < listEditBox.size(); ++i) {
		listEditBox[i]->Draw(hMemDC);
	}


	//从内存设备拷贝到窗口
	HDC hDC = GetDC(hWnd);
	BitBlt(hDC, 0, 0, nScreenWidth, nScreenHeight, hMemDC, 0, 0, SRCCOPY);
	ReleaseDC(hWnd, hDC);

	getMessage();
}

void getMessage() {
	MSG msg;
	while (1) {
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&msg, NULL, 0, 0)) break;
		DispatchMessage(&msg);
	}
}

void freeSomethingForEngine() {
	//DestroyCaret();//销毁光标
	if (hMemDC) {
		if (hBitmapDevice) {
			SelectObject(hMemDC, hBitmapDevice);
			hBitmapDevice = NULL;
		}
		DeleteDC(hMemDC);
		hMemDC = NULL;
	}
	if (hBitmapBuffer) {
		DeleteObject(hBitmapBuffer);
		hBitmapBuffer = NULL;
	}
	if (hWnd) {
		CloseWindow(hWnd);
		hWnd = NULL;
	}

	//Player::_StaticDestruct();
}


//UI Button

void Button:: Draw(HDC hMemDC) {
	int nOldBkMode = SetBkMode(hMemDC, TRANSPARENT);//绘制字体时设置透明
	DrawRect(rect, currentBackgroundColor);//绘制背景
	DrawEdge(hMemDC, &rect, EDGE_ETCHED, BF_RECT);//传统按钮风格
	COLORREF nOldTextColor = SetTextColor(hMemDC, getForegroundColor().getColorRef());
	DrawText(hMemDC, title.c_str(), -1, &rect, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	SetTextColor(hMemDC, nOldTextColor);
	SetBkMode(hMemDC, nOldBkMode);//恢复模式
}

//Description:
//	增加一个按钮对象,如果该对象已被加入，则不会重复加入.
//Paramter: 
//	Button * button 将要加入对象的地址
//Return Value:
//	NONE
void AddButton(Button* button) {
	bool isExist = false;
	for (int i = 0; i < listButton.size(); ++i) {
		if (listButton[i] == button) {
			isExist = true;
			break;
		}
	}
	if (!isExist) {
		listButton.push_back(button);
	}
}

//Description:
//	从按钮对象列表中删掉一个对象。
//Paramter: 
//	Button * button 将要删除的对象地址
//Return Value:
//	NONE
void RemoveButton(Button* button)
{
	std::vector<Button*>::iterator it;
	for (it = listButton.begin(); it != listButton.end(); ++it) {
		if ((*it) == button) {
			listButton.erase(it);
			break;
		}
	}
}

//UI Label

void Label::Draw(HDC hMemDC) {
	int nOldBkMode = SetBkMode(hMemDC, TRANSPARENT);//绘制字体时设置透明
	DrawRect(rect, currentBackgroundColor);
	COLORREF nOldTextColor = SetTextColor(hMemDC, getForegroundColor().getColorRef());
	DrawText(hMemDC, title.c_str(), -1, &rect, DT_CENTER | DT_EXPANDTABS);
	SetTextColor(hMemDC, nOldTextColor);
	SetBkMode(hMemDC, nOldBkMode);//恢复模式
}

//Description:
//	增加一个标签对象,如果该对象已被加入，则不会重复加入.
//Paramter: 
//	Label* label 将要加入对象的地址
//Return Value:
//	NONE
void AddLabel(Label* label)
{
	bool isExist = false;
	for (int i = 0; i < listLabel.size(); ++i) {
		if (listLabel[i] == label) {
			isExist = true;
			break;
		}
	}
	if (!isExist) {
		listLabel.push_back(label);
	}
}

//Description:
//	从标签对象列表中删掉一个对象。
//Paramter: 
//	Label* label 将要删除的对象地址
//Return Value:
//	NONE
void RemoveLabel(Label* label)
{
	std::vector<Label*>::iterator it;
	for (it = listLabel.begin(); it != listLabel.end(); ++it) {
		if ((*it) == label) {
			listLabel.erase(it);
			break;
		}
	}
}


//UI EditBox

void EditBox::Draw(HDC hMemDC) {
	int nOldBkMode = SetBkMode(hMemDC, TRANSPARENT);//绘制字体时设置透明
	DrawRect(rect, currentBackgroundColor);
	DrawEdge(hMemDC, &rect, EDGE_SUNKEN, BF_RECT);//传统编辑框风格

	COLORREF nOldTextColor = SetTextColor(hMemDC, getForegroundColor().getColorRef());
	DrawText(hMemDC, text.c_str(), -1, &rect, DT_VCENTER | DT_SINGLELINE);
	SetTextColor(hMemDC, nOldTextColor);

	//判断是否需要绘制光标
	if (isShowCaret) {
		if ((sumFPS++) > (frame * 5)) {
			stateCaret = !stateCaret;
		}
		//光标闪烁
		if (stateCaret) {
			//显示
			DrawLine(pointCaret.x, pointCaret.y - getHeight(),
				pointCaret.x, pointCaret.y,
				0, 0, 0);
		}
		else {
			//
		}
	}
	SetBkMode(hMemDC, nOldBkMode);//恢复模式
}
//Description:
//	增加一个编辑框对象,如果该对象已被加入，则不会重复加入.
//Paramter: 
//	Label* label 将要加入对象的地址
//Return Value:
//	NONE
void AddEditBox(EditBox* editBox)
{
	bool isExist = false;
	for (int i = 0; i < listEditBox.size(); ++i) {
		if (listEditBox[i] == editBox) {
			isExist = true;
			break;
		}
	}
	if (!isExist) {
		listEditBox.push_back(editBox);
	}
}

//Description:
//	从编辑框对象列表中删掉一个对象。
//Paramter: 
//	EditBox* editBox 将要删除的对象地址
//Return Value:
//	NONE
void RemoveEditBox(EditBox* editBox)
{
	std::vector<EditBox*>::iterator it;
	for (it = listEditBox.begin(); it != listEditBox.end(); ++it) {
		if ((*it) == editBox) {
			listEditBox.erase(it);
			break;
		}
	}
}



//处理消息
static LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CLOSE: {
		//DebugPrintln("%d\t%d", wParam, wParam & 511);
		break;
	}
	case WM_CHAR: {
		Point p(lParam & 0x0000ffff, lParam >> 16);

		TCHAR code = wParam;

		//遍历EditBox
		for (int i = 0; i < listEditBox.size(); ++i) {
			if ((*listEditBox[i]) == p) {//寻找鼠标点击的点所在的EditBox
				listEditBox[i]->text.insert(listEditBox[i]->nPosCaret, 1, (char)code);
				listEditBox[i]->moveCaret(1);

				break;
			}
		}
		break;
	}
	case WM_KEYDOWN: {
		//遍历EditBox
		for (int i = 0; i < listEditBox.size(); ++i) {
			if (listEditBox[i]->isShowCaret) {//寻找鼠标点击的点所在的EditBox
				return 0;//不再响应热键消息
			}
		}

		screen_keys[wParam & 511] = 1;
		break;
	}
	case WM_KEYUP: {
		//遍历EditBox
		for (int i = 0; i < listEditBox.size(); ++i) {
			if (listEditBox[i]->isShowCaret) {//寻找鼠标点击的点所在的EditBox
				switch (wParam)
				{
				case VK_LEFT: {
					listEditBox[i]->moveCaret(-1);
					break;
				}
				case VK_RIGHT: {
					listEditBox[i]->moveCaret(1);
					break;
				}
				case VK_DELETE: {
					if (listEditBox[i]->text.length() == 0) {
						break;
					}
					if (listEditBox[i]->nPosCaret < listEditBox[i]->text.length()) {
						listEditBox[i]->text.erase(listEditBox[i]->nPosCaret, 1);
					}
					break;
				}
				case VK_BACK: {
					if (listEditBox[i]->text.length() == 0) {
						break;
					}
					if (listEditBox[i]->nPosCaret!=0) {
						listEditBox[i]->text.erase(--listEditBox[i]->nPosCaret, 1);
					}
					break;
				}
				default:
					switch (listEditBox[i]->getInputType())
					{
					case EB_IT_TEXT: {
						//文本类型
						if (wParam <= 'Z' && wParam >= 'A') {
							listEditBox[i]->text.insert(listEditBox[i]->nPosCaret, 1, (char)wParam);
							listEditBox[i]->moveCaret(1);
							if (listEditBox[i]->lpValueChange != NULL) {
								listEditBox[i]->lpValueChange();
							}
						}
					}
					case EB_IT_NUMBER: {
						//数字类型
						if (wParam <= '9' && wParam >= '0') {
							listEditBox[i]->text.insert(listEditBox[i]->nPosCaret, 1, (char)wParam);
							listEditBox[i]->moveCaret(1);
							if (listEditBox[i]->lpValueChange != NULL) {
								listEditBox[i]->lpValueChange();
							}
						}
					}
					default:
						break;
					}
					
				}
				//break;//不再响应热键消息


				HDC hDC = GetDC(hWnd);
				int strLength = listEditBox[i]->text.length();
				SIZE size = { 0 };
				int nPosStr = 0;
				for (nPosStr = 0; nPosStr <= strLength; ++nPosStr) {
					GetTextExtentPoint(hDC, listEditBox[i]->text.c_str(), nPosStr, &size);//返回当前字符的起始位置
					if (listEditBox[i]->nPosCaret <= nPosStr) {
						break;
					}
				}

				size.cy = listEditBox[i]->getHeight();
				if (nPosStr == 0) {
					size.cx = 0;
				}

				Point newPoint = { size.cx  ,size.cy };
				newPoint + listEditBox[i]->getRect();
				listEditBox[i]->pointCaret = newPoint;//设置光标位置
				//listEditBox[i]->isShowCaret = true;//显示光标

				ReleaseDC(hWnd, hDC);
				return 0;//不再响应热键消息
			}
		}
		
		screen_keys[wParam & 511] = 0;
		break;
	}
	case WM_MOUSEMOVE: {
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_HOVER;
		tme.dwHoverTime = 1;
		tme.hwndTrack = hWnd;
		TrackMouseEvent(&tme);//注册鼠标悬停消息

		//tme.dwFlags = TME_LEAVE;
		//TrackMouseEvent(&tme);//注册鼠标离开消息  好像没什么用
		//DebugPrintln("WM_MOUSEMOVE");
		break;
	}
	case WM_MOUSEHOVER: {
		//判断是否在ROI
		Point p(lParam & 0x0000ffff, lParam >> 16);

		for (int i = 0; i < listButton.size(); ++i) {
			if ((*listButton[i]) == p) {//寻找鼠标停留点所在的按钮
				listButton[i]->currentBackgroundColor = DEFAULT_FOCUS_BC;//悬停在按钮上时  修改背景颜色
			}
			else {
				listButton[i]->currentBackgroundColor = listButton[i]->getBackgroundColor();//离开时恢复
			}
		}
		//DebugPrintln("WM_MOUSEHOVER %d %d ",p.x,p.y);
		break;
	}
	case WM_NCMOUSEHOVER: {
		DebugPrintln("WM_NCMOUSEHOVER");
		break;
	}
	case WM_LBUTTONDOWN: {
		//DebugPrintln("WM_LBUTTONDOWN");
		//坐标  在lparam 中
		//高16位是y 低16位是x
		//DebugPrintln("%d %d", lParam >> 16, lParam & 0x0000ffff);
		Point p(lParam & 0x0000ffff, lParam >> 16);

		//遍历Button
		for (int i = 0; i < listButton.size(); ++i) {
			if ((*listButton[i]) == p) {//寻找鼠标点击的点所在的按钮
				listButton[i]->isClick = true;
				listButton[i]->currentBackgroundColor = DEFAULT_DOWN_BC;
				//DebugPrintln("WM_LBUTTONDOWN");
				//break;
				return 0;
			}
		}

		//遍历EditBox
		for (int i = 0; i < listEditBox.size(); ++i) {
			if ((*listEditBox[i]) == p) {//寻找鼠标点击的点所在的EditBox
				//鼠标点击了 该编辑框 计算光标位置
				Point pointForRect = p;
				pointForRect - listEditBox[i]->rect;//相对于RECT的坐标

				HDC hDC = GetDC(hWnd);
				int strLength = listEditBox[i]->text.length();
				SIZE size = { 0 };
				int nPosStr = 0;
				for (nPosStr = 0; nPosStr < strLength; ++nPosStr) {
					GetTextExtentPoint(hDC, listEditBox[i]->text.c_str(), nPosStr, &size);//返回当前字符的起始位置
					if (pointForRect.x < size.cx) {
						++nPosStr;
						break;
					}
				}

				size.cy = listEditBox[i]->getHeight();
				if (nPosStr == 0) {
					size.cx = 0;
					
				}
				else {
					SIZE size2 = { 0 };
					GetTextExtentPoint(hDC, listEditBox[i]->text.c_str(), nPosStr, &size2);
					//判断鼠标点击位置距离哪个字符更近
					if ((pointForRect.x - size.cx) < (abs(size2.cx - pointForRect.x))) {
						//前者
						listEditBox[i]->nPosCaret = nPosStr-1;
					}
					else {
						//距离后者更近
						size.cx = size2.cx;
						listEditBox[i]->nPosCaret = nPosStr ;
					}
				}
				pointForRect.x = size.cx;
				pointForRect.y = size.cy;
				pointForRect + listEditBox[i]->rect;//相对于的坐标
				
				
				listEditBox[i]->pointCaret = pointForRect;//设置光标位置
				listEditBox[i]->isShowCaret = true;//显示光标

				ReleaseDC(hWnd, hDC);
			}
			else {
				listEditBox[i]->isShowCaret = false;//隐藏光标
			}
		}
		break;
	}
	case WM_LBUTTONUP: {
		//DebugPrintln("WM_LBUTTONUP");
		//坐标  在lparam 中
		//高16位是y 低16位是x
		//DebugPrintln("%d %d", lParam >> 16, lParam & 0x0000ffff);
		Point p(lParam & 0x0000ffff, lParam >> 16);

		for (int i = 0; i < listButton.size(); ++i) {
			if ((*listButton[i]) == p) {//寻找当前点所对应的button
				if (listButton[i]->isClick) {//判断 down 和 up 是同一个button
					if (listButton[i]->lpClickL != NULL) {//判断该事件是否被定义
						//DebugPrintln("WM_LBUTTONUP");
						listButton[i]->lpClickL();//调用该按钮所定义的函数
					}
					listButton[i]->isClick = false;
					listButton[i]->currentBackgroundColor = listButton[i]->getBackgroundColor();//单击之后恢复
					break;
				}
				break;//up 消息与 down 不是同一个button
			}
		}
		break;
	}
	
	default: {
		//DebugPrintln("%x", message);
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}
	return 0;
}

//Description:
//	创建并维护一个窗口,目前不对参数合法性进行检查.
//	该窗口显示的内容依赖于window结构体里所指向的img
//Paramter: 
//	LPVOID window一个指向Window结构体的指针
//Return:
//	0:		正常
//	other:	由GetLastError()返回的错误代码
int _CreateWindow(const char* title, int nWidth, int nHeight) {
	HINSTANCE hInstance = GetModuleHandle(NULL);

	WNDCLASS wc;
	memset(&wc, 0, sizeof(wc));
	wc.style = CS_BYTEALIGNCLIENT;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.hInstance = hInstance;//GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = (char*)title;
	wc.lpszMenuName = (char*)title;

	RegisterClass(&wc);

	//Create a new window
	hWnd = CreateWindow(
		(char*)title,//class name
		(char*)title,//title
		WS_OVERLAPPEDWINDOW,//style
		CW_USEDEFAULT, CW_USEDEFAULT,//position
		nWidth, nHeight,//size
		NULL,//parent window
		NULL,//menu
		hInstance,//application instance
		NULL//prarameters
	);
	if (hWnd == 0) {
		return GetLastError();
	}

	//创建内存缓冲画板
	HDC hDC = GetDC(hWnd);
	hMemDC = CreateCompatibleDC(hDC);//创建与窗口兼容的内存设备上下文环境(创建内存缓冲画板)
	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), nWidth, -nHeight, 1, 32, BI_RGB,//BI_BITFIELDS 无法使用
		nWidth * nHeight * 4, 0, 0, 0, 0 } };
	LPVOID ptr;
	hBitmapBuffer = CreateDIBSection(hMemDC, &bi, DIB_RGB_COLORS, &ptr, 0, 0);//创建缓冲区画布
	DebugPrintln("%d",GetLastError());

	hBitmapDevice = (HBITMAP)SelectObject(hMemDC, hBitmapBuffer);//选择缓冲区画布到缓冲区画板，并保留原始画布
	buffer = (BYTE*)ptr;
	ReleaseDC(hWnd, hDC);

	//CreateCaret(hWnd, NULL, 1, 10);

	ShowWindow(hWnd, SW_NORMAL);
	UpdateWindow(hWnd);

	return 0;
}

