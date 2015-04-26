/*
* Copyright 2014.215B������
* All rights reserved.
*
* �ļ����ƣ�Attendance_Defination.cpp
* ժ	Ҫ������ϯ���ͳ�ơ����̺���ʵ��
*
* ��ǰ�汾��1.0
* ��	��: chendaxixi/���ٱ�
* ������ڣ�2014��4��17��
*
* ȡ���汾��1.0
* ԭ����  ��chendaxixi/���ٱ�
* ������ڣ�2014��4��17��
*/

#include "stdafx.h"
#include "Attendance.h"

BOOL		people[MAX_PEOPLE];						// ��¼�������
int 		size;									// ��Ա��ģ
char		str_in[MAX_CHARIN];						// ��ǰ�����ַ���
int 		str_len;								// ��ǰ�����ַ�������
char*		strHint;								// ��ʾ�ַ���
int			strHint_len;							// ��ʾ�ַ�������
char		buffer[MAX_CHARIN];						// �ַ���������
BOOL		flag = true;							// �ж��Ƿ��½�
RECT		workRegion;								// ��������
BOOL		Stop;									// �ж��Ƿ��ֹ
int 		HandleCode;								// ��ǰ�������
int 		picture_num;							// ��ʾ��ͼƬ���
int			region_x1[5] = {0, 0, 0, 8, 8};
int			region_y1[5] = {0, 1, 3, 1, 11};
int			region_x2[5] = {25, 8, 8, 25, 25};
int			region_y2[5] = {1, 3, 20, 11, 20};

void Init(HWND hWnd)
{
	size = 0;
	str_len = 0;
	str_in[0] = 0;
	strHint = NULL;
	strHint_len = 0;
	Stop = false;
	HandleCode = 0;
	if (flag)
	{
		workRegion.left = 0;
		workRegion.bottom = 0;
		workRegion.right = nWidth * GRID;
		workRegion.top = nHeight * GRID;
		srand((unsigned)time(NULL));
		picture_num = 0;
		flag = false;
	}
}

void HandleKeyDown(HWND hWnd, WPARAM wParam)
{
	switch(wParam)
	{
	case VK_F1:
		Init(hWnd);
		InvalidateRect(hWnd, &workRegion, TRUE);
		break;
	case VK_F3:
		Stop = true;
		strHint = "��ǰ�ѽ�ֹ";
		strHint_len = 5;
		if (HandleCode == 2) HandleCode = 0;
		InvalidateRect(hWnd, &workRegion, TRUE);
		break;
	case VK_F8:
		Stop = false;
		strHint = "������¼";
		strHint_len = 4;
		if (HandleCode == 2) HandleCode = 0;
		InvalidateRect(hWnd, &workRegion, TRUE);
		break;
	case VK_F2:
		SetSize(hWnd);
		break;
	case VK_ESCAPE:
		PostQuitMessage(0);
		break;
	case VK_F5:
		Save(hWnd);
		break;
	case VK_F6:
		Load(hWnd);
		break;
	case VK_F7:
		PutResult(hWnd);
		break;
	case VK_RETURN:
		HandleStr(hWnd);
		break;
	case VK_BACK:
		if (str_len > 0)
		{	
			str_len--;
			str_in[str_len] = 0;
			InvalidateRect(hWnd, &workRegion, TRUE);
		}
		break;
	default:
		Add(hWnd, wParam);
		break;
	}
}

void SetSize(HWND hWnd)
{
	strHint = "���������������Ǹ�����)";
	strHint_len = 12;
	HandleCode = 1;
	InvalidateRect(hWnd, &workRegion, TRUE);
}

void Save(HWND hWnd)
{
	ofstream* fileOut = new ofstream(DATAFILENAME, ios::out);
	if (fileOut->fail())
	{
		strHint = "�������ʧ��";
		strHint_len = 6;
		if (HandleCode == 2) HandleCode = 0;
		InvalidateRect(hWnd, &workRegion, TRUE);
	}
	else
	{
		(*fileOut) << size << endl;
		int i = 0;
		for (i = 0;i < size;i++)
			(*fileOut) << people[i] << " ";
		(*fileOut) << endl;
		fileOut->close();
		delete fileOut;
		strHint = "�������ɹ�";
		strHint_len = 6;
		if (HandleCode == 2) HandleCode = 0;
		InvalidateRect(hWnd, &workRegion, TRUE);
	}
}

