#define PDC_DLL_BUILD
#include "Curses/curses.h"
#include "Game.h"
#include <Windows.h>

extern RANK_INFO Rank[RANK_LIMIT + 1];
RANK_INFO* p = Rank;
int ThisScore = 0;  // ������Ϸ�÷�

static int Eat_Apple_Bonus = 1;
static int Sneak_Speed = 100;

/*��ʼ���ն���Ļ����*/
void ScreenInit() {
	initscr();  // ������������Curses����г�ʼ��
	curs_set(0);
	resize_term(GAME_WINDOWS_HEIGHT, GAME_WINDOWS_WIDTH);
	raw();
	start_color();
	noecho();
	timeout(0);  // �ر�getch()����

	init_pair(1, COLOR_YELLOW, COLOR_BLACK);  // �ڵ׻���
	init_pair(2, COLOR_RED, COLOR_BLACK);  // �ڵ׺���
	init_pair(3, COLOR_WHITE, COLOR_BLACK);  // �ڵװ���
	init_pair(4, COLOR_BLUE, COLOR_BLACK);  // �ڵ�����
}

/*��ʾSneak����Ϸ����ͼƬ*/
static void ScreenSneakPic(WINDOW* OpWindow, int y, int x) {
	clear();
	wrefresh(OpWindow);

	int i = 0;

	FILE* fp;
	char* FileName = "Sneak.txt";
	char LineStr[257] = { '\0' };
	if ((fp = fopen(FileName, "r+")) != NULL) {
		wattroff(OpWindow, A_REVERSE);
		while (fgets(LineStr, sizeof(LineStr), fp) != NULL) {
			mvwprintw(OpWindow, y + i, x, LineStr);
			i++;
		}
		wrefresh(OpWindow);
		fclose(fp);
	}
}

/*������ʾ��*/
static void ScreenTipHub(WINDOW* OpWindow, char* Tips) {
	for (int i = 0; i < GAME_WINDOWS_WIDTH; i++) {  // ��Ȼ������Vim���������Һ���NeoVim�����Ǹ���������Ϊ���Ÿо�������
		wattron(OpWindow, A_REVERSE);
		mvwprintw(OpWindow, GAME_WINDOWS_HEIGHT - 1, i, " ");
	}
	mvwprintw(OpWindow, GAME_WINDOWS_HEIGHT - 1, 0, " Tips:");
	mvwprintw(OpWindow, GAME_WINDOWS_HEIGHT - 1, GAME_WINDOWS_WIDTH / 2 - (int)strlen(Tips) / 2, "%s", Tips);
	wattroff(OpWindow, A_REVERSE);
	wrefresh(OpWindow);
}

/*�˵��Ӻ�������ת����Ϸ��ʼ*/
static void ScreenMenuWindowFuncStart() {
	clear();

	ScreenGameWindowLoop();
}

