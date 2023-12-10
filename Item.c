#include "Game.h"

/*新建一个物品*/
ITEM* NewItem(ITEM* LastItem, BODY* Head, int PosXMax, int PosYMax, void (*Func)(), int RandMin, int RandMax, int DeathTime) {  // ..., 蛇头位置(为了获取蛇头及蛇身坐标), 设置位置最大随机数 , 该物品功能, 设置最小随机值, 设置最大随机值, 设置死亡时间
	ITEM* NewItem = (ITEM*)malloc(sizeof(ITEM) * 1);
	BODY* SneakPointer = Head;
	POS* SneakPos = (POS*)malloc(sizeof(POS) * GetSneakLen(Head));  // 创建坐标列表，包含蛇的所有坐标

	if (NewItem != NULL && SneakPointer != NULL && SneakPos != NULL) {

		for (int i = 0; SneakPointer->next != NULL; i++, SneakPointer = SneakPointer->next) {
			SneakPos[i] = SneakPointer->Pos;
		}

		int RandPosX = 0, RandPosY = 0;

		int GetFinalPos = 0;
		while (!GetFinalPos) {  // 随机出X坐标
			for (int i = 0; i < GetSneakLen(Head); i++) {
				RandPosX = 1 + rand() % PosXMax;
				if (RandPosX == SneakPos[i].x) {
					break;
				}
				else {
					GetFinalPos = 1;
					break;
				}
			}
		}
		GetFinalPos = 0;
		while (!GetFinalPos) {  // 随机出Y坐标
			for (int i = 0; i < GetSneakLen(Head); i++) {
				RandPosY = 1 + rand() % PosYMax;
				if (RandPosY == SneakPos[i].y) {
					break;
				}
				else {
					GetFinalPos = 1;
					break;
				}
			}
		}
		//printf("(%d,%d)\n", RandPosX, RandPosY);
		NewItem->Pos.x = RandPosX;
		NewItem->Pos.y = RandPosY;

		NewItem->next = LastItem;
		NewItem->Func = Func;  // 设置物品功能
		NewItem->Time = RandMin + rand() % RandMax;  // 设置已掉落时间
		NewItem->DeathTime = DeathTime;  // 设置物品死亡时间
	}

	return NewItem;
}

/*获取当前场上物品数量*/
int GetItemLen(ITEM* Item) {
	int Len = 0;
	ITEM* Pointer = Item;

	while (Pointer->next != NULL) {
		Pointer = Pointer->next;
		Len++;
	}

	return Len;
}

/*杀死到达死亡时间的物品*/
//void DeleteDeathItem(ITEM* Item) {
//	ITEM* Pointer = Item;
//
//	while (Pointer->next != NULL) {  // 如果没到尽头
//		while (Pointer->next->next->Time >= Pointer->next->next->DeathTime) {  // 下下个节点到达死亡时间
//			Pointer = Pointer->next;  // 移动到上一节点
//		}
//
//		if (Pointer->next->next->next != NULL) {  // 将下一个节点直接跳过下下个节点转而链接下下下个节点(如果不是NULL的话)
//			Pointer->next = Pointer->next->next->next;
//		}
//
//		free(Pointer->next->next);  // 释放并置空死亡时间到达的物品
//		Pointer->next->next = NULL;
//	}
//}

/*清除掉落物*/
void ClearItem(ITEM* Item) {
	free(Item);
}

/*物品功能子函数：无功能*/
void TestFunc() {
}
