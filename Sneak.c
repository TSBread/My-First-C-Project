#include "Game.h"

/*�½�һ����ͷ(��Ҫ��New���ʼ�����겢��next�ÿ�)*/
BODY* NewHead(BODY* LastHead) {
	BODY* NewHead = (BODY*)malloc(sizeof(BODY) * 1);  // Newһ�����岢ָ������ڴ�

	if (NewHead != NULL) {
		NewHead->next = LastHead;  // ���ڵ��������
	}

	return NewHead;
}

/*ɾ����β*/
void DeleteTail(BODY* Head) {
	BODY* Pointer = Head;  // ����һ��ָ��ָ����ͷ

	while (Pointer->next->next != NULL) {  // ��ָ�뽫������β��ǰһ���ڵ�
		Pointer = Pointer->next;
	}

	free(Pointer->next->next);  // ���ͷ��ڴ��ֹ�ڴ�й©
	Pointer->next = NULL;  // β��ǰһ�������ÿ�
}

/*��ȡָ���ߵĳ���(��0����)*/
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

/*�ж���ͷ�Ƿ�ײ������*/
int IsHeadHitBody(BODY* Head) {
	int SneakLen = GetSneakLen(Head), Index = 0;
	POS* PosArr = (POS*)malloc(sizeof(POS) * SneakLen);
	BODY* Pointer = Head;

	while (Pointer->next != NULL) {  // ����ͷ������������������
		if (PosArr != NULL) {
			PosArr[Index].x = Pointer->Pos.x;
			PosArr[Index].y = Pointer->Pos.y;
		}

		Pointer = Pointer->next;
		Index++;
	}

	if (PosArr != NULL) {
		for (int i = 0; i < SneakLen; i++) {  // ���ζԱ�xy�Ƿ����ظ�����
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

/*������תΪPOS*/
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

/*�ж��Ƿ�Ե�������*/
int IsEat(BODY* Head, ITEM* Item) {
	if (Head->Pos.x == Item->Pos.x && Head->Pos.y == Item->Pos.y) {
		return 1;
	}
	return 0;
}
