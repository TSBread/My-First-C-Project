#include "Game.h"
#include <Windows.h>
#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")

void PlaySounds(SOUNDS Name) {
	;
	switch (Name) {
	case Arrow:
		PlaySound(TEXT("Res/Sounds/Arrow.wav"), NULL, SND_FILENAME | SND_ASYNC);
		break;
	case Select:
		PlaySound(TEXT("Res/Sounds/Select.wav"), NULL, SND_FILENAME | SND_ASYNC);
		break;
	case Menu:
		/*mciSendString("open Res/Sounds/Menu.mp3 alias Menu", NULL, 0, NULL);
		mciSendString("play Menu wait", NULL, 0, NULL);*/
		//PlaySound(TEXT("Res/Sounds/Menu.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		break;
	case Game:
		/*mciSendString("play Res/Sounds/Game.wav repeat", NULL, 0, NULL);*/
		//PlaySound(TEXT("Res/Sounds/Game.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		break;
	case Eat:
		PlaySound(TEXT("Res/Sounds/Eat.wav"), NULL, SND_FILENAME | SND_ASYNC);
		break;
	case End:
		PlaySound(TEXT("Res/Sounds/End.wav"), NULL, SND_FILENAME | SND_ASYNC);
		break;
	}
}
