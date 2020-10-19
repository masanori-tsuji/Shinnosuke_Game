//====================================================================
//
// サウンド処理 (sound.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"

//================================================
// クラス宣言
//================================================

// サウンドのクラス
class CSound
{
public:
    typedef enum
    {
        LABEL_BGM_TITLE = 0,	    // BGM タイトル
        LABEL_BGM_GAME1,            // BGM ゲーム1
        LABEL_BGM_BOSS1,            // BGM ボス1
        LABEL_BGM_GAME2,            // BGM ゲーム2
        LABEL_BGM_BOSS2,            // BGM ボス2
        LABEL_BGM_GAME3,            // BGM ゲーム3
        LABEL_BGM_BOSS3,            // BGM ボス3
        LABEL_BGM_RESULT,           // BGM リザルト
        LABEL_SE_DECISION,          // SE 決定音
        LABEL_SE_SHOT_FIRE,         // SE 炎のショット音
        LABEL_SE_SHOT_WATER,        // SE 水のショット音
        LABEL_SE_SHOT_LIGHT1,       // SE 雷のショット音1
        LABEL_SE_SHOT_LIGHT2,       // SE 雷のショット音2
        LABEL_SE_HIT,               // SE ヒット音
        LABEL_SE_EXPLOSION,         // SE 爆発音
        LABEL_SE_ITEM,              // SE アイテム音
        LABEL_SE_WARNING,           // SE 警告音
        LABEL_SE_SELECT,            // SE 選択音
        LABEL_SE_PAUSE,             // SE ポーズ音
        LABEL_MAX,
    } LABEL;
    CSound();
    ~CSound();  // 親クラスのデストラクタにvirtualをつけることで、メモリリークを抑える
    HRESULT Init(HWND hWnd);
    void Uninit(void);
    HRESULT Play(LABEL label);
    void Stop(LABEL label);
    void StopAll(void);

private:
    typedef struct
    {
        char *pFilename;	// ファイル名
        int nCntLoop;		// ループカウント (-1でループ、0でループなし)
    }PARAM;
    HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
    HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

    IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
    IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイス
    IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// ソースボイス
    BYTE *m_apDataAudio[LABEL_MAX];					    // オーディオデータ
    DWORD m_aSizeAudio[LABEL_MAX];					    // オーディオデータサイズ
    static PARAM m_aParam[LABEL_MAX];                   // 各音素材のパラメータ
};

#endif