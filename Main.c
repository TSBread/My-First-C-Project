#include "Game.h"

//static void Debug() {
//	/*Debug:NewHead*/
//	int DebugPosX = 114, DebugPosY = 514;
//	BODY* Head = (BODY*)malloc(sizeof(BODY) * 1);
//	if (Head != NULL) {
//		Head->next = NULL;
//
//		Head->Pos.x = DebugPosX;
//		Head->Pos.y = DebugPosY;
//
//		for (int i = 0; i < 3; i++) {
//			Head = NewHead(Head);
//			Head->Pos.x = DebugPosX + i;
//			Head->Pos.y = DebugPosY + i;
//		}
//
//		printf("\nLen=%d\n", GetSneakLen(Head));
//	}
//
//	/*Debug:DeleteTail*/
//	DeleteTail(Head);
//	printf("\nLen=%d\n", GetSneakLen(Head));
//
//	/*碰身子*/
//	//for (int i = 0; i < 1000000; i++)
//	printf("%d ", IsHeadHitBody(Head));
//
//	/*ITEM* Item = (ITEM*)malloc(sizeof(ITEM) * 1);
//	Item = NewItem(Item, Debug, 0, 2, 11);*/
//
//	extern RANK_INFO Rank[RANK_LIMIT+1];
//	for (int i = 0; i < RANK_LIMIT + 1; i++) {
//		Rank[RANK_LIMIT] = SaveScore(i * rand());
//		for (int i = 0; i < RANK_LIMIT; i++) {
//			RANK_INFO* p = SortScores(Rank);
//			printf("%d ", p[i].Score);
//		}
//		printf("\n");
//	}
//
//
//	TIME* Time = TimeStampInterpret(TimeStampFuncGetTimeStamp());
//	printf("\n%d.%d.%d - %2d:%2d:%2d ", Time->year, Time->month, Time->day, Time->hour, Time->min, Time->s);
//}

int main() {
	/*初始化*/
	ScreenInit();  // 初始化PDCurses
	srand((unsigned int)(time(NULL)));  // 初始化随机数种子

	/*Debug*/
	//Debug();
	

	/*循环*/
	PlaySounds(Menu);
	ScreenMenuWindowLoop();  // 进入菜单界面

	return 0;
}