void Load(HWND hWnd)
{
	ifstream* fileIn = new ifstream(DATAFILENAME, ios::in);
	if (fileIn->fail())
	{
		strHint = "���ݴ�ʧ��";
		strHint_len = 6;
		if (HandleCode == 2) HandleCode = 0;
		InvalidateRect(hWnd, &workRegion, TRUE);
	}
	else
	{
		(*fileIn) >> size;
		int i = 0;
		for (i = 0;i < size;i++)
			(*fileIn) >> people[i];
		fileIn->close();
		delete fileIn;
		strHint = "���ݶ�ȡ�ɹ�";
		strHint_len = 6;
		if (HandleCode == 2) HandleCode = 0;
		InvalidateRect(hWnd, &workRegion, TRUE);
	}
}

void PutResult(HWND hWnd)
{
	ofstream* fileOut = new ofstream(RESULTFILENAME, ios::out);
	if (fileOut->fail())
	{
		strHint = "������ʧ��";
		strHint_len = 6;
		if (HandleCode == 2) HandleCode = 0;
		InvalidateRect(hWnd, &workRegion, TRUE);
	}
	else
	{
		(*fileOut) << "��������" << size << endl;
		int numAbsent = GetAbsent();
		int numPresent = size - numAbsent;
		(*fileOut) << "��ϯ����:" << numPresent << endl;
		(*fileOut) << "ȱϯ����:" << numAbsent << endl;
		(*fileOut) << "ȱϯ��Ա:" << endl;
		int i = 0;int j = 0;
		for (i = 0;i < size;i++)
			if (people[i] == 0)
			{
				(*fileOut) << i << " ";
				j++;
				if (j == 10)
				{
					(*fileOut) << endl;
					j = 0;
				}
			}
		(*fileOut) << endl;
		(*fileOut) << "��ϯ��Ա:" << endl;
		for (i = 0;i < size;i++)
			if (people[i] == 1)
			{
				(*fileOut) << i << " ";
				j++;
				if (j == 10)
				{
					(*fileOut) << endl;
					j = 0;
				}
			}
		(*fileOut) << endl;
		fileOut->close();
		delete fileOut;
		strHint = "�������ɹ�";
		strHint_len = 6;
		if (HandleCode == 2) HandleCode = 0;
		InvalidateRect(hWnd, &workRegion, TRUE);
	}
}

int GetAbsent(void)
{
	int i = 0;
	int result = 0;
	for (i = 0;i < size;i++)
		if (people[i] == 0)
			result++;
	return result;
}

void HandleStr(HWND hWnd)
{
	if (str_len == 0) return;
	int num = GetNumFromStr();
	if (HandleCode == 1)
	{
		if (num >= MAX_PEOPLE)
		{
			strHint = "��ģ̫������������";
			strHint_len = 10;
			if (HandleCode == 2) HandleCode = 0;
		}
		else
		{
			size = num;
			int i = 0;
			for (i = 0;i < size;i++)
				people[i] = 0;
			strHint = "size����ɹ�";
			strHint_len = 8;
			HandleCode = 0;
		}
	}
	else
	{
		if (Stop == 1)
		{
			strHint = "�Ѿ���ֹ���޷�����ǩ��";
			strHint_len = 11;
			if (HandleCode == 2) HandleCode = 0;
		}
		else
		{
			if (num >= size)
			{
				strHint = "�޴˱�ţ�����������";
				strHint_len = 10;
				HandleCode = 0;
			}
			else
			{
				people[num] = 1;
				strHint_len = sprintf_s(buffer, MAX_CHARIN, "���%dǩ���ɹ�", num);
				HandleCode = 2;
				picture_num = GetRandomNum(picture_num);
			}
		}
	}
	str_len = 0;
	str_in[0] = 0;
	InvalidateRect(hWnd, &workRegion, TRUE);
}

int GetNumFromStr(void)
{
	int num = 0;
	int i = 0;
	for (i = 0;i < str_len;i++)
	{
		num = num * 10 + (str_in[i] - '0');
	}
	return num;
}

void Add(HWND hWnd, WPARAM wParam)
{
	if ((wParam >= '0') && (wParam <= '9'))
	{
		str_in[str_len] = wParam;
		str_len++;
		str_in[str_len] = 0;
		InvalidateRect(hWnd, &workRegion, TRUE);
	}
}

int GetRandomNum(int num)
{
	int n = num;
	while (n == num)
	{
		n = rand() % PictureNum;
	}
	return n;
}

