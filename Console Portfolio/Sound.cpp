#include "Sound.h"

Sound::Sound(){}
Sound::~Sound(){}

/**
	@param	SOUND_PLAY �÷��� ���� ��ȣ
	@brief	��������� ����Ѵ�.
*/
void Sound::playBGM(SOUND_PLAY play)
{
	switch (play)
	{
	case SOUND_BGM_LIFE1:
		PlaySound(TEXT("resource/bgm/track01.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_SAD1:
		PlaySound(TEXT("resource/bgm/track02.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_SAD2:
		PlaySound(TEXT("resource/bgm/track02r.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_ENEMY:
		PlaySound(TEXT("resource/bgm/track03.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_QUESTION:
		PlaySound(TEXT("resource/bgm/track04.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_URGENCY1:
		PlaySound(TEXT("resource/bgm/track05r.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_OPENING1:
		PlaySound(TEXT("resource/bgm/track06.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_SAD_ENDING:
		PlaySound(TEXT("resource/bgm/track08.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_LIFE2:
		PlaySound(TEXT("resource/bgm/track10.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_URGENCY2:
		PlaySound(TEXT("resource/bgm/track11r.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_TENSION:
		PlaySound(TEXT("resource/bgm/track12.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_SUSPENSE:
		PlaySound(TEXT("resource/bgm/track14r.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_DARK:
		PlaySound(TEXT("resource/bgm/track15.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case SOUND_BGM_FUN:
		PlaySound(TEXT("resource/bgm/track19.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	}
}

/**
	@brief	���带 �����Ѵ�.
*/
void Sound::stop()
{
	PlaySound(NULL, NULL, NULL);
}

