#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#pragma comment(lib,"winmm.lib")

using namespace std;

/**
@enum SOUND_PLAY
@brief 사운드를 지정한 상수 값
*/
enum SOUND_PLAY
{
	SOUND_BGM_OPENING1,		/**< 오프닝 사운드 */
	SOUND_BGM_LIFE1,		/**< 일상 1 */
	SOUND_BGM_LIFE2,		/**< 일상 2 */
	SOUND_BGM_FUN,			/**< 웃김, 개그 */
	SOUND_BGM_SAD1,			/**< 슬픔 1 */
	SOUND_BGM_SAD2,			/**< 슬픔 2 */
	SOUND_BGM_ENEMY,		/**< 적 */
	SOUND_BGM_DARK,			/**< 어두움 */
	SOUND_BGM_QUESTION,		/**< 궁금함 */
	SOUND_BGM_URGENCY1,		/**< 긴급함 1 */
	SOUND_BGM_URGENCY2,		/**< 긴급함 2 */
	SOUND_BGM_TENSION,		/**< 긴장감 */
	SOUND_BGM_SUSPENSE,		/**< 긴박함 */
	SOUND_BGM_SAD_ENDING	/**< 슬픈 엔딩 */
};

class GameSound
{
private:
	//GameMain* _game;
	GameSound();
	~GameSound();

	static string _resourcePath;

public:

	static void play(string);
	static void playBGM(SOUND_PLAY);	/**< 배경음악을 플레이. */
	static void stop();					/**< 사운드 정지 */

};