void PutPicture(HDC hdc, int num)
{
	HDC hbuf;
	HBITMAP bmp;
	TCHAR str[100];
	wsprintf(str,TEXT("%c.bmp"), 'a' + num);
	hbuf = CreateCompatibleDC(NULL);
	bmp = (HBITMAP)LoadImage(NULL, str, IMAGE_BITMAP, 
		(region_x2[1] - region_x1[1]) * GRID, 8 * GRID, LR_LOADFROMFILE);
	SelectObject(hbuf, bmp);
	BitBlt(hdc, region_x1[2] * GRID, (region_y2[2] - 8) * GRID, 
		(region_x2[1] - region_x1[1]) * GRID, 8 * GRID, hbuf, 0, 0, SRCCOPY);
	DeleteObject(bmp);
	DeleteObject(hbuf);
}

int GetPos(int num)
{
	if (num == 0) return 1;
	int result = 0;
	while (num > 0)
	{
		num /= 10;
		result += 1;
	}
	return result;
}

void PutPeople(HDC hdc, TCHAR* str, int state, int l_max, int x, int y)
{
	char temp[MAX_CHARIN];
	int len = 0;
	temp[len] = 0;
	int i = 0;
	int j = 0;
	int r = 0;
	int k1 = 0;
	int k2 = 0;
	int k3 = 0;
	int slen = 0;
	for (i = 0;i < size;i++)
		if (people[i] == state)
		{
			temp[len] = ' ';
			len++;temp[len] = 0;
			r = GetPos(i);
			for (k1 = r;k1 > 0;k1--)
			{
				k2 = i;
				for (k3 = 1;k3 < k1;k3++)
					k2 /= 10;
				k2 %= 10;
				temp[len] = '0' + k2;
				len++;temp[len] = 0;
			}
			j++;
			if (j == l_max)
			{
				slen = MultiByteToWideChar(CP_ACP, 0, temp, -1, NULL, 0);
				MultiByteToWideChar(CP_ACP, 0, temp, -1, str, slen);
				TextOut(hdc, x, y, str, slen);		
				y += GRID;
				len = 0;temp[len] = 0;
				j = 0;
			}
		}
	if (j != 0)
	{
		slen = MultiByteToWideChar(CP_ACP, 0, temp, -1, NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, temp, -1, str, slen);
		TextOut(hdc, x, y, str, slen);	
	}
}

