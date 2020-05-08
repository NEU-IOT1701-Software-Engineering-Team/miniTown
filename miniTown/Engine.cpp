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
//���ƶ���
std::vector<ObjectPointer> drawList;
//std::vector<BaseUI*> listBaseUI;
std::vector<Button*> listButton;
std::vector<Label*> listLabel;
std::vector<EditBox*> listEditBox;



//Description:
//	����ƶ����м���һ���¶���,����ö����ѱ����룬�򲻻��ظ�����.
//Paramter: 
//	Object* object ��Ҫ�������ĵ�ַ
//Return Value:
//	NONE
void AddDrawObject(Object* object)
{
	ObjectPointer op(object);
	bool isExist = false;//�ö����Ƿ��Ѵ���
	for (std::vector<ObjectPointer>::iterator it = drawList.begin();
		it != drawList.end(); ++it) {
		if ((*it) == op) {
			isExist = true;
			break;
		}
	}
	if (!isExist) {
		//��������Ҫ���
		drawList.push_back(op);
	}
}

//Description:
//	�ӻ��ƶ�����ɾ��һ������
//Paramter: 
//	Object* object ��Ҫɾ���Ķ����ַ
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
		int posBuffer = (nScreenWidth * (y + i) + x) * 4;//����ÿ�е����
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
	//������
	if ((y + pic->getHeight()) < 0) {
		//��ȫ�����ϱ߽�
		//������
		return;
	}
	else if (y < 0) {
		//���ֳ����ϱ߽�
		rect.top = -y;
		rect.bottom = pic->getHeight();
		//_x = x;
		_y = 0;
	}
	else if ((y + pic->getHeight()) < nScreenHeight) {
		//��ȫ�ڴ�����
		rect.top = 0;
		rect.bottom = pic->getHeight();
		//_x = x;
		_y = y;
	}
	else if (y < nScreenHeight) {
		//���ֳ����±߽�
		rect.top = 0;
		rect.bottom = nScreenHeight - y;
		//_x = x;
		_y = y;
	}
	else {
		//��ȫ�����±߽�
		//������
		return;
	}

	//������
	if ((x + pic->getWidth()) < 0) {
		//��ȫ������߽�
		//������
		return;
	}
	else if (x < 0) {
		//���ֳ�����߽�
		rect.left = -x;
		rect.right = pic->getWidth();
		_x = 0;
		//_y = y;
	}
	else if ((x + pic->getWidth()) < nScreenWidth) {
		//��ȫ�ڴ�����
		rect.left = 0;
		rect.right = pic->getWidth();
		_x = x;
		//_y = y;
	}
	else if (x < nScreenWidth) {
		//���ֳ����ұ߽�
		rect.left = 0;
		rect.right = nScreenWidth - x;
		_x = x;
		//_y = y;
	}
	else {
		//��ȫ�����±߽�
		//������
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
//	P����P0˳ʱ����תangle
//Paramter: 
//	POINT p		����ת��
//	POINT p0	���ĵ�
//	float angle	��ת�Ƕ�
//	POINT& p_	��ת���
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
	//���ڿ��� ��Ϊobject�ĳ�Ա����
	float angle = obj->getAngle(1);

	if (angle == 0)//Ӧ�ÿ���
	{
		//����Ҫ��ת
		DrawPic(obj->point.x, obj->point.y, obj->pic);
	}
	else {
		

		int nWidth = obj->pic->getWidth();
		int nHeight = obj->pic->getHeight();


		int count = 0;
		int size = nWidth * nHeight;

		float co = cos(angle);
		float si = sin(angle);
		int rotateW, rotateH; //��ת��Ŀ��

		int xMin, xMax, yMin, yMax;
		float xSrc = 0;
		float ySrc = 0;  //�任��ͼ���������ԭͼ�е�����

		int xOff = nWidth / 2; //x���ĵ�ƫ����
		int yOff = nHeight / 2; //y���ĵ�ƫ����

		int nchannel = 3;  //��ɫͨ��


		float a1, a2, a3, a4;


		//��ת������귶Χ
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
				//����ԭͼ�е�����
				ySrc = si * float(x - xOff) + co * float(y - yOff) + float(int(nHeight / 2));
				xSrc = co * float(x - xOff) - si * float(y - yOff) + float(int(nWidth / 2));

				//�����ԭͼ��Χ��
				if (ySrc >= 0. && ySrc < nHeight - 0.5 && xSrc >= 0. && xSrc < nWidth - 0.5)
				{
					//��ʼ��ֵ
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

					valueTempG = (1 - ux) * (1 - uy) * a1 + (1 - ux) * uy * a3 + (1 - uy) * ux * a2 + ux * uy * a4+1; //��������

					a1 = (xSmall >= 0 && ySmall >= 0 ? obj->pic->pChannelB[ySmall * nWidth + xSmall] : 0);
					a2 = (xBig < nWidth&& ySmall >= 0 ? obj->pic->pChannelB[ySmall * nWidth + xBig] : 0);
					a3 = (xSmall >= 0 && yBig < nHeight ? obj->pic->pChannelB[yBig * nWidth + xSmall] : 0);
					a4 = (xBig < nWidth&& yBig < nHeight ? obj->pic->pChannelB[yBig * nWidth + xBig] : 0);

					valueTempB = (1 - ux) * (1 - uy) * a1 + (1 - ux) * uy * a3 + (1 - uy) * ux * a2 + ux * uy * a4+1; //��������

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
						valueTempA = (1 - ux) * (1 - uy) * a1 + (1 - ux) * uy * a3 + (1 - uy) * ux * a2 + ux * uy * a4; //��������

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
//	�����ػ�����
//	������Ϣ
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

	//����UI
	/*
	for (int i = 0; i < listBaseUI.size(); ++i) {
		listBaseUI[i]->Draw();
	}*/  //�ݲ�����
	for (int i = 0; i < listButton.size(); ++i) {
		listButton[i]->Draw(hMemDC);
	}
	for (int i = 0; i < listLabel.size(); ++i) {
		listLabel[i]->Draw(hMemDC);
	}
	for (int i = 0; i < listEditBox.size(); ++i) {
		listEditBox[i]->Draw(hMemDC);
	}


	//���ڴ��豸����������
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
	//DestroyCaret();//���ٹ��
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
	int nOldBkMode = SetBkMode(hMemDC, TRANSPARENT);//��������ʱ����͸��
	DrawRect(rect, currentBackgroundColor);//���Ʊ���
	DrawEdge(hMemDC, &rect, EDGE_ETCHED, BF_RECT);//��ͳ��ť���
	COLORREF nOldTextColor = SetTextColor(hMemDC, getForegroundColor().getColorRef());
	DrawText(hMemDC, title.c_str(), -1, &rect, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	SetTextColor(hMemDC, nOldTextColor);
	SetBkMode(hMemDC, nOldBkMode);//�ָ�ģʽ
}

//Description:
//	����һ����ť����,����ö����ѱ����룬�򲻻��ظ�����.
//Paramter: 
//	Button * button ��Ҫ�������ĵ�ַ
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
//	�Ӱ�ť�����б���ɾ��һ������
//Paramter: 
//	Button * button ��Ҫɾ���Ķ����ַ
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
	int nOldBkMode = SetBkMode(hMemDC, TRANSPARENT);//��������ʱ����͸��
	DrawRect(rect, currentBackgroundColor);
	COLORREF nOldTextColor = SetTextColor(hMemDC, getForegroundColor().getColorRef());
	DrawText(hMemDC, title.c_str(), -1, &rect, DT_CENTER | DT_EXPANDTABS);
	SetTextColor(hMemDC, nOldTextColor);
	SetBkMode(hMemDC, nOldBkMode);//�ָ�ģʽ
}

//Description:
//	����һ����ǩ����,����ö����ѱ����룬�򲻻��ظ�����.
//Paramter: 
//	Label* label ��Ҫ�������ĵ�ַ
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
//	�ӱ�ǩ�����б���ɾ��һ������
//Paramter: 
//	Label* label ��Ҫɾ���Ķ����ַ
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
	int nOldBkMode = SetBkMode(hMemDC, TRANSPARENT);//��������ʱ����͸��
	DrawRect(rect, currentBackgroundColor);
	DrawEdge(hMemDC, &rect, EDGE_SUNKEN, BF_RECT);//��ͳ�༭����

	COLORREF nOldTextColor = SetTextColor(hMemDC, getForegroundColor().getColorRef());
	DrawText(hMemDC, text.c_str(), -1, &rect, DT_VCENTER | DT_SINGLELINE);
	SetTextColor(hMemDC, nOldTextColor);

	//�ж��Ƿ���Ҫ���ƹ��
	if (isShowCaret) {
		if ((sumFPS++) > (frame * 5)) {
			stateCaret = !stateCaret;
		}
		//�����˸
		if (stateCaret) {
			//��ʾ
			DrawLine(pointCaret.x, pointCaret.y - getHeight(),
				pointCaret.x, pointCaret.y,
				0, 0, 0);
		}
		else {
			//
		}
	}
	SetBkMode(hMemDC, nOldBkMode);//�ָ�ģʽ
}
//Description:
//	����һ���༭�����,����ö����ѱ����룬�򲻻��ظ�����.
//Paramter: 
//	Label* label ��Ҫ�������ĵ�ַ
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
//	�ӱ༭������б���ɾ��һ������
//Paramter: 
//	EditBox* editBox ��Ҫɾ���Ķ����ַ
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



//������Ϣ
static LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CLOSE: {
		//DebugPrintln("%d\t%d", wParam, wParam & 511);
		break;
	}
	case WM_CHAR: {
		Point p(lParam & 0x0000ffff, lParam >> 16);

		TCHAR code = wParam;

		//����EditBox
		for (int i = 0; i < listEditBox.size(); ++i) {
			if ((*listEditBox[i]) == p) {//Ѱ��������ĵ����ڵ�EditBox
				listEditBox[i]->text.insert(listEditBox[i]->nPosCaret, 1, (char)code);
				listEditBox[i]->moveCaret(1);

				break;
			}
		}
		break;
	}
	case WM_KEYDOWN: {
		//����EditBox
		for (int i = 0; i < listEditBox.size(); ++i) {
			if (listEditBox[i]->isShowCaret) {//Ѱ��������ĵ����ڵ�EditBox
				return 0;//������Ӧ�ȼ���Ϣ
			}
		}

		screen_keys[wParam & 511] = 1;
		break;
	}
	case WM_KEYUP: {
		//����EditBox
		for (int i = 0; i < listEditBox.size(); ++i) {
			if (listEditBox[i]->isShowCaret) {//Ѱ��������ĵ����ڵ�EditBox
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
						//�ı�����
						if (wParam <= 'Z' && wParam >= 'A') {
							listEditBox[i]->text.insert(listEditBox[i]->nPosCaret, 1, (char)wParam);
							listEditBox[i]->moveCaret(1);
							if (listEditBox[i]->lpValueChange != NULL) {
								listEditBox[i]->lpValueChange();
							}
						}
					}
					case EB_IT_NUMBER: {
						//��������
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
				//break;//������Ӧ�ȼ���Ϣ


				HDC hDC = GetDC(hWnd);
				int strLength = listEditBox[i]->text.length();
				SIZE size = { 0 };
				int nPosStr = 0;
				for (nPosStr = 0; nPosStr <= strLength; ++nPosStr) {
					GetTextExtentPoint(hDC, listEditBox[i]->text.c_str(), nPosStr, &size);//���ص�ǰ�ַ�����ʼλ��
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
				listEditBox[i]->pointCaret = newPoint;//���ù��λ��
				//listEditBox[i]->isShowCaret = true;//��ʾ���

				ReleaseDC(hWnd, hDC);
				return 0;//������Ӧ�ȼ���Ϣ
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
		TrackMouseEvent(&tme);//ע�������ͣ��Ϣ

		//tme.dwFlags = TME_LEAVE;
		//TrackMouseEvent(&tme);//ע������뿪��Ϣ  ����ûʲô��
		//DebugPrintln("WM_MOUSEMOVE");
		break;
	}
	case WM_MOUSEHOVER: {
		//�ж��Ƿ���ROI
		Point p(lParam & 0x0000ffff, lParam >> 16);

		for (int i = 0; i < listButton.size(); ++i) {
			if ((*listButton[i]) == p) {//Ѱ�����ͣ�������ڵİ�ť
				listButton[i]->currentBackgroundColor = DEFAULT_FOCUS_BC;//��ͣ�ڰ�ť��ʱ  �޸ı�����ɫ
			}
			else {
				listButton[i]->currentBackgroundColor = listButton[i]->getBackgroundColor();//�뿪ʱ�ָ�
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
		//����  ��lparam ��
		//��16λ��y ��16λ��x
		//DebugPrintln("%d %d", lParam >> 16, lParam & 0x0000ffff);
		Point p(lParam & 0x0000ffff, lParam >> 16);

		//����Button
		for (int i = 0; i < listButton.size(); ++i) {
			if ((*listButton[i]) == p) {//Ѱ��������ĵ����ڵİ�ť
				listButton[i]->isClick = true;
				listButton[i]->currentBackgroundColor = DEFAULT_DOWN_BC;
				//DebugPrintln("WM_LBUTTONDOWN");
				//break;
				return 0;
			}
		}

		//����EditBox
		for (int i = 0; i < listEditBox.size(); ++i) {
			if ((*listEditBox[i]) == p) {//Ѱ��������ĵ����ڵ�EditBox
				//������� �ñ༭�� ������λ��
				Point pointForRect = p;
				pointForRect - listEditBox[i]->rect;//�����RECT������

				HDC hDC = GetDC(hWnd);
				int strLength = listEditBox[i]->text.length();
				SIZE size = { 0 };
				int nPosStr = 0;
				for (nPosStr = 0; nPosStr < strLength; ++nPosStr) {
					GetTextExtentPoint(hDC, listEditBox[i]->text.c_str(), nPosStr, &size);//���ص�ǰ�ַ�����ʼλ��
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
					//�ж������λ�þ����ĸ��ַ�����
					if ((pointForRect.x - size.cx) < (abs(size2.cx - pointForRect.x))) {
						//ǰ��
						listEditBox[i]->nPosCaret = nPosStr-1;
					}
					else {
						//������߸���
						size.cx = size2.cx;
						listEditBox[i]->nPosCaret = nPosStr ;
					}
				}
				pointForRect.x = size.cx;
				pointForRect.y = size.cy;
				pointForRect + listEditBox[i]->rect;//����ڵ�����
				
				
				listEditBox[i]->pointCaret = pointForRect;//���ù��λ��
				listEditBox[i]->isShowCaret = true;//��ʾ���

				ReleaseDC(hWnd, hDC);
			}
			else {
				listEditBox[i]->isShowCaret = false;//���ع��
			}
		}
		break;
	}
	case WM_LBUTTONUP: {
		//DebugPrintln("WM_LBUTTONUP");
		//����  ��lparam ��
		//��16λ��y ��16λ��x
		//DebugPrintln("%d %d", lParam >> 16, lParam & 0x0000ffff);
		Point p(lParam & 0x0000ffff, lParam >> 16);

		for (int i = 0; i < listButton.size(); ++i) {
			if ((*listButton[i]) == p) {//Ѱ�ҵ�ǰ������Ӧ��button
				if (listButton[i]->isClick) {//�ж� down �� up ��ͬһ��button
					if (listButton[i]->lpClickL != NULL) {//�жϸ��¼��Ƿ񱻶���
						//DebugPrintln("WM_LBUTTONUP");
						listButton[i]->lpClickL();//���øð�ť������ĺ���
					}
					listButton[i]->isClick = false;
					listButton[i]->currentBackgroundColor = listButton[i]->getBackgroundColor();//����֮��ָ�
					break;
				}
				break;//up ��Ϣ�� down ����ͬһ��button
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
//	������ά��һ������,Ŀǰ���Բ����Ϸ��Խ��м��.
//	�ô�����ʾ������������window�ṹ������ָ���img
//Paramter: 
//	LPVOID windowһ��ָ��Window�ṹ���ָ��
//Return:
//	0:		����
//	other:	��GetLastError()���صĴ������
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

	//�����ڴ滺�廭��
	HDC hDC = GetDC(hWnd);
	hMemDC = CreateCompatibleDC(hDC);//�����봰�ڼ��ݵ��ڴ��豸�����Ļ���(�����ڴ滺�廭��)
	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), nWidth, -nHeight, 1, 32, BI_RGB,//BI_BITFIELDS �޷�ʹ��
		nWidth * nHeight * 4, 0, 0, 0, 0 } };
	LPVOID ptr;
	hBitmapBuffer = CreateDIBSection(hMemDC, &bi, DIB_RGB_COLORS, &ptr, 0, 0);//��������������
	DebugPrintln("%d",GetLastError());

	hBitmapDevice = (HBITMAP)SelectObject(hMemDC, hBitmapBuffer);//ѡ�񻺳������������������壬������ԭʼ����
	buffer = (BYTE*)ptr;
	ReleaseDC(hWnd, hDC);

	//CreateCaret(hWnd, NULL, 1, 10);

	ShowWindow(hWnd, SW_NORMAL);
	UpdateWindow(hWnd);

	return 0;
}