/*�˵��Ӻ�������ʾ������ʷ���а�*/
static void ScreenMenuWindowFuncRank() {
	clear();

	WINDOW* MenuRank = newwin(GAME_WINDOWS_HEIGHT, GAME_WINDOWS_WIDTH, 0, 0);
	keypad(MenuRank, TRUE);

	wclear(MenuRank);

	int StartLine = 2, SplitSpace = 1;  // ��ʼ�С���Ϣ�����

	char KeyClick = '\0';
	while (KeyClick = getch()) {
		ScreenTipHub(MenuRank, "Q : Back");
		mvwprintw(MenuRank, StartLine, GAME_WINDOWS_WIDTH / 2 - (int)strlen("Your History Rank:") / 2, "Your History Rank:");
		for (int i = 0; i < RANK_LIMIT; i++) {
			TIME* ThisTS = TimeStampInterpret(p[i].TimeStamp);
			if (ThisTS->year != 1970) {
				if (i == 0) {
					wattron(MenuRank, YELLOW);
					mvwprintw(MenuRank, (StartLine + 0) + (i + 1) * (SplitSpace + 1), GAME_WINDOWS_WIDTH / 2 - (int)strlen("%2d. %5d -- %d.%d.%d %2d:%2d:%2d") / 2, "%2d. %04d -- %d.%02d.%02d %02d:%02d:%02d", i + 1, p[i].Score, ThisTS->year, ThisTS->month, ThisTS->day, ThisTS->hour, ThisTS->min, ThisTS->s);
					wattroff(MenuRank, YELLOW);
				}
				else if (i == 1) {
					wattron(MenuRank, RED);
					mvwprintw(MenuRank, (StartLine + 0) + (i + 1) * (SplitSpace + 1), GAME_WINDOWS_WIDTH / 2 - (int)strlen("%2d. %5d -- %d.%d.%d %2d:%2d:%2d") / 2, "%2d. %04d -- %d.%02d.%02d %02d:%02d:%02d", i + 1, p[i].Score, ThisTS->year, ThisTS->month, ThisTS->day, ThisTS->hour, ThisTS->min, ThisTS->s);
					wattroff(MenuRank, RED);
				}
				else {
					mvwprintw(MenuRank, (StartLine + 0) + (i + 1) * (SplitSpace + 1), GAME_WINDOWS_WIDTH / 2 - (int)strlen("%2d. %5d -- %d.%d.%d %2d:%2d:%2d") / 2, "%2d. %04d -- %d.%02d.%02d %02d:%02d:%02d", i + 1, p[i].Score, ThisTS->year, ThisTS->month, ThisTS->day, ThisTS->hour, ThisTS->min, ThisTS->s);
				}
			}
			if (i == 0 && ThisTS->year == 1970) {
				wattron(MenuRank, RED);
				mvwprintw(MenuRank, GAME_WINDOWS_HEIGHT / 2, GAME_WINDOWS_WIDTH / 2 - (int)strlen("No History") / 2, "No History");
				wattroff(MenuRank, RED);
				break;
			}
		}

		wrefresh(MenuRank);

		if (KeyClick == 'Q' || KeyClick == 'q') break;
	}
	wclear(MenuRank);
	delwin(MenuRank);
	ScreenMenuWindowLoop();
}

/*�˵��Ӻ�������Ϸ����*/
static void ScreenMenuWindowFuncSetting() {
	clear();

	WINDOW* MenuSetting = newwin(GAME_WINDOWS_HEIGHT, GAME_WINDOWS_WIDTH, 0, 0);
	keypad(MenuSetting, TRUE);

	typedef struct OPTION {
		char* name;  // ѡ������
		int* value;  // ѡ��Ĭ��ֵ
	} OPTION;
	OPTION Info[2] = {
		{"Eat Apple Bonus : ",&Eat_Apple_Bonus},
		{"Sneak Speed (ms) : ",&Sneak_Speed}
	};

	int Select = 0;  // ѡ��ѡ��

	int StartLine = 2, SplitSpace = 2;  // ��ʼ�С���Ϣ�����

	char KeyClick = '\0';
	while (KeyClick = getch()) {
		ScreenTipHub(MenuSetting, "W : Up, S : Down, A : Add Value, D : Remove Value, Q : Back");
		mvwprintw(MenuSetting, StartLine, GAME_WINDOWS_WIDTH / 2 - (int)strlen("Game Setting:") / 2, "Game Setting:");
		for (int i = 0; i < sizeof(Info) / sizeof(Info[0]); i++) {
			if (i == Select) {
				wattron(MenuSetting, YELLOW);
			}
			else {
				wattroff(MenuSetting, YELLOW);
			}
			mvwprintw(MenuSetting, (StartLine + 0) + (i + 1) * (SplitSpace + 1), GAME_WINDOWS_WIDTH / 2 - (int)strlen(Info[i].name) / 2, "%s%d", Info[i].name, *Info[i].value);
		}

		wattroff(MenuSetting, YELLOW);
		wrefresh(MenuSetting);

		switch (KeyClick) {
		case 'W':case 'w':
			Select = (Select <= 0) ? 0 : --Select;
			break;
		case 'S':case 's':
			Select = (Select >= sizeof(Info) / sizeof(Info[0]) - 1) ? sizeof(Info) / sizeof(Info[0]) - 1 : ++Select;
			break;
		case 'A':case 'a':
			wclear(MenuSetting);
			(*Info[Select].value) = (*Info[Select].value) <= 1 ? 1 : --(*Info[Select].value);
			break;
		case 'D':case 'd':
			wclear(MenuSetting);
			(*Info[Select].value)++;
			break;
		case 'Q':case 'q':
			delwin(MenuSetting);
			ScreenMenuWindowLoop();
			break;
		}
	}
}

