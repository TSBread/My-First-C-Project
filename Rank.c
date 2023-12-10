#include "Game.h"
#include <time.h>

RANK_INFO Rank[RANK_LIMIT + 1] = { 0 };  // ����������Ҫʹ��ֻ��extern�ñ����������һλ��Ϊ������Ϸ�Ļ����������������ʾ��������ʾ

/*�������*/
RANK_INFO SaveScore(int Score) {
	RANK_INFO New = { .Score = Score,.TimeStamp = TimeStampFuncGetTimeStamp() };

	return New;
}

/*�Է�����������*/
RANK_INFO* SortScores(RANK_INFO p[]) {
	int ArrLen = RANK_LIMIT + 1;
	RANK_INFO Temp;  // ����
	for (int i = 0; i < ArrLen - 1; i++) {
		for (int j = 0; j < ArrLen - i - 1; j++) {
			if (p[j].Score > p[j + 1].Score) {
				Temp = p[j];
				p[j] = p[j + 1];
				p[j + 1] = Temp;
			}
		}
	}
	for (int i = 0; i < ArrLen / 2; i++) {  // ��ת
		Temp = p[i];
		p[i] = p[ArrLen - 1 - i];
		p[ArrLen - 1 - i] = Temp;
	}

	return p;
}

/*TimeStamp�����Ӻ�������ȡ��ǰTimeStamp(s)*/
int TimeStampFuncGetTimeStamp() {
	time_t tt;
	struct tm* st;
	time(&tt);

	return tt;
}

/*TimeStamp�����Ӻ������ж��Ƿ�Ϊ����*/
static int TimeStampFuncIsLeap(int year) {
	if (year % 4 == 0) return 1;
	else return 0;
}

/*TimeStamp����*/
TIME* TimeStampInterpret(int timestamp) {
	TIME* Time = (TIME*)malloc(sizeof(TIME) * 1);
	if (Time != NULL) {
		timestamp += GMT * 3600;

		int all_month[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		int year = 1970;
		int count = 0; // ѭ���ۼƵ�ǰ ��(s)
		int count_temp = 0;
		while (count < timestamp) {
			count_temp = count;
			count += 31536000; // ��ݼ���ƽ��
			if (TimeStampFuncIsLeap(year)) count += 86400; // �����һ��
			year++; // �������
		}
		year -= 1;
		if (TimeStampFuncIsLeap(year)) all_month[1] = 29; // ����2����29��
		count = timestamp - count_temp; // �������Ѿ���������
		count_temp = count / 86400; // �������Ѿ���������
		count -= count_temp * 86400; // �����Ѿ���������
		int hour = count / 3600;
		int min = count % 3600 / 60;
		int s = count % 60;
		int month = 0;
		int day = 0;
		for (int i = 0; i < 12; i++) {
			if (count_temp < all_month[i]) {
				month = i + 1;
				day = count_temp + 1;
				break;
			}
			count_temp -= all_month[i];
		}

		Time->year = year;
		Time->month = month;
		Time->day = day;
		Time->hour = hour;
		Time->min = min;
		Time->s = s;

		return Time;
	}
	else return 0;
}
