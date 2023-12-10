#include "Game.h"

/*�½�һ����Ʒ*/
ITEM* NewItem(ITEM* LastItem, BODY* Head, int PosXMax, int PosYMax, void (*Func)(), int RandMin, int RandMax, int DeathTime) {  // ..., ��ͷλ��(Ϊ�˻�ȡ��ͷ����������), ����λ���������� , ����Ʒ����, ������С���ֵ, ����������ֵ, ��������ʱ��
	ITEM* NewItem = (ITEM*)malloc(sizeof(ITEM) * 1);
	BODY* SneakPointer = Head;
	POS* SneakPos = (POS*)malloc(sizeof(POS) * GetSneakLen(Head));  // ���������б������ߵ���������

	if (NewItem != NULL && SneakPointer != NULL && SneakPos != NULL) {

		for (int i = 0; SneakPointer->next != NULL; i++, SneakPointer = SneakPointer->next) {
			SneakPos[i] = SneakPointer->Pos;
		}

		int RandPosX = 0, RandPosY = 0;

		int GetFinalPos = 0;
		while (!GetFinalPos) {  // �����X����
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
		while (!GetFinalPos) {  // �����Y����
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
		NewItem->Func = Func;  // ������Ʒ����
		NewItem->Time = RandMin + rand() % RandMax;  // �����ѵ���ʱ��
		NewItem->DeathTime = DeathTime;  // ������Ʒ����ʱ��
	}

	return NewItem;
}

/*��ȡ��ǰ������Ʒ����*/
int GetItemLen(ITEM* Item) {
	int Len = 0;
	ITEM* Pointer = Item;

	while (Pointer->next != NULL) {
		Pointer = Pointer->next;
		Len++;
	}

	return Len;
}

/*ɱ����������ʱ�����Ʒ*/
//void DeleteDeathItem(ITEM* Item) {
//	ITEM* Pointer = Item;
//
//	while (Pointer->next != NULL) {  // ���û����ͷ
//		while (Pointer->next->next->Time >= Pointer->next->next->DeathTime) {  // ���¸��ڵ㵽������ʱ��
//			Pointer = Pointer->next;  // �ƶ�����һ�ڵ�
//		}
//
//		if (Pointer->next->next->next != NULL) {  // ����һ���ڵ�ֱ���������¸��ڵ�ת�����������¸��ڵ�(�������NULL�Ļ�)
//			Pointer->next = Pointer->next->next->next;
//		}
//
//		free(Pointer->next->next);  // �ͷŲ��ÿ�����ʱ�䵽�����Ʒ
//		Pointer->next->next = NULL;
//	}
//}

/*���������*/
void ClearItem(ITEM* Item) {
	free(Item);
}

/*��Ʒ�����Ӻ������޹���*/
void TestFunc() {
}