/*�˵��Ӻ�������ʾ������Ϣ*/
static void ScreenMenuWindowFuncAuthor() {
	clear();

	WINDOW* MenuAuthor = newwin(GAME_WINDOWS_HEIGHT, GAME_WINDOWS_WIDTH, 0, 0);
	keypad(MenuAuthor, TRUE);

	typedef struct INFO {  // ��mc��� {"text":"string","color":"COLOR"}
		char* text;
		void (*color)();
	} INFO;
	INFO Info[5] = {
		{"GitHub : TSBread (https://github.com/TSBread)",NULL},
		{"BiliBili : TSBread2960278 (https://space.bilibili.com/450655172)",BLUE},
		{"School ID : 230807011065 (Electronic Information Engineering 2303)",YELLOW},
		{" ",NULL},
		{"More Infomation Please Open `README.md`",WHITE}
	};

	int StartLine = 2, SplitSpace = 2;  // ��ʼ�С���Ϣ�����

	char KeyClick = '\0';
	while (KeyClick = getch()) {
		ScreenTipHub(MenuAuthor, "Ctrl + MouseLeft : Open URL | Q : Back");

		wattron(MenuAuthor, YELLOW);
		mvwprintw(MenuAuthor, StartLine, GAME_WINDOWS_WIDTH / 2 - (int)strlen("My Infomation:") / 2, "My Infomation:");
		wattroff(MenuAuthor, YELLOW);
		for (int i = 0; i < sizeof(Info) / sizeof(Info[0]); i++) {
			wattron(MenuAuthor, Info[i].color);
			mvwprintw(MenuAuthor, (StartLine + 2) + (i + 1) * (SplitSpace + 1), GAME_WINDOWS_WIDTH / 2 - (int)strlen(Info[i].text) / 2, "%s", Info[i].text);
			wattroff(MenuAuthor, Info[i].color);
		}

		wrefresh(MenuAuthor);

		if (KeyClick == 'Q' || KeyClick == 'q') break;
	}
	wclear(MenuAuthor);
	delwin(MenuAuthor);
	ScreenMenuWindowLoop();
}

/*�˵��Ӻ������˳�����*/
static void ScreenMenuWindowFuncExit() {
	endwin();
	exit(0);
}

