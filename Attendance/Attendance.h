/*
* Copyright 2014.215B工作室
* All rights reserved.
*
* 文件名称：Attendance.h
* 摘	要：“出席情况统计”工程声明
*
* 当前版本：1.0
* 作	者: chendaxixi/陈少滨
* 完成日期：2014年4月17日
*
* 取代版本：1.0
* 原作者  ：chendaxixi/陈少滨
* 完成日期：2014年4月17日
*/

#pragma once

#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include "resource.h"
using namespace std;

#define MAX_PEOPLE 1000000
#define MAX_CHARIN 1000
#define DATAFILENAME "data.txt"
#define RESULTFILENAME "result.txt"
#define GRID 30
#define nWidth 25
#define nHeight 21
#define timer 300
#define PictureNum 10

void	Init(HWND hWnd);		//初始化
void	HandleKeyDown(HWND hWnd, WPARAM wParam);	//处理案件响应
void	Paint(HWND hWnd, HDC hdc);		//绘图
void	SetSize(HWND hWnd);		//设定总人数
void	Save(HWND hWnd);		//保存当前结果
void	Load(HWND hWnd);		//读取数据文件
void	PutResult(HWND hWnd);	//输出结果
void	HandleStr(HWND hWnd);	//处理输入结果
void	Add(HWND hWnd, WPARAM wParam);		//输入
int		GetAbsent(void);		//获得缺席人数
int 	GetNumFromStr(void);	//将输入字符串转换成数字
void	PutPicture(HDC hdc, int num);	//放置图片
int		GetRandomNum(int num);	//获得不与num重复的随机数
void	PutPeople(HDC hdc, TCHAR* str, int state, int l_max, int x, int y);
	// 输出满足条件state的人的编号，每行输l_max个，从(x,y)点开始输出
int		GetPos(int num);	//获得编号num在十进制下的位数

#endif