void Paint(HWND hWnd, HDC hdc)
{
	HDC		cdc = CreateCompatibleDC(hdc);
	HBITMAP	bmpMem = CreateCompatibleBitmap(hdc, nWidth * GRID, nHeight * GRID);
	SelectObject(cdc, bmpMem);

	HBRUSH hBrush;
	HPEN hPen;
	HBITMAP bmp;

	hBrush = CreateSolidBrush(RGB(127, 127, 127)); 
	SelectObject(cdc, hBrush); 
	hPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	SelectObject(cdc, hPen);
	Rectangle(cdc, region_x1[0] * GRID, region_y1[0] * GRID, region_x2[0] * GRID, region_y2[0] * GRID); 
	DeleteObject(hBrush);
	DeleteObject(hPen);

	hBrush = CreateSolidBrush(RGB(255, 255, 255)); 
	SelectObject(cdc, hBrush); 
	hPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	SelectObject(cdc, hPen);
	Rectangle(cdc, region_x1[1] * GRID, region_y1[1] * GRID, region_x2[1] * GRID, region_y2[1] * GRID); 
	DeleteObject(hBrush);
	DeleteObject(hPen);

	hBrush = CreateSolidBrush(RGB(0, 0, 255)); 
	SelectObject(cdc, hBrush); 
	hPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	SelectObject(cdc, hPen);
	Rectangle(cdc, region_x1[2] * GRID, region_y1[2] * GRID, region_x2[2] * GRID, region_y2[2] * GRID); 
	DeleteObject(hBrush);
	DeleteObject(hPen);

	bmp = (HBITMAP)LoadImage(NULL, TEXT("present.bmp"), IMAGE_BITMAP, 
		(region_x2[3] - region_x1[3]) * GRID, (region_y2[3] - region_y1[3]) * GRID, LR_LOADFROMFILE);
	hBrush = CreatePatternBrush(bmp);
	SelectObject(cdc, hBrush); 
	hPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	SelectObject(cdc, hPen);
	Rectangle(cdc, region_x1[3] * GRID, region_y1[3] * GRID, region_x2[3] * GRID, region_y2[3] * GRID); 
	DeleteObject(hBrush);
	DeleteObject(hPen);
	DeleteObject(bmp);

	bmp = (HBITMAP)LoadImage(NULL, TEXT("absent.bmp"), IMAGE_BITMAP, 
		(region_x2[3] - region_x1[3]) * GRID, (region_y2[3] - region_y1[3]) * GRID, LR_LOADFROMFILE);
	hBrush = CreatePatternBrush(bmp);
	SelectObject(cdc, hBrush); 
	hPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	SelectObject(cdc, hPen);
	Rectangle(cdc, region_x1[4] * GRID, region_y1[4] * GRID, region_x2[4] * GRID, region_y2[4] * GRID); 
	DeleteObject(hBrush);
	DeleteObject(hPen);
	DeleteObject(bmp);

	hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	SelectObject(cdc, hPen);
	MoveToEx(cdc, region_x1[1] * GRID, region_y1[1] * GRID, NULL);
	LineTo(cdc, region_x2[0] * GRID, region_y1[1] * GRID);
	MoveToEx(cdc, region_x1[1] * GRID, region_y2[1] * GRID, NULL);
	LineTo(cdc, region_x2[1] * GRID, region_y2[1] * GRID);
	MoveToEx(cdc, region_x2[1] * GRID, region_y2[3] * GRID, NULL);
	LineTo(cdc, region_x2[3] * GRID, region_y2[3] * GRID);
	MoveToEx(cdc, region_x2[1] * GRID, region_y1[1] * GRID, NULL);
	LineTo(cdc, region_x2[2] * GRID, region_y2[2] * GRID);
	DeleteObject(hPen);

	SetBkMode(cdc, TRANSPARENT);
	TCHAR str[MAX_CHARIN];
	int len;
	if (strHint_len > 0)
	{
		SetTextColor(cdc, RGB(255, 0, 0));
		if (HandleCode == 2)
		{
			len = MultiByteToWideChar(CP_ACP, 0, buffer, -1, NULL, 0);
			MultiByteToWideChar(CP_ACP, 0, buffer, -1, str, len);
		}
		else
		{
			len = MultiByteToWideChar(CP_ACP, 0, strHint, -1, NULL, 0);
			MultiByteToWideChar(CP_ACP, 0, strHint, -1, str, len);
		}
		TextOut(cdc, (int)(nWidth * 0.4) * GRID, region_y1[0] & GRID, str, len);
	}

	SetTextColor(cdc, RGB(0, 0, 0));
	TextOut(cdc, 0, region_y1[1] * GRID, TEXT("���룺"), 3);
	if (str_len > 0)
	{
		len = MultiByteToWideChar(CP_ACP, 0, str_in, -1, NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, str_in, -1, str, len);
		TextOut(cdc, 0, (region_y1[1] + 1) * GRID, str, len); 
	}

	TextOut(cdc, 0, region_y1[2] * GRID, TEXT("��Ҫ˵����"), 5);
	TextOut(cdc, 15, (region_y1[2] + 1) * GRID, TEXT("���㣺F1"), 5);
	TextOut(cdc, 15, (region_y1[2] + 2) * GRID, TEXT("������������F2"), 8);
	TextOut(cdc, 15, (region_y1[2] + 3) * GRID, TEXT("ǩ����ֹ��F3"), 7);
	TextOut(cdc, 15, (region_y1[2] + 4) * GRID, TEXT("ǩ��������F8"), 7);
	TextOut(cdc, 15, (region_y1[2] + 5) * GRID, TEXT("���浱ǰ���ݣ�F5"), 9);
	TextOut(cdc, 15, (region_y1[2] + 6) * GRID, TEXT("��ȡ���ݣ�F6"), 7);
	TextOut(cdc, 15, (region_y1[2] + 7) * GRID, TEXT("����˴ν����F7"), 9);
	TextOut(cdc, 15, (region_y1[2] + 8) * GRID, TEXT("�˳���ESC"), 6);

	int numAbsent = GetAbsent();
	int numPresent = size - numAbsent;
	SetTextColor(cdc, RGB(0, 0, 0));
	len = wsprintf(str,TEXT("�ѳ�ϯ��Ա��%d�ˣ���%d��"), numPresent, size);	
	TextOut(cdc, region_x1[3] * GRID, region_y1[3] * GRID, str, len);
	PutPeople(cdc, str, 1, 20, region_x1[3] * GRID, (region_y1[3] + 1) * GRID);
	SetTextColor(cdc, RGB(0, 0, 0));
	len = wsprintf(str,TEXT("δ��ϯ��Ա��%d�ˣ���%d��"), numAbsent, size);	
	TextOut(cdc, region_x1[4] * GRID, region_y1[4] * GRID, str, len);	
	PutPeople(cdc, str, 0, 20, region_x1[4] * GRID, (region_y1[4] + 1) * GRID);

	PutPicture(cdc, picture_num);

	//�ͷ���Դ
	BitBlt(hdc, 0, 0, nWidth * GRID, nHeight * GRID, cdc, 0, 0, SRCCOPY); 
	ReleaseDC(hWnd, hdc);
	DeleteObject(bmpMem); 
	DeleteObject(cdc);  
}