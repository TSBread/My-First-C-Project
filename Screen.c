#define PDC_DLL_BUILD
#include "Curses/curses.h"
#include "Game.h"
#include <Windows.h>

extern RANK_INFO Rank[RANK_LIMIT + 1];
RANK_INFO* p = Rank;
int ThisScore = 0;  // 本轮游戏得分

static int Eat_Apple_Bonus = 1;
static int Sneak_Speed = 100;

/*初始化终端屏幕设置*/
void ScreenInit() {
	initscr();  // 按照需求依照Curses库进行初始化
	curs_set(0);
	resize_term(GAME_WINDOWS_HEIGHT, GAME_WINDOWS_WIDTH);
	raw();
	start_color();
	noecho();
	timeout(0);  // 关闭getch()阻塞

	init_pair(1, COLOR_YELLOW, COLOR_BLACK);  // 黑底黄字
	init_pair(2, COLOR_RED, COLOR_BLACK);  // 黑底红字
	init_pair(3, COLOR_WHITE, COLOR_BLACK);  // 黑底白字
	init_pair(4, COLOR_BLUE, COLOR_BLACK);  // 黑底蓝字
}

/*显示Sneak与游戏名称图片*/
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

/*操作提示栏*/
static void ScreenTipHub(WINDOW* OpWindow, char* Tips) {
	for (int i = 0; i < GAME_WINDOWS_WIDTH; i++) {  // 虽然不会用Vim但不妨碍我韩走NeoVim下面那个白条，因为看着感觉很美观
		wattron(OpWindow, A_REVERSE);
		mvwprintw(OpWindow, GAME_WINDOWS_HEIGHT - 1, i, " ");
	}
	mvwprintw(OpWindow, GAME_WINDOWS_HEIGHT - 1, 0, " Tips:");
	mvwprintw(OpWindow, GAME_WINDOWS_HEIGHT - 1, GAME_WINDOWS_WIDTH / 2 - (int)strlen(Tips) / 2, "%s", Tips);
	wattroff(OpWindow, A_REVERSE);
	wrefresh(OpWindow);
}

/*菜单子函数：跳转至游戏开始*/
static void ScreenMenuWindowFuncStart() {
	clear();

	ScreenGameWindowLoop();
}

