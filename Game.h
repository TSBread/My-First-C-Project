#pragma once
#ifndef __GAME_H__
#define __GAME_H__


/*
* ͨ��ͷ�ļ�
*/
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
* ��Ŀ�꺯��
*/
#define GAME_WINDOWS_HEIGHT 30  // ������ϷTui��С
#define GAME_WINDOWS_WIDTH 120

#define MENU_WINDOWS_OPTION_NUM 5  // ����˵�ѡ������

#define TITLE_NAME "Res/Sneak.txt"  // �����ļ���Ե�ַ

#define YELLOW COLOR_PAIR(1)  // ������������
#define RED COLOR_PAIR(2)
#define WHITE COLOR_PAIR(3)
#define BLUE COLOR_PAIR(4)
#define MAGENTA COLOR_PAIR(5)
#define CYAN COLOR_PAIR(6)

#define RANK_LIMIT 10  // �������а������ʾ����(�����ʼ��=1����Ϣ�����=1�����ƻ����۵���������������13)

#define GMT 8  // ʱ����


/*
* �ṹ�����Ͷ���
*/
typedef struct POS {  // ��������ṹ
	int x;
	int y;
} POS;

typedef struct BODY {  // ��ͷ����ṹ��ֻ���ȡ��ͷ��ַ�����峤�ȼ��ɻ�ȡ�����ߵ���Ϣ
	POS Pos;
	struct BODY* next;
} BODY;

typedef struct ITEM {  // ���������ṹ
	POS Pos;
	void (*Func)();  // ��ͬ������ִ�к���

	int Time;  // ����ʱ��(��mc���)
	int DeathTime;  // ����ʱ��
	struct ITEM* next;
} ITEM;

//typedef struct PLAYER {  // ��Ҷ���ṹ
//	BODY* SneakHead;
//	POS HeadPos;
//
//	int Score;  // ���뱾���������÷���
//	char* Name;  // ���뱾��������������
//} PLAYER;


/*
* Rank.c
*/
typedef struct TIME {
	int year;
	int month;
	int day;
	int hour, min, s;
} TIME;

typedef struct RANK_INFO {
	int TimeStamp;
	int Score;
} RANK_INFO;

RANK_INFO SaveScore(int Score);
RANK_INFO* SortScores(RANK_INFO p[]);
int TimeStampFuncGetTimeStamp();
TIME* TimeStampInterpret(int timestamp);

/*
* Item.c
*/
typedef struct ITEM_FUNC {
	int* ScoreP;
	int ExtraBonus;
} ITEM_FUNC;

ITEM* NewItem(ITEM* LastItem, BODY* Head, int PosXMax, int PosYMax, int (*Func)(ITEM_FUNC), int RandMin, int RandMax, int DeathTime);
int GetItemLen(ITEM* Item);
//void DeleteDeathItem(ITEM* Item);
void ClearItem(ITEM* Item);
void TestFunc();
int AddScore(ITEM_FUNC Info);

/*
* Screen.c
*/
void ScreenInit();
void ScreenMenuWindowLoop();
void ScreenGameWindowLoop();

/*
* Sneak.c
*/
BODY* NewHead(BODY* LastHead);
void DeleteTail(BODY* Head);
int GetSneakLen(BODY* Head);
int IsHeadHitBody(BODY* Head);
POS MotionToPos(POS OriginPos, int Motion);
int IsEat(BODY* Head, ITEM* Item);

/*
* Sounds.c
*/
typedef enum SOUNDS {  // ������Ƶ����ö��
	Arrow,
	Select,
	Menu,
	Game,
	Eat,
	End
} SOUNDS;

void PlaySounds(SOUNDS Name);


#endif
