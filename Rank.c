#include "Game.h"
#include <time.h>

RANK_INFO Rank[RANK_LIMIT + 1] = { 0 };  // 其他函数若要使用只需extern该变量即可最后一位作为本轮游戏的缓冲区，如果超出显示上限则不显示

/*保存分数*/
RANK_INFO SaveScore(int Score) {
	RANK_INFO New = { .Score = Score,.TimeStamp = TimeStampFuncGetTimeStamp() };

	return New;
}

/*对分数进行排序*/
RANK_INFO* SortScores(RANK_INFO p[]) {
	int ArrLen = RANK_LIMIT + 1;
	RANK_INFO Temp;  // 排序
	for (int i = 0; i < ArrLen - 1; i++) {
		for (int j = 0; j < ArrLen - i - 1; j++) {
			if (p[j].Score > p[j + 1].Score) {
				Temp = p[j];
				p[j] = p[j + 1];
				p[j + 1] = Temp;
			}
		}
	}
	for (int i = 0; i < ArrLen / 2; i++) {  // 反转
		Temp = p[i];
		p[i] = p[ArrLen - 1 - i];
		p[ArrLen - 1 - i] = Temp;
	}

	return p;
}

/*TimeStamp解算子函数：获取当前TimeStamp(s)*/
int TimeStampFuncGetTimeStamp() {
	time_t tt;
	struct tm* st;
	time(&tt);

	return tt;
}

/*TimeStamp解算子函数：判断是否为闰年*/
static int TimeStampFuncIsLeap(int year) {
	if (year % 4 == 0) return 1;
	else return 0;
}

/*TimeStamp解算*/
TIME* TimeStampInterpret(int timestamp) {
	TIME* Time = (TIME*)malloc(sizeof(TIME) * 1);
	if (Time != NULL) {
		timestamp += GMT * 3600;

		int all_month[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		int year = 1970;
		int count = 0; // 循环累计当前 年(s)
		int count_temp = 0;
		while (count < timestamp) {
			count_temp = count;
			count += 31536000; // 年份加上平年
			if (TimeStampFuncIsLeap(year)) count += 86400; // 闰年多一天
			year++; // 年份增加
		}
		year -= 1;
		if (TimeStampFuncIsLeap(year)) all_month[1] = 29; // 闰年2月有29天
		count = timestamp - count_temp; // 今年中已经过的秒数
		count_temp = count / 86400; // 今年中已经过的天数
		count -= count_temp * 86400; // 今天已经过的秒数
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