/*菜单子函数：显示本地历史排行榜*/
static void ScreenMenuWindowFuncRank() {
	clear();

	WINDOW* MenuRank = newwin(GAME_WINDOWS_HEIGHT, GAME_WINDOWS_WIDTH, 0, 0);
	keypad(MenuRank, TRUE);

	wclear(MenuRank);

	int StartLine = 2, SplitSpace = 1;  // 起始行、信息间隔行

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

/*菜单子函数：游戏设置*/
static void ScreenMenuWindowFuncSetting() {
	clear();

	WINDOW* MenuSetting = newwin(GAME_WINDOWS_HEIGHT, GAME_WINDOWS_WIDTH, 0, 0);
	keypad(MenuSetting, TRUE);

	typedef struct OPTION {
		char* name;  // 选项名称
		int* value;  // 选项默认值
	} OPTION;
	OPTION Info[2] = {
		{"Eat Apple Bonus : ",&Eat_Apple_Bonus},
		{"Sneak Speed (ms) : ",&Sneak_Speed}
	};

	int Select = 0;  // 选择选项

	int StartLine = 2, SplitSpace = 2;  // 起始行、信息间隔行

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

/*菜单子函数：显示作者信息*/
static void ScreenMenuWindowFuncAuthor() {
	clear();

	WINDOW* MenuAuthor = newwin(GAME_WINDOWS_HEIGHT, GAME_WINDOWS_WIDTH, 0, 0);
	keypad(MenuAuthor, TRUE);

	typedef struct INFO {  // 玩mc玩的 {"text":"string","color":"COLOR"}
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

	int StartLine = 2, SplitSpace = 2;  // 起始行、信息间隔行

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

/*菜单子函数：退出程序*/
static void ScreenMenuWindowFuncExit() {
	endwin();
	exit(0);
}

/*菜单主循环函数*/
void ScreenMenuWindowLoop() {
	WINDOW* Menu = newwin(GAME_WINDOWS_HEIGHT, GAME_WINDOWS_WIDTH, 0, 0);  // 新建窗口并返回功能键，其他函数中出现则功能相同
	keypad(Menu, TRUE);

	char* Option[MENU_WINDOWS_OPTION_NUM] = { "Start Game","History Rank","Setting","Author Info","Exit Game" };  // 存储菜单选项名称
	static void (*Func[MENU_WINDOWS_OPTION_NUM])() = {  // 存储函数指针数组
		ScreenMenuWindowFuncStart,
		ScreenMenuWindowFuncRank,
		ScreenMenuWindowFuncSetting,
		ScreenMenuWindowFuncAuthor,
		ScreenMenuWindowFuncExit
	};

	static int Select = 0;  // 从auto改为static类型变量，保证子函数返回依旧是进入前的选项

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
		case 'W':case 'w':  // 上
			Select = (Select <= 0) ? 0 : --Select;
			break;
		case 'S':case 's':  // 下
			Select = (Select >= MENU_WINDOWS_OPTION_NUM - 1) ? MENU_WINDOWS_OPTION_NUM - 1 : ++Select;
			break;
		case 'E':case 'e':
			delwin(Menu);  // 释放菜单窗口进入子函数中，防止子函数再次带调用菜单窗口重复创建窗口及旧窗口内存泄露
			Func[Select]();  // 进入选择的子函数
			break;
		}
	}
}

/*打印分数*/
static void ScreenScoreBoard(WINDOW* OpWindow, int x, int y, int Scores, int Len, chtype Color) {  // ...,起始x ,起始y , 要显示的分数, 显示长度(作用类似域宽)
	// 手打的py，生成的三维数组。使用算法支持任意大小的bitmap输入，不用修改任何参数(该函数，py程序还是得定义一下bitmap的长宽)
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

	int end_x = x + SingleBitMapW * Len;  // 绘制结束时x坐标位置。因为算法是从数字低位开始绘制所以需要获取最终位置的x坐标

	int Op1 = 10, Op2 = 1;
	wattron(OpWindow, Color);
	for (int Len_Pointer = 0; Len_Pointer < Len; Len_Pointer++) {  // 首先以长度起手开始第一层循环嵌套

		int SingleNum = Scores % Op1 / Op2;
		Op1 *= 10;  // 操作数乘平方
		Op2 *= 10;

		for (int BitMap_H = 0; BitMap_H < SingleBitMapH; BitMap_H++) {  // 第二层嵌套进入Bitmap数组中，绘制Bitmap。由于前面已经将单个的数字提出，则将其作为数组索引即可到对应的bitmap上
			int SingleBitMap_X = end_x - SingleBitMapW * (Len_Pointer + 1);  // 计算当前Bitmap左侧坐标
			for (int BitMap_W = 0; BitMap_W < SingleBitMapW; BitMap_W++) {  // 第三层嵌套与第二层嵌套仅是为了绘制数字
				mvwprintw(OpWindow, y + BitMap_H, SingleBitMap_X + BitMap_W, "%c", bitmap[SingleNum][BitMap_H][BitMap_W]);
			}
		}
	}

	wrefresh(OpWindow);
	wattroff(OpWindow, Color);
}

/*游戏子函数：判断游戏是否结束*/
static int ScreenGameWindowFuncIsEnd(WINDOW* GameWindow, BODY* Head) {
	if (IsHeadHitBody(Head) || Head->Pos.x + 1 >= GameWindow->_maxx || Head->Pos.y + 1 >= GameWindow->_maxy || Head->Pos.x <= 0 || Head->Pos.y <= 0) {
		return 1;
	}
	return 0;
}

/*游戏子函数：打印果子蛇和作弊条*/
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

/*游戏画面窗口*/
void ScreenGameWindowLoop() {
	/*窗口部分*/
	int GameWindow_W = GAME_WINDOWS_WIDTH * 70 / 100;  // 游戏窗口高度不变，修改下宽度让计分器有足够空间
	WINDOW* Hub = newwin(GAME_WINDOWS_HEIGHT, GAME_WINDOWS_WIDTH, 0, 0);
	WINDOW* Game = subwin(Hub, GAME_WINDOWS_HEIGHT - 2, GameWindow_W, 0, 0);  // 创建子窗口，刷新Hub即可刷新Game，同时Game的框也不会被因刷新Hub而覆盖
	keypad(Hub, TRUE);
	keypad(Game, TRUE);
	box(Game, 0, 0);  // 给游戏窗口绘个边

	/*游戏初始化*/
	ThisScore = 0;

	POS SneakHead_Pos = { Game->_maxx / 2,Game->_maxy / 2 };  // 创建蛇头坐标
	int ConstInput = rand() % 3;

	BODY* Head = (BODY*)malloc(sizeof(BODY) * 1);  // 创建拥有一个身子的蛇，且方向随机
	if (Head != NULL) {
		Head->next = NULL;  // 给新创建的蛇头赋值
		Head->Pos = SneakHead_Pos;

		Head = NewHead(Head);  // 将旧头作为蛇身，依据随机的向量方向创建新的蛇头
		Head->Pos = SneakHead_Pos = MotionToPos(SneakHead_Pos, ConstInput);
	}

	ITEM* Item = (ITEM*)malloc(sizeof(ITEM) * 1);  // 先生成一个果子，重复随机坐标不要让果子与蛇身重叠
	if (Item != NULL) {
		Item = NewItem(Item, Head, Game->_maxx - 2, Game->_maxy - 2, TestFunc, 0, 1, 0);
	}

	/*循环部分*/
	char KeyClick = '\0';
	int PauseGame = 0;
	while (KeyClick = getch()) {
		if (!PauseGame) {

			SneakHead_Pos = MotionToPos(SneakHead_Pos, ConstInput);  // 因为向量不变则函数一直生成新的坐标
			Head = NewHead(Head);  // 创建新头
			Head->Pos = SneakHead_Pos;

			ScreenGameWindowFuncPrint(Game, Head, Item);  // 打印

			if (IsEat(Head, Item)) {
				ThisScore += Eat_Apple_Bonus;
				ClearItem(Item);
				Item = NewItem(Item, Head, Game->_maxx - 2, Game->_maxy - 2, TestFunc, 0, 1, 0);
			}
			else {
				DeleteTail(Head);  // 删除蛇尾
			}

			if (ScreenGameWindowFuncIsEnd(Game, Head)) {  // 判断是否游戏结束
				for (int i = 0; i < 5; i++) {  // 分数闪烁
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

			/*打印Hub和计分器*/
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
				ScreenTipHub(Hub, "W : Up, A : Left, S : Down, D : Right | Q : Continue Game");  // 打印暂停时的Hub
				timeout(999999);  // 咋瓦鲁多
			}
			else timeout(0);
			break;
		}
		Sleep(Sneak_Speed);
	}
}