/*�˵���ѭ������*/
void ScreenMenuWindowLoop() {
	WINDOW* Menu = newwin(GAME_WINDOWS_HEIGHT, GAME_WINDOWS_WIDTH, 0, 0);  // �½����ڲ����ع��ܼ������������г���������ͬ
	keypad(Menu, TRUE);

	char* Option[MENU_WINDOWS_OPTION_NUM] = { "Start Game","History Rank","Setting","Author Info","Exit Game" };  // �洢�˵�ѡ������
	static void (*Func[MENU_WINDOWS_OPTION_NUM])() = {  // �洢����ָ������
		ScreenMenuWindowFuncStart,
		ScreenMenuWindowFuncRank,
		ScreenMenuWindowFuncSetting,
		ScreenMenuWindowFuncAuthor,
		ScreenMenuWindowFuncExit
	};

	static int Select = 0;  // ��auto��Ϊstatic���ͱ�������֤�Ӻ������������ǽ���ǰ��ѡ��

	char KeyClick = '\0';
	while (1) {
		ScreenSneakPic(Menu, 0, 0);
		ScreenTipHub(Menu, "W : Up, S : Down, E : Select");
		for (int i = 0; i < MENU_WINDOWS_OPTION_NUM; i++) {
			if (i == Select) {
				wattron(Menu, YELLOW);
			}
			else {
				wattroff(Menu, YELLOW);
			}
			mvwprintw(Menu, (GAME_WINDOWS_HEIGHT / 2) + i + 5, (GAME_WINDOWS_WIDTH - sizeof(*Option[i])) * 7 / 10, "%s", Option[i]);
		}
		wattroff(Menu, YELLOW);
		wrefresh(Menu);

		KeyClick = wgetch(Menu);
		switch (KeyClick) {
		case 'W':case 'w':  // ��
			Select = (Select <= 0) ? 0 : --Select;
			break;
		case 'S':case 's':  // ��
			Select = (Select >= MENU_WINDOWS_OPTION_NUM - 1) ? MENU_WINDOWS_OPTION_NUM - 1 : ++Select;
			break;
		case 'E':case 'e':
			delwin(Menu);  // �ͷŲ˵����ڽ����Ӻ����У���ֹ�Ӻ����ٴδ����ò˵������ظ��������ڼ��ɴ����ڴ�й¶
			Func[Select]();  // ����ѡ����Ӻ���
			break;
		}
	}
}

