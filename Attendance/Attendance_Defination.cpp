/*
* Copyright 2014.215B工作室
* All rights reserved.
*
* 文件名称：Attendance_Defination.cpp
* 摘	要：“出席情况统计”工程函数实现
*
* 当前版本：1.0
* 作	者: chendaxixi/陈少滨
* 完成日期：2014年4月17日
*
* 取代版本：1.0
* 原作者  ：chendaxixi/陈少滨
* 完成日期：2014年4月17日
*/

#include "stdafx.h"
#include "Attendance.h"

BOOL		people[MAX_PEOPLE];						// 记录出勤情况
int 		size;									// 人员规模
char		str_in[MAX_CHARIN];						// 当前输入字符串
int 		str_len;								// 当前输入字符串长度
char*		strHint;								// 提示字符串
int			strHint_len;							// 提示字符串长度
char		buffer[MAX_CHARIN];						// 字符串缓冲区
BOOL		flag = true;							// 判断是否新建
RECT		workRegion;								// 工作区域
BOOL		Stop;									// 判断是否截止
int 		HandleCode;								// 当前操作序号
int 		picture_num;							// 显示的图片序号
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
		strHint = "当前已截止";
		strHint_len = 5;
		if (HandleCode == 2) HandleCode = 0;
		InvalidateRect(hWnd, &workRegion, TRUE);
		break;
	case VK_F8:
		Stop = false;
		strHint = "继续记录";
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
	strHint = "请输入总人数（非负整数)";
	strHint_len = 12;
	HandleCode = 1;
	InvalidateRect(hWnd, &workRegion, TRUE);
}

void Save(HWND hWnd)
{
	ofstream* fileOut = new ofstream(DATAFILENAME, ios::out);
	if (fileOut->fail())
	{
		strHint = "结果保存失败";
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
		strHint = "结果保存成功";
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
		strHint = "数据打开失败";
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
		strHint = "数据读取成功";
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
		strHint = "结果输出失败";
		strHint_len = 6;
		if (HandleCode == 2) HandleCode = 0;
		InvalidateRect(hWnd, &workRegion, TRUE);
	}
	else
	{
		(*fileOut) << "总人数：" << size << endl;
		int numAbsent = GetAbsent();
		int numPresent = size - numAbsent;
		(*fileOut) << "出席人数:" << numPresent << endl;
		(*fileOut) << "缺席人数:" << numAbsent << endl;
		(*fileOut) << "缺席人员:" << endl;
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
		(*fileOut) << "出席人员:" << endl;
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
		strHint = "结果输出成功";
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
			strHint = "规模太大，请重新输入";
			strHint_len = 10;
			if (HandleCode == 2) HandleCode = 0;
		}
		else
		{
			size = num;
			int i = 0;
			for (i = 0;i < size;i++)
				people[i] = 0;
			strHint = "size输入成功";
			strHint_len = 8;
			HandleCode = 0;
		}
	}
	else
	{
		if (Stop == 1)
		{
			strHint = "已经截止，无法继续签到";
			strHint_len = 11;
			if (HandleCode == 2) HandleCode = 0;
		}
		else
		{
			if (num >= size)
			{
				strHint = "无此编号，请重新输入";
				strHint_len = 10;
				HandleCode = 0;
			}
			else
			{
				people[num] = 1;
				strHint_len = sprintf_s(buffer, MAX_CHARIN, "编号%d签到成功", num);
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
	TextOut(cdc, 0, region_y1[1] * GRID, TEXT("输入："), 3);
	if (str_len > 0)
	{
		len = MultiByteToWideChar(CP_ACP, 0, str_in, -1, NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, str_in, -1, str, len);
		TextOut(cdc, 0, (region_y1[1] + 1) * GRID, str, len); 
	}

	TextOut(cdc, 0, region_y1[2] * GRID, TEXT("简要说明："), 5);
	TextOut(cdc, 15, (region_y1[2] + 1) * GRID, TEXT("归零：F1"), 5);
	TextOut(cdc, 15, (region_y1[2] + 2) * GRID, TEXT("设置总人数：F2"), 8);
	TextOut(cdc, 15, (region_y1[2] + 3) * GRID, TEXT("签到截止：F3"), 7);
	TextOut(cdc, 15, (region_y1[2] + 4) * GRID, TEXT("签到继续：F8"), 7);
	TextOut(cdc, 15, (region_y1[2] + 5) * GRID, TEXT("保存当前数据：F5"), 9);
	TextOut(cdc, 15, (region_y1[2] + 6) * GRID, TEXT("读取数据：F6"), 7);
	TextOut(cdc, 15, (region_y1[2] + 7) * GRID, TEXT("输出此次结果：F7"), 9);
	TextOut(cdc, 15, (region_y1[2] + 8) * GRID, TEXT("退出：ESC"), 6);

	int numAbsent = GetAbsent();
	int numPresent = size - numAbsent;
	SetTextColor(cdc, RGB(0, 0, 0));
	len = wsprintf(str,TEXT("已出席人员：%d人；共%d人"), numPresent, size);	
	TextOut(cdc, region_x1[3] * GRID, region_y1[3] * GRID, str, len);
	PutPeople(cdc, str, 1, 20, region_x1[3] * GRID, (region_y1[3] + 1) * GRID);
	SetTextColor(cdc, RGB(0, 0, 0));
	len = wsprintf(str,TEXT("未出席人员：%d人；共%d人"), numAbsent, size);	
	TextOut(cdc, region_x1[4] * GRID, region_y1[4] * GRID, str, len);	
	PutPeople(cdc, str, 0, 20, region_x1[4] * GRID, (region_y1[4] + 1) * GRID);

	PutPicture(cdc, picture_num);

	//释放资源
	BitBlt(hdc, 0, 0, nWidth * GRID, nHeight * GRID, cdc, 0, 0, SRCCOPY); 
	ReleaseDC(hWnd, hdc);
	DeleteObject(bmpMem); 
	DeleteObject(cdc);  
}