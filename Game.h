#pragma once
#ifndef __GAME_H__
#define __GAME_H__


/*
* 通用头文件
*/
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
* 项目宏函数
*/
#define GAME_WINDOWS_HEIGHT 30  // 定义游戏Tui大小
#define GAME_WINDOWS_WIDTH 120

#define MENU_WINDOWS_OPTION_NUM 5  // 定义菜单选项数量

#define TITLE_NAME "Res/Sneak.txt"  // 标题文件相对地址

#define YELLOW COLOR_PAIR(1)  // 设置字体属性
#define RED COLOR_PAIR(2)
#define WHITE COLOR_PAIR(3)
#define BLUE COLOR_PAIR(4)
#define MAGENTA COLOR_PAIR(5)
#define CYAN COLOR_PAIR(6)

#define RANK_LIMIT 10  // 设置排行榜最大显示数量(如果起始行=1、信息间隔行=1，则不破坏美观的情况下最大限制是13)

#define GMT 8  // 时区差


/*
* 结构体类型定义
*/
typedef struct POS {  // 对象坐标结构
	int x;
	int y;
} POS;

typedef struct BODY {  // 蛇头及身结构，只需获取蛇头地址与身体长度即可获取整条蛇的信息
	POS Pos;
	struct BODY* next;
} BODY;

typedef struct ITEM {  // 掉落物对象结构
	POS Pos;
	void (*Func)();  // 不同掉落物执行函数

	int Time;  // 掉落时常(玩mc玩的)
	int DeathTime;  // 死亡时常
	struct ITEM* next;
} ITEM;

//typedef struct PLAYER {  // 玩家对象结构
//	BODY* SneakHead;
//	POS HeadPos;
//
//	int Score;  // 加入本场比赛所得分数
//	char* Name;  // 加入本场比赛所用名称
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
typedef enum SOUNDS {  // 播放音频类型枚举
	Arrow,
	Select,
	Menu,
	Game,
	Eat,
	End
} SOUNDS;

void PlaySounds(SOUNDS Name);


#endif