/*��ӡ����*/
static void ScreenScoreBoard(WINDOW* OpWindow, int x, int y, int Scores, int Len, chtype Color) {  // ...,��ʼx ,��ʼy , Ҫ��ʾ�ķ���, ��ʾ����(�����������)
	// �ִ��py�����ɵ���ά���顣ʹ���㷨֧�������С��bitmap���룬�����޸��κβ���(�ú�����py�����ǵö���һ��bitmap�ĳ���)
	char bitmap[10][8][8] = {
			{
					' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
					' ', ' ', ' ', '#', '#', '#', ' ', ' ',
					' ', ' ', '#', ' ', ' ', ' ', '#', ' ',
					' ', ' ', '#', ' ', ' ', '#', '#', ' ',
					' ', ' ', '#', ' ', '#', ' ', '#', ' ',
					' ', ' ', '#', '#', ' ', ' ', '#', ' ',
					' ', ' ', '#', ' ', ' ', ' ', '#', ' ',
					' ', ' ', ' ', '#', '#', '#', ' ', ' ',
			},
			{
					' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
					' ', ' ', ' ', ' ', '#', ' ', ' ', ' ',
					' ', ' ', ' ', '#', '#', ' ', ' ', ' ',
					' ', ' ', ' ', ' ', '#', ' ', ' ', ' ',
					' ', ' ', ' ', ' ', '#', ' ', ' ', ' ',
					' ', ' ', ' ', ' ', '#', ' ', ' ', ' ',
					' ', ' ', ' ', ' ', '#', ' ', ' ', ' ',
					' ', ' ', '#', '#', '#', '#', '#', ' ',
			},
			{
					' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
					' ', ' ', ' ', '#', '#', '#', ' ', ' ',
					' ', ' ', '#', ' ', ' ', ' ', '#', ' ',
					' ', ' ', ' ', ' ', ' ', ' ', '#', ' ',
					' ', ' ', ' ', ' ', '#', '#', ' ', ' ',
					' ', ' ', ' ', '#', ' ', ' ', ' ', ' ',
					' ', ' ', '#', ' ', ' ', ' ', '#', ' ',
					' ', ' ', '#', '#', '#', '#', '#', ' ',
			},
			{
					' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
					' ', ' ', ' ', '#', '#', '#', ' ', ' ',
					' ', ' ', '#', ' ', ' ', ' ', '#', ' ',
					' ', ' ', ' ', ' ', ' ', ' ', '#', ' ',
					' ', ' ', ' ', ' ', '#', '#', ' ', ' ',
					' ', ' ', ' ', ' ', ' ', ' ', '#', ' ',
					' ', ' ', '#', ' ', ' ', ' ', '#', ' ',
					' ', ' ', ' ', '#', '#', '#', ' ', ' ',
			},
			{
					' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
					' ', ' ', ' ', ' ', ' ', '#', '#', ' ',
					' ', ' ', ' ', ' ', '#', ' ', '#', ' ',
					' ', ' ', ' ', '#', ' ', ' ', '#', ' ',
					' ', ' ', '#', ' ', ' ', ' ', '#', ' ',
					' ', ' ', '#', '#', '#', '#', '#', ' ',
					' ', ' ', ' ', ' ', ' ', ' ', '#', ' ',
					' ', ' ', ' ', ' ', ' ', ' ', '#', ' ',
			},
			{
					' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
					' ', ' ', '#', '#', '#', '#', '#', ' ',
					' ', ' ', '#', ' ', ' ', ' ', ' ', ' ',
					' ', ' ', '#', '#', '#', '#', ' ', ' ',
					' ', ' ', ' ', ' ', ' ', ' ', '#', ' ',
					' ', ' ', ' ', ' ', ' ', ' ', '#', ' ',
					' ', ' ', '#', ' ', ' ', ' ', '#', ' ',
					' ', ' ', ' ', '#', '#', '#', ' ', ' ',
			},
			{
					' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
					' ', ' ', ' ', ' ', '#', '#', ' ', ' ',
					' ', ' ', ' ', '#', ' ', ' ', ' ', ' ',
					' ', ' ', '#', ' ', ' ', ' ', ' ', ' ',
					' ', ' ', '#', '#', '#', '#', ' ', ' ',
					' ', ' ', '#', ' ', ' ', ' ', '#', ' ',
					' ', ' ', '#', ' ', ' ', ' ', '#', ' ',
					' ', ' ', ' ', '#', '#', '#', ' ', ' ',
			},
			{
					' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
					' ', ' ', '#', '#', '#', '#', '#', ' ',
					' ', ' ', '#', ' ', ' ', ' ', '#', ' ',
					' ', ' ', ' ', ' ', ' ', ' ', '#', ' ',
					' ', ' ', ' ', ' ', ' ', '#', ' ', ' ',
					' ', ' ', ' ', ' ', '#', ' ', ' ', ' ',
					' ', ' ', ' ', ' ', '#', ' ', ' ', ' ',
					' ', ' ', ' ', ' ', '#', ' ', ' ', ' ',
			},
			{
					' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
					' ', ' ', ' ', '#', '#', '#', ' ', ' ',
					' ', ' ', '#', ' ', ' ', ' ', '#', ' ',
					' ', ' ', '#', ' ', ' ', ' ', '#', ' ',
					' ', ' ', ' ', '#', '#', '#', ' ', ' ',
					' ', ' ', '#', ' ', ' ', ' ', '#', ' ',
					' ', ' ', '#', ' ', ' ', ' ', '#', ' ',
					' ', ' ', ' ', '#', '#', '#', ' ', ' ',
			},
			{
					' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
					' ', ' ', ' ', '#', '#', '#', ' ', ' ',
					' ', ' ', '#', ' ', ' ', ' ', '#', ' ',
					' ', ' ', '#', ' ', ' ', ' ', '#', ' ',
					' ', ' ', ' ', '#', '#', '#', '#', ' ',
					' ', ' ', ' ', ' ', ' ', ' ', '#', ' ',
					' ', ' ', ' ', ' ', ' ', '#', ' ', ' ',
					' ', ' ', ' ', '#', '#', ' ', ' ', ' ',
			},
	};

	int SingleBitMapW = sizeof(bitmap[0][0]) / sizeof(bitmap[0][0][0]);
	int SingleBitMapH = sizeof(bitmap[0]) / sizeof(bitmap[0][0]);

	int end_x = x + SingleBitMapW * Len;  // ���ƽ���ʱx����λ�á���Ϊ�㷨�Ǵ����ֵ�λ��ʼ����������Ҫ��ȡ����λ�õ�x����

	int Op1 = 10, Op2 = 1;
	wattron(OpWindow, Color);
	for (int Len_Pointer = 0; Len_Pointer < Len; Len_Pointer++) {  // �����Գ������ֿ�ʼ��һ��ѭ��Ƕ��

		int SingleNum = Scores % Op1 / Op2;
		Op1 *= 10;  // ��������ƽ��
		Op2 *= 10;

		for (int BitMap_H = 0; BitMap_H < SingleBitMapH; BitMap_H++) {  // �ڶ���Ƕ�׽���Bitmap�����У�����Bitmap������ǰ���Ѿ������������������������Ϊ�����������ɵ���Ӧ��bitmap��
			int SingleBitMap_X = end_x - SingleBitMapW * (Len_Pointer + 1);  // ���㵱ǰBitmap�������
			for (int BitMap_W = 0; BitMap_W < SingleBitMapW; BitMap_W++) {  // ������Ƕ����ڶ���Ƕ�׽���Ϊ�˻�������
				mvwprintw(OpWindow, y + BitMap_H, SingleBitMap_X + BitMap_W, "%c", bitmap[SingleNum][BitMap_H][BitMap_W]);
			}
		}
	}

	wrefresh(OpWindow);
	wattroff(OpWindow, Color);
}

