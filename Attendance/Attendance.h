/*
* Copyright 2014.215B������
* All rights reserved.
*
* �ļ����ƣ�Attendance.h
* ժ	Ҫ������ϯ���ͳ�ơ���������
*
* ��ǰ�汾��1.0
* ��	��: chendaxixi/���ٱ�
* ������ڣ�2014��4��17��
*
* ȡ���汾��1.0
* ԭ����  ��chendaxixi/���ٱ�
* ������ڣ�2014��4��17��
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

void	Init(HWND hWnd);		//��ʼ��
void	HandleKeyDown(HWND hWnd, WPARAM wParam);	//��������Ӧ
void	Paint(HWND hWnd, HDC hdc);		//��ͼ
void	SetSize(HWND hWnd);		//�趨������
void	Save(HWND hWnd);		//���浱ǰ���
void	Load(HWND hWnd);		//��ȡ�����ļ�
void	PutResult(HWND hWnd);	//������
void	HandleStr(HWND hWnd);	//����������
void	Add(HWND hWnd, WPARAM wParam);		//����
int		GetAbsent(void);		//���ȱϯ����
int 	GetNumFromStr(void);	//�������ַ���ת��������
void	PutPicture(HDC hdc, int num);	//����ͼƬ
int		GetRandomNum(int num);	//��ò���num�ظ��������
void	PutPeople(HDC hdc, TCHAR* str, int state, int l_max, int x, int y);
	// �����������state���˵ı�ţ�ÿ����l_max������(x,y)�㿪ʼ���
int		GetPos(int num);	//��ñ��num��ʮ�����µ�λ��

#endif