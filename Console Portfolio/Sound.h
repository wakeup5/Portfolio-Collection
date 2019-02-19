#pragma once
#include <iostream>
#include <Windows.h>
#pragma comment(lib,"winmm.lib")

/**
	@enum SOUND_PLAY
	@brief ���带 ������ ��� ��
*/
static enum SOUND_PLAY
{
	SOUND_BGM_OPENING1,		/**< ������ ���� */
	SOUND_BGM_LIFE1,		/**< �ϻ� 1 */
	SOUND_BGM_LIFE2,		/**< �ϻ� 2 */
	SOUND_BGM_FUN,			/**< ����, ���� */
	SOUND_BGM_SAD1,			/**< ���� 1 */
	SOUND_BGM_SAD2,			/**< ���� 2 */
	SOUND_BGM_ENEMY,		/**< �� */
	SOUND_BGM_DARK,			/**< ��ο� */
	SOUND_BGM_QUESTION,		/**< �ñ��� */
	SOUND_BGM_URGENCY1,		/**< ����� 1 */
	SOUND_BGM_URGENCY2,		/**< ����� 2 */
	SOUND_BGM_TENSION,		/**< ���尨 */
	SOUND_BGM_SUSPENSE,		/**< ����� */
	SOUND_BGM_SAD_ENDING	/**< ���� ���� */
};

class Sound
{
private:
	//GameMain* _game;
	Sound();
	~Sound();
public:
	static void playBGM(SOUND_PLAY);	/**< ��������� �÷���. */
	static void stop();					/**< ���� ���� */
};