/*��Ϸ�Ӻ������ж���Ϸ�Ƿ����*/
static int ScreenGameWindowFuncIsEnd(WINDOW* GameWindow, BODY* Head) {
	if (IsHeadHitBody(Head) || Head->Pos.x + 1 >= GameWindow->_maxx || Head->Pos.y + 1 >= GameWindow->_maxy || Head->Pos.x <= 0 || Head->Pos.y <= 0) {
		return 1;
	}
	return 0;
}

/*��Ϸ�Ӻ�������ӡ�����ߺ�������*/
static void ScreenGameWindowFuncPrint(WINDOW* OpWindow, BODY* Head, ITEM* Item) {
	wclear(OpWindow);
	box(OpWindow, 0, 0);

	wattron(OpWindow, RED);
	mvwprintw(OpWindow, Item->Pos.y, Item->Pos.x, "O");
	wattroff(OpWindow, RED);

	BODY* Pointer = Head;
	for (int i = 0; i < GetSneakLen(Head); i++) {
		if (i == 0) {
			mvwprintw(OpWindow, Pointer->Pos.y, Pointer->Pos.x, "@");
		}
		else {
			mvwprintw(OpWindow, Pointer->Pos.y, Pointer->Pos.x, "+");
		}
		Pointer = Pointer->next;
	}

	wrefresh(OpWindow);
}

