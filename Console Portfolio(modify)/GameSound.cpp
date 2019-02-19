#include "GameSound.h"

GameSound::GameSound(){}
GameSound::~GameSound(){}

/**
@param	SOUND_PLAY 플레이 사운드 번호
@brief	배경음악을 재생한다.
*/
void GameSound::playBGM(SOUND_PLAY play)
{
	//return;
	//stop();
	switch (play)
	{
	case SOUND_BGM_LIFE1:
		PlaySound(TEXT("resource/sound/bgm/track01.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_SAD1:
		PlaySound(TEXT("resource/sound/bgm/track02.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_SAD2:
		PlaySound(TEXT("resource/sound/bgm/track02r.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_ENEMY:
		PlaySound(TEXT("resource/sound/bgm/track03.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_QUESTION:
		PlaySound(TEXT("resource/sound/bgm/track04.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_URGENCY1:
		PlaySound(TEXT("resource/sound/bgm/track05r.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_OPENING1:
		PlaySound(TEXT("resource/sound/bgm/track06.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_SAD_ENDING:
		PlaySound(TEXT("resource/sound/bgm/track08.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_LIFE2:
		PlaySound(TEXT("resource/sound/bgm/track10.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_URGENCY2:
		PlaySound(TEXT("resource/sound/bgm/track11r.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_TENSION:
		PlaySound(TEXT("resource/sound/bgm/track12.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_SUSPENSE:
		PlaySound(TEXT("resource/sound/bgm/track14r.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_DARK:
		PlaySound(TEXT("resource/sound/bgm/track15.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_FUN:
		PlaySound(TEXT("resource/sound/bgm/track19.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	}
}

/**
@brief	사운드를 정지한다.
*/
void GameSound::stop()
{
	PlaySound(NULL, NULL, NULL);
}

