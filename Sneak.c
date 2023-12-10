#include "Game.h"

/*新建一个蛇头(需要在New后初始化坐标并将next置空)*/
BODY* NewHead(BODY* LastHead) {
	BODY* NewHead = (BODY*)malloc(sizeof(BODY) * 1);  // New一个身体并指向分配内存

	if (NewHead != NULL) {
		NewHead->next = LastHead;  // 将节点接入链表
	}

	return NewHead;
}

/*删除蛇尾*/
void DeleteTail(BODY* Head) {
	BODY* Pointer = Head;  // 分配一个指针指向蛇头

	while (Pointer->next->next != NULL) {  // 该指针将移至蛇尾的前一个节点
		Pointer = Pointer->next;
	}

	free(Pointer->next->next);  // 先释放内存防止内存泄漏
	Pointer->next = NULL;  // 尾部前一个蛇身置空
}

/*获取指定蛇的长度(从0计数)*/
int GetSneakLen(BODY* Head) {
	int Len = 0;
	BODY* Pointer = Head;

	while (Pointer->next != NULL) {
		//printf("(%d,%d)\n", Pointer->Pos.x, Pointer->Pos.y);  /*Debug*/
		Pointer = Pointer->next;
		Len++;
	}

	return Len;
}

/*判断蛇头是否撞到蛇身*/
int IsHeadHitBody(BODY* Head) {
	int SneakLen = GetSneakLen(Head), Index = 0;
	POS* PosArr = (POS*)malloc(sizeof(POS) * SneakLen);
	BODY* Pointer = Head;

	while (Pointer->next != NULL) {  // 将蛇头及蛇身的坐标存入数组
		if (PosArr != NULL) {
			PosArr[Index].x = Pointer->Pos.x;
			PosArr[Index].y = Pointer->Pos.y;
		}

		Pointer = Pointer->next;
		Index++;
	}

	if (PosArr != NULL) {
		for (int i = 0; i < SneakLen; i++) {  // 依次对比xy是否有重复部分
			int XBuffer = PosArr[i].x;
			int YBuffer = PosArr[i].y;

			for (int j = i + 1; j < SneakLen; j++) {
				if (XBuffer == PosArr[j].x && YBuffer == PosArr[j].y) {
					return 1;
				}
			}
		}

		free(PosArr);
		PosArr = NULL;
	}

	return 0;
}

/*将向量转为POS*/
POS MotionToPos(POS OriginPos, int Motion) {
	POS Pos = OriginPos;
	switch (Motion) {
	case 0:  // w
		Pos.y -= 1;
		break;
	case 1:  // a
		Pos.x -= 1;
		break;
	case 2:  // d
		Pos.x += 1;
		break;
	case 3:  // s
		Pos.y += 1;
		break;
	}

	return Pos;
}

/*判断是否吃到掉落物*/
int IsEat(BODY* Head, ITEM* Item) {
	if (Head->Pos.x == Item->Pos.x && Head->Pos.y == Item->Pos.y) {
		return 1;
	}
	return 0;
}