/*��Ϸ���洰��*/
void ScreenGameWindowLoop() {
	/*���ڲ���*/
	int GameWindow_W = GAME_WINDOWS_WIDTH * 70 / 100;  // ��Ϸ���ڸ߶Ȳ��䣬�޸��¿���üƷ������㹻�ռ�
	WINDOW* Hub = newwin(GAME_WINDOWS_HEIGHT, GAME_WINDOWS_WIDTH, 0, 0);
	WINDOW* Game = subwin(Hub, GAME_WINDOWS_HEIGHT - 2, GameWindow_W, 0, 0);  // �����Ӵ��ڣ�ˢ��Hub����ˢ��Game��ͬʱGame�Ŀ�Ҳ���ᱻ��ˢ��Hub������
	keypad(Hub, TRUE);
	keypad(Game, TRUE);
	box(Game, 0, 0);  // ����Ϸ���ڻ����

	/*��Ϸ��ʼ��*/
	ThisScore = 0;

	POS SneakHead_Pos = { Game->_maxx / 2,Game->_maxy / 2 };  // ������ͷ����
	int ConstInput = rand() % 3;

	BODY* Head = (BODY*)malloc(sizeof(BODY) * 1);  // ����ӵ��һ�����ӵ��ߣ��ҷ������
	if (Head != NULL) {
		Head->next = NULL;  // ���´�������ͷ��ֵ
		Head->Pos = SneakHead_Pos;

		Head = NewHead(Head);  // ����ͷ��Ϊ��������������������򴴽��µ���ͷ
		Head->Pos = SneakHead_Pos = MotionToPos(SneakHead_Pos, ConstInput);
	}

	ITEM* Item = (ITEM*)malloc(sizeof(ITEM) * 1);  // ������һ�����ӣ��ظ�������겻Ҫ�ù����������ص�
	if (Item != NULL) {
		Item = NewItem(Item, Head, Game->_maxx - 2, Game->_maxy - 2, TestFunc, 0, 1, 0);
	}

	/*ѭ������*/
	char KeyClick = '\0';
	int PauseGame = 0;
	while (KeyClick = getch()) {
		if (!PauseGame) {

			SneakHead_Pos = MotionToPos(SneakHead_Pos, ConstInput);  // ��Ϊ������������һֱ�����µ�����
			Head = NewHead(Head);  // ������ͷ
			Head->Pos = SneakHead_Pos;

			ScreenGameWindowFuncPrint(Game, Head, Item);  // ��ӡ

			if (IsEat(Head, Item)) {
				ThisScore += Eat_Apple_Bonus;
				ClearItem(Item);
				Item = NewItem(Item, Head, Game->_maxx - 2, Game->_maxy - 2, TestFunc, 0, 1, 0);
			}
			else {
				DeleteTail(Head);  // ɾ����β
			}

			if (ScreenGameWindowFuncIsEnd(Game, Head)) {  // �ж��Ƿ���Ϸ����
				for (int i = 0; i < 5; i++) {  // ������˸
					ScreenScoreBoard(Hub, GAME_WINDOWS_WIDTH * 73 / 100, 2, ThisScore, 4, WHITE);
					Sleep(250);
					ScreenScoreBoard(Hub, GAME_WINDOWS_WIDTH * 73 / 100, 2, ThisScore, 4, RED);
					Sleep(250);
				}
				Rank[RANK_LIMIT] = SaveScore(ThisScore);
				p = SortScores(Rank);
				/*Debug*/
				delwin(Hub);
				delwin(Game);
				ScreenMenuWindowLoop();
				break;
			}

			/*��ӡHub�ͼƷ���*/
			ScreenTipHub(Hub, "W : Up, A : Left, S : Down, D : Right | Q : Pause Game");
			ScreenScoreBoard(Hub, GAME_WINDOWS_WIDTH * 73 / 100, 2, ThisScore, 4, YELLOW);
		}

		wrefresh(Hub);

		switch (KeyClick) {
		case 'W':case 'w':
			if (!PauseGame && ConstInput != 3) ConstInput = 0;
			break;
		case 'A':case 'a':
			if (!PauseGame && ConstInput != 2) ConstInput = 1;
			break;
		case 'D':case 'd':
			if (!PauseGame && ConstInput != 1) ConstInput = 2;
			break;
		case 'S':case 's':
			if (!PauseGame && ConstInput != 0) ConstInput = 3;
			break;
		case 'Q':case 'q':
			PauseGame = ~PauseGame;
			if (PauseGame) {
				ScreenTipHub(Hub, "W : Up, A : Left, S : Down, D : Right | Q : Continue Game");  // ��ӡ��ͣʱ��Hub
				timeout(999999);  // զ��³��
			}
			else timeout(0);
			break;
		}
		Sleep(Sneak_Speed);
	}
}